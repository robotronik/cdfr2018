#ifndef __ENCODER_H_
#define __ENCODER_H_

#include "stm32f3xx_hal.h"

#define ENCODER_MAX 1440

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
}Encoder;

void init_encoder(Encoder *encoder, TIM_HandleTypeDef *htim);
/**
 * Initialise encoder.
 */

int update_encoder(Encoder *encoder);

void start_encoder(Encoder *encoder);

#endif
