#include "fsm_stack.h"

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
  if(fsm->n>MAX_PERIOD_DETECT_CUBE)
  {
    if(encoder.steps<PpresenceLim) fsm->run=FSM_Err_Pos_3;//pas de cube
    else fsm->run=FSM_Pos_2;
    fsm->n=0;
  }
}

void FSM_Pos_2(FSM_Stack *fsm)
{
  imp_goal=P2;
  if(reached(&pid_z,imp_goal-encoder.steps))
  {
      fsm->run=FSM_Open;
  }
}

void FSM_Open(FSM_Stack *fsm)
{
  Z_Open();

if(Z_Is_Open())
    FSM_STACK_NEXT(fsm, FSM_Go_Down, 1000);
}

void FSM_Go_Down(FSM_Stack *fsm)
{
  if(FSM_STACK_TIMEOUT_REACHED(fsm)) fsm->run=FSM_Err_Pos_3;
  imp_goal=P0;
  if(reached(&pid_z,imp_goal-encoder.steps))
  {
      FSM_STACK_NEXT(fsm, FSM_Close, 500);
  }
}

void FSM_Close(FSM_Stack *fsm)
{
  uint16_t load_d,load_g;
  AX_Wheel_Direction d;
  if(FSM_STACK_TIMEOUT_REACHED(fsm)) fsm->run=FSM_Err_Close;
  Z_Close();
  AX_Get_Current_Load(&servo_g, &d, &load_g);
  AX_Get_Current_Load(&servo_d, &d, &load_d);
  if(load_d>AX_CLOSE_LOAD && load_g>AX_CLOSE_LOAD) fsm->run=FSM_Go_Down;
  fsm->run=FSM_Err_Open;//TODO time out
}

void FSM_Stack_Lift(FSM_Stack *fsm)
{
  imp_goal=P3;
  if(encoder.steps>P1)
  {
      fsm->status=FSM_SUCCESS;
  }
}

void FSM_Err_Open(FSM_Stack *fsm)
{
  Z_Open();
  if(Z_Is_Open())
    fsm->run = FSM_Err_Pos_3;
}

void FSM_Err_Pos_3(FSM_Stack *fsm)
{
  imp_goal=P3;
  if(reached(&pid_z,imp_goal-encoder.steps))
  {
      fsm->run=FSM_Err_Close;
  }
}

void FSM_Err_Close(FSM_Stack *fsm)
{
  uint16_t load_d,load_g;
  AX_Wheel_Direction d;
  Z_Close();
  AX_Get_Current_Load(&servo_g, &d, &load_g);
  AX_Get_Current_Load(&servo_d, &d, &load_d);
  if(load_d>AX_CLOSE_LOAD && load_g>AX_CLOSE_LOAD) fsm->run=FSM_Err;
}
