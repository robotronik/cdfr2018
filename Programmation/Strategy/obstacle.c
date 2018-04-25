#include "obstacle.h"

#include <inttypes.h>
#include <stdbool.h>
#include <math.h>
#include "strategy.h"
#include "interface.h"

Obstacle obstacle[N_MAX_OBSTACLES];
int nb_obstacles = 0;

static int Compute_Obstacle(Obstacle *obs, const Robot *ref, int16_t x_rel, int16_t y_rel);

void Update_Obstacles(const Robot *ref, uint16_t fl_d, uint16_t fr_d, uint16_t rl_d, uint16_t rr_d){
  Obstacle in_range[4];
  bool present[4];
  uint16_t d[4] = {
    [FRONT_LEFT] = fl_d,
    [FRONT_RIGHT] = fr_d,
    [REAR_LEFT] = rl_d,
    [REAR_RIGHT] = rr_d};
  int16_t x_rel[4] = {
    [FRONT_LEFT] = SENSOR_DIST_TANGENT + fl_d,
    [FRONT_RIGHT] = SENSOR_DIST_TANGENT + fr_d,
    [REAR_LEFT] = -(SENSOR_DIST_TANGENT + rl_d),
    [REAR_RIGHT] = -(SENSOR_DIST_TANGENT + rr_d)};
  int16_t y_rel[4] = {
    [FRONT_LEFT] = SENSOR_DIST_PERP,
    [FRONT_RIGHT] = -SENSOR_DIST_PERP,
    [REAR_LEFT] = SENSOR_DIST_PERP,
    [REAR_RIGHT] = -SENSOR_DIST_PERP};
  uint32_t ticks = Get_Ticks();

  //Compute obstacles
  int i;
  for(i = 0; i < 4; i++){
    present[i] = d[i] && !Compute_Obstacle(&in_range[i], ref, x_rel[i], y_rel[i]);
  }

  Obstacle updated[N_MAX_OBSTACLES];
  int n = 0;

  //Front
  if(present[FRONT_LEFT] && present[FRONT_RIGHT]
     && (dist(in_range[FRONT_LEFT].x, in_range[FRONT_LEFT].y,
	      in_range[FRONT_RIGHT].x, in_range[FRONT_RIGHT].y)
	 < 2*OBS_RADIUS)){
    Compute_Obstacle(&updated[n++], ref, SENSOR_DIST_TANGENT + (fl_d + fr_d)/2, 0);
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
    Compute_Obstacle(&updated[n++], ref, -(SENSOR_DIST_TANGENT + (rl_d + rr_d)/2), 0);
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
    if(ticks - obs->last_detection > OBSTACLE_LIFETIME){
      continue;
    }
    
    obs->distance_c = dist(ref->x, ref->y, obs->x_c, obs->y_c);

    //Is it a duplicate ?
    int j = 0;
    for(j = 0; j < n; j++){
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

      if((y_ro >= -SENSOR_DIST_TANGENT && y_ro <= SENSOR_DIST_TANGENT)//In range
	 && (
	     (x_ro > 0
	     && (!fl_d || (x_ro + OBS_RADIUS < fl_d))//No detection at left
	     && (!fr_d || (x_ro + OBS_RADIUS < fr_d))//No detection at right
	     )
	     ||
	     (x_ro < 0
	      && (!rl_d || (-x_ro + OBS_RADIUS < rl_d))
	      && (!rr_d || (-x_ro + OBS_RADIUS < rr_d))
	      )
	     )
	 ){
	obs->no_detect++;
      }
    }
    
    if(obs->no_detect == OBS_NODETECT_COUNT)
      break;
    
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
  float coeff = 1. + (float) OBS_RADIUS / (float) obs->distance;
  obs->x_c = ref->x + coeff * (float) x_ro;
  obs->y_c = ref->y + coeff * (float) y_ro;
  obs->distance_c = dist(ref->x, ref->y, obs->x_c, obs->y_c);
  //Print("(%d, %d)\n", obs->x_c, obs->y_c);
  
  //Check if the obstacle is within the map
  if((obs->x_c >= OBS_RADIUS)
     && (obs->y_c >= OBS_RADIUS)
     && (obs->x_c <= AREA_WIDTH - OBS_RADIUS)
     && (obs->y_c <= AREA_HEIGHT - OBS_RADIUS)){
    obs->no_detect = 0;
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
