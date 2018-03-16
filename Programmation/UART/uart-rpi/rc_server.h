/**
 * Library Remote Call Server @Robotronik
 * rc_server.h
 * This library is meant to run a remote call server between µC. 
 */
#ifndef __RC_SERVER_H_
#define __RC_SERVER_H_

#include <stdint.h>

#define RC_NB_FUNCTIONS 256
#define RC_MAX_RANGE RC_NB_FUNCTIONS-1

#if RC_NB_FUNCTIONS > 256 || RC_NB_FUNCTIONS < 0
#error "RC_NB_FUNCTIONS must be between 0 and 256"
#endif

typedef enum RC_Type{
  RC_IMMEDIATE, RC_DELAYED
}RC_Type;

typedef struct RC_Function_S{
  RC_Type type;
  void (*call)(uint8_t*);
  uint32_t data_size;
}RC_Function;

typedef struct RC_Request_S{
  RC_Function *function;
  uint8_t *data;
}RC_Request;

typedef struct RC_Server_S{
  RC_Function functions[RC_NB_FUNCTIONS];
  RC_Request request;
  uint8_t pending;
}RC_Server;

//==================================================
//              INIT FUNCTIONS
//==================================================

void RC_Server_Init(RC_Server *server);

void RC_Server_Add_Function(RC_Server *server, uint8_t id, void (*function)(uint8_t*), uint32_t data_size, RC_Type type);

//==================================================
//              ERRORS
//==================================================

typedef enum RC_Status_E{
  RC_SUCCESS,
  RC_UNDEFINED_FUNCTION,
  RC_BAD_SIZE
}RC_Status;

//==================================================
//              CALL
//==================================================

RC_Status RC_Get_Request(RC_Server *server, uint8_t id, uint8_t *data, uint32_t data_size);

uint8_t RC_Poll(RC_Server *server);

#endif
