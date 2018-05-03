#include "position_client.h"
#include "strategy.h"
#include "remote_call.h"
#include "usart.h"
static RC_Client pos_client;

typedef enum Pos_Client_Function_E{
  POS_GET_STATE,
  POS_RESET,
  POS_SET_ASSER_SUM,
  POS_SET_ASSER_DIFF,
  POS_SET_N_POINTS_ASSER,
  POS_SET_ODO,
  POS_GET_ODO,
  POS_BALEC,
  POS_SET_N_POINTS,
  POS_GET_N_POINTS,
  POS_GO_FORWARD,
  POS_SET_ANGLE,
  POS_BRAKE,
  POS_SET_POSITION_X_Y
}Pos_Client_Function;

int Position_Init(){
  RC_Client_Init(&pos_client, &pos_iface, 0);
  
  if(RC_Client_Add_Function(&pos_client, POS_RESET, "", "")) return -1;
  if(RC_Client_Add_Function(&pos_client, POS_SET_ASSER_SUM, "fffif", "")) return -1;
  if(RC_Client_Add_Function(&pos_client, POS_SET_ASSER_DIFF, "fffif", "")) return -1;
  if(RC_Client_Add_Function(&pos_client, POS_SET_N_POINTS_ASSER, "ffffffff", "")) return -1;
  if(RC_Client_Add_Function(&pos_client, POS_SET_ODO, "BB", "")) return -1;
  if(RC_Client_Add_Function(&pos_client, POS_GET_ODO, "", "BBf")) return -1;
  if(RC_Client_Add_Function(&pos_client, POS_GO_FORWARD, "ff", "")) return -1;
  if(RC_Client_Add_Function(&pos_client, POS_SET_ANGLE, "ff", "")) return -1;
  if(RC_Client_Add_Function(&pos_client, POS_SET_POSITION_X_Y, "ffBB", "")) return -1;
  if(RC_Client_Add_Function(&pos_client, POS_SET_N_POINTS, "i", "")) return -1;
  if(RC_Client_Add_Function(&pos_client, POS_GET_N_POINTS, "BB", "")) return -1;
  if(RC_Client_Add_Function(&pos_client, POS_GET_STATE, "", "b")) return -1;
  if(RC_Client_Add_Function(&pos_client, POS_BALEC, "", "")) return -1;
  if(RC_Client_Add_Function(&pos_client, POS_BRAKE, "", "")) return -1;

  return 0;
}

int Pos_Reset(){
  return RC_Call(&pos_client, POS_RESET);
}

int Pos_Config_Curve(float z, float w, float vc, float vr, float P, float I, float D, float speed_percent_tolerance){
  return RC_Call(&pos_client, POS_SET_N_POINTS_ASSER, z, w, vc, vr, P, I, D, speed_percent_tolerance);//z w vc vr P I D speed_percent_tolerance
}

int Pos_Init_Position(uint16_t x0, uint16_t y0){
  return RC_Call(&pos_client, POS_SET_ODO, x0, y0);
}

int Pos_Get_Position(){
  return RC_Call(&pos_client, POS_GET_ODO, &me.x, &me.y, &me.angle);
}

int Pos_Go_Forward(float speed, float distance){
  return RC_Call(&pos_client, POS_GO_FORWARD, speed, distance);
}

int Pos_Set_Angle(float speed, float angle){
  return RC_Call(&pos_client, POS_SET_ANGLE, speed, angle);
}

int Pos_Brake(){
  return RC_Call(&pos_client, POS_BRAKE);
}
