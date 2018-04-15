#include "interpol.h"

extern volatile int ENCODER_DIST;//distance between encoders
extern volatile int ENCODER_STEP_DIST;//distance for 1 encoder step/2
extern volatile int deltaL;

extern Odometry odometry;

void interpol_calc(Interpol *data)
{
  int i=0,j,n=data->n,n1=n-1;
  float tfact,t,tpow;

  for(i=0;i<RESULT_LENGTH-1;i++)
  {
    t=(float)i/(RESULT_LENGTH-1);
    tfact=t/(1-t);
    data->r_x[i]=0;
    data->r_y[i]=0;
    tpow=pow(1-t,n-1);
    for(j=0;j<n;j++)
    {
      data->r_x[i]+=tpow*data->x[j];
      data->r_y[i]+=tpow*data->y[j];
      tpow*=tfact*(n1-j)/(j+1);
    }
  }
  data->r_x[RESULT_LENGTH-1]=data->x[n-1];
  data->r_y[RESULT_LENGTH-1]=data->y[n-1];//to avoid a division by 0
}

float d2(float x1,float y1,float x2, float y2)
{
  float dx=x1-x2,dy=y1-y2;
  return dx*dx+dy*dy;
}

float Kc(Interpol *data,float z,float w,float *speed_percent)
{
  float robot_x=odometry.x,robot_y=odometry.y,robot_theta=odometry.theta;

  float dmin=d2(robot_x,robot_y,data->r_x[0],data->r_y[0]),d,dx,dy,dx2,dy2,theta_des,flexion,theta_e,k1=w*w,k2=2*z*w,completion;
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

  completion=(float)imin/(RESULT_LENGTH-1);//speed generator
  if(completion<0.333333) *speed_percent=0.05+(completion/0.333333)*(1-0.05);
  else if(completion<0.666667) *speed_percent=1;
  else *speed_percent=(1-completion)/(1-0.666667);

  if(imin==0) imin=1;//to avoid errors during derivation operations
  if(imin==RESULT_LENGTH-1) imin=RESULT_LENGTH-2;


  dx=(data->r_x[imin+1]-data->r_x[imin-1])/2;//angle of the curve
  dy=(data->r_y[imin+1]-data->r_y[imin-1])/2;
  if(dx==0) theta_des=PI/2;
  else theta_des=atan(dy/dx);

  dx2=(data->r_x[imin+1]+data->r_x[imin-1]-2*data->r_x[imin])/4;
  dy2=(data->r_y[imin+1]+data->r_y[imin-1]-2*data->r_y[imin])/4;
  flexion=(dx*dy2-dy*dx2)/pow(sqrt(dx*dx+dy*dy),3);//flexion of the curve

  if(robot_theta>PI/2)
  {
    robot_theta=robot_theta-PI;//keep the direction
  }
  else if(robot_theta<-PI/2)
  {
    robot_theta=robot_theta+PI;//keep the direction
  }

  theta_e=robot_theta-theta_des;//limits the robot angle to -PI/2 PI/2
  if(theta_e>PI) theta_e=theta_e-2*PI;
  else if(theta_e<=-PI) theta_e=theta_e+2*PI;

  return flexion-k1*sqrt(dmin)-k2*theta_e;
}
