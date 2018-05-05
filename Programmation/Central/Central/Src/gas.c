#include "gas.h"
#include "map.h"
#include "strategy.h"
#include "obstacle.h"
#include "interface.h"

#include <stddef.h>
#include <math.h>

//==================================================
//               Rotation
//==================================================

typedef struct FSM_Rotation_S{
  void (run*)(struct FSM_Rotation_S* fsm);
  int retry_count;
  Position_State state;
  float goal;
}FSM_Rotation;

static void FSM_ROTATION_INIT(FSM_Rotation *fsm);
static void FSM_ROTATION_CMD(FSM_Rotation *fsm);
static void FSM_ROTATION_WAIT(FSM_Rotation *fsm);
static void FSM_ROTATION_STOP(FSM_Rotation *fsm);
static void FSM_ROTATION_END(FSM_Rotation *fsm);

int Rotate(float angle){
  FSM_Rotation fsm;
  fsm.goal = angle;
  fsm.retry_count = ROTATE_RETRY_COUNT;
  fsm.state = RUNNING;
  fsm.run = FSM_ROTATION_INIT();
  do{
    fsm.run(&fsm);
  }while(fsm.state == RUNNING);
  return (fsm.state == SUCCESS)?(0):(-1);
}

static void FSM_ROTATION_INIT(FSM_Rotation *fsm){
  if(fsm->retry_count <= 0){
    fsm->state = ERROR;
    fsm->run = FSM_ROTATION_END;
    return;
  }
  
  if(Can_Rotate()){
    fsm->run = FSM_ROTATION_CMD;
  }else{
    fsm->retry_count--;
    HAL_Delay(WAIT_TIME);
    //Same state
  }
}

static void FSM_ROTATION_CMD(FSM_Rotation *fsm){
  if(Pos_Set_Angle(ROATION_SPEED, me.angle - goal) != 0){
    PI_Log("Pos_Set_Angle : pas de réponse.\n");
    fsm->state = ERROR;
    fsm->run = FSM_ROTATION_END;
    return;
  }
  
  fsm->run = FSM_ROTATION_WAIT;
}

static void FSM_ROTATION_WAIT(FSM_Rotation *fsm){
  Position_State pos_state;
  if(Pos_Get_State(&pos_state) != 0){
    PI_Log("Pos_Get_State : pas de réponse.\n");
    fsm->state = ERROR;
    fsm->run = FSM_ROTATION_END;
    return;
  }

  if(pos_state == SUCCESS){
    fsm->state = SUCCESS;
    fsm->run = FSM_ROTATION_END;
    return;
  }

  if(!Can_Rotate() || pos_state == ERROR){
    fsm->retry_count--;
    fsm->run = FSM_ROTATION_STOP;
    return;
  }

  HAL_Delay(WAIT_TIME);
}

static void FSM_ROTATION_STOP(FSM_Rotation *fsm){
  while(Pos_Brake() != 0){
    PI_Log("Pos_Brake : pas de réponse.\n");
    HAL_Delay(10);
  }
  HAL_Delay(WAIT_TIME);
  fsm->run = FSM_ROTATION_INIT;
}

static void FSM_ROTATION_END(FSM_Rotation *fsm){
  //NOP NOP NOP
}

//==================================================
//               Straight move
//==================================================


//==================================================
//               Curved Path move
//==================================================
static int Check_Path(Cell* current_path);
static Cell* Make_Path(uint16_t x_goal, uint16_t y_goal);
static int Update_Path(uint16_t x_goal, uint16_t y_goal, Cell **current_path);
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

static int Update_Path(uint16_t x_goal, uint16_t y_goal, Cell **current_path){
  Refresh_Map();
  Materialize_Obstacles(MARGIN_MIN);
  if(Check_Path(*current_path) == 0){
    return 0;
  }
  
  *current_path = Make_Path(x_goal, y_goal);
  return 1;
}

static void Brake(){
  //Stop position
}

static int Can_Rotate(){
  int i;
  for(i = 0; i < nb_obstacles; i++){
    Obstacle *const obs = &obstacle[i];
    if(dist(me.x, me.y, obs->x, obs->y) <= ROBOT_RADIUS){
      return 0;
    }
  }

  return 1;
}

static int Is_Blocked(){//For curved movement
  Cell *pos = Cell_From_Pos(me.x, me.y);
  if(pos == NULL){
    return 1;
  }

  int i;
  for(i = 0; i < nb_obstacles; i++){
    Obstacle *const obs = &obstacle[i];
    //The obstacle is too close to generate a path
    if(Is_Too_Close(obs)){
      return 1;
    }
  }

  if(!Can_Rotate()){
    return 1;
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

static void Obstacle_Dist(uint16_t *obs_forward, uint16_t *obs_backward){
  *obs_forward = 0;
  *obs_backward = 0;
  if(sensor_raw[FRONT_LEFT]){
    *obs_forward = sensor_raw[FRONT_LEFT];
    if(sensor_raw[FRONT_RIGHT]){
      *obs_forward = min(*obs_forward, sensor_raw[FRONT_RIGHT]);
    }
  }else{
    *obs_forward = sensor_raw[FRONT_RIGHT];
  }
  
  if(sensor_raw[REAR_LEFT]){
    *obs_backward = sensor_raw[REAR_LEFT];
    if(sensor_raw[REAR_RIGHT]){
      *obs_backward = min(*obs_backward, sensor_raw[REAR_RIGHT]);
    }
  }else{
    *obs_backward = sensor_raw[REAR_RIGHT];
  }
}

static void Unblock(){
  while(Is_Blocked()){
    uint16_t obs_forward = 0, obs_backward = 0;
    uint16_t free_forward, free_backward;

    Obstacle_Dist(&obs_forward, &obs_backward);

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

int Rotate(float angle){
  int active = 0;
  do{
    if(!Can_Rotate()){
      Brake();
      active = 0;
      Unblock();
    }

    if(!active){
      //Control position
      active = 1;
    }else{
      //If goal reached, return
    }
  }while(1);
}

int Go_Straight(uint16_t x, uint16_t y){
  Rotate(angle(me.x, me.y, x, y));
  uint16_t obs_fwd, obs_bwd;
  Obstacle_Dist(&obs_fwd, &obs_bwd);

  if(obs_fwd + MARGIN_MIN < dist(me.x, me.y, x, y)){
    return -1;
  }else{
    //Control position
  }
  
  do{
    Obstacle_Dist(&obs_fwd, &obs_bwd);
    if(obs_fwd + MARGIN_MIN < dist(me.x, me.y, x, y)){
      Brake();
      return -1;
    }

    //If goal reached, return 0;
  }while(1);
}
    
int GOGOGO(uint16_t x, uint16_t y){
  Cell *path_end = NULL;
  int success = 0;
  int new_path = 0;

  do{
    Refresh_Map();
    
    //If the robot is blocked, unblock
    if(Is_Blocked()){
      path_end = NULL;
      Brake();
      Unblock();
    }

    //TODO : unblock if on obstacle
    
    //Update the path if necessary
    new_path = Update_Path(x, y, &path_end);
    if(path_end == NULL){
      success = 0;
      break;
    }
    
    //Control position board
    uint16_t obs_fwd, obs_bwd;
    Obstacle_Dist(&obs_fwd, &obs_bwd);
    float speed = min(1., max(((float) (obs_fwd-MARGIN_MIN) / 1000.), 0.));

    //If goal reached, return 0

    if(new_path){
      //Send path
    }else{
      //Set speed
    }
    
  }while(1);

  //Stop position board
  Brake();

  return (success)?(0):(-1);
}
