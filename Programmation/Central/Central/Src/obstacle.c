#include "obstacle.h"

#include <inttypes.h>
#include <stdbool.h>
#include <stdlib.h>
#include <math.h>
#include "strategy.h"
#include "map.h"
#include "interface.h"

Obstacle obstacle[N_MAX_OBSTACLES];
int nb_obstacles = 0;
uint16_t sensor_raw[4];

static int Compute_Obstacle(Obstacle *obs, const Robot *ref, int16_t x_rel, int16_t y_rel);

void Update_Obstacles(const Robot *ref, uint16_t fl_d, uint16_t fr_d, uint16_t rl_d, uint16_t rr_d){
  Obstacle in_range[4];
  bool present[4];
  sensor_raw[FRONT_LEFT] = fl_d;
  sensor_raw[FRONT_RIGHT] = fr_d;
  sensor_raw[REAR_LEFT] = rl_d;
  sensor_raw[REAR_RIGHT] = rr_d;
  int16_t x_rel[4] = {
    [FRONT_LEFT] = SENSOR_DIST_TANGENT + fl_d,
    [FRONT_RIGHT] = SENSOR_DIST_TANGENT + fr_d,
    [REAR_LEFT] = -(SENSOR_DIST_TANGENT + rl_d),
    [REAR_RIGHT] = -(SENSOR_DIST_TANGENT + rr_d)};
  int16_t y_rel[4] = {
    [FRONT_LEFT] = -SENSOR_DIST_PERP,
    [FRONT_RIGHT] = SENSOR_DIST_PERP,
    [REAR_LEFT] = -SENSOR_DIST_PERP,
    [REAR_RIGHT] = SENSOR_DIST_PERP};
  uint32_t ticks = Get_Ticks();

  //Compute obstacles
  int i;
  for(i = 0; i < 4; i++){
    //If the sensor detect an obstacle, compute it
    present[i] = sensor_raw[i] && !Compute_Obstacle(&in_range[i], ref, x_rel[i], y_rel[i]);
  }

  Obstacle updated[N_MAX_OBSTACLES];
  int n = 0;

  //Front
  if(present[FRONT_LEFT] && present[FRONT_RIGHT]
     && (dist(in_range[FRONT_LEFT].x, in_range[FRONT_LEFT].y,
	      in_range[FRONT_RIGHT].x, in_range[FRONT_RIGHT].y)
	 < 2*OBS_RADIUS)){
    //If the two sensors detect the same obstacle
    Compute_Obstacle(&updated[n++], ref, SENSOR_DIST_TANGENT + min(fl_d, fr_d), 0);
  }else{
    if(present[FRONT_LEFT])
      updated[n++] = in_range[FRONT_LEFT];
    if(present[FRONT_RIGHT])
      updated[n++] = in_range[FRONT_RIGHT];
  }

  //Rear
  if(present[REAR_LEFT] && present[REAR_RIGHT]
     && (dist(in_range[REAR_LEFT].x, in_range[REAR_LEFT].y,
	      in_range[REAR_RIGHT].x, in_range[REAR_RIGHT].y)
	 < 2*OBS_RADIUS)){
    Compute_Obstacle(&updated[n++], ref, -(SENSOR_DIST_TANGENT + min(rl_d, rr_d)), 0);
  }else{
    if(present[REAR_LEFT])
      updated[n++] = in_range[FRONT_LEFT];
    if(present[REAR_RIGHT])
      updated[n++] = in_range[FRONT_RIGHT];
  }

  //Fill the updated table with old entries that have not expired and
  //are not a duplicate
  for(i = 0; n < N_MAX_OBSTACLES && i < nb_obstacles; i++){
    Obstacle *const obs = &obstacle[i];

    //Has it expired ?
    if((ticks - obs->last_detection) > OBSTACLE_LIFETIME){
      continue;
    }

    //Update distances
    obs->distance_c = dist(ref->x, ref->y, obs->x_c, obs->y_c);
    obs->distance = dist(ref->x, ref->y, obs->x, obs->y);
    
    //Is it a duplicate ?
    int j = 0;
    for(j = 0; j < n; j++){
      //If it overlaps with another obstacle
      if(dist(obs->x_c, obs->y_c, updated[j].x_c, updated[j].y_c) < 2*OBS_RADIUS)
	break;
    }
    if(j != n){
      break;
    }

    //Is it explicitely NOT in range ?
    {
      int16_t x_ro = obs->x_c - ref->x;
      int16_t y_ro = obs->y_c - ref->y;
      
      //Rotate
      x_ro = (float) x_ro*cos(-ref->angle) - (float) y_ro*sin(-ref->angle);
      y_ro = (float) x_ro*sin(-ref->angle) + (float) y_ro*cos(-ref->angle);

      if((y_ro >= -SENSOR_DIST_TANGENT && y_ro <= SENSOR_DIST_TANGENT)){//Should be in range
	//Update range
	obs->range = (x_ro > 0)?IN_RANGE_FORWARD:IN_RANGE_BACKWARD;
	
	if((x_ro > 0
	     && (!fl_d || (x_ro + OBS_RADIUS < fl_d))//No detection at left
	     && (!fr_d || (x_ro + OBS_RADIUS < fr_d))//No detection at right
	    )
	   ||
	   (x_ro < 0
	    && (!rl_d || (-x_ro + OBS_RADIUS < rl_d))
	    && (!rr_d || (-x_ro + OBS_RADIUS < rr_d))
	    )){
	  //Not in range but should be
	  obs->no_detect++;
	}
      }else{
	//Not in range
	obs->range = OUT_OF_RANGE;
      }
    }
    
    if(obs->no_detect == OBS_NODETECT_COUNT)
      break;

    /*
     * From this point, we know that this obstacle hasn't expired yet,
     * does not overlap any other known obstacle, and it has not moved
     * away. So we keep track of this obstacle with updated
     * measurements.
     */
    updated[n++] = *obs;
  }

  //Then copy the updated table to the global table
  for(i = 0; i < n; i++){
    obstacle[i] = updated[i];
  }
  nb_obstacles = n;
}

