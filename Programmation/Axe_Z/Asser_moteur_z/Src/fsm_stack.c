#include "fsm_stack.h"

void FSM_Stack_Init(FSM_Instance *fsm){
  fsm->status=FSM_RUNNING;
  FSM_Stack *fsm_stack=(FSM_Stack *) fsm;
  fsm_stack->n = 0;
  fsm->status=FSM_RUNNING;
  fsm->run=FSM_Pos_3;
}

void FSM_Err(FSM_Instance *fsm)
{
  fsm->status=FSM_ERROR;
}

void FSM_Pos_3(FSM_Instance *fsm)
{
  imp_goal=P3;
  if(reached(&pid_z,imp_goal-encoder.steps))
  {
      fsm->run=FSM_Detect_Cube;
  }
}

void FSM_Detect_Cube(FSM_Instance *fsm)
{
  FSM_Stack *fsm_stack=(FSM_Stack *) fsm;
  imp_goal=Ppresence;
  fsm_stack->n++;
  if(fsm_stack->n>MAX_PERIOD_DETECT_CUBE)
  {
    if(encoder.steps<PpresenceLim) fsm->run=FSM_Err_Pos_3;//pas de cube
    else fsm->run=FSM_Pos_2;
    fsm_stack->n=0;
  }
}

void FSM_Pos_2(FSM_Instance *fsm)
{
  imp_goal=P2;
  if(reached(&pid_z,imp_goal-encoder.steps))
  {
      fsm->run=FSM_Open;
  }
}

void FSM_Open(FSM_Instance *fsm)
{
  Z_Open();

if(Z_Is_Open())
    FSM_NEXT(fsm, FSM_Go_Down, 1000);
}

void FSM_Go_Down(FSM_Instance *fsm)
{
  if(FSM_TIMEOUT_REACHED(fsm)) fsm->run=FSM_Err_Pos_3;
  imp_goal=P0;
  if(reached(&pid_z,imp_goal-encoder.steps))
  {
      FSM_NEXT(fsm, FSM_Close, 500);
  }
}

void FSM_Close(FSM_Instance *fsm)
{
  uint16_t load_d,load_g;
  AX_Wheel_Direction d;
  if(FSM_TIMEOUT_REACHED(fsm)) fsm->run=FSM_Err_Close;
  Z_Close();
  AX_Get_Current_Load(&servo_g, &d, &load_g);
  AX_Get_Current_Load(&servo_d, &d, &load_d);
  if(load_d>AX_CLOSE_LOAD && load_g>AX_CLOSE_LOAD) fsm->run=FSM_Go_Down;
}

void FSM_Instance_Lift(FSM_Instance *fsm)
{
  FSM_Stack *fsm_stack=(FSM_Stack *) fsm;
  if(fsm_stack->last) imp_goal=P1;
  else imp_goal=P3;
  if(encoder.steps>P1)
  {
      fsm->run=FSM_End;
  }
}

void FSM_Err_Open(FSM_Instance *fsm)
{
  Z_Open();
  if(Z_Is_Open())
    fsm->run = FSM_Err_Pos_3;
}

void FSM_Err_Pos_3(FSM_Instance *fsm)
{
  imp_goal=P3;
  if(reached(&pid_z,imp_goal-encoder.steps))
  {
      fsm->run=FSM_Err_Close;
  }
}

void FSM_Err_Close(FSM_Instance *fsm)
{
  uint16_t load_d,load_g;
  AX_Wheel_Direction d;
  Z_Close();
  AX_Get_Current_Load(&servo_g, &d, &load_g);
  AX_Get_Current_Load(&servo_d, &d, &load_d);
  if(load_d>AX_CLOSE_LOAD && load_g>AX_CLOSE_LOAD) fsm->run=FSM_Err;
}

void FSM_End(FSM_Instance *fsm)
{
  fsm->status=FSM_SUCCESS;
}
