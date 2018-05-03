#ifndef POSITION_CLIENT_H
#define POSITION_CLIENT_H

#include <stdint.h>

int Position_Init();
int Pos_Reset();
int Pos_Config_Curve(float z, float w, float vc, float vr, float P, float I, float D, float speed_percent_tolerance);
int Pos_Init_Position(uint16_t x0, uint16_t y0);
int Pos_Get_Position();
int Pos_Go_Forward(float speed, float distance);
int Pos_Set_Angle(float speed, float angle);
int Pos_Brake();

#endif
