#include "fsm_start.h"

void FSM_Start_Init(FSM_Instance *fsm){
  fsm->run = FSM_START_WAIT_AX;
  fsm->status = FSM_RUNNING;
  FSM_SET_TIME_BUDGET(fsm, 500);
}

void FSM_START_WAIT_AX(FSM_Instance *fsm){
  if(FSM_TIMEOUT_REACHED(fsm)){
    err = ERR_AX_LINK;
    fsm->status = FSM_ERROR;
    fsm->run = FSM_NOP;
  }

  if(Z_Check_AX() == 0){
    FSM_NEXT(fsm, FSM_START_ACTIVATE, 50);
  }
}

void FSM_START_ACTIVATE(FSM_Instance *fsm){
  Z_Enable_AX(true);
  Z_Close();
  AX_Set_Goal_Position(&servo_ar, AX_ARM_START, AX_NOW);
  Z_Set_Goal(P0);
  FSM_NEXT(fsm, FSM_START_ACTIVATE, 1000);
}

void FSM_START_WAIT_GOAL(FSM_Instance *fsm){
  if(FSM_TIMEOUT_REACHED(fsm)){
    err = ERR_Z_GOAL;
    fsm->status = FSM_ERROR;
    fsm->run = FSM_NOP;
  }

  if(Z_Goal_Reached()){
    fsm->status = FSM_SUCCESS;
    fsm->run = FSM_NOP;
  }
}
