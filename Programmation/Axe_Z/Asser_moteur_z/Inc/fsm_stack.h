#ifndef __FSM_STACK_H_
#define __FSM_STACK_H_

#include "fsm_master.h"
#include "Z_axis.h"
#include "encoder.h"
#include "Robotronik_corp_pid.h"

typedef struct FSM_Stack_S{
  void (*run)(struct FSM_Stack_S*);
  FSM_Status status;
  uint32_t time_limit;
  int n;
}FSM_Stack;

#define FSM_STACK_SET_TIME_BUDGET(p_instance, t_ms) {p_instance->time_limit = HAL_GetTick() + t_ms;}
#define FSM_STACK_TIMEOUT_REACHED(p_instance) (HAL_GetTick() > p_instance->time_limit)
#define FSM_STACK_NEXT(p_instance, next_state, t_ms) {p_instance->run = next_state; FSM_STACK_SET_TIME_BUDGET(p_instance, t_ms);}

void FSM_Stack_Init(FSM_Stack*);

void FSM_Pos_3(FSM_Stack*);
void FSM_Detect_Cube(FSM_Stack*);
void FSM_Pos_2(FSM_Stack*);
void FSM_Open(FSM_Stack*);
void FSM_Go_Down(FSM_Stack*);
void FSM_Close(FSM_Stack*);
void FSM_Stack_Lift(FSM_Stack*);

void FSM_Err_Open(FSM_Stack*);
void FSM_Err_Pos_3(FSM_Stack*);
void FSM_Err_Close(FSM_Stack*);
void FSM_Err(FSM_Stack*);

#endif
