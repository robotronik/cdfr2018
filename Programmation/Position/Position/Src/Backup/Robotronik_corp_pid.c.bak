#include "Robotronik_corp_pid.h"

void pid_init(PID_DATA *pid)
{
  pid->prev_eps=0;
  pid->integral=0;
}

float pid(PID_DATA *pid, int eps)
{
  float output;
  pid->integral=pid->integral+pid->Te*eps;
  output=eps*pid->Kp+pid->Ki*pid->integral+pid->Kd*(eps-pid->prev_eps)/pid->Te;
  pid->prev_eps=eps;
  return output;
}
