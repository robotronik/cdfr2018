#ifndef __SERVER_H_
#define __SERVER_H_

#include "robotronik_protocol.h"
#include "robotronik_protocol_stm32f3.h"
#include "remote_call.h"
#include "stm32f3xx_it.h"
#include "Robotronik_corp_pid.h"
#include "fsm_master.h"


typedef enum Z_Functions_E{
  Z_GET_STATE,
  Z_RESET,
  Z_SET_ASSER_SUM,
  Z_SET_ASSER_DIFF,
  Z_BALEC
}Z_Functions;

typedef enum Z_States_E{
  Z_READY,
  Z_BUSY,
  Z_ERROR
}Z_States;

void reset(RC_Server* pserver);
void get_state(RC_Server* pserver);
void reset(RC_Server* pserver);
void set_asser_sum(RC_Server* pserver);
void set_asser_diff(RC_Server* pserver);
void balec(RC_Server* pserver);

#endif
