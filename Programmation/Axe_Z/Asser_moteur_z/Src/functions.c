#include "functions.h"

extern RP_Interface Z_interface;
extern RC_Server Z_server;

extern uint8_t punch_bee_order;
void punch_bee(){
  uint8_t i;
  RC_Server_Get_Args(&Z_server,&i);
  punch_bee_order = i;
}
