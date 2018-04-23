#ifndef __ENCODER_H_
#define __ENCODER_H_

#include "stm32f3xx_hal.h"
#include <stdlib.h>

#define ENCODER_MAX 14400

//TEST ENCODER
#define TEST_ENCODER 0
#if TEST_ENCODER != 0
extern uint8_t led_level;
#if TEST_ENCODER != 1 && TEST_ENCODER != 2
#error "TEST_ENCODER MUST EQUALS 1 OR 2"
#endif
#warning "PROGRAM IS IN TEST_ENCODER MODE"
#endif

typedef enum Encoder_Dir_E{
  FORWARD, BACKWARD
}Encoder_Dir;

typedef struct Encoder_S{
  //Position (angle)
  volatile int last_pos;

  //Nombre de pas
  volatile int steps;

  //Timers
  TIM_HandleTypeDef *htim;
  TIM_HandleTypeDef *htim2;
}Encoder;

void init_encoder(volatile Encoder *encoder, TIM_HandleTypeDef *htim, TIM_HandleTypeDef *htim2);
/**
 * Initialise encoder.
 */

int update_encoder(volatile Encoder *encoder);

void start_encoder(volatile Encoder *encoder);

#endif
