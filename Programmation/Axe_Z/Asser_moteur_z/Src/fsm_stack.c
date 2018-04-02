#include "fsm_stack.h"

void FSM_Stack_Init(FSM_Stack *fsm){
  fsm->var = 0;
}

void FSM_Stack_LIFT(FSM_Stack *fsm){
  if(fsm->var){
    fsm->run = FSM_Stack_LIFT;
  }
}
