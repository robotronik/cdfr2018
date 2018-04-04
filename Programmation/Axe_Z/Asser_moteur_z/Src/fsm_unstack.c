#include "fsm_unstack.h"

void FSM_Unstack_Init(FSM_Instance *fsm)
{
  fsm->status=FSM_RUNNING;
  imp_goal=P0;
  if(reached(&pid_z,imp_goal-encoder.steps))
  {
      fsm->run=FSM_Place_Open;
  }
}

void FSM_Unstack_Open(FSM_Instance *fsm)
{
  Z_Open();
  if(Z_Is_Open())
    fsm->run=FSM_Unstack_P3;
}

void FSM_Unstack_P3(FSM_Instance *fsm)
{
  imp_goal=P3;
  if(reached(&pid_z,imp_goal-encoder.steps))
  {
      fsm->run=FSM_Unstack_Close;
  }
}

void FSM_Unstack_Close(FSM_Instance *fsm)
{
  uint16_t load_d,load_g;
  AX_Wheel_Direction d;
  Z_Close();
  AX_Get_Current_Load(&servo_g, &d, &load_g);
  AX_Get_Current_Load(&servo_d, &d, &load_d);
  if(load_d>AX_CLOSE_LOAD && load_g>AX_CLOSE_LOAD) fsm->run=FSM_Unstack_End;
}

void FSM_Unstack_End(FSM_Instance *fsm)
{
  fsm->status=FSM_SUCCESS;
}
