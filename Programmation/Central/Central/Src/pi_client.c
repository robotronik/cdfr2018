#include "pi_client.h"

static RC_Client pi_client;

int PI_Init(){
  RC_Client_Init(&pi_client, &pi_iface, 0);
  
  if(RC_Client_Add_Function(&pi_client, PI_START, "", "")) return -1;
  if(RC_Client_Add_Function(&pi_client, PI_STOP, "", "")) return -1;
  if(RC_Client_Add_Function(&pi_client, PI_ERROR, "", "")) return -1;
  if(RC_Client_Add_Function(&pi_client, PI_SCORE, "B", "")) return -1;
  if(RC_Client_Add_Function(&pi_client, PI_PLAN, "", "bs")) return -1;
  if(RC_Client_Add_Function(&pi_client, PI_LOG, "s", "")) return -1;
  if(RC_Client_Add_Function(&pi_client, PI_ASSER, "b", "bffff")) return -1;
  
  return 0;
}

int PI_Start(){
  /*
   * RP_Sync is mandatory here : the Raspberry's fsm receiver is in an
   * indeterminate state at program startup.
   */
  RP_Sync(&pi_iface, RC_TRANSFERT_TIMEOUT);
  
  if(RC_Call(&pi_client, PI_START) != 0){
    return -1;
  }
  
  return 0;
}

int PI_Stop(){
  if(RC_Call(&pi_client, PI_STOP) != 0){
    return -1;
  }
  
  return 0;
}

int PI_Error(){
  if(RC_Call(&pi_client, PI_ERROR) != 0){
    return -1;
  }

  return 0;
}

int PI_Log(const char *fmt, ...){
  va_list args;
  va_start(args, fmt);
  
  char log[RC_STR_SIZE];
  vsnprintf(log, RC_STR_SIZE, fmt, args);  

  va_end(args);

  return RC_Call(&pi_client, PI_LOG, log);
}

int PI_Asser_Test(){
  uint8_t ok;
  float Te, Kp, Ki, Kd;
  RC_Call(&pi_client, PI_ASSER, 2, &ok, &Te, &Kp, &Ki, &Kd);

  PI_Log("Kp Axe Z : %lf", Kp);

  return 0;
}
