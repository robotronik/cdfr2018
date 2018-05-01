#ifndef FSM_UNSTACK_H
#define FSM_UNSTACK_H

#include "fsm_master.h"
#include "Z_axis.h"

typedef struct FSM_Unstack_S{
  FSM_Instance instance;
  int imp_start;
  int nramp;
}FSM_Unstack;

void FSM_Unstack_Init(FSM_Instance *fsm);
void FSM_Unstack_P0(FSM_Instance *fsm);
void FSM_Unstack_Open(FSM_Instance *fsm);
void FSM_Unstack_P3(FSM_Instance *fsm);
void FSM_Unstack_Close(FSM_Instance *fsm);
void FSM_Unstack_End(FSM_Instance *fsm);

#endif
