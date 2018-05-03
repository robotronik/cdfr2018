#include "position_client.h"

static RC_Client pos_client;

typedef enum Pos_Client_Function_E{
  POS_RESET,
  POS_SET_ASSER_SUM,
  POS_SET_ASSER_DIFF,
  POS_SET_N_POINTS_ASSER,
  POS_SET_ODO,
  POS_GET_ODO,
  POS_SET_POS,
  POS_SET_ANGLE,
  POS_SET_POSITION_X_Y,
  POS_SET_N_POINTS,
  POS_GET_N_POINTS,
  POS_GET_STATE,
  POS_BALEC,
  POS_BRAKE
}Pos_Client_Function;

int Position_Init(){
  RC_Client_Init(&pos_client, &pos_iface, 0);
  
  if(RC_Client_Add_Function(&pos_client, POS_RESET, "", "")) return -1;
  if(RC_Client_Add_Function(&pos_client, POS_SET_ASSER_SUM, "fffif", "")) return -1;
  if(RC_Client_Add_Function(&pos_client, POS_SET_ASSER_DIFF, "fffif", "")) return -1;
  if(RC_Client_Add_Function(&pos_client, POS_SET_N_POINTS_ASSER, "ffffffff", "")) return -1;
  if(RC_Client_Add_Function(&pos_client, POS_SET_ODO, "ff", "")) return -1;
  if(RC_Client_Add_Function(&pos_client, POS_GET_ODO, "", "fff")) return -1;
  if(RC_Client_Add_Function(&pos_client, POS_SET_POS, "ff", "")) return -1;
  if(RC_Client_Add_Function(&pos_client, POS_SET_ANGLE, "ff", "")) return -1;
  if(RC_Client_Add_Function(&pos_client, POS_SET_POSITION_X_Y, "ffff", "")) return -1;
  if(RC_Client_Add_Function(&pos_client, POS_SET_N_POINTS, "i", "")) return -1;
  if(RC_Client_Add_Function(&pos_client, POS_GET_N_POINTS, "ff", "")) return -1;
  if(RC_Client_Add_Function(&pos_client, POS_GET_STATE, "", "b")) return -1;
  if(RC_Client_Add_Function(&pos_client, POS_BALEC, "", "b")) return -1;
  if(RC_Client_Add_Function(&pos_client, POS_BRAKE, "", "")) return -1;
}
