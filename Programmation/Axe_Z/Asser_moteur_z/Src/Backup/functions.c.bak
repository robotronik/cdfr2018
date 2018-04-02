#include "functions.h"

extern RP_Interface Z_interface;
extern RC_Server Z_server;

extern uint8_t punch_bee_order;
void punch_bee(RC_Server* pserver){
  uint8_t i;
  RC_Server_Get_Args(pserver,&i);
  punch_bee_order = i;
  RC_Server_Return(pserver);
}
