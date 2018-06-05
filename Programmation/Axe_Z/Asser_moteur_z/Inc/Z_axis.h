/*
 * Library for Cdfr2018 @Robotronik
 * You may freely rage with this software
 */
#ifndef Z_AXIS_H
#define Z_AXIS_H

#include <stdbool.h>
#include <stdlib.h>
#include "stm32f3xx_hal.h" 
#include "ax_12a.h"
#include "ax_12a_hal.h"
#include "Robotronik_corp_pid.h"
#include "fsm_master.h"
#include "encoder.h"
#include "fsm_stack.h"
#include "fsm_arm.h"
#include "fsm_unstack.h"
#include "fsm_place.h"

/*** Global variables ***/
extern Encoder encoder;
extern volatile int imp_goal;
extern volatile PID_DATA pid_z;
extern AX servo_ar, servo_g, servo_d;
extern int err;
FSM_Instance *volatile fsm;

#define Z_DELAY 10

/*** SERVOS ***/
#define AX_ARM_ID 1
#define AX_LEFT_ID 2
#define AX_RIGHT_ID 3

#define AX_LEFT_CLOSE 540
#define AX_RIGHT_CLOSE 480

#define AX_LEFT_OPEN 430
#define AX_RIGHT_OPEN 590

#define AX_LEFT_OPEN_SMALL 500
#define AX_RIGHT_OPEN_SMALL 520

#define AX_CLOSE_LOAD 100

#define AX_ARM_START 270
#define AX_ARM_DEPLOY 525
#define AX_ARM_END 680

#define AX_EPSILON 10

#define MAX_PERIOD_DETECT_CUBE 25



/*** Functions ***/
void Z_Init_AX();
/**
 * Init the servos.
 */

int Z_Check_AX();
/**
 * Check if the servos are connected and ready to use. Returns 0 if
 * servos are ready, -1 otherwise.
 */

void Z_Enable_AX(bool enable);
/**
 * If enable == true, turns the servomotors on, otherwise turns them
 * off.
 */

void Z_Open();
void Z_Open_Small();
/**
 * Opens the pliers.
 */

int Z_Is_Open();
int Z_Is_Open_Small();
/**
 * Return 1 if the pliers is open, 0 if it's not, -1 on error.
 */

void Z_Close();
/**
 * Close the pliers.
 */

int Z_Is_Closed();
int Z_Is_Closed_Cube();
/**
 * Return 1 if the pliers is closed, 0 if it's not, -1 on error.
 */

void Z_Set_Goal(int goal);

int Z_Goal_Reached();

int ramp_generator(int goal, int start, int *nactual, int ntime);

/*** PID ***/
#define P0 32000 //Position basse
#define P1 28000 //Rouler
#define P2 2000  //Au dessus du cube
#define P3 1800 //Max
#define Ppresence P2+200
#define PpresenceLim P2+100

#define NCYCLEBIG 20
#define NCYCLELITTLE 10

#define PWM_MAX 70//max 255
#define VOLTAGE_FC 6

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
      HAL_GPIO_WritePin(IN1_GPIO_Port, IN1_Pin,1);\
      HAL_GPIO_WritePin(IN2_GPIO_Port, IN2_Pin,0);\
      value = (uint16_t) (voltage * (255.0 / 12.0));\
    }\
    else{\
      HAL_GPIO_WritePin(IN1_GPIO_Port, IN1_Pin,0);\
      HAL_GPIO_WritePin(IN2_GPIO_Port, IN2_Pin,1);\
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
