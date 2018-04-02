#ifndef __functions_H
#define __functions_H

#include "robotronik_protocol.h"
#include "robotronik_protocol_stm32f3.h"
#include "remote_call.h"
#include "stm32f3xx_it.h"
#include "Robotronik_corp_pid.h"

typedef enum Z_Functions_E{
  Z_GET_STATE,
  Z_RESET,
  Z_SET_ASSER,
  Z_BALEC,
  Z_PUNCH_BEE,
  Z_ARM_OUT,
  Z_ARM_IN,
  Z_STACK,
  Z_STACK_LAST,
  Z_UNSTACK,
  Z_PLACE
}Z_Functions;

typedef enum Z_States_E{
  Z_READY,
  Z_BUSY,
  Z_ERROR
}Z_States;

void get_state(RC_Server* pserver);
void reset(RC_Server* pserver);
void set_asser(RC_Server* pserver);
void balec(RC_Server* pserver);
void punch_bee(RC_Server* pserver);
void arm_in(RC_Server* pserver);
void arm_out(RC_Server* pserver);
void stack(RC_Server* pserver);
void stack_last(RC_Server* pserver);
void unstack(RC_Server* pserver);
void place(RC_Server* pserver);

#endif
