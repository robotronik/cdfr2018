#include "ax_12a_hal.h"

uint8_t AX_Receive_HAL(uint8_t *buffer, uint16_t size, uint32_t timeout){
  HAL_StatusTypeDef status = HAL_UART_Receive(&huart1, buffer, size, timeout);
  return (status==HAL_OK)?0:1;
}

uint8_t AX_Send_HAL(uint8_t *data, uint16_t size, uint32_t timeout){
  HAL_StatusTypeDef status = HAL_UART_Transmit(&huart1, data, size, timeout);
  return (status==HAL_OK)?0:1;
}

void AX_Set_Direction_HAL(AX_Direction dir){
  HAL_GPIO_WritePin(EN_Servo_GPIO_Port, EN_Servo_Pin, (dir==AX_SEND)?GPIO_PIN_SET:GPIO_PIN_RESET);
}
