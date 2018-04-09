#include "fsm_position_abs.h"
//http://manubatbat.free.fr/doc/positionning/node5.html

extern PID_DATA pid_sum;
extern PID_DATA pid_diff;
extern int sum_goal;
extern int diff_goal;
extern Odometry odometry;

extern volatile int ENCODER_DIST;//distance between encoders
extern volatile int ENCODER_STEP_DIST;//distance for 1 encoder step/2
extern volatile int deltaL;

void FSM_Pos_Init(FSM_Instance *fsm)
{
  FSM_Position_Abs *fsm_pos=(FSM_Position_Abs *) fsm;
  fsm->status=FSM_RUNNING;
  fsm_pos->n=0;
  fsm_pos->initial_sum=sum_goal;
  fsm->run=FSM_Pos_Generator;
}

void FSM_Pos_Generator(FSM_Instance *fsm)
{
  FSM_Position_Abs *fsm_pos=(FSM_Position_Abs *) fsm;
  if(fsm_pos->pos/fsm_pos->linear_speed >fsm_pos->n*pid_sum.Te)
  {
    sum_goal=fsm_pos->initial_sum+fsm_pos->n*fsm_pos->linear_speed*pid_sum.Te/ENCODER_STEP_DIST;
    fsm_pos->n++;
  }
  else
  {
    sum_goal=fsm_pos->initial_sum+fsm_pos->pos/ENCODER_STEP_DIST;
    fsm->run=FSM_Pos_Wait;
  }
}

void FSM_Pos_Wait(FSM_Instance *fsm)
{
  if(reached(&pid_sum,sum_goal- 0.5 * (odometry.encoder_l.steps + odometry.encoder_r.steps))) fsm->run=FSM_Abs_End;
}

void FSM_Abs_End(FSM_Instance *fsm)
{
  fsm->status=FSM_SUCCESS;
}

void FSM_Angle_Init(FSM_Instance *fsm)
{
  FSM_Position_Abs *fsm_pos=(FSM_Position_Abs *) fsm;
  fsm->status=FSM_RUNNING;
  fsm_pos->n=0;
  fsm_pos->angle+=odometry.theta;//no value limitation
  fsm->run=FSM_Angle_Generator;
}

void FSM_Angle_Generator(FSM_Instance *fsm)
{
  FSM_Position_Abs *fsm_pos=(FSM_Position_Abs *) fsm;
  if(fsm_pos->angle/fsm_pos->angular_speed >fsm_pos->n*pid_diff.Te)
  {
    diff_goal=fsm_pos->n*fsm_pos->angular_speed*pid_diff.Te*ENCODER_DIST/ENCODER_STEP_DIST;
    fsm_pos->n++;
  }
  else
  {
    diff_goal=fsm_pos->angle*ENCODER_DIST/ENCODER_STEP_DIST;
    fsm->run=FSM_Angle_Wait;
  }
}

void FSM_Angle_Wait(FSM_Instance *fsm)
{
  if(reached(&pid_diff,diff_goal - (odometry.encoder_r.steps - odometry.encoder_l.steps))) fsm->run=FSM_Abs_End;
}
