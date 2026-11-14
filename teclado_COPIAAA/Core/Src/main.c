/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "i2c.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "keyboard.h"
#include <stdio.h>
#include <string.h>
#include "display.h"
#define AddressReg1 0x00
#define Reg0 (0<<7u | 3u<<4u | 3u)//00110011   /// CH [7] | Decseg[6-4] | Seg [3-0]
#define Reg1 ( 1u<<4u | 5u)	  /// x[7] | DecMin[6-4] | Min[3-0]
#define Reg2 ( 0<<6u | 1u<<4u | 2u)	  /// x[7] | 12-24 hrs_set [6] (24low) | DecHora[5-4] AM-PM[5] | Hora[3-0]
#define Reg3 ( 0b1)	  /// X[7-3] | Dia [2-0]
#define Reg4 ( 2<<4 | 7)  /// X[7-6] | DecFecha [5-4] | Fecha[3-0]
#define Reg5 (1<<4 | 0)   /// X[7-5] | DecMes [4] | Mes[3-0]
#define Reg6 ( 2<<4 | 6)   /// DecAño [7:4] | Año[3-0]
#define Reg7 0b00000000   /// Square/out config
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
uint8_t ActualizacionTeclado = 0;
uint8_t acumulador_actualizacion_display = 0;
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
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */
	static uint8_t tx_buffer[20];


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
  MX_USART2_UART_Init();
  MX_TIM6_Init();
  MX_TIM7_Init();
  MX_I2C1_Init();
  /* USER CODE BEGIN 2 */
	HAL_TIM_Base_Start_IT(&htim6);
	CS_Enable();
	HAL_Delay(100);

	HAL_I2C_Init(&hi2c2);
	//HAL_I2C_Master_Transmit(&hi2c2, SlaveAddW, RTCConfig, 9, 100);


	uint8_t tx_id[2] = {0xD0 | 0x80, 0x00}; // 0xD0|0x80 = 0xD0 (la dirección de ID del BME280 es 0xD0, bit7 ya es 1)
	uint8_t rx_id[2] = {0};

	 Config_BMX280();
	keyboard_init();
	//snprintf((char*)tx_buffer, 20, "Iniciando teclado\n");
	HAL_UART_Transmit(&huart2, tx_buffer, strlen((const char*)tx_buffer),1000);

	setvbuf(stdout, (char*)NULL, _IONBF, 0);
	display_init();
	display_test_write_cgram_ms();
	bienvenida();
	display_clear();
		display_set_pos(0x00);
		printf("     ELO312     \n\rGrupo 2");
		printf("%c %c %c", 0x02,0x02,0x02);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
	  if(ActualizacionTeclado){
		  keyboard_tec_matrix(); //funcion teclao cada 10[ms]

		  if((++acumulador_actualizacion_display)==100){
			  acumulador_actualizacion_display=0;
			  Actualizar_Display();
		  }

		  ActualizacionTeclado=0;
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
void bienvenida(void){
	display_clear();
	display_set_pos(0x00);
	printf("  Sistema de   \n\rmedicion ELO312");
	HAL_Delay(2000);

	//message de vienbenida 2024
		//

}
int _write(int fd, char* ptr, int len)
{
    for(int i = 0; i < len ; i++)
    {
      display_putchar(ptr[i]);
    }
    return len;
}

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */

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
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
