#include "interpol.h"

void interpol_calc(Interpol *data)
{
  int i;
  float t1,t,t1t2,t1t1,tt;

  for(i=0;i<RESULT_LENGTH;i++)
  {
    t=(float)i/(RESULT_LENGTH-1);
    t1=1-t;
    tt=t*t;
    t1t1=t1*t1;
    t1t2=2*t*t1;
    data->r_x[i]=t1t1*data->x[0]+t1t2*data->x[1]+tt*data->x[2];
    data->r_y[i]=t1t1*data->y[0]+t1t2*data->y[1]+tt*data->y[2];
  }
}
