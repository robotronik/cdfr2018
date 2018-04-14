#include "interpol.h"

#define PI 3.14159265358979323846//TODO remove this

void interpol_calc(Interpol *data)
{
  int i,j,n=data->n,n1=n-1;
  float tfact,t,tpow;

  data->r_x[0]=data->x[0];
  data->r_y[0]=data->y[0];//to avoid a division by 0
  for(i=1;i<RESULT_LENGTH;i++)
  {
    t=(float)i/(RESULT_LENGTH-1);
    tfact=(1-t)/t;
    data->r_x[i]=0;
    data->r_y[i]=0;
    tpow=pow(t,n-1);
    for(j=0;j<n;j++)
    {
      data->r_x[i]+=tpow*data->x[j];
      data->r_y[i]+=tpow*data->y[j];
      tpow*=tfact*(n1-j)/(j+1);
    }
  }
}

float d2(float x1,float y1,float x2, float y2)
{
  float dx=x1-x2,dy=y1-y2;
  return dx*dx+dy*dy;
}

float Kc(Interpol *data,float z,float w,float robot_x,float robot_y, float robot_theta,int *iret,float *thedes,float *flexio)//TODO on F3 global variables
{
  float dmin=d2(robot_x,robot_y,data->r_x[0],data->r_y[0]),d,dx,dy,dx2,dy2,theta_des,flexion,theta_e,k1=w*w,k2=2*z*w;
  int i,imin=0;

  for(i=1;i<RESULT_LENGTH;i++)//minimal distance calculation
  {
    dx=robot_x-data->r_x[i];
    dy=robot_y-data->r_y[i];
    d=dx*dx+dy*dy;
    if(d<dmin)
    {
      dmin=d;
      imin=i;
    }
  }

  dx=(data->r_x[imin+1]-data->r_x[imin-1])/2;//angle of the curve
  dy=(data->r_y[imin+1]-data->r_y[imin-1])/2;
  if(dx==0) theta_des=0;
  else theta_des=atan(dy/dx);

  dx2=(data->r_x[imin+1]+data->r_x[imin-1]-2*data->r_x[imin])/4;
  dy2=(data->r_y[imin+1]+data->r_y[imin-1]-2*data->r_y[imin])/4;
  flexion=(dx*dy2-dy*dx2)/pow(sqrt(dx*dx+dy*dy),3);//flexion of the curve

  theta_e=robot_theta-theta_des;
  if(theta_e>PI) theta_e=theta_e-2*PI;
  else if(theta_e<=-PI) theta_e=theta_e+2*PI;


  *iret=imin;//TODO remove this
  *thedes=theta_des;
  *flexio=flexion;

  return flexion-k1*sqrt(dmin)-k2*theta_e;
}
