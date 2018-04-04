#include "fsm_place.h"

void FSM_Place_Init(FSM_Instance *fsm)
{
  fsm->status=FSM_RUNNING;
  imp_goal=P0;
  if(reached(&pid_z,imp_goal-encoder.steps))
  {
      fsm->run=FSM_Place_Open;
  }
}

void FSM_Place_Open(FSM_Instance *fsm)
{
  Z_Open();
  if(Z_Is_Open())
    fsm->run=FSM_Place_End;
}

void FSM_Place_End(FSM_Instance *fsm)
{
  fsm->status=FSM_SUCCESS;
}
