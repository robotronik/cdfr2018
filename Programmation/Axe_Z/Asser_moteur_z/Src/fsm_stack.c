#include "fsm_stack.h"

extern int imp_goal;
extern Encoder encoder;
extern PID_DATA pid_z;

void FSM_Stack_Init(FSM_Stack *fsm){
  fsm->n = 0;
  fsm->status=FSM_RUNNING;
  fsm->run=FSM_Pos_3;
}

void FSM_Err(FSM_Stack *fsm)
{
  fsm->status=FSM_ERROR;
}

void FSM_Pos_3(FSM_Stack *fsm)
{
  imp_goal=P3;
  if(reached(&pid_z,imp_goal-encoder.steps))
  {
      fsm->run=FSM_Detect_Cube;
  }
}

void FSM_Detect_Cube(FSM_Stack *fsm)
{
  imp_goal=Ppresence;
  fsm->n++;
  if(fsm->n>25)
  {
    if(encoder.steps<PpresenceLim) fsm->run=FSM_Err;//pas de cube
    else fsm->run=FSM_Open;
    fsm->n=0;
  }
}

void FSM_Open(FSM_Stack *fsm)
{
  
}

void FSM_Go_Down(FSM_Stack *fsm)
{

}

void FSM_Close(FSM_Stack *fsm)
{

}

void FSM_Stack_Lift(FSM_Stack *fsm)
{
  fsm->status=FSM_SUCCESS;
}
