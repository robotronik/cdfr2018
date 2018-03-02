#include "odometry.h"

void update_odometry(Odometry *odometry){
  int dl_l = update_encoder(&odometry->encoder_l);
  int dl_r = update_encoder(&odometry->encoder_r);
  
  odometry->x = odometry->x
    + cos(odometry->theta)*ENCODER_STEP_DIST*(dl_l + dl_r);
  
  odometry->y = odometry->y
    + sin(odometry->theta)*ENCODER_STEP_DIST*(dl_l + dl_r);

  odometry->theta = odometry->theta+deltaL*(dl_l - dl_r);

  if(odometry->theta>PI)//angle limitation to -PI +PI
  {
    odometry->theta=2*PI-odometry->theta;
  }
  else if(odometry->theta<=-PI)
  {
    odometry->theta=2*PI+odometry->theta;
  }
}

void init_odometry(Odometry *odometry, TIM_HandleTypeDef *htim_l, TIM_HandleTypeDef *htim_r, TIM_HandleTypeDef *htim_poll){
  odometry->x = odometry->y = 0;
  odometry->theta = 0;

  init_encoder(&odometry->encoder_l, htim_l);
  init_encoder(&odometry->encoder_r, htim_r);

  start_encoder(&odometry->encoder_l);
  start_encoder(&odometry->encoder_r);
  
  HAL_TIM_Base_Start_IT(htim_poll);
}

#if 0
#if TEST_ENCODER==1
  led_level = (int) (((float) encoder1.current / ENCODER_MAX)*255);
#endif

#if TEST_ENCODER==2
  led_level = (int) (((float) encoder2.current / ENCODER_MAX)*255);
#endif
#endif
