#ifndef __SERVER_H_
#define __SERVER_H_

#include "robotronik_protocol.h"
#include "robotronik_protocol_stm32f3.h"
#include "remote_call.h"
#include "stm32f3xx_it.h"
#include "Robotronik_corp_pid.h"
#include "fsm_master.h"


typedef enum P_Functions_E{
  P_GET_STATE,
  P_RESET,
  P_SET_ASSER_SUM,
  P_SET_ASSER_DIFF,
  P_BALEC
}P_Functions;

typedef enum P_States_E{
  P_READY,
  P_BUSY,
  P_ERROR
}P_States;

void reset(RC_Server* pserver);
void get_state(RC_Server* pserver);
void reset(RC_Server* pserver);
void set_asser_sum(RC_Server* pserver);
void set_asser_diff(RC_Server* pserver);
void balec(RC_Server* pserver);

#endif
