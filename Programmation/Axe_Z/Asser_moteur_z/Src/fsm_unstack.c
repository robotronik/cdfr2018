#include "fsm_unstack.h"

void FSM_Unstack_Init(FSM_Instance *fsm)
{
  FSM_Unstack *fsm_unstack=(FSM_Unstack *) fsm;
  fsm_unstack->nramp=0;//ramp ramp_generator initialisation
  fsm_unstack->imp_start=encoder.steps;
  fsm->status=FSM_RUNNING;
  fsm->run=FSM_Unstack_P0;
}

void FSM_Unstack_P0(FSM_Instance *fsm)
{
  FSM_Unstack *fsm_unstack=(FSM_Unstack *) fsm;
  //imp_goal=P0;
  imp_goal=ramp_generator(P0,fsm_unstack->imp_start,&(fsm_unstack->nramp),NCYCLEBIG);
  if(reached(&pid_z,P0-encoder.steps))
  {
      fsm->run=FSM_Unstack_Open;
  }
}

void FSM_Unstack_Open(FSM_Instance *fsm)
{
  FSM_Unstack *fsm_unstack=(FSM_Unstack *) fsm;
  Z_Open_Small();
  if(Z_Is_Open_Small())
  {
    fsm->run=FSM_Unstack_P3;
    fsm_unstack->nramp=0;//ramp ramp_generator initialisation
    fsm_unstack->imp_start=encoder.steps;
  }
}

void FSM_Unstack_P3(FSM_Instance *fsm)
{
  FSM_Unstack *fsm_unstack=(FSM_Unstack *) fsm;
  //imp_goal=P3;
  ramp_generator(P3,fsm_unstack->imp_start,&(fsm_unstack->nramp),NCYCLEBIG);
  if(reached(&pid_z,P3-encoder.steps))
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
