/*
 * Library for Cdfr2018 @Robotronik
 * You may freely rage with this software
 */
#ifndef __Robotronik_corp_Z_axis_H
#define __Robotronik_corp_Z_axis_H

#define P0 -4000
#define P1 -4050
#define P2 -3000
#define P3 -3050
#define Ppresence P2-20
#define PpresenceLim P2-10

#define PWM_MAX 100//max 255
#define VOLTAGE_FC 2

int cube_present();

#define MOTOR_INIT \
  TIM_OC_InitTypeDef sConfigOC;\
  sConfigOC.OCMode = TIM_OCMODE_PWM1;\
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;\
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;\
  HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_4); \
  HAL_GPIO_WritePin(EN_GPIO_Port, EN_Pin,1); \
  HAL_GPIO_WritePin(INV_GPIO_Port, INV_Pin,0); \
  HAL_GPIO_WritePin(D1_GPIO_Port, D1_Pin,0); \

//if positive, goes up
#define MOTOR_VOLTAGE(voltage){\
    uint16_t value;\
    if(voltage > 0){\
      HAL_GPIO_WritePin(IN1_GPIO_Port, IN1_Pin,0);\
      HAL_GPIO_WritePin(IN2_GPIO_Port, IN2_Pin,1);\
      value = (uint16_t) (voltage * (255.0 / 12.0));\
    }\
    else{\
      HAL_GPIO_WritePin(IN1_GPIO_Port, IN1_Pin,1);\
      HAL_GPIO_WritePin(IN2_GPIO_Port, IN2_Pin,0);\
      value = (uint16_t) (-voltage * (255.0 / 12.0));\
    }\
    \
    if(value>PWM_MAX) value=PWM_MAX;\
    sConfigOC.Pulse = value;\
    HAL_TIM_PWM_ConfigChannel(&htim3, &sConfigOC, TIM_CHANNEL_4);\
    HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_4);\
}

#define MOTOR_FC {\
  int i=0;\
  MOTOR_VOLTAGE(VOLTAGE_FC);\
  while(HAL_GPIO_ReadPin(FC_GPIO_Port,FC_Pin)==0) i++;\
  MOTOR_VOLTAGE(0);\
}

#endif
