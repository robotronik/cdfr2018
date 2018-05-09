#include "pi_client.h"
#include "game_defs.h"
#include "strategy.h"

static RC_Client pi_client;

int PI_Init(){
  RC_Client_Init(&pi_client, &pi_iface, 0);
  
  if(RC_Client_Add_Function(&pi_client, PI_START, "", "")) return -1;
  if(RC_Client_Add_Function(&pi_client, PI_STOP, "", "")) return -1;
  if(RC_Client_Add_Function(&pi_client, PI_ERROR, "", "")) return -1;
  if(RC_Client_Add_Function(&pi_client, PI_SCORE, "B", "")) return -1;
  if(RC_Client_Add_Function(&pi_client, PI_PLAN, "", "bs")) return -1;
  if(RC_Client_Add_Function(&pi_client, PI_LOG, "s", "")) return -1;
  //if(RC_Client_Add_Function(&pi_client, PI_ASSER, "b", "bfffii")) return -1;

  /*
   * RP_Sync is mandatory here : the Raspberry's fsm receiver is in an
   * indeterminate state at program startup.
   */
  RP_Sync(&pi_iface, RC_TRANSFERT_TIMEOUT);
  
  return 0;
}

int PI_Start(){

  
  return RC_Call(&pi_client, PI_START);
}

int PI_Stop(){
  return RC_Call(&pi_client, PI_STOP);
}

int PI_Error(){
  return RC_Call(&pi_client, PI_ERROR);
}

int PI_Score(uint16_t score){
  return RC_Call(&pi_client, PI_SCORE, score);
}
    
int PI_Log(const char *fmt, ...){
  va_list args;
  va_start(args, fmt);
  
  char log[RC_STR_SIZE];
  vsnprintf(log, RC_STR_SIZE, fmt, args);  

  va_end(args);

  return RC_Call(&pi_client, PI_LOG, log);
}

typedef enum CV_State_E{
  CV_RUNNING,
  CV_ERR_TIMEOUT,
  CV_ERR,
  CV_OK
}CV_State;

static inline Cube_Color CV_To_Color(char c){
  switch(c){
  case 'O':
    return ORANGE;
    break;
  case 'B':
    return BLACK;
    break;
  case 'G':
    return GREEN;
    break;
  case 'Y':
    return YELLOW;
    break;
  case 'b':
    return BLUE;
    break;
  default:
    return 0;//Should not happen
  }
}

int PI_Plan(){
  char str[RC_STR_SIZE];
  uint8_t state;
  
  if(RC_Call(&pi_client, PI_PLAN, &state, str) == -1){
    return -1;
  }

  HAL_Delay(1000);
  int retry;
  for(retry = 0; retry < 3; retry++){
    if(RC_Call(&pi_client, PI_PLAN, &state, str) == -1){
      return -1;
    }
    switch((CV_State) state){
    case CV_RUNNING:
      HAL_Delay(1000);
      break;
    case CV_ERR:
      return 0;
      break;
    case CV_ERR_TIMEOUT:
      return -1;
      break;
    case CV_OK:
      Set_Construction_Plan(CV_To_Color(str[0]),
			    CV_To_Color(str[1]),
			    CV_To_Color(str[2]));
      return 1;
      break;
    }
  }

  return -1;
}

/*int PI_Asser_Test(){
  uint8_t ok;
  float Te, Kp, Ki, Kd;
  RC_Call(&pi_client, PI_ASSER, 2, &ok, &Te, &Kp, &Ki, &Kd);

  PI_Log("Kp Axe Z : %lf", Kp);

  return 0;
  }*/
