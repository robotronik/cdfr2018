#include "encoder.h"

#if TEST_ENCODER != 0
uint8_t led_level;
#endif

void init_encoder(volatile Encoder *encoder, TIM_HandleTypeDef *htim, TIM_HandleTypeDef *htim2){
  encoder->last_pos = encoder->steps = 0;
  encoder->htim = htim;
  encoder->htim2 = htim2;
}

int update_encoder(volatile Encoder *encoder){
  const int cnt = encoder->htim->Instance->CNT;

  int dl = cnt - encoder->last_pos;
  encoder->last_pos = cnt;
  dl=dl%(ENCODER_MAX-1);

  encoder->steps += dl;
  return dl;
}

void start_encoder(volatile Encoder *encoder){
  HAL_TIM_Encoder_Start(encoder->htim, TIM_CHANNEL_ALL);
  HAL_TIM_Base_Start_IT(encoder->htim2);
}
