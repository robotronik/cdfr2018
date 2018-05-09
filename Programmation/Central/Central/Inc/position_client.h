#ifndef POSITION_CLIENT_H
#define POSITION_CLIENT_H

#include <stdint.h>
#include "map.h"

#define POS_Z 1.
#define POS_W 1.
#define POS_P 1.
#define POS_I 1.
#define POS_D 1.
#define POS_SPEED_PERCENT_TOLERANCE 1.

typedef enum Position_State_E{
  POS_RUNNING, POS_SUCCESS, POS_ERROR
}Position_State;

typedef enum Pos_Client_Function_E{
  POS_GET_STATE,
  POS_RESET,
  POS_SET_ASSER_SUM,
  POS_SET_ASSER_DIFF,
  POS_SET_ODO,
  POS_GET_ODO,
  POS_BALEC,
  POS_GO_FORWARD,
  POS_SET_ANGLE,
  POS_BRAKE,
  POS_SET_POSITION_X_Y,
  POS_INIT_PATH,
  POS_ADD_POINT,
  POS_FOLLOW_PATH,
  POS_CONFIG_CURVE
}Pos_Client_Function;

int Position_Init();
int Position_Init_Odometry();
int Pos_Reset();
int Pos_Config_Curve(float z, float w, float vc, float vr, float P, float I, float D, float speed_percent_tolerance);
int Pos_Send_Path(Cell *path_end, uint16_t x_goal, uint16_t y_goal);
int Pos_Init_Position(uint16_t x0, uint16_t y0);
int Pos_Get_Position();
int Pos_Go_Forward(float speed, float distance);
int Pos_Set_Angle(float speed, float angle);
int Pos_Get_State(Position_State *state);
int Pos_Brake();

#endif
