#ifndef FSM_POSITION_H
#define FSM_POSITION_H

#include "fsm_master.h"
#include "Robotronik_corp_pid.h"

typedef struct FSM_Position_S{
  FSM_Instance instance;
}FSM_Position;

void FSM_Stack_Init(FSM_Instance*);

#endif
