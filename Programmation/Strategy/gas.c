#include "gas.h"
#include "map.h"
#include "strategy.h"
#include "obstacle.h"
#include "interface.h"

#include <stddef.h>
#include <math.h>

static int Check_Path(Cell* current_path);
static Cell* Update_Path(uint16_t x_goal, uint16_t y_goal, Cell *current_path);
static Cell* Update_Path(uint16_t x_goal, uint16_t y_goal, Cell *current_path);
static inline Cell* Cell_From_Pos(uint16_t x, uint16_t y);
static void Brake();
static int Is_Blocked();
static void Unblock();

static inline Cell* Cell_From_Pos(uint16_t x, uint16_t y){
  int const i = y / SQUARE_SIZE;
  int const j = x / SQUARE_SIZE;

  if(i < 0 || i >= MAP_HEIGHT || j < 0 || j >= MAP_WIDTH){
    return NULL;
  }

  return &map[i][j];
}

static int Check_Path(Cell* current_path){
  if(current_path == NULL){
    return -1;
  }
  
  for(; current_path != NULL; current_path = current_path->pred){
    if(current_path->obstacle){
      return -1;
    }
  }

  return 0;
}

static Cell* Make_Path(uint16_t x_goal, uint16_t y_goal){
  Cell *const start = Cell_From_Pos(me.x, me.y);
  Cell *const goal = Cell_From_Pos(x_goal, y_goal);

  //If obstacles can't be drawn
  if((Materialize_Obstacles(MARGIN_MAX) != 0)
     && (Materialize_Obstacles(MARGIN_MIN) != 0)){
    return NULL;
  }

  return Compute_Path(start, goal);
}

static Cell* Update_Path(uint16_t x_goal, uint16_t y_goal, Cell *current_path){
  Refresh_Map();
  Materialize_Obstacles(MARGIN_MIN);
  return (Check_Path(current_path) == 0)?(current_path):(Make_Path(x_goal, y_goal));
}

static void Brake(){
  //Stop position
}

static int Is_Blocked(){
  Cell *pos = Cell_From_Pos(me.x, me.y);
  if(pos == NULL || pos->obstacle){
    return 1;
  }

  int i;
  for(i = 0; i < nb_obstacles; i++){
    Obstacle *const obs = &obstacle[i];
    //The obstacle is too close to generate a path
    if(Is_Too_Close(obs, MARGIN_MIN)){
      return 1;
    }
    //The obstacle is too close to rotate
    if(dist(me.x, me.y, obs->x, obs->y) <= ROBOT_RADIUS){
      return 1;
    }
  }
  
  return 0;
}

static int Free_Blocks_Dir(float angle){
  float dx = 10*cos(angle);
  float dy = 10*sin(angle);

  float x_l = 0, y_l = 0;
  float pos_x = me.x, pos_y = me.y;
  Cell *current_cell;
  while((current_cell = Cell_From_Pos(pos_x+dx, pos_y+dy)) != NULL && !current_cell->obstacle){
    pos_x += dx;
    pos_y += dy;
  }
  x_l = pos_x - me.x;
  y_l = pos_y - me.y;

  return dist(0, 0, x_l, y_l);
}

static void Unblock(){
  while(Is_Blocked()){
    uint16_t obs_forward = 0, obs_backward = 0;
    uint16_t free_forward, free_backward;

    if(sensor_raw[FRONT_LEFT]){
      obs_forward = sensor_raw[FRONT_LEFT];
      if(sensor_raw[FRONT_RIGHT]){
	obs_forward = min(obs_forward, sensor_raw[FRONT_RIGHT]);
      }
    }else{
      obs_forward = sensor_raw[FRONT_RIGHT];
    }

    if(sensor_raw[REAR_LEFT]){
      obs_backward = sensor_raw[REAR_LEFT];
      if(sensor_raw[REAR_RIGHT]){
	obs_backward = min(obs_backward, sensor_raw[REAR_RIGHT]);
      }
    }else{
      obs_backward = sensor_raw[REAR_RIGHT];
    }

    free_forward = Free_Blocks_Dir(me.angle);
    if(obs_forward){
      free_forward = min(free_forward, obs_forward);
    }
    free_backward = Free_Blocks_Dir(me.angle+3.141592);
    if(obs_backward){
      free_backward = min(free_backward, obs_backward);
    }

    uint16_t go_backward = min(free_backward, max(0, MARGIN_MAX-free_forward));
    uint16_t go_forward = min(free_forward, max(0, MARGIN_MAX-free_backward));

    if(go_forward && go_forward > go_backward){
      Go_Straight(me.x + cos(me.angle)*go_forward, me.y + sin(me.angle)*go_forward);
    }else if(go_backward){
      Go_Straight(me.x - cos(me.angle)*go_backward, me.y - sin(me.angle)*go_backward);
    }else{
      Delay(1000);
    }

    Refresh_Map();
  }
}

int Go_Straight(uint16_t x, uint16_t y){
  
}
    
int GOGOGO(uint16_t x, uint16_t y){
  Cell *path_end = NULL;
  int success = 0;

  do{
    Refresh_Map();
    
    //If the robot is blocked, unblock
    if(Is_Blocked()){
      path_end = NULL;
      Brake();
      Unblock();
    }
    
    //Update the path if necessary
    path_end = Update_Path(x, y, path_end);
    if(path_end == NULL){
      success = 0;
      break;
    }
    
    //Control position board
    

  }while(1);

  //Stop position board
  Brake();

  return (success)?(0):(-1);
}
