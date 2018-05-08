#ifndef Z_CLIENT_H
#define Z_CLIENT_H

#include <stdbool.h>

typedef enum Z_State_E{
  Z_RUNNING, Z_SUCCESS, Z_ERROR
}Z_State;

typedef enum Z_Function_E{
  Z_GET_STATE,
  Z_RESET,
  Z_SET_ASSER,
  Z_BALEC,
  Z_PUNCH_BEE,
  Z_ARM_OUT,
  Z_ARM_IN,
  Z_STACK_FIRST,
  Z_STACK,
  Z_STACK_LAST,
  Z_UNSTACK,
  Z_PLACE
}Z_Function;

int Z_Init();
int Z_Get_State(Z_State *state);
int Z_Reset();
int Z_Set_Asser(float Kp, float Ki, float Kd, int position_tolerance, float speed_tolerance);
int Z_Punch_Bee();
int Z_Arm_In();
int Z_Arm_Out();
int Z_Stack(bool last);
int Z_Unstack();
int Z_Place();

int Z_Wait_State(Z_State *state);

#endif
