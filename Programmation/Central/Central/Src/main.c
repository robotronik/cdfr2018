/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  ** This notice applies to any and all portions of this file
  * that are not between comment pairs USER CODE BEGIN and
  * USER CODE END. Other portions of this file, whether 
  * inserted by the user or by software development tools
  * are owned by their respective copyright owners.
  *
  * COPYRIGHT(c) 2018 STMicroelectronics
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stm32f4xx_hal.h"
#include "dma.h"
#include "i2c.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* USER CODE BEGIN Includes */
#include "system.h"
#include "tof.h"
#include "pi_client.h"
#include <stdio.h>
/* USER CODE END Includes */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
/* Private variables ---------------------------------------------------------*/

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);

/* USER CODE BEGIN PFP */
/* Private function prototypes -----------------------------------------------*/

/* USER CODE END PFP */

/* USER CODE BEGIN 0 */
#define TOF_FRONT_LEFT 0x52
#define TOF_FRONT_RIGHT 0x54
#define TOF_REAR_LEFT 0x56
#define TOF_REAR_RIGHT 0x58

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  *
  * @retval None
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration----------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_I2C1_Init();
  MX_I2C3_Init();
  MX_USART1_UART_Init();
  MX_USART2_UART_Init();
  MX_USART6_UART_Init();
  MX_TIM1_Init();
  MX_TIM2_Init();
  /* USER CODE BEGIN 2 */

  LED_OFF;
  //==================================================//
  //           UART PROTOCOL INIT                     //
  //==================================================//

  //Raspberry
  RP_Init_Interface(&pi_iface, USART1, RP_UART_Transmit, HAL_GetTick);
  RP_INIT_UART_DMA(DMA2, LL_DMA_STREAM_2, USART1, pi_iface);

  //Position
  RP_Init_Interface(&pos_iface, USART2, RP_UART_Transmit, HAL_GetTick);
  RP_INIT_UART_DMA(DMA1, LL_DMA_STREAM_5, USART2, pos_iface);

  //Z-Axis
  RP_Init_Interface(&z_iface, USART6, RP_UART_Transmit, HAL_GetTick);
  RP_INIT_UART_DMA(DMA2, LL_DMA_STREAM_1, USART6, z_iface);

  //==================================================//
  //           REMOTE CALL CLIENT INIT                //
  //==================================================//

  //Raspberry
  PI_Init();
  
  //==================================================//
  //                 WAIT START                       //
  //==================================================//

  //Team t = wait_start();
  //PI_Start();
  RP_Sync(&pi_iface, 0);

  HAL_Delay(1000);
  //PI_Asser_Test();

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  ToF_Dev tof_front_left, tof_front_right, tof_rear_left, tof_rear_right;
  ToF_Params params;
  ToF_Data tof_data;

  GPIO_InitTypeDef GPIO_InitStruct;
  GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  
  ToF_Init_Struct(&tof_front_left, &hi2c3, TOF_FRONT_LEFT);
  ToF_Init_Struct(&tof_front_right, &hi2c3, TOF_FRONT_LEFT);
  ToF_Init_Struct(&tof_rear_left, &hi2c3, TOF_FRONT_LEFT);
  ToF_Init_Struct(&tof_rear_right, &hi2c3, TOF_FRONT_LEFT);

  GPIO_InitStruct.Pin = TOF_RR_RESET_Pin;
  HAL_GPIO_Init(TOF_RR_RESET_GPIO_Port, &GPIO_InitStruct);
  HAL_Delay(100);
  if(ToF_Set_Address(&tof_rear_right, TOF_REAR_RIGHT) != -1){
    //LED_ON;
  }

  GPIO_InitStruct.Pin = TOF_RL_RESET_Pin;
  HAL_GPIO_Init(TOF_RL_RESET_GPIO_Port, &GPIO_InitStruct);
  HAL_Delay(100);
  if(ToF_Set_Address(&tof_rear_left, TOF_REAR_LEFT) != -1){
    //LED_ON;
  }

  GPIO_InitStruct.Pin = TOF_FR_RESET_Pin;
  HAL_GPIO_Init(TOF_FR_RESET_GPIO_Port, &GPIO_InitStruct);
  HAL_Delay(100);
  if(ToF_Set_Address(&tof_front_right, TOF_FRONT_RIGHT) != -1){
    //LED_ON;
  }

  GPIO_InitStruct.Pin = TOF_FL_RESET_Pin;
  HAL_GPIO_Init(TOF_FL_RESET_GPIO_Port, &GPIO_InitStruct);
  ToF_Set_Address(&tof_front_left, TOF_FRONT_LEFT);

  ToF_Init_Device(&tof_front_left);
  TOF_LONG_RANGE_CONFIG(params);
  ToF_Configure_Device(&tof_front_left, &params);

  ToF_Init_Device(&tof_front_right);
  TOF_LONG_RANGE_CONFIG(params);
  ToF_Configure_Device(&tof_front_right, &params);

  ToF_Init_Device(&tof_rear_left);
  TOF_LONG_RANGE_CONFIG(params);
  ToF_Configure_Device(&tof_rear_left, &params);

  ToF_Init_Device(&tof_rear_right);
  TOF_LONG_RANGE_CONFIG(params);
  ToF_Configure_Device(&tof_rear_right, &params);
  
  
  if(ToF_Poke(&tof_front_left) != -1){
    LED_ON;
    HAL_Delay(1000);
    LED_OFF;
  }
    HAL_Delay(1000);
    
  if(ToF_Poke(&tof_rear_left) != -1){
    LED_ON;
    HAL_Delay(1000);
    LED_OFF;
  }
    HAL_Delay(1000);
  if(ToF_Poke(&tof_rear_right) != -1){
    LED_ON;
    HAL_Delay(1000);
    LED_OFF;
  }
    HAL_Delay(1000);
  if(ToF_Poke(&tof_front_right) != -1){
    LED_ON;
    HAL_Delay(1000);
    LED_OFF;
  }
  
  /*
  do{
    //Check the initial address
    if(ToF_Poke(&tof_front_left) == -1){
      break;
    }
    else{
      PI_Log("ToF @0x%2.2X\n", TOF_FRONT_LEFT);
    }

    //Set a new address
    //if(ToF_Set_Address(&tof_front_left, 0x58) == -1)
    //  break;
    
    //Init the device
    if(ToF_Init_Device(&tof_front_left) == -1)
      break;
    else
      PI_Log("ToF init successful\n");

    //Long range config
    TOF_LONG_RANGE_CONFIG(params);

    //Configure the device
    if(ToF_Configure_Device(&tof_front_left, &params)== -1)
      break;
    else
      PI_Log("ToF config successful");
  }while(0);
  */
  
  while (1)
  {
    ToF_Perform_Measurement(&tof_front_left, &tof_data);
    ToF_Perform_Measurement(&tof_rear_left, &tof_data);
    ToF_Perform_Measurement(&tof_front_right, &tof_data);
    ToF_Perform_Measurement(&tof_rear_right, &tof_data);
    if(tof_data.RangeStatus == 0){
      //tof_front_left.LeakyRange (mm)
      //LED_ON;
    }else{
      //Out of range
      //HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_RESET);
    }
  /* USER CODE END WHILE */

  /* USER CODE BEGIN 3 */

  }
  /* USER CODE END 3 */

}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{

  RCC_OscInitTypeDef RCC_OscInitStruct;
  RCC_ClkInitTypeDef RCC_ClkInitStruct;

    /**Configure the main internal regulator output voltage 
    */
  __HAL_RCC_PWR_CLK_ENABLE();

  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE2);

    /**Initializes the CPU, AHB and APB busses clocks 
    */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = 16;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 16;
  RCC_OscInitStruct.PLL.PLLN = 336;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV4;
  RCC_OscInitStruct.PLL.PLLQ = 7;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

    /**Initializes the CPU, AHB and APB busses clocks 
    */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

    /**Configure the Systick interrupt time 
    */
  HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);

    /**Configure the Systick 
    */
  HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);

  /* SysTick_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);
}

/* USER CODE BEGIN 4 */
void HardFault_Handler(void){
  PI_Error();
  hlt();
}
/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @param  file: The file name as string.
  * @param  line: The line in file as a number.
  * @retval None
  */
void _Error_Handler(char *file, int line)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  while(1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{ 
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
