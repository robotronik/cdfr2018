#ifndef FSM_START_H
#define FSM_START_H

#include "fsm_master.h"

typedef struct FSM_Start_S{
  FSM_Instance instance;
}FSM_Start;

void FSM_Start_Init(FSM_Instance *fsm);

void FSM_START_WAIT_AX(FSM_Instance *fsm);
void FSM_START_ACTIVATE(FSM_Instance *fsm);
void FSM_START_WAIT_GOAL(FSM_Instance *fsm);

#endif
