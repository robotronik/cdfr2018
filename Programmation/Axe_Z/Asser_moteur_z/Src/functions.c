#include "functions.h"

extern RP_Interface Z_interface;
extern RC_Server Z_server;

uint8_t global_state;
uint8_t punch_bee_order;

void reset(RC_Server* pserver)
{
  HAL_NVIC_SystemReset();
}

void set_asser(RC_Server* pserver)
{

  RC_Server_Return(pserver);
}

void punch_bee(RC_Server* pserver){
  uint8_t i;
  RC_Server_Get_Args(pserver,&i);
  punch_bee_order = i;
  RC_Server_Return(pserver,global_state);
}
