#ifndef _H__INTERPOL
#define _H__INTERPOL

#include <math.h>

#define TAB_POINT_LENGTH 400
#define RESULT_LENGTH 1000

typedef struct Interp_S{
  float x[TAB_POINT_LENGTH];
  float y[TAB_POINT_LENGTH];
  int n;//number of points
  float r_x[RESULT_LENGTH];
  float r_y[RESULT_LENGTH];
}Interpol;

void interpol_calc(Interpol *data);
void proj(Interpol *data,float *ptheta_curve,float *pcurve_distance,float *pcurve_flexion,float robot_x,float robot_y, float robot_theta);
float Kc(Interpol *data,float z,float w,float robot_x,float robot_y, float robot_theta,int *iret,float *thedes,float *flexio);

#endif
