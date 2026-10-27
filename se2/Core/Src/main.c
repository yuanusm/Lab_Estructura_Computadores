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
#include "i2c.h"
#include "spi.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "getSensors.h"
#include "interrupciones.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

//UART
uint8_t flag_escribir_uart;
uint8_t enable_S_uart;
uint8_t comando_completo[51];
uint8_t TimeSet[];
volatile float LecturaLDR;
unsigned char recibido2 = '0';
uint8_t index1 = 0;
uint8_t buffer[10];
uint8_t recib1;
uint8_t flag_segundo_caracter;
uint8_t flag_leer_uart;
//RTC
//char *LecturaRTC;
//float LecturaPresion;
uint8_t timeset =0;
uint8_t RTCConfig[9] = {0x00,
						Reg0,
						Reg1,
						Reg2,
						Reg3,
						Reg4,
						Reg5,
						Reg6,
						Reg7
};

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
  MX_I2C2_Init();
  MX_SPI2_Init();
  MX_TIM6_Init();
  MX_USART2_UART_Init();
  MX_TIM7_Init();
  /* USER CODE BEGIN 2 */
  HAL_ADC_Start_IT(&hadc1);
  HAL_TIM_Base_Start_IT(&htim6);
  enable_S_uart = 0;
  Config_Sensor();
  //ptr_recib1 = recib1;
  HAL_UART_Receive_IT(&huart2, &recib1, 1);

  uint8_t year, month, day;
  uint8_t hour, minute, second;
  char comando; // 2 caracteres y el byte 0
  char* com;
  flag_leer_uart=0;




  HAL_I2C_Init(&hi2c2);
  //HAL_I2C_Master_Transmit(&hi2c2, SlaveAddW, RTCConfig, 9, 100);


  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
//	  if(flag_ConfigRTC){
//		  HAL_UART_Receive(&huart2, RTCconfig, 30, 300);
//		  flag_ConfigRTC = 0;
//	  }else {
//		  HAL_UART_Receive(&huart2, comando_completo, 2, 300);
//	  }

	  if(flag_escribir_uart && enable_S_uart){
		  //LecturaRTC = get_rtc();
		  //LecturaPresion = get_pressure();
		  LecturaLDR = get_ldr();

		  printf("%s %3.1f hPa %03u lux \n",get_rtc(),(float)get_pressure()/100.0f, (uint16_t)LecturaLDR);//Escribe en la consola serial

		  flag_escribir_uart = 0;
	  }

	  if(flag_leer_uart){

		  HAL_UART_Receive_IT(&huart2, &recib1, 1);


		  switch(recib1){
		  	  	case 'P':
		  	  		printf("%s %3.1f hPa %03u lux\n",get_rtc(),(float)get_pressure()/100.0f, (uint16_t)get_ldr());

		  	  		  break;
		  	  	case 'Q':
		  	  		printf("%s %03u lux\n",get_rtc(),(uint16_t)get_ldr());

		  	  		  		  break;
		  	  	case 'R':
		  	  		printf("%s %3.1f hPa\n",get_rtc(),(float)get_pressure()/100.0f);

		  	  		  		  break;
		  	  	case 'S': enable_S_uart = 1;

		  	  		  		  break;
		  	  	case 'T':

		  	  				HAL_Delay(1);
								if(recib1=='S'){
									timeset =1;
								}else{
									enable_S_uart = 0;
								}




		  	  		  		  break;
		  	  	default:
		  	  			printf("Error comando desconocido\n%c",0);
		  	  			break;
		  	  	//case "TS":


		  	  }


		  flag_leer_uart = 0;
	  }
	 if(timeset){
		buffer[index1] = recib1;

		if(index1>=21){
			index1 = 0;
			timeset =0;
			sscanf(buffer, "%d-%d-%d %d:%d:%d", &year,&month,&day,&hour,&minute,&second);
			RTCConfig[7] = year;
			HAL_I2C_Master_Transmit(&hi2c2, SlaveAddW, RTCConfig, 9, 100);
		}
		index1++;
	 }





//	sscanf(comando_completo, "%2s", comando);

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
