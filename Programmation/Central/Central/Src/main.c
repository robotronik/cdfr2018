
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
#include "pi_client.h"
#include "tof.h"
#include <stdio.h>
#include "strategy.h"
#include "map.h"
#include "position_client.h"
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

ToF_Handler tof[NB_TOF];

static int Init_ToF(ToF_Handler *htof, uint8_t i2c_addr, GPIO_TypeDef *xshut_port, uint32_t xshut_pin);

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
  MX_TIM2_Init();
  MX_TIM3_Init();
  /* USER CODE BEGIN 2 */

  //==================================================//
  //            ToF Initialisation                    //
  //==================================================//
  //Wait for the devices to reset
  HAL_Delay(TOF_DELAY);

  //Init the time of flight sensors
  Init_ToF(&tof[TOF_FRONT_LEFT], 0x54, TOF_FL_RESET_GPIO_Port, TOF_FL_RESET_Pin);
  Init_ToF(&tof[TOF_FRONT_RIGHT], 0x56, TOF_FR_RESET_GPIO_Port, TOF_FR_RESET_Pin);
  Init_ToF(&tof[TOF_REAR_LEFT], 0x58, TOF_RL_RESET_GPIO_Port, TOF_RL_RESET_Pin);
  Init_ToF(&tof[TOF_REAR_RIGHT], 0x60, TOF_RR_RESET_GPIO_Port, TOF_RR_RESET_Pin);

  //Enable the polling timer
  HAL_TIM_Base_Start_IT(&htim3);
  
  //==================================================//
  //           UART Protocol Init                     //
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
  //           Remote Call Client Init                //
  //==================================================//

  //Raspberry
  PI_Init();

  //Position
  Position_Init();
  
  //==================================================//
  //                 Wait Start                       //
  //==================================================//

  Team t = wait_start();
  PI_Start();

  /*
  Init_Strategy(GREEN_TEAM);
  Set_Construction_Plan(ORANGE, GREEN, BLUE);
  volatile uint32_t ticks = HAL_GetTick();
  Refresh_Map();//8ms
  Cell *start = &Get_Cell(me.x, me.y);
  int x_goal = MAP_WIDTH-3;
  int y_goal = MAP_HEIGHT-3;
  Cell *end = Compute_Path(start, &map[y_goal][x_goal]);//11ms
  Compute_Building_Strategy();//110ms
  ticks = HAL_GetTick() - ticks;
  HAL_Delay(ticks);*/


  Pos_Brake();
  Pos_Go_Forward(10., 100.);
  
  //volatile int jean_michel_segfault = *((int*)99999999999);
  //PI_Asser_Test();

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  
  
  while (1){
    //Test des capteurs
    HAL_Delay(250);
    PI_Log("AvG : %d\t", ToF_Get_Last_Range(&tof[TOF_FRONT_LEFT]));
    PI_Log("AvD : %d\t", ToF_Get_Last_Range(&tof[TOF_FRONT_RIGHT]));
    PI_Log("ArG : %d\t", ToF_Get_Last_Range(&tof[TOF_REAR_LEFT]));
    PI_Log("ArD : %d\n", ToF_Get_Last_Range(&tof[TOF_REAR_RIGHT]));

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

static int Init_ToF(ToF_Handler *htof, uint8_t i2c_addr, GPIO_TypeDef *xshut_port, uint32_t xshut_pin){
  //You definitely don't want to know what we're doing
  //there. Seriously, don't do that. Please.
  GPIO_InitTypeDef GPIO_InitStruct;
  GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Pin = xshut_pin;
  
  HAL_GPIO_Init(xshut_port, &GPIO_InitStruct);

  //Wait for the chip to start
  HAL_Delay(TOF_DELAY);

  //Init structure
  ToF_Init_Struct(htof, &hi2c3);

  ToF_Params params;
    
  //Check the initial address
  if(ToF_Poke(&htof->dev) == -1)
    return -1;

  //Set the new address
  if(ToF_Set_Address(&htof->dev, i2c_addr) == -1)
    return -1;
    
  //Init the device
  if(ToF_Init_Device(&htof->dev) == -1)
    return -1;

  //Long range config
  TOF_LONG_RANGE_CONFIG(params);

  //Configure the device
  if(ToF_Configure_Device(&htof->dev, &params, VL53L0X_DEVICEMODE_CONTINUOUS_RANGING)== -1)
    return -1;

  //Start the device
  if(VL53L0X_StartMeasurement(&htof->dev) == -1)
    return -1;
    
  return 0;
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