static int Compute_Obstacle(Obstacle *obs, const Robot *ref, int16_t x_rel, int16_t y_rel){
  //Get ticks at detection time
  obs->last_detection = Get_Ticks();

  //Distance ref -> obstacle
  obs->distance = sqrt(x_rel*x_rel + y_rel*y_rel);
  //Print("Distance : %"PRId16"\n", obs->distance);

  //Change coordinate system to compute ref -> obstacle vector 
  int16_t x_ro = (float) x_rel*cos(ref->angle) - (float) y_rel*sin(ref->angle);
  int16_t y_ro = (float) x_rel*sin(ref->angle) + (float) y_rel*cos(ref->angle);
  //Print("(%d, %d)\n", x_ro, y_ro);
  
  //Compute obstacle position
  obs->x = ref->x + x_ro;
  obs->y = ref->y + y_ro;
  //Print("(%d, %d)\n", obs->x, obs->y);
  
  //Guess center of obstacle, assuming it's a robot
  obs->x_c = obs->x + OBS_RADIUS * cos(ref->angle);
  obs->y_c = obs->y + OBS_RADIUS * sin(ref->angle);
  obs->distance_c = dist(ref->x, ref->y, obs->x_c, obs->y_c);
  //Print("(%d, %d)\n", obs->x_c, obs->y_c);
  
  //Check if the obstacle is within the map
  if((obs->x_c >= OBS_RADIUS)
     && (obs->y_c >= OBS_RADIUS)
     && (obs->x_c <= AREA_WIDTH - OBS_RADIUS)
     && (obs->y_c <= AREA_HEIGHT - OBS_RADIUS)){
    obs->no_detect = 0;
    obs->range = (x_rel > 0)?IN_RANGE_FORWARD:IN_RANGE_BACKWARD;
    return 0;
  }else{
    return -1;
  }
}

void Print_Obstacles(void){
  int i = 0;
  for(; i < nb_obstacles; i++){
    Obstacle *const obs = &obstacle[i];
    Print("Obstacle %d : %u, %" PRIu16 ", (%" PRId16 ", %" PRId16"), (%"PRId16", %"PRId16")\n",
	  i, obs->last_detection, obs->distance, obs->x, obs->y, obs->x_c, obs->y_c);
  }
}


