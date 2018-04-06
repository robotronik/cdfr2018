#include "server.h"

FSM_Instance *volatile fsm;
extern PID_DATA pid_sum;
extern PID_DATA pid_diff;

void get_state(RC_Server* pserver)
{
  RC_Server_Return(pserver,fsm->status);
}

void reset(RC_Server* pserver)
{
  RC_Server_Return(pserver);
  HAL_NVIC_SystemReset();
}

void set_asser_sum(RC_Server* pserver)
{
  RC_Server_Get_Args(pserver,
		     &pid_sum.Kp,
		     &pid_sum.Ki,
		     &pid_sum.Kd,
		     &pid_sum.position_tolerance,
		     &pid_sum.speed_tolerance);
  RC_Server_Return(pserver);
}

void set_asser_diff(RC_Server* pserver)
{
  RC_Server_Get_Args(pserver,
		     &pid_diff.Kp,
		     &pid_diff.Ki,
		     &pid_diff.Kd,
		     &pid_diff.position_tolerance,
		     &pid_diff.speed_tolerance);
  RC_Server_Return(pserver);
}

void balec(RC_Server* pserver)
{
  RC_Server_Return(pserver);
}
