#include "fsm_stack.h"

#define abs(a) ((a >= 0)?a:(-a))

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
  uint16_t pos_d,pos_g;
  AX_Set_Goal_Position(&servo_g, 430, AX_NOW);
  AX_Set_Goal_Position(&servo_d, 590, AX_NOW);
  AX_Get_Current_Position(&servo_g, &pos_g);
  AX_Get_Current_Position(&servo_d, &pos_d);
  if(abs(pos_g-430)<5 && abs(pos_d-590)<5) fsm->run=FSM_Go_Down;
}

void FSM_Go_Down(FSM_Stack *fsm)
{
  imp_goal=P0;
  if(reached(&pid_z,imp_goal-encoder.steps))
  {
      fsm->run=FSM_Open;
  }
}

void FSM_Close(FSM_Stack *fsm)
{
  uint16_t load_d,load_g;
  AX_Wheel_Direction d;
  AX_Set_Goal_Position(&servo_g, 540, AX_NOW);
  AX_Set_Goal_Position(&servo_d, 480, AX_NOW);
  AX_Get_Current_Load(&servo_g, &d, &load_g);
  AX_Get_Current_Load(&servo_d, &d, &load_d);//TODO check direction
  if(load_d>250 && load_g>250) fsm->run=FSM_Go_Down;//TODO check the value
}

void FSM_Stack_Lift(FSM_Stack *fsm)
{
  imp_goal=P3;
  if(encoder.steps>P1)
  {
      fsm->status=FSM_SUCCESS;
  }
}
