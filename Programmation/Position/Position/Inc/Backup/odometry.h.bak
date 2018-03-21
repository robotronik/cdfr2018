#ifndef __ODOMETRY_H_
#define __ODOMETRY_H_


#include "main.h"
#include "arm_math.h"

#include "encoder.h"

#define ENCODER_DIST 20//distance between encoders
#define ENCODER_STEP_DIST 0.01//distance for 1 encoder step/2
#define deltaL ENCODER_STEP_DIST/ENCODER_DIST

typedef struct Odometry_S{
  Encoder encoder_l;
  Encoder encoder_r;
  volatile float x;
  volatile float y;
  volatile float theta;
}Odometry;

void init_odometry(Odometry *odometry, TIM_HandleTypeDef *htim_l, TIM_HandleTypeDef *htim_r, TIM_HandleTypeDef *htim_poll);

void update_odometry(Odometry *odometry);

#endif
