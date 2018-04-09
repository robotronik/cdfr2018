#include "server.h"

FSM_Instance *volatile fsm;
FSM_Position fsm_pos;
extern volatile int ENCODER_DIST;//distance between encoders
extern volatile int ENCODER_STEP_DIST;//distance for 1 encoder step/2
extern volatile int deltaL;
extern PID_DATA pid_sum;
extern PID_DATA pid_diff;
extern Odometry odometry;

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

void set_odo(RC_Server* pserver)
{
  RC_Server_Get_Args(pserver,
		     &ENCODER_DIST,
		     &ENCODER_STEP_DIST);
  deltaL=ENCODER_STEP_DIST/ENCODER_DIST;
  RC_Server_Return(pserver);
}

void get_odo(RC_Server* pserver)
{
  RC_Server_Return(pserver, odometry.x,odometry.y,odometry.theta);
}
