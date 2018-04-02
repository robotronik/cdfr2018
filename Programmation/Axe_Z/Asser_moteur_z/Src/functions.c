#include "functions.h"

extern RP_Interface Z_interface;
extern RC_Server Z_server;
extern PID_DATA pid_z;

volatile uint8_t global_state, punch_bee_order, balec_order, arm_in_order, arm_out_order;

void get_state(RC_Server* pserver)
{
  RC_Server_Return(pserver,global_state);
}

void reset(RC_Server* pserver)
{
  HAL_NVIC_SystemReset();
}

void set_asser(RC_Server* pserver)
{
  RC_Server_Get_Args(pserver,&(pid_z.Kp));
  RC_Server_Get_Args(pserver,&(pid_z.Ki));
  RC_Server_Get_Args(pserver,&(pid_z.Kd));
  RC_Server_Get_Args(pserver,&(pid_z.position_tolerance));
  RC_Server_Get_Args(pserver,&(pid_z.speed_tolerance));
  RC_Server_Return(pserver);
}

void balec(RC_Server* pserver)
{
  balec_order=1;
  RC_Server_Return(pserver);
}

void punch_bee(RC_Server* pserver){
  punch_bee_order = 1;
  RC_Server_Return(pserver);
}

void arm_in(RC_Server* pserver)
{
  arm_in_order=1;
  RC_Server_Return(pserver);
}

void arm_out(RC_Server* pserver)
{
  arm_out_order=1;
  RC_Server_Return(pserver);
}
