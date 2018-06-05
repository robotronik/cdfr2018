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
    HAL_Delay(500);
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

  //  PI_Log("ROTATION INIT\n");
  
  if(Can_Rotate()){
    fsm->run = FSM_ROTATION_CMD;
  }else{
    fsm->retry_count--;
    HAL_Delay(WAIT_TIME);
    //Same state
  }
}

static void FSM_ROTATION_CMD(FSM_Rotation *fsm){
  if(Pos_Set_Angle(ROTATION_SPEED, fsm->goal) != 0){
    PI_Log("Pos_Set_Angle : pas de réponse.\n");
    fsm->state = FSM_ERROR;
    fsm->run = FSM_ROTATION_END;
    return;
  }
  //PI_Log("ROTATION CMD\n");
  fsm->run = FSM_ROTATION_WAIT;
}

static void FSM_ROTATION_WAIT(FSM_Rotation *fsm){
  //PI_Log("ROTATION WAIT\n");
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
  PI_Log("ROTATION STOP\n");
  Brake();
  HAL_Delay(WAIT_TIME);
  fsm->run = FSM_ROTATION_INIT;//TODO
}

static void FSM_ROTATION_END(FSM_Rotation *fsm){
  //NOP NOP NOP
}

//==================================================
//               Straight move
//==================================================

int Balec_Move(uint16_t x, uint16_t y, bool forward, float speed_ratio){
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

  float msr;
  float distance = ((forward)?(1):(-1)) * dist(me.x, me.y, x, y);
  if(!Can_Move(distance, forward, &msr)){
    return -1;
  }
  float speed = min(MAX_SPEED * speed_ratio,
		    MIN_SPEED + (MAX_SPEED-MIN_SPEED)*msr);
  

  Pos_Go_Forward(speed, distance);

  Position_State state;
  do{
    if(Pos_Get_State(&state) != 0){
      PI_Log("Pos_Get_State : pas de réponse.\n");
    }
    float fwd, bwd;
    Get_In_Range_Obstacle_Dist(&fwd, &bwd);
    if((forward && fwd < dist(me.x, me.y, x, y))
       || (!forward && bwd < dist(me.x, me.y, x, y))){
      Brake();
      return -1;
    }
      
  }while(state == POS_RUNNING);

  return (state==POS_SUCCESS)?0:-1;
}

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
  if(dist(me.x, me.y, x, y) < 20){
    return 0;
  }

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
    .speed_ratio = max(0., min(speed_ratio, 1.))
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
  //PI_Log("%d %d %d %d\n", (int) me.x, (int) me.y, (int) fsm->x, (int) fsm->y);
  //PI_Log("MOVE_CMD max_speed_ratio %f speed %f dist %f\n", fsm->max_speed_ratio, speed, distance);
  if(Pos_Go_Forward(speed, distance) != 0){
    PI_Log("Pos_Go_Forward : pas de réponse.\n");
    fsm->state = FSM_ERROR;
    fsm->run = FSM_MOVE_END;
    return;
  }else{
    PI_Log("cmd %f %f", speed, distance);
  }
  
  fsm->run = FSM_MOVE_WAIT;
}

static void FSM_MOVE_WAIT(FSM_Move *fsm){
  //PI_Log("MOVE_WAIT\n");
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
    PI_Log("dist : %f\n", fsm->g_dist);
    Print_Position();
    PI_Log("x_goal %d y_goal %d\n", (int)fsm->x, (int)fsm->y);
    fsm->retry_count--;
    fsm->run = FSM_MOVE_STOP;
    return;
  }

  if(fsm->y == 50 && !(fsm->retry_count--)){
    fsm->run = FSM_MOVE_END;
  }
  
  //fsm->run = FSM_MOVE_CMD;
  HAL_Delay(WAIT_TIME);
}

static void FSM_MOVE_STOP(FSM_Move *fsm){
  PI_Log("MOVE_STOP");
  Brake();
  HAL_Delay(WAIT_TIME);
  fsm->run = FSM_MOVE_INIT;//TODO
}

static void FSM_MOVE_END(FSM_Move *fsm){
  //PI_Log("MOVE_END");
  //Brake();
  //NOP NOP NOP
}

//==================================================
//               Escape strategy
//==================================================

void Break_Free(){
  float fwd_dist, bwd_dist;
  do{
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
  }while(!Can_Rotate());
}

//==================================================
//               Curved Path move
//==================================================

static int Update_Path(uint16_t x_goal, uint16_t y_goal, Cell **current_path);
static int Check_Path(Cell* current_path);
static Cell* Make_Path(uint16_t x_goal, uint16_t y_goal);


int GOGOGO(uint16_t x, uint16_t y){
  Cell *path_end = NULL;
  int retry_count = GO_RETRY_COUNT;
  
  do{
    //Avoid robots if needed
    if(!Can_Rotate()){
      Brake();
      HAL_Delay(1000);
      Break_Free();
      path_end = NULL;//Reinit the process
    }

    if(retry_count == 0){
      return -1;
    }
    
    //Compute path
    int new_path = Update_Path(x, y, &path_end);
    if(path_end == NULL){
      Brake();
      HAL_Delay(WAIT_TIME);
      retry_count--;
      continue;
    }

    //Config curve
    float fwd_dist, bwd_dist;
    Get_In_Range_Obstacle_Dist(&fwd_dist, &bwd_dist);
    float speed = MIN_SPEED + (MAX_SPEED - MIN_SPEED) * min(1., max(0., (fwd_dist - (ROBOT_RADIUS + MARGIN_MAX)) / 1000.));
    if(Pos_Config_Curve(POS_Z, POS_W, speed, ROTATION_SPEED, POS_P, POS_I, POS_D, POS_SPEED_PERCENT_TOLERANCE) == -1){
      PI_Log("Pos_Config_Curve : pas de réponse\n");
    }
    
    //If the path changed, send it
    if(new_path && Pos_Send_Path(path_end, x, y) == -1){
      //Check that this does not mess with UART
      PI_Log("Pos_Send_Path : pas de réponse\n");
      Brake();
      retry_count--;
      path_end = NULL;
      HAL_Delay(WAIT_TIME);
      continue;
    }

    //Check goal
    Position_State state;
    if(Pos_Get_State(&state) == -1){
      PI_Log("Pos_Get_State : pas de réponse\n");
    }else if(state == POS_SUCCESS){
      return 0;
    }else if(state == POS_ERROR){
      PI_Log("Position path error\n");
      Brake();
      retry_count--;
      path_end = NULL;
      HAL_Delay(WAIT_TIME);
      continue;
    }

    HAL_Delay(WAIT_TIME);
  }while(1);
}

static int Update_Path(uint16_t x_goal, uint16_t y_goal, Cell **current_path){
  Refresh_Map();
  Materialize_Obstacles(MARGIN_MAX);
  if(Check_Path(*current_path) == 0){
    return 0;
  }
  
  *current_path = Make_Path(x_goal, y_goal);
  return 1;
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
