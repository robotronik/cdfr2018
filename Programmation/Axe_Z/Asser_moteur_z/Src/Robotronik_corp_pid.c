#include "Robotronik_corp_pid.h"

#define abs(a) ((a >= 0)?a:(-a))

void pid_init(volatile PID_DATA *pid)
{
  pid->prev_eps=0;
  pid->integral=0;
}

float pid(volatile PID_DATA *pid, int eps)
{
  float output;
  pid->integral=pid->integral+pid->Te*eps;
  output=eps*pid->Kp+pid->Ki*pid->integral+pid->Kd*(eps-pid->prev_eps)/pid->Te;
  pid->prev_eps=eps;
  return output;
}

int reached(volatile PID_DATA *pid,int eps)
{
  if(abs(eps)<pid->position_tolerance && (abs(eps-pid->prev_eps))/pid->Te<pid->speed_tolerance) return 1;
  return 0;
}
