#include "z_client.h"
#include "remote_call.h"
#include "usart.h"

static RC_Client z_client;

int Z_Init(){
  RC_Client_Init(&z_client, &z_iface, 0);

  if(RC_Client_Add_Function(&z_client, Z_GET_STATE, "", "b") != 0) return -1;
  if(RC_Client_Add_Function(&z_client, Z_RESET, "", "") != 0) return -1;
  if(RC_Client_Add_Function(&z_client, Z_SET_ASSER, "fffif", "") != 0) return -1;
  if(RC_Client_Add_Function(&z_client, Z_BALEC, "", "") != 0) return -1;
  if(RC_Client_Add_Function(&z_client, Z_PUNCH_BEE, "", "") != 0) return -1;
  if(RC_Client_Add_Function(&z_client, Z_ARM_OUT, "", "") != 0) return -1;
  if(RC_Client_Add_Function(&z_client, Z_ARM_IN, "", "") != 0) return -1;
  if(RC_Client_Add_Function(&z_client, Z_STACK, "", "") != 0) return -1;
  if(RC_Client_Add_Function(&z_client, Z_STACK_LAST, "", "") != 0) return -1;
  if(RC_Client_Add_Function(&z_client, Z_UNSTACK, "", "") != 0) return -1;
  if(RC_Client_Add_Function(&z_client, Z_PLACE, "", "") != 0) return -1;
  
  return 0;
}

int Z_Get_State(Z_State *state){
  return RC_Call(&z_client, Z_GET_STATE, state);
}
#include "pi_client.h"
int Z_Wait_State(Z_State *state){
  if(Z_Get_State(state)) return -1;

  while((Z_Get_State(state) == 0) && (*state == Z_RUNNING)){
    PI_Log("%d\n", *state);
    HAL_Delay(100);
  }

  if(*state == Z_RUNNING) return -1;
  
  return 0;
}

int Z_Reset(){
  return RC_Call(&z_client, Z_RESET);
}

int Z_Set_Asser(float Kp, float Ki, float Kd, int position_tolerance, float speed_tolerance){
  return RC_Call(&z_client, Z_SET_ASSER, Kp, Ki, Kd, position_tolerance, speed_tolerance);
}

int Z_Punch_Bee(){
  return RC_Call(&z_client, Z_PUNCH_BEE);
}

int Z_Arm_In(){
  return RC_Call(&z_client, Z_ARM_IN);
}

int Z_Arm_Out(){
  return RC_Call(&z_client, Z_ARM_OUT);
}

int Z_Stack(bool last){
  return RC_Call(&z_client, (last)?Z_STACK_LAST:Z_STACK);
}

int Z_Unstack(){
  return RC_Call(&z_client, Z_UNSTACK);
}

int Z_Place(){
  return RC_Call(&z_client, Z_PLACE);
}
