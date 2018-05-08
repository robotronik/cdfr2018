#include "system.h"

#define JACK_PLUGGED (HAL_GPIO_ReadPin(JACK_START_GPIO_Port, JACK_START_Pin) == GPIO_PIN_RESET)

Team wait_start(){
  Team team = GREEN_TEAM;
  int i = JACK_FILTER;

  while(--i){
    if(JACK_PLUGGED)
      i = JACK_FILTER;

    team = HAL_GPIO_ReadPin(SELEC_GPIO_Port, SELEC_Pin);
    HAL_GPIO_WritePin(LED_RED_GPIO_Port, LED_RED_Pin, (team==ORANGE_TEAM));
    HAL_GPIO_WritePin(LED_GREEN_GPIO_Port, LED_GREEN_Pin, (team==GREEN_TEAM));

    HAL_Delay(1);
  }

  HAL_TIM_Base_Start_IT(&htim2);
  
  return team;
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

  //Stop PI
  PI_Stop();

  //Halt
  hlt();
}

void hlt(){
  __disable_irq();
  do{}while(1);
}
