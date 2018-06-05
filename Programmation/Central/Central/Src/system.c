#include "system.h"
#include "position_client.h"
#include "gas.h"
#include "tof.h"
#include <stdio.h>
#include <inttypes.h>
#include <stdbool.h>
#include <stdlib.h>

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

  //Enable timer for end of match
  HAL_TIM_Base_Start_IT(&htim2);
  //Enable the polling timer for ToF
  HAL_TIM_Base_Start_IT(&htim3);
  
  PI_Log("GAS !");
 
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
  while(1){
    Brake();
    HAL_Delay(500);
  }
  
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


void Print_Position(){
  PI_Log("(%" PRIu16 ", %" PRIu16 ") %f\n", me.x, me.y, me.angle);
}

void Test_Odometry(){
  while(1){
    Print_Position();
    HAL_Delay(500);
  }
}

void Test_Line(){
  while(1){
    Pos_Go_Forward(100, 500);
    HAL_Delay(10000);
    Print_Position();
    Pos_Go_Forward(100, -500);
    HAL_Delay(10000);
    Print_Position();
  }
}

void Test_Rotation(){
  while(1){
    Print_Position();
    Pos_Set_Angle(1, 3.14/2);
    HAL_Delay(5000);
    
    Print_Position();
    Pos_Set_Angle(1, 0);
    HAL_Delay(5000);
    
    Print_Position();
    Pos_Set_Angle(1, -3.14/2);
    HAL_Delay(5000);

    Print_Position();
    Pos_Set_Angle(1, 0);
    HAL_Delay(5000);
  }
}
