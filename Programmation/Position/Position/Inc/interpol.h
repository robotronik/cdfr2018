#ifndef _H__INTERPOL
#define _H__INTERPOL

#include "odometry.h"

#define TAB_POINT_LENGTH 100
#define RESULT_LENGTH 250
#define STACK_SIZE TAB_POINT_LENGTH

typedef struct Point_S{
  uint16_t x;
  uint16_t y;
}Point;

typedef struct Stack_S{
  Point data[STACK_SIZE];
  uint8_t start;
  uint8_t size;
}Stack;

void Init_Stack(Stack *s);
int Stack_Point(uint16_t x, uint16_t y, Stack *s);
int Unstack_Point(uint16_t *x, uint16_t *y, Stack *s);

typedef struct Interp_S{
  float x[TAB_POINT_LENGTH];
  float y[TAB_POINT_LENGTH];
  int n;//number of points
  float r_x[RESULT_LENGTH];
  float r_y[RESULT_LENGTH];
}Interpol;

void interpol_calc(Interpol *data);
float Kc(Interpol *data,float z,float w,float *speed_percent);

#endif
