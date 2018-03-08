#include "encoder.h"

#if TEST_ENCODER != 0
uint8_t led_level;
#endif

void init_encoder(Encoder *encoder, TIM_HandleTypeDef *htim){
  encoder->last_pos = encoder->steps = 0;
  encoder->htim = htim;
}

int update_encoder(Encoder *encoder){
  const int cnt = encoder->htim->Instance->CNT;

  int dl = cnt - encoder->last_pos;
  encoder->last_pos = cnt;
  if(dl > 1){
    dl = -1;
  }else if(dl < -1){
    dl = +1;
  }
  
  encoder->steps += dl;

  return dl;
}

void start_encoder(Encoder *encoder){
  HAL_TIM_Encoder_Start(encoder->htim, TIM_CHANNEL_ALL);
}
