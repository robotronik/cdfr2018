#include "system.h"
#include "position_client.h"
#include "gas.h"
#include "tof.h"

#define JACK_PLUGGED (HAL_GPIO_ReadPin(JACK_START_GPIO_Port, JACK_START_Pin) == GPIO_PIN_RESET)

void wait_start(){
  PI_Log("Waiting for start");
    
  team = GREEN_TEAM;
  int i = JACK_FILTER;

  while(--i){
    if(JACK_PLUGGED)
      i = JACK_FILTER;

    team = HAL_GPIO_ReadPin(SELEC_GPIO_Port, SELEC_Pin);
    HAL_GPIO_WritePin(LED_RED_GPIO_Port, LED_GREEN_Pin, (team==ORANGE_TEAM));
    HAL_GPIO_WritePin(LED_GREEN_GPIO_Port, LED_RED_Pin, (team==GREEN_TEAM));

    HAL_Delay(1);
  }

  PI_Log("GAS !");
  
  //Enable timer for end of match
  HAL_TIM_Base_Start_IT(&htim2);
  //Enable the polling timer for ToF
  HAL_TIM_Base_Start_IT(&htim3);
}

void remote_stop(){
  int i = JACK_FILTER;
  while(--i){
    if(!JACK_PLUGGED){
      i = JACK_FILTER;
    }
  }
  HAL_NVIC_SystemReset();
}

void stop(){
  //Stop match timer
  HAL_TIM_Base_Stop_IT(&htim2);
  HAL_TIM_Base_Stop_IT(&htim3);

  //Stop PI
  PI_Stop();
  Brake();
  
  //Halt
  hlt();
}

void hlt(){
  __disable_irq();
  do{}while(1);
}

extern ToF_Handler tof[NB_TOF];
void Print_ToF(){
  PI_Log("AvG : %d\t", ToF_Get_Last_Range(&tof[TOF_FRONT_LEFT]));
  PI_Log("AvD : %d\t", ToF_Get_Last_Range(&tof[TOF_FRONT_RIGHT]));
  PI_Log("ArG : %d\t", ToF_Get_Last_Range(&tof[TOF_REAR_LEFT]));
  PI_Log("ArD : %d\n", ToF_Get_Last_Range(&tof[TOF_REAR_RIGHT]));
}
