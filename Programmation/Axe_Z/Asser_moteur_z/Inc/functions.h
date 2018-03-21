#ifndef __functions_H
#define __functions_H

#include "robotronik_protocol.h"
#include "robotronik_protocol_stm32f3.h"
#include "remote_call.h"

typedef enum Z_Functions_E{
  PUNCH_BEE
}Z_Functions;

void punch_bee(RC_Server* pserver);

#endif
