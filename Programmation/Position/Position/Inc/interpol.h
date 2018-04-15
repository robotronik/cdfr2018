#ifndef _H__INTERPOL
#define _H__INTERPOL

#include "odometry.h"

#define TAB_POINT_LENGTH 100
#define RESULT_LENGTH 250

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
