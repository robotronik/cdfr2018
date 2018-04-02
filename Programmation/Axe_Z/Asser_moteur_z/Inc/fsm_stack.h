#ifndef __FSM_STACK_H_
#define __FSM_STACK_H_

typedef struct FSM_Stack_S{
  void (*run)(struct FSM_Stack_S*);

  int var;
}FSM_Stack;

void FSM_Stack_Init(FSM_Stack*);

void FSM_Stack_Lift(FSM_Stack*);
void FSM_Stack_Detect_Cube(FSM_Stack*);

#endif
