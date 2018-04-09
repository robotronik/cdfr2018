#include "server.h"

FSM_Instance *volatile fsm;
FSM_Position_Pts fsm_pos_pts;
FSM_Position_Abs fsm_pos_abs;
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

void set_2_points(RC_Server* pserver)//5 parameters
{
  //fsm_position_pts.instance.run=;
  fsm = (FSM_Instance*volatile) &fsm_pos_pts;
  RC_Server_Return(pserver);
}

void set_pos(RC_Server* pserver) //2 parameters
{
  RC_Server_Get_Args(pserver,
         &fsm_pos_abs.linear_speed,
         &fsm_pos_abs.pos);
  fsm_pos_pts.instance.run=FSM_Pos_Init;
  fsm = (FSM_Instance*volatile) &fsm_pos_abs;
  RC_Server_Return(pserver);
}

void set_angle(RC_Server* pserver)//2 parameters
{
  RC_Server_Get_Args(pserver,
         &fsm_pos_abs.angular_speed,
         &fsm_pos_abs.angle);
  fsm_pos_pts.instance.run=FSM_Angle_Init;
  fsm = (FSM_Instance*volatile) &fsm_pos_abs;
  RC_Server_Return(pserver);
}
