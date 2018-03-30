#ifndef __PI_CLIENT_H_
#define __PI_CLIENT_H_

#include <stdarg.h>

#include "usart.h"
#include "remote_call.h"

typedef enum Pi_Client_Functions_E{
  PI_START = 0,
  PI_STOP = 1,
  PI_ERROR = 2,
  PI_SCORE = 3,
  PI_PLAN = 4,
  PI_LOG = 5,
  PI_ASSER = 6,
}Pi_Client_Functions;

int PI_Init();
int PI_Start();
int PI_Stop();
int PI_Error();
int PI_Log(const char *fmt, ...);

int PI_Asser_Test();

#endif
