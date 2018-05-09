#include "fsm_arm.h"

void FSM_ARM_OUT(FSM_Instance *fsm)
{
  fsm->status=FSM_RUNNING;
  uint16_t pos_ar;
  AX_Set_Goal_Position(&servo_ar, AX_ARM_DEPLOY, AX_NOW);
  AX_Get_Current_Position(&servo_ar, &pos_ar);
  if(abs(pos_ar-AX_ARM_DEPLOY)<10) fsm->run=FSM_ARM_END;
}

void FSM_ARM_IN(FSM_Instance *fsm)
{
  uint16_t pos_ar;
  AX_Set_Goal_Position(&servo_ar, AX_ARM_START, AX_NOW);
  AX_Get_Current_Position(&servo_ar, &pos_ar);
  if(abs(pos_ar-AX_ARM_START)<10) fsm->run=FSM_ARM_END;
}

void FSM_ARM_END(FSM_Instance *fsm)
{
  fsm->status=FSM_SUCCESS;
}
