#include "server.h"

extern RP_Interface Z_interface;
extern RC_Server Z_server;
extern PID_DATA pid_z;

volatile uint8_t global_state=Z_BUSY, punch_bee_order=0, balec_order=0, arm_in_order=0, arm_out_order=0,stack_order=0,
                  stack_last_order=0,unstack_order=0, place_order=0;

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
  RC_Server_Get_Args(pserver,
		     &pid_z.Kp,
		     &pid_z.Ki,
		     &pid_z.Kd,
		     &pid_z.position_tolerance,
		     &pid_z.speed_tolerance);
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

void stack(RC_Server* pserver)
{
  stack_order=1;
  RC_Server_Return(pserver);
}

void stack_last(RC_Server* pserver)
{
  stack_last_order=1;
  RC_Server_Return(pserver);
}

void unstack(RC_Server* pserver)
{
  unstack_order=1;
  RC_Server_Return(pserver);
}

void place(RC_Server* pserver)
{
  place_order=1;
  RC_Server_Return(pserver);
}
