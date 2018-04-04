#ifndef __FSM_STACK_H_
#define __FSM_STACK_H_

#include "fsm_master.h"
#include "Z_axis.h"
#include "encoder.h"
#include "Robotronik_corp_pid.h"

typedef struct FSM_Stack_S{
  FSM_Instance instance;
  int n;
  int last;
}FSM_Stack;

void FSM_Stack_Init(FSM_Instance*);

void FSM_Pos_3(FSM_Instance*);
void FSM_Detect_Cube(FSM_Instance*);
void FSM_Pos_2(FSM_Instance*);
void FSM_Open(FSM_Instance*);
void FSM_Go_Down(FSM_Instance*);
void FSM_Close(FSM_Instance*);
void FSM_Stack_Lift(FSM_Instance*);

void FSM_Err_Open(FSM_Instance*);
void FSM_Err_Pos_3(FSM_Instance*);
void FSM_Err_Close(FSM_Instance*);
void FSM_Err(FSM_Instance*);
void FSM_End(FSM_Instance*);

#endif
