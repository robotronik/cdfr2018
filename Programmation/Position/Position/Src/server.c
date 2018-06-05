#include "server.h"
#include <math.h>

FSM_Instance *volatile fsm;
FSM_Position_Pts fsm_pos_pts;
FSM_Position_Abs fsm_pos_abs;
extern PID_DATA pid_sum;
extern PID_DATA pid_diff;
extern Odometry odometry;
extern volatile int sum_goal,diff_goal;

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
  uint16_t X0, Y0;
  float Theta0;
  RC_Server_Get_Args(pserver,
		     &X0,
		     &Y0,
         &Theta0);
  odometry.x=(float)X0;
  odometry.y=(float)Y0;
  odometry.theta=Theta0;
  RC_Server_Return(pserver);
}

void get_odo(RC_Server* pserver)
{
  RC_Server_Return(pserver, (uint16_t) odometry.x, (uint16_t) odometry.y, odometry.theta);
}

void go_forward(RC_Server* pserver) //2 parameters
{
  RC_Server_Get_Args(pserver,
         &fsm_pos_abs.linear_speed,
		     &fsm_pos_abs.pos);
  fsm_pos_abs.instance.run=FSM_Pos_Init;
  fsm = (FSM_Instance*volatile) &fsm_pos_abs;
  RC_Server_Return(pserver);
}

void set_angle(RC_Server* pserver)//2 parameters
{
  RC_Server_Get_Args(pserver,
         &fsm_pos_abs.angular_speed,
         &fsm_pos_abs.angle);
  fsm_pos_abs.instance.run=FSM_Angle_Init;
  fsm = (FSM_Instance*volatile) &fsm_pos_abs;
  RC_Server_Return(pserver);
}

void brake(RC_Server* pserver)
{
  RC_Server_Get_Args(pserver);
  fsm_pos_abs.instance.run=FSM_Pos_Wait;
  fsm->status=FSM_RUNNING;
  fsm = (FSM_Instance*volatile) &fsm_pos_abs;
  sum_goal=odometry.encoder_l.steps + odometry.encoder_r.steps;
  diff_goal=odometry.encoder_r.steps - odometry.encoder_l.steps;//to stay in place
  RC_Server_Return(pserver);
}

void set_position_x_y(RC_Server* pserver)
{
  uint16_t x_, y_;
  RC_Server_Get_Args(pserver,
                    &fsm_pos_abs.angular_speed,
                    &fsm_pos_abs.linear_speed,
                    &x_,
                    &y_);
  fsm_pos_abs.x = (float) x_;
  fsm_pos_abs.y = (float) y_;
  fsm_pos_abs.instance.run=FSM_X_Y_Init;
  fsm = (FSM_Instance*volatile) &fsm_pos_abs;
  RC_Server_Return(pserver);
}

void init_path(RC_Server *pserver){
  Init_Stack(&fsm_pos_pts.stack_r);
  RC_Server_Return(pserver);
}

void add_point(RC_Server *pserver){
  uint16_t x, y;
  RC_Server_Get_Args(pserver, &x, &y);
  Stack_Point(x, y, &fsm_pos_pts.stack_r);
  RC_Server_Return(pserver);
}

void follow_path(RC_Server *pserver){
  fsm_pos_pts.points.n = fsm_pos_pts.stack_r.size;

  uint16_t x, y;
  int i;
  for(i = 0; i < TAB_POINT_LENGTH && Unstack_Point(&x, &y, &fsm_pos_pts.stack_r) != -1; i++){
    fsm_pos_pts.points.x[i] = (float) x;
    fsm_pos_pts.points.y[i] = (float) y;
  }

  RC_Server_Return(pserver);

  interpol_calc(&fsm_pos_pts.points);
  pid_speed_init(&fsm_pos_pts.pid_speed_l);
  pid_speed_init(&fsm_pos_pts.pid_speed_r);
  fsm_pos_pts.instance.run=FSM_Pts_Run;
  fsm = (FSM_Instance*volatile) &fsm_pos_pts;//all data received
  fsm->status=FSM_RUNNING;
}

void config_curve(RC_Server *pserver){
  float Kp,Ki,Kd;
  RC_Server_Get_Args(pserver,
                     &fsm_pos_pts.z,
                     &fsm_pos_pts.w,
                     &fsm_pos_pts.vc,
                     &fsm_pos_pts.vr,
                     &Kp,
                     &Ki,
                     &Kd,
                     &fsm_pos_pts.speed_percent_tolerance);
  RC_Server_Return(pserver);
  fsm_pos_pts.pid_speed_l.Kp=Kp;
  fsm_pos_pts.pid_speed_l.Ki=Ki;
  fsm_pos_pts.pid_speed_l.Kd=Kd;
  fsm_pos_pts.pid_speed_r.Kp=Kp;
  fsm_pos_pts.pid_speed_r.Ki=Ki;
  fsm_pos_pts.pid_speed_r.Kd=Kd;
}
