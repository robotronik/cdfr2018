#ifndef _H__INTERPOL
#define _H__INTERPOL

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

#endif
