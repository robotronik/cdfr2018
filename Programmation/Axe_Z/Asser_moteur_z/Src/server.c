#include "server.h"

extern FSM_Stack fsm_stack;
extern FSM_Arm fsm_arm;
extern FSM_Unstack fsm_unstack;
extern FSM_Place fsm_place;

void get_state(RC_Server* pserver)
{
  RC_Server_Return(pserver,fsm->status);
}

void reset(RC_Server* pserver)
{
  RC_Server_Return(pserver);
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
  RC_Server_Return(pserver);
}

void punch_bee(RC_Server* pserver){
  fsm_arm.instance.run=FSM_ARM_OUT;
  fsm = (FSM_Instance*volatile) &fsm_arm;
  RC_Server_Return(pserver);
}

void arm_in(RC_Server* pserver)
{
  __disable_irq();
  RC_Server_Return(pserver);
  AX_Set_Goal_Position(&servo_ar, AX_ARM_START, AX_NOW);
  __enable_irq();
}

void arm_out(RC_Server* pserver)
{
  __disable_irq();
  RC_Server_Return(pserver);
  AX_Set_Goal_Position(&servo_ar, AX_ARM_DEPLOY, AX_NOW);
  __enable_irq();
}

void stack(RC_Server* pserver)
{
  fsm_stack.instance.run=FSM_Stack_Init;
  fsm_stack.last=0;
  fsm = (FSM_Instance*volatile) &fsm_stack;
  RC_Server_Return(pserver);
}

void stack_last(RC_Server* pserver)
{
  fsm_stack.instance.run=FSM_Stack_Init;
  fsm_stack.last=1;
  fsm = (FSM_Instance*volatile) &fsm_stack;
  RC_Server_Return(pserver);
}

void unstack(RC_Server* pserver)
{
  fsm_unstack.instance.run=FSM_Unstack_Init;
  fsm = (FSM_Instance*volatile) &fsm_unstack;
  RC_Server_Return(pserver);
}

void place(RC_Server* pserver)
{
  fsm_place.instance.run=FSM_Place_Init;
  fsm = (FSM_Instance*volatile) &fsm_place;
  RC_Server_Return(pserver);
}
