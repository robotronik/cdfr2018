#include "Z_axis.h"

Encoder encoder;

static AX_Interface ax_iface;
AX servo_ar, servo_g, servo_d;

volatile int imp_goal;
volatile PID_DATA pid_z;
FSM_Instance *volatile fsm;
int err;

FSM_Stack fsm_stack;
FSM_Arm fsm_arm;
FSM_Unstack fsm_unstack;
FSM_Place fsm_place;


void Z_Init_AX(){
  ax_iface.receive = AX_Receive_HAL;
  ax_iface.send = AX_Send_HAL;
  ax_iface.set_direction = AX_Set_Direction_HAL;
  ax_iface.delay = HAL_Delay;

  servo_ar.id = AX_ARM_ID;
  servo_ar.interface = &ax_iface;

  servo_g.id = AX_LEFT_ID;
  servo_g.interface = &ax_iface;

  servo_d.id = AX_RIGHT_ID;
  servo_d.interface = &ax_iface;
}

int Z_Check_AX(){
  if(AX_Ping(&servo_ar) != 0) return -1;
  if(AX_Ping(&servo_g) != 0) return -1;
  if(AX_Ping(&servo_d) != 0) return -1;
  return 0;
}

void Z_Enable_AX(bool enable){
  if(enable){
    AX_Power_On(&servo_ar, AX_NOW);
    AX_Power_On(&servo_g, AX_NOW);
    AX_Power_On(&servo_d, AX_NOW);
  }else{
    AX_Power_Off(&servo_ar, AX_NOW);
    AX_Power_Off(&servo_g, AX_NOW);
    AX_Power_Off(&servo_d, AX_NOW);
  }
}

void Z_Open(){
  AX_Set_Goal_Position(&servo_g, AX_LEFT_OPEN, AX_NOW);
  AX_Set_Goal_Position(&servo_d, AX_RIGHT_OPEN, AX_NOW);
}

void Z_Open_Small(){
  AX_Set_Goal_Position(&servo_g, AX_LEFT_OPEN_SMALL, AX_NOW);
  AX_Set_Goal_Position(&servo_d, AX_RIGHT_OPEN_SMALL, AX_NOW);
}

void Z_Close(){
  AX_Set_Goal_Position(&servo_g, AX_LEFT_CLOSE, AX_NOW);
  AX_Set_Goal_Position(&servo_d, AX_RIGHT_CLOSE, AX_NOW);
}

#define AX_GOAL_REACHED(pos, goal) (abs(pos-goal) < AX_EPSILON)

static int Z_Check_AX_Goal(uint16_t goal_left, uint16_t goal_right){
  uint16_t pos_g, pos_d;

  if(AX_Get_Current_Position(&servo_g, &pos_g) != 0){
    return -1;
  }

  if(AX_Get_Current_Position(&servo_d, &pos_d) != 0){
    return -1;
  }

  return AX_GOAL_REACHED(pos_g, goal_left) && AX_GOAL_REACHED(pos_d, goal_right);
}

inline int Z_Is_Open(){
  return Z_Check_AX_Goal(AX_LEFT_OPEN, AX_RIGHT_OPEN);
}

inline int Z_Is_Open_Small(){
  return Z_Check_AX_Goal(AX_LEFT_OPEN_SMALL, AX_RIGHT_OPEN_SMALL);
}

inline int Z_Is_Closed(){
  return Z_Check_AX_Goal(AX_LEFT_CLOSE, AX_RIGHT_CLOSE);
}

inline int Z_Is_Closed_Cube(){
  return Z_Check_AX_Goal(AX_LEFT_CLOSE-20, AX_RIGHT_CLOSE+20);
}

void Z_Set_Goal(int goal){
  imp_goal = goal;
}

inline int Z_Goal_Reached(){
  return reached(&pid_z, imp_goal-encoder.steps);
}

int ramp_generator(int goal, int start, int *nactual,int ntime)
{
  if(*nactual<ntime)
  {
    (*nactual)++;
  }
  return start+((float)(*nactual)/ntime)*(goal-start);
}
