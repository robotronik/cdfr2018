#include "rc_server.h"

//Error code
static int err = 0;

//Private functions
static int RC_Copy_Format(char *dst, const char fmt[]);

//Macros
#define RC_CHECK_TYPE(type)			\
  ({						\
    int valid = 1;				\
    switch(type){				\
    case RC_UINT8_T: break;			\
    case RC_UINT16_T: break;			\
    case RC_UINT32_T: break;			\
    case RC_INT: break;				\
    case RC_FLOAT: break;			\
    case RC_DOUBLE: break;			\
    case RC_STRING: break;			\
    default: valid = 0; break;			\
    }						\
    valid;					\
  })						\

void RC_Server_Init(RC_Server *server){
  int i;
  
  for(i = 0; i < RC_NB_FUNCTIONS; i++){
    server->functions[i].call = 0;
  }  
}

static int RC_Copy_Format(char *dst, const char fmt[]){
  int i;
  char c;
  
  for(i = 0; i < RC_FMT_SIZE && (c = dst[i] = fmt[i]) != '\0' && RC_CHECK_TYPE(c); i++);
  
  if(c != '\0'){
    err = RC_WRONG_FORMAT;
    return -1;
  }

  return 0;
}

int RC_Server_Add_Function(RC_Server *server,
			   int id,
			   void (*function)(uint8_t, uint8_t*, uint8_t),
			   const char params_fmt[],
			   const char return_fmt[],
			   RC_Call_Type call_type){
  //Check ID range
  if(id < 0 || id >= RC_NB_FUNCTIONS){
    err = RC_BAD_ID;
    return -1;
  }

  //Get function struct by id
  RC_Function *fun = &server->functions[id];

  //Function pointer
  fun->call = function;

  //Params format
  if(RC_Copy_Format(fun->params_fmt, params_fmt) == -1){
    return -1;
  }

  //Return format
  if(RC_Copy_Format(fun->return_fmt, return_fmt) == -1){
    return -1;
  }

  //Call type
  fun->call_type = call_type;

  return 0;
}


#ifdef RC_LITTLE_ENDIAN
#define RC_PACK_VAR_START(var) (uint8_t*) (&var)
#define RC_PACK_VAR_GET_BYTE(ptr) *(ptr++)
#else
#define RC_PACK_VAR_START(var) (uint8_t*) (&var) + (sizeof(var) - 1)
#define RC_PACK_VAR_GET_BYTE(ptr) *(ptr--)
#endif

#define RC_PACK_VAR(args, type, promoted, out, out_len, success)	\
  {									\
    type var = (type) va_arg(args, promoted);				\
    uint8_t *byte  = RC_PACK_VAR_START(var);				\
    int _end = out_len - sizeof(var);					\
    success = (_end >= 0);						\
    									\
    if(success){							\
      for(; out_len > _end; out_len--){					\
	*(out++) = RC_PACK_VAR_GET_BYTE(byte);				\
      }									\
    }									\
  }								
  
#define RC_PACK_STRING(args, out, out_len, success)	\
  {							\
    char *str = va_arg(args, char*);			\
    char c;						\
    /* Copy a character from str to out until the end of str or out is reached */ \
    while(out_len != 0 && (c = (*out++) = *(str++)) != '\0'){	\
      out_len--;					\
    }							\
    /* if the last character was '\0', out_len must be decremented */	\
    (c == '\0')?(out_len--, success = 1):(success = 0);	\
  }						

int RC_Pack_Vars(RC_Server *server, int id, uint8_t *out, int out_len,  ...){
  int out_len_tmp = out_len;
  va_list args;
  va_start(args, out_len);

  //Check ID range
  if(id < 0 || id >= RC_NB_FUNCTIONS){
    err = RC_BAD_ID;
    return -1;
  }

  const char *fmt = server->functions[id].params_fmt;
  char c;
  int success = 1;
  while((c = *(fmt++)) != '\0' && success){
    if(c == RC_UINT8_T){
      RC_PACK_VAR(args, uint8_t, int, out, out_len, success);
    }
    else if(c == RC_UINT16_T){
      RC_PACK_VAR(args, uint16_t, int, out, out_len, success);
    }
    else if(c == RC_UINT32_T){
      RC_PACK_VAR(args, uint32_t, int, out, out_len, success);      
    }
    else if(c == RC_INT){
      RC_PACK_VAR(args, int, int, out, out_len, success);
    }
    else if(c == RC_FLOAT){
      RC_PACK_VAR(args, float, double, out, out_len, success);
    }
    else if(c == RC_DOUBLE){
      RC_PACK_VAR(args, double, double, out, out_len, success);      
    }
    else if(c == RC_STRING){
      RC_PACK_STRING(args, out, out_len, success);
    }else{
      //Should not happen because format is checked when the function
      //is initialized.
      err = RC_WRONG_FORMAT;
      return -1;
    }
  }

  va_end(args);
  
  return (success)?(out_len_tmp - out_len):-1;
}


#if 0
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

int RC_Get_Error(){
  return err;
}
#endif
