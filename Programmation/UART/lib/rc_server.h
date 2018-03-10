/**
 * Library Remote Call Server @Robotronik
 * rc_server.h
 * This library is meant to run a remote call server between µC. 
 */
#ifndef __RC_SERVER_H_
#define __RC_SERVER_H_

#include <stdlib.h>
#include <stdint.h>
#include <stdarg.h>
#include "robotronik_protocol.h"

#define RC_NB_FUNCTIONS 256
#define RC_MAX_DATA RP_MAX_PACKET_SIZE-1//-1 because one byte is used for id
#define RC_FMT_SIZE 10
#define RC_STR_SIZE 64
#define RC_TRANSFERT_TIMEOUT 1
#define RC_CALL_TIMEOUT 10

//GCC macro
#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
#define RC_LITTLE_ENDIAN
#elif __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__
#define RC_BIG_ENDIAN
#else
#error "RC_SERVER_H : Unsupported endianness"
#endif

#if RC_NB_FUNCTIONS > 256 || RC_NB_FUNCTIONS < 0
#error "RC_NB_FUNCTIONS must be between 0 and 256"
#endif


typedef enum RC_Type_E{
  RC_UINT8_T = 'b', //uint8_t
  RC_UINT16_T = 'B', //uint16_t
  RC_UINT32_T = 'u', //uint32_t
  RC_INT = 'i', //int
  RC_FLOAT = 'f', //float
  RC_DOUBLE = 'F', //double
  RC_STRING = 's' //string
}RC_Type;

typedef enum RC_Call_Type_E{
  RC_IMMEDIATE, RC_DELAYED
}RC_Call_Type;

typedef struct RC_Function_S{
  //Address of the function
  void (*call)(int id, uint8_t *data, int len);
  /*
   * Each function need its id and the data that corresponds to its
   * 'virtual' arguments that are defined according to params_fmt and
   * return_fmt (see below).
   */
  
  //"Virtual" prototype
  char params_fmt[RC_FMT_SIZE];
  char return_fmt[RC_FMT_SIZE];
  /*
   * As the protocol needs a fixed prototype for each function, we
   * need a mechanism to simulate custom prototypes, ie. we must
   * define how data from and for remote functions should be
   * packed/unpacked from link layer frames. This then allows us to
   * have ready to use functions for such operations and prevent the
   * users from having to do it manually for each
   * function. Concretely, we define our virtual prototype as a suite
   * of parameters among authorized types. Packing/unpacking such a
   * suite which is defined on the fly reminds us what printf and
   * scanf are actually doing. We don't want to use these functions
   * directly, because we need a binary-oriented packing, and not
   * text-oriented (doing so would take 3 bytes to write 250 instead
   * of one).
   *
   * This RC lib then has its own format conventions. The format is a
   * suite of authorized characters whose list and meaning are defined
   * in RC_Type enum. Any other character will trigger an error.
   */
  
  RC_Call_Type call_type;
}RC_Function;

//==================================================
//              ERRORS
//==================================================
typedef enum RC_Error_E{
  RC_BAD_ID,
  RC_WRONG_FORMAT,
  RC_UNDEFINED_FUNCTION,
  RC_LINK_ERROR,
  RC_ERR_CALL_TIMEOUT,
  RC_INVALID_RETURN,
}RC_Error;

int RC_Get_Error();

//==================================================//
//                  RC SERVER                       //
//==================================================//

typedef struct RC_Request_S{
  RC_Function *function;
  int id;
  uint8_t *data;
  int len;
}RC_Request;

typedef struct RC_Server_S{
  RP_Interface *interface;
  RC_Function functions[RC_NB_FUNCTIONS];
  RC_Request request;
  uint8_t pending;
}RC_Server;

//Init functions
void RC_Server_Init(RC_Server *server, RP_Interface *interface);
/**
 * Init the server, ie. set all functions to undefined.
 */

int RC_Server_Add_Function(RC_Server *server,
			   int id,
			   void (*function)(int, uint8_t*, int),
			   const char params_fmt[],
			   const char return_fmt[],
			   RC_Call_Type call_type);
/**
 * Initializes the function n°id of the server with the given
 * informations. Returns 0 on success, -1 on error.  On error,
 * RC_Get_Error() can give additionnal informations.
 */

//Request management
int RC_Server_Get_Request(RC_Server *server, int id, uint8_t *data, int len);
int RC_Server_Poll(RC_Server *server);

int RC_Server_Get_Args(RC_Server *server, int id, uint8_t *data, int len, ...);
int RC_Server_Return(RC_Server *server, int id, ...);

//==================================================//
//                  RC CLIENT                       //
//==================================================//

typedef struct RC_Client_S{
  RP_Interface *interface;
  RC_Function functions[RC_NB_FUNCTIONS];
}RC_Client;

void RC_Client_Init(RC_Client *client, RP_Interface *interface);
int RC_Client_Add_Function(RC_Client *server,
			   int id,
			   void (*function)(int, uint8_t*, int),
			   const char params_fmt[],
			   const char return_fmt[]);
int RC_Call(RC_Client *client, int id, ...);

#endif
