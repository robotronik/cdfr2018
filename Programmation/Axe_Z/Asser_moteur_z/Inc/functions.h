#ifndef __functions_H
#define __functions_H

#include "robotronik_protocol.h"
#include "robotronik_protocol_stm32f3.h"
#include "remote_call.h"
#include "stm32f3xx_it.h"

typedef enum Z_Functions_E{
  Z_RESET,
  Z_SET_ASSER,
  Z_BALEC,
  Z_PUNCH_BEE,
  Z_ARM_OUT,
  Z_ARM_IN,
  Z_STACK,
  Z_UNSTACK,
  Z_PLACE
}Z_Functions;

typedef enum Z_States_E{
  Z_READY,
  Z_BUSY,
  Z_ERROR
}Z_States;

void reset(RC_Server* pserver);
void set_asser(RC_Server* pserver);
void punch_bee(RC_Server* pserver);

#endif
