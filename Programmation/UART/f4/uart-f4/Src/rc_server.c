#include "rc_server.h"

void RC_Server_Init(RC_Server *server){
  int i;
  
  for(i = 0; i < RC_NB_FUNCTIONS; i++){
    server->functions[i].call = 0;
  }  
}

void RC_Server_Add_Function(RC_Server *server, uint8_t id, void (*function)(uint8_t*), uint32_t data_size, RC_Type type){
  RC_Function *fun = &server->functions[id];
  fun->call = function;
  fun->data_size = data_size;
  fun->type = type;
}

RC_Status RC_Get_Request(RC_Server *server, uint8_t id, uint8_t *data, uint32_t data_size){
  RC_Function *const fun = &server->functions[id];
  
  if(id > RC_MAX_RANGE || fun->call == 0){
    return RC_UNDEFINED_FUNCTION;
  }
  
  if(data_size != fun->data_size){
    return RC_BAD_SIZE;
  }

  switch(fun->type){
  case RC_IMMEDIATE:
    fun->call(data);
    break;
  case RC_DELAYED:
    server->request.function = &server->functions[id];
    server->request.data = data;
    server->pending = 1;
    break;
  default:
    break;
  }
  
  return RC_SUCCESS;
}

uint8_t RC_Poll(RC_Server *server){
  if(!server->pending){
    return 0;
  }

  server->pending = 0;
  server->request.function->call(server->request.data);
  
  return 1;
}
