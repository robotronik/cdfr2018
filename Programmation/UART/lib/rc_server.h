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

#define RC_NB_FUNCTIONS 256
#define RC_MAX_RANGE RC_NB_FUNCTIONS-1
#define RC_FMT_SIZE 10

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

typedef enum RC_Error_E{
  RC_BAD_ID,
  RC_WRONG_FORMAT,
}RC_Error;

typedef enum RC_Call_Type_E{
  RC_IMMEDIATE, RC_DELAYED
}RC_Call_Type;

typedef enum RC_Type_E{
  RC_UINT8_T = 'b', //uint8_t
  RC_UINT16_T = 'B', //uint16_t
  RC_UINT32_T = 'u', //uint32_t
  RC_INT = 'i', //int
  RC_FLOAT = 'f', //float
  RC_DOUBLE = 'F', //double
  RC_STRING = 's' //string
}RC_Type;

typedef struct RC_Function_S{
  //Address of the function
  void (*call)(uint8_t id, uint8_t *data, uint8_t len);
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
/**
 * Init the server, ie. set all functions to undefined.
 */

int RC_Server_Add_Function(RC_Server *server,
			   int id,
			   void (*function)(uint8_t, uint8_t*, uint8_t),
			   const char params_fmt[],
			   const char return_fmt[],
			   RC_Call_Type call_type);
/**
 * Initializes the function n°id of the server with the given
 * informations. Returns 0 on success, -1 on error.  On error,
 * RC_Get_Error() can give additionnal informations.
 */

//==================================================
//              ERRORS
//==================================================

int RC_Get_Error();

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
