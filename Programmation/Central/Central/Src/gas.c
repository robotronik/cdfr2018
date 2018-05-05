#include "gas.h"
#include "map.h"
#include "strategy.h"
#include "obstacle.h"
#include "interface.h"
#include "position_client.h"

#include <stddef.h>
#include <math.h>

//==================================================
//               Brake
//==================================================
void Brake(){
  while(Pos_Brake() != 0){
    PI_Log("Pos_Brake : pas de réponse.\n");
    HAL_Delay(10);
  }
}

//==================================================
//               Rotation
//==================================================

typedef enum FSM_State_E{
  FSM_RUNNING, FSM_SUCCESS, FSM_ERROR
}FSM_State;

typedef struct FSM_Rotation_S{
  void (*run)(struct FSM_Rotation_S* fsm);
  FSM_State state;
  int retry_count;
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
  fsm.state = FSM_RUNNING;
  fsm.run = FSM_ROTATION_INIT;
  do{
    fsm.run(&fsm);
  }while(fsm.state == FSM_RUNNING);
  return (fsm.state == FSM_SUCCESS)?(0):(-1);
}

static void FSM_ROTATION_INIT(FSM_Rotation *fsm){
  if(fsm->retry_count <= 0){
    fsm->state = FSM_ERROR;
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
  if(Pos_Set_Angle(ROTATION_SPEED, me.angle - fsm->goal) != 0){
    PI_Log("Pos_Set_Angle : pas de réponse.\n");
    fsm->state = FSM_ERROR;
    fsm->run = FSM_ROTATION_END;
    return;
  }
  
  fsm->run = FSM_ROTATION_WAIT;
}

static void FSM_ROTATION_WAIT(FSM_Rotation *fsm){
  Position_State pos_state;
  if(Pos_Get_State(&pos_state) != 0){
    PI_Log("Pos_Get_State : pas de réponse.\n");
    fsm->state = FSM_ERROR;
    fsm->run = FSM_ROTATION_END;
    return;
  }

  if(pos_state == POS_SUCCESS){
    fsm->state = FSM_SUCCESS;
    fsm->run = FSM_ROTATION_END;
    return;
  }

  if(!Can_Rotate() || pos_state == POS_ERROR){
    fsm->retry_count--;
    fsm->run = FSM_ROTATION_STOP;
    return;
  }

  HAL_Delay(WAIT_TIME);
}

static void FSM_ROTATION_STOP(FSM_Rotation *fsm){
  Brake();
  HAL_Delay(WAIT_TIME);
  fsm->run = FSM_ROTATION_INIT;
}

static void FSM_ROTATION_END(FSM_Rotation *fsm){
  //NOP NOP NOP
}

//==================================================
//               Straight move
//==================================================
typedef struct FSM_Move_S{
  void (*run)(struct FSM_Move_S* fsm);
  FSM_State state;
  int retry_count;
  uint16_t x;
  uint16_t y;
  bool forward;
  float speed_ratio;
  float max_speed_ratio;
  float g_dist;
}FSM_Move;

static void FSM_MOVE_INIT(FSM_Move *fsm);
static void FSM_MOVE_CMD(FSM_Move *fsm);
static void FSM_MOVE_WAIT(FSM_Move *fsm);
static void FSM_MOVE_STOP(FSM_Move *fsm);
static void FSM_MOVE_END(FSM_Move *fsm);

static int Go_Straight_Direct(uint16_t x, uint16_t y, bool forward, float speed_ratio);
/**
 * WARNING : this function assumes the robot already performed a
 * rotation to get in the right angle. If not, this will lead to
 * undefined behavior. The reason it takes (x, y) and not just a
 * distance is that it uses the effective position of the robot and
 * the goal to determine whether or not the move is safe.
 */

int Go_Straight(uint16_t x, uint16_t y, bool forward, float speed_ratio){
  //Compute angle and distance
  float g_angle = angle(me.x, me.y, x, y);
  if(!forward){
    g_angle += M_PI;
  }
  if(g_angle > M_PI){
    g_angle -= 2*M_PI;
  }

  //Rotate and cancel if it fails
  if(Rotate(g_angle) != 0){
    return -1;
  }

  return Go_Straight_Direct(x, y, forward, speed_ratio);
}

static int Go_Straight_Direct(uint16_t x, uint16_t y, bool forward, float speed_ratio){
    FSM_Move fsm = {
    .run = FSM_MOVE_INIT,
    .state = FSM_RUNNING,
    .retry_count = MOVE_RETRY_COUNT,
    .x = x,
    .y = y,
    .forward = forward,
    .speed_ratio = min(0., max(speed_ratio, 1.))
  };
  

  do{
    fsm.run(&fsm);
  }while(fsm.state == FSM_RUNNING);
  return (fsm.state == FSM_SUCCESS)?(0):(-1);
}

static void FSM_MOVE_INIT(FSM_Move *fsm){
  if(fsm->retry_count <= 0){
    fsm->state = FSM_ERROR;
    fsm->run = FSM_MOVE_END;
    return;
  }

  fsm->g_dist = dist(me.x, me.y, fsm->x, fsm->y);

  if(Can_Move(fsm->g_dist, fsm->forward, &fsm->max_speed_ratio)){
    fsm->run = FSM_MOVE_CMD;
  }else{
    fsm->retry_count--;
    HAL_Delay(WAIT_TIME);
    //Same state
  }
}

static void FSM_MOVE_CMD(FSM_Move *fsm){
  float speed = min(MAX_SPEED * fsm->speed_ratio,
		    MIN_SPEED + (MAX_SPEED-MIN_SPEED)*fsm->max_speed_ratio);
  float distance = ((fsm->forward)?(1):(-1)) * fsm->g_dist;
  if(Pos_Go_Forward(speed, distance) != 0){
    PI_Log("Pos_Go_Forward : pas de réponse.\n");
    fsm->state = FSM_ERROR;
    fsm->run = FSM_MOVE_END;
    return;
  }

  fsm->run = FSM_MOVE_WAIT;
}

static void FSM_MOVE_WAIT(FSM_Move *fsm){
  Position_State state;
  if(Pos_Get_State(&state) != 0){
    PI_Log("Pos_Get_State : pas de réponse.\n");
    fsm->state = FSM_ERROR;
    fsm->run = FSM_MOVE_END;
    return;
  }

  //If the goal is reached
  if(state == POS_SUCCESS){
    fsm->state = FSM_SUCCESS;
    fsm->run = FSM_MOVE_END;
    return;
  }

  //If there is an obstacle
  fsm->g_dist = dist(me.x, me.y, fsm->x, fsm->y);
  if(!Can_Move(fsm->g_dist, fsm->forward, &fsm->max_speed_ratio) || state == POS_ERROR){
    fsm->retry_count--;
    fsm->run = FSM_MOVE_STOP;
    return;
  }

  fsm->run = FSM_MOVE_CMD;
  HAL_Delay(WAIT_TIME);
}

static void FSM_MOVE_STOP(FSM_Move *fsm){
  Brake();
  HAL_Delay(WAIT_TIME);
  fsm->run = FSM_MOVE_INIT;
}

static void FSM_MOVE_END(FSM_Move *fsm){
  //NOP NOP NOP
}

//==================================================
//               Escape strategy
//==================================================

void Break_Free(){
  float fwd_dist, bwd_dist;
  while(!Can_Rotate()){
    Get_Avoidance_Flexibility(&fwd_dist, &bwd_dist);
    float fwd_goal = min(fwd_dist, max(0, MARGIN_MAX-bwd_dist));
    float bwd_goal = min(bwd_dist, max(0, MARGIN_MAX-fwd_dist));
    if(fwd_goal < 1 && bwd_goal < 1){
      HAL_Delay(WAIT_TIME);
      continue;
    }

    uint16_t x, y;
    uint8_t fwd;
    if(fwd_goal < 1){
      x = me.x - bwd_goal*cos(me.angle);
      y = me.y - bwd_goal*sin(me.angle);
      fwd = 0;
    }
    else if(bwd_goal < 1){
      x = me.x + bwd_goal*cos(me.angle);
      y = me.y + bwd_goal*sin(me.angle);
      fwd = 1;
    }else{
      float goal = (fwd_goal - bwd_goal)/2.;
      x = me.x + goal*cos(me.angle);
      y = me.y + goal*sin(me.angle);
      fwd = (goal > 0)?1:0;
    }

    Go_Straight_Direct(x, y, fwd, AVOIDANCE_SPEED_RATIO);
  }
}

//==================================================
//               Curved Path move
//==================================================
static int Check_Path(Cell* current_path);
static Cell* Make_Path(uint16_t x_goal, uint16_t y_goal);
static int Update_Path(uint16_t x_goal, uint16_t y_goal, Cell **current_path);

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