int Materialize_Obstacle(Obstacle *obs, uint16_t margin){
  //Check if the obstacle will not overlay our robot
  uint16_t r_obs = OBS_RADIUS + ROBOT_RADIUS;
  //Print("Radius : %" PRIu16 "\n", r);
  if(dist(me.x, me.y, obs->x_c, obs->y_c) <= (r_obs + 1.415*SQUARE_SIZE)){
    return -1;
  }

  uint16_t X0 = obs->x_c/SQUARE_SIZE, Y0 = obs->y_c/SQUARE_SIZE;
  uint16_t dx = obs->x_c%SQUARE_SIZE, dy = obs->y_c%SQUARE_SIZE;

  //Number of cubes
  int r = r_obs + margin;
  int N = 1 + (r - max(dx, max(dy, max(SQUARE_SIZE-dx, SQUARE_SIZE-dy)))) / SQUARE_SIZE;
  //Print("N : %d\n", N);
  
  //Check if this circle can be drawn
  if((X0 < N-1) || (X0 > MAP_WIDTH - N) || (Y0 < N-1) || (Y0 > MAP_HEIGHT - N)){
    return -1;
  }

  //Draw the circle
  int X = 0;
  float R = N*SQUARE_SIZE - (float) SQUARE_SIZE/2.;
  int DX = 0;

  //Vector O->R
  int X_OR = me.x/SQUARE_SIZE - X0, Y_OR = me.y/SQUARE_SIZE - Y0;
  float N_OR = sqrt(X_OR*X_OR + Y_OR*Y_OR);
  X_OR *= (1. - 1.415 / N_OR);
  Y_OR *= (1. - 1.415 / N_OR);
  N_OR -= 1.415;

  int treshold = N_OR * N_OR - 1;
  for(X = 0; X < N; X++, DX += SQUARE_SIZE){
    float DY = sqrt(R*R - DX*DX);
    int NB_Y = 1 + (DY-(float)SQUARE_SIZE/2.)/SQUARE_SIZE;
    int Y;
    for(Y = 0; Y < NB_Y; Y++){
      int p_scal = X_OR*X + Y_OR*Y;
      if(p_scal <= 0 || p_scal < treshold)
	map[Y0 + Y][X0 + X].obstacle = 1;

      p_scal = -X_OR*X + Y_OR*Y;
      if(p_scal <= 0 || p_scal < treshold)
	map[Y0 + Y][X0 - X].obstacle = 1;

      p_scal = X_OR*X - Y_OR*Y;
      if(p_scal <= 0 || p_scal < treshold)
	map[Y0 - Y][X0 + X].obstacle = 1;
 
      p_scal = -X_OR*X - Y_OR*Y;
      if(p_scal <= 0 || p_scal < treshold)
	map[Y0 - Y][X0 - X].obstacle = 1;
    }
  }
  
  return 0;
}

int Materialize_Obstacles(uint16_t margin){
  int i;
  int status = 0;
  for(i = 0; i < nb_obstacles; i++){
    if(Materialize_Obstacle(&obstacle[i], margin) != 0){
      status = -1;
    }
  }
  return status;
}

int Can_Rotate(){
  int i;
  for(i = 0; i < nb_obstacles; i++){
    Obstacle *const obs = &obstacle[i];
    if(dist(me.x, me.y, obs->x, obs->y) < ROBOT_RADIUS + MARGIN_MIN){
      break;
    }
  }
  
  return (i == nb_obstacles);
}

int Can_Move(float distance, bool forward, float *max_speed_ratio){
  int i;
  float dist_min = 10000.;
  for(i = 0; i < nb_obstacles; i++){
    Obstacle *const obs = &obstacle[i];
    float obs_dist = dist(me.x, me.y, obs->x, obs->y);

    //If the obstacle is in direction range
    if((forward && obs->range == IN_RANGE_FORWARD)
       || (!forward && obs->range == IN_RANGE_BACKWARD)){
      if(distance > obs_dist - (ROBOT_RADIUS + MARGIN_MIN)){
        return 0;
      }
      if(obs_dist < dist_min){
	dist_min = obs_dist;
      }
    }
  }

  *max_speed_ratio = min(max(0., (dist_min - (ROBOT_RADIUS + MARGIN_MIN))/1000.), 1.);

  return 1;
}

/**
int Is_Too_Close(Obstacle *obs){
  return (dist(me.x, me.y, obs->x_c, obs->y_c) <= (OBS_RADIUS + ROBOT_RADIUS + MARGIN_MIN + 1.415*SQUARE_SIZE));
}

int Is_In_Range(Obstacle *obs, const Robot *ref){
  int16_t x_ro = obs->x_c - ref->x;
  int16_t y_ro = obs->y_c - ref->y;
      
  //Rotate
  x_ro = (float) x_ro*cos(-ref->angle) - (float) y_ro*sin(-ref->angle);
  y_ro = (float) x_ro*sin(-ref->angle) + (float) y_ro*cos(-ref->angle);
  
  if(!(y_ro >= -SENSOR_DIST_TANGENT && y_ro <= SENSOR_DIST_TANGENT)){
    return 0;
  }

  if(x_ro > 0){
    return 1;
  }
  
  return -1;
  }*/
