#include "fsm_stack.h"

void FSM_Stack_Init(FSM_Instance *fsm){
  fsm->status=FSM_RUNNING;
  FSM_Stack *fsm_stack=(FSM_Stack *) fsm;
  fsm_stack->n = 0;
  fsm->status=FSM_RUNNING;
  pid_z.integral = 0;
  fsm->run=FSM_Pos_3;

  fsm_stack->nramp=0;//ramp ramp_generator initialisation
  fsm_stack->imp_start=encoder.steps;
}

void FSM_Err(FSM_Instance *fsm)
{
  fsm->status=FSM_ERROR;
}

void FSM_Pos_3(FSM_Instance *fsm)
{
  FSM_Stack *fsm_stack=(FSM_Stack *) fsm;
  //imp_goal=P3;
  imp_goal=ramp_generator(P3,fsm_stack->imp_start,&(fsm_stack->nramp),NCYCLEBIG);
  if(reached(&pid_z,P3-encoder.steps))
  {
    pid_z.integral = 0;
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
    fsm_stack->nramp=0;//ramp ramp_generator initialisation
    fsm_stack->imp_start=encoder.steps;
    pid_z.integral = 0;
    if(encoder.steps<PpresenceLim) fsm->run=FSM_Err_Pos_3;//pas de cube
    else fsm->run=FSM_Pos_2;
    fsm_stack->n=0;
  }
}

void FSM_Pos_2(FSM_Instance *fsm)
{
  FSM_Stack *fsm_stack=(FSM_Stack *) fsm;
  //imp_goal=P2;
  imp_goal=ramp_generator(P2,fsm_stack->imp_start,&(fsm_stack->nramp),NCYCLELITTLE);
  if(reached(&pid_z,P2-encoder.steps))
  {
    pid_z.integral = 0;
    fsm->run=FSM_Open;
  }
}

void FSM_Open(FSM_Instance *fsm)
{
  Z_Open_Small();

  if(Z_Is_Open_Small()){
    FSM_Stack *fsm_stack=(FSM_Stack *) fsm;
    fsm_stack->nramp=0;//ramp ramp_generator initialisation
    fsm_stack->imp_start=encoder.steps;
    pid_z.integral = 0;
    FSM_NEXT(fsm, FSM_Go_Down, 1000);
  }
}

void FSM_Go_Down(FSM_Instance *fsm)
{
  FSM_Stack *fsm_stack=(FSM_Stack *) fsm;
  if(FSM_TIMEOUT_REACHED(fsm))
  {
    fsm->run=FSM_Err_Pos_3;
    fsm_stack->nramp=0;//ramp ramp_generator initialisation
    fsm_stack->imp_start=encoder.steps;
  }
  //imp_goal=P0;
  imp_goal=ramp_generator(P0,fsm_stack->imp_start,&(fsm_stack->nramp),NCYCLEBIG);
  if(reached(&pid_z,P0-encoder.steps))
  {
    pid_z.integral = 0;
    FSM_NEXT(fsm, FSM_Close, 500);
  }
}

void FSM_Close(FSM_Instance *fsm)
{
  FSM_Stack *fsm_stack=(FSM_Stack *) fsm;
  uint16_t load_d,load_g;
  AX_Wheel_Direction d;
  if(FSM_TIMEOUT_REACHED(fsm)){
    fsm->run=FSM_Err_Close;
    pid_z.integral = 0;
    fsm_stack->nramp=0;//ramp ramp_generator initialisation
    fsm_stack->imp_start=encoder.steps;
  }
  Z_Close();
  AX_Get_Current_Load(&servo_g, &d, &load_g);
  AX_Get_Current_Load(&servo_d, &d, &load_d);
  if(load_d>AX_CLOSE_LOAD && load_g>AX_CLOSE_LOAD && Z_Is_Closed_Cube()){
    pid_z.integral = 0;
    fsm_stack->nramp=0;//ramp ramp_generator initialisation
    fsm_stack->imp_start=encoder.steps;
    fsm->run = FSM_Stack_Lift;
  }
}

void FSM_Stack_Lift(FSM_Instance *fsm)
{
  FSM_Stack *fsm_stack=(FSM_Stack *) fsm;//TODO modif here
  if(fsm_stack->last)
  {
     imp_goal=ramp_generator(P1,fsm_stack->imp_start,&(fsm_stack->nramp),NCYCLELITTLE);
     if(reached(&pid_z,P1-encoder.steps))
     {
       fsm->run=FSM_End;
     }
  }
  else
  {
    imp_goal=ramp_generator(P3,fsm_stack->imp_start,&(fsm_stack->nramp),NCYCLEBIG);
    if(reached(&pid_z,P3-encoder.steps))
    {
      fsm->run=FSM_End;
    }
  }
}

void FSM_Err_Open(FSM_Instance *fsm)
{
  Z_Open_Small();
  if(Z_Is_Open_Small()){
    pid_z.integral = 0;
    fsm->run = FSM_Err_Pos_3;
  }
}

void FSM_Err_Pos_3(FSM_Instance *fsm)
{
  FSM_Stack *fsm_stack=(FSM_Stack *) fsm;
  //imp_goal=P3;
  imp_goal=ramp_generator(P3,fsm_stack->imp_start,&(fsm_stack->nramp),NCYCLEBIG);
  if(reached(&pid_z,P3-encoder.steps))
  {
    pid_z.integral = 0;
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
  if(load_d>AX_CLOSE_LOAD && load_g>AX_CLOSE_LOAD){
    pid_z.integral = 0;
    fsm->run=FSM_Err;
  }
}

void FSM_End(FSM_Instance *fsm)
{
  fsm->status=FSM_SUCCESS;
}
