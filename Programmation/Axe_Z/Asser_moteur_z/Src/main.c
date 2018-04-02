
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
#include "stm32f3xx_hal.h"
#include "dma.h"
#include "i2c.h"
#include "tim.h"
#include "usart.h"
#include "wwdg.h"
#include "gpio.h"

/* USER CODE BEGIN Includes */
#include "Robotronik_corp_pid.h"
#include "ax_12a_hal.h"
#include "Z_axis.h"
#include "encoder.h"

#include "robotronik_protocol.h"
#include "robotronik_protocol_stm32f3.h"
#include "remote_call.h"
#include "functions.h"
/* USER CODE END Includes */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
/* Private variables ---------------------------------------------------------*/
volatile PID_DATA pid_z;
volatile Encoder encoder;
volatile int imp_goal;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);

/* USER CODE BEGIN PFP */
/* Private function prototypes -----------------------------------------------*/

AX_Interface interface;

/* USER CODE END PFP */

/* USER CODE BEGIN 0 */
void HAL_WWDG_EarlyWakeupCallback(WWDG_HandleTypeDef* p_hwwdg){
  if(p_hwwdg == &hwwdg){

  }
}
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
  MX_TIM2_Init();
  MX_TIM3_Init();
  MX_USART1_UART_Init();
  MX_USART2_UART_Init();
  MX_TIM15_Init();
  MX_WWDG_Init();
  /* USER CODE BEGIN 2 */
  extern RP_Interface Z_interface;
  extern RC_Server Z_server;

  //==================================================
  //                    UART
  //==================================================
  RP_Init_Interface(&Z_interface, USART2, RP_UART_Transmit, HAL_GetTick);
  RP_INIT_UART_DMA(DMA1, LL_DMA_CHANNEL_6, USART2, Z_interface);

  //==================================================
  //              Remote Call Server
  //==================================================
  RC_Server_Init(&Z_server,&Z_interface);
  RC_Server_Add_Function(&Z_server, Z_GET_STATE, get_state, "", "", RC_IMMEDIATE);
  RC_Server_Add_Function(&Z_server, Z_RESET,reset,"","", RC_IMMEDIATE);
  RC_Server_Add_Function(&Z_server, Z_SET_ASSER,set_asser,"fffif","", RC_IMMEDIATE);
  RC_Server_Add_Function(&Z_server, Z_BALEC,balec, "","", RC_IMMEDIATE);
  RC_Server_Add_Function(&Z_server, Z_PUNCH_BEE,punch_bee, "","", RC_IMMEDIATE);
  RC_Server_Add_Function(&Z_server, Z_ARM_IN,arm_in, "","", RC_IMMEDIATE);
  RC_Server_Add_Function(&Z_server, Z_ARM_OUT,arm_out, "","", RC_IMMEDIATE);
  RC_Server_Add_Function(&Z_server, Z_STACK,stack, "","", RC_IMMEDIATE);
  RC_Server_Add_Function(&Z_server, Z_STACK_LAST, stack_last,"","",RC_IMMEDIATE);
  RC_Server_Add_Function(&Z_server, Z_UNSTACK,unstack, "","", RC_IMMEDIATE);
  RC_Server_Add_Function(&Z_server, Z_PLACE,place, "","", RC_IMMEDIATE);


  //==================================================
  //                  AX-12A
  //==================================================
  interface.receive = AX_Receive_HAL;
  interface.send = AX_Send_HAL;
  interface.set_direction = AX_Set_Direction_HAL;
  interface.delay = HAL_Delay;

  servo_ar.id = 1;
  servo_ar.interface = &interface;
  servo_g.id = 2;
  servo_g.interface = &interface;
  servo_d.id = 3;
  servo_d.interface = &interface;
  //AX_Say_Hello(&servo);
  /*AX_Configure_ID(&servo, 2);
  //AX_Configure_Angle_Limit(&servo_ar, 255, 750);
  //AX_Configure_Angle_Limit(&servo_g, 430, 540);
  //AX_Configure_Angle_Limit(&servo_d, 480, 590);
  while(1)
  {
    HAL_Delay(10);
  }
  */
  /*AX_Set_Goal_Position(&servo_ar, 255, AX_NOW);
  AX_Set_Goal_Position(&servo_g, 430, AX_NOW);
  AX_Set_Goal_Position(&servo_d, 590, AX_NOW);
  HAL_Delay(2000);
  HAL_GPIO_TogglePin(LED_GPIO_Port,LED_Pin);
  AX_Set_Goal_Position(&servo_ar, 750, AX_NOW);
  AX_Set_Goal_Position(&servo_g, 540, AX_NOW);
  AX_Set_Goal_Position(&servo_d, 480, AX_NOW);
  HAL_Delay(2000);*/
  int Te=10;
  pid_z.Kp=0.001;
  pid_z.Ki=0;
  pid_z.Kd=0;
  pid_z.Te=0.01;
  pid_z.position_tolerance=100;
  pid_z.speed_tolerance=100;
  pid_init(&pid_z);

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */


  //https://www.pololu.com/product/1212
  MOTOR_INIT;
  MOTOR_FC;
  init_encoder(&encoder,&htim2,&htim15);
  start_encoder(&encoder);

  imp_goal=0;//warning no positive values
  while (1)
  {
    HAL_WWDG_Refresh(&hwwdg);
    float voltage=pid(&pid_z,imp_goal-encoder.steps);
    MOTOR_VOLTAGE(voltage);
    HAL_Delay(Te);
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
  RCC_PeriphCLKInitTypeDef PeriphClkInit;

    /**Initializes the CPU, AHB and APB busses clocks
    */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = 16;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL16;
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

  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USART1|RCC_PERIPHCLK_I2C1;
  PeriphClkInit.Usart1ClockSelection = RCC_USART1CLKSOURCE_PCLK1;
  PeriphClkInit.I2c1ClockSelection = RCC_I2C1CLKSOURCE_HSI;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
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
    ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
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
