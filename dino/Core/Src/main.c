/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "adc.h"
#include "tim.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdio.h>
#include <stdlib.h>
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define ADC_ARRAY_SIZE 5
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
unsigned char recibido1 = '1';
unsigned char recibido2 = '0';
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
uint8_t flag = 0;
float ADC_ARRAY[ADC_ARRAY_SIZE];
uint8_t index = 0;
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

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
  MX_ADC1_Init();
  MX_TIM6_Init();
  /* USER CODE BEGIN 2 */
  //HAL_UART_Receive_IT(&huart2, &recibido2, 1);
  //HAL_UART_Receive_IT(&huart1, &recibido1, 1);
  /* para el manipulador de uart2 le pasa la direccion de mi variable
   * asignandole tamaño 1byte
   * cuando llega 1byte se guarda y se termina la funcion
   * cuando termina de recibir el byte activa el callback
   * el callback inicia la nueva recepcion de un byte sin bloquear la CPU
   *
   * */
  setvbuf(stdout,NULL,_IONBF,0);
  unsigned char character = 'U';
  double conversion = 0.000811233211233; // (2^12-1)/vref
  float resultado = 0.0f;
  float lecturaADC = 0.0f;
  HAL_TIM_Base_Start_IT(&htim6);
  double resultado_promediado = 0.0f;
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {



	  if(flag){
		  __disable_irq();
		  //HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin,0);// medido 1.947us
		  HAL_ADC_Start(&hadc1);  //teorico 234ns
		  	  	  	  	  	  	  //medido 5.169us
		  //HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin,1);

		  lecturaADC = HAL_ADC_GetValue(&hadc1);

		  resultado = lecturaADC * conversion;
		  //HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin,0); // 3.095us
		  //printf("%.1f\n",resultado); // 4.26ms
		  //HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin,1);
		  flag = 0;
		  ADC_ARRAY[index] = resultado;
		  index = (index +1)%ADC_ARRAY_SIZE;

		  __enable_irq();
		  //HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin,0);// 1.58us
	  }
	  for(uint8_t i = 0; i<ADC_ARRAY_SIZE; i++){
		  resultado_promediado += ADC_ARRAY[i];
	  }
	  resultado_promediado/= ADC_ARRAY_SIZE;

	  if(resultado_promediado<0.5){
		  HAL_GPIO_WritePin(SOLENOIDE_GPIO_Port, SOLENOIDE_Pin,1);
	  }else{
		  HAL_GPIO_WritePin(SOLENOIDE_GPIO_Port, SOLENOIDE_Pin,0);
	  }

	  //printf("p: %.4f\n",resultado_promediado);

	//HAL_UART_Transmit(&huart1, &character, 1, 500);
	//HAL_UART_Transmit(&huart1, &character, 1, 500);
	  /*Caracter 0101_0101, se verá en el discovery
				*...111111_0inicio_10101010_1termino111111...
				*Son 10 bit en total Inicio Byte Termino
				*cada bit se demora 1/2400 = 416.67 [us]
				*10 bits se demoran 1/240 = 4.1667 [ms]
				*
				*
				* 208.3us (START) ->
				* 625us (LSB) -> 1.042ms (B1) -> 1.458ms (B2) -> 1.875ms (B3) ->
				* 2.29ms (B4) -> 2.708ms (B5) -> 3.125ms (B6) -> 3.542ms (B7) ->
				* 3.96ms (STOP)
				*
				*
				*/
	//HAL_Delay(100);
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */

	  /*ACTIVIDAD 2*/
/*
		switch(recibido2){
			case 'A':
				HAL_GPIO_TogglePin(LD2_GPIO_Port, LD2_Pin);
				HAL_Delay(400);
				break;
			case 'B':
				HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, 0);
				break;
			default:
				HAL_GPIO_TogglePin(LD2_GPIO_Port, LD2_Pin);
				HAL_Delay(80);
				break;
		}
*/
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  if (HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 1;
  RCC_OscInitStruct.PLL.PLLN = 10;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV7;
  RCC_OscInitStruct.PLL.PLLQ = RCC_PLLQ_DIV2;
  RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */
//void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart){
//
//	if(huart->Instance == USART1){
//
//		HAL_UART_Receive_IT(&huart1, &recibido1, 1);
//	}else if(huart->Instance == USART2){
//
//		HAL_UART_Receive_IT(&huart2, &recibido2, 1);
//	}
//}
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim){
	if(htim->Instance == TIM6){
		//HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin,1);
		flag = 1;
	}

}

//int _write(int file, char *ptr, int len) {
//  HAL_UART_Transmit(&huart2, (uint8_t *)ptr, len,1000); //Polling
//  return len;
//}
/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}
#ifdef USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
