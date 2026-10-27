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
#include "i2c.h"
#include "spi.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "interrupcion.h"
#include <stdio.h>
#include "Compensada.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
//I2C


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


uint8_t RTCRead[7];

uint8_t segundos;
uint8_t minutos;
uint8_t seg;
uint8_t Decseg;
uint8_t	min;
uint8_t	Decmin;
uint8_t	hr;
uint8_t	Dechr;
uint8_t DiaSel;

uint8_t ufecha;
uint8_t	Dechfecha;
uint8_t	mehs;
uint8_t	Decmes;
uint8_t	anuel;
uint8_t	AnuelBr;

char *day;
char semana[7][9]= {"Lunes",
				"Martes",
				"Miercoles",
				"Jueves",
				"Viernes",
				"Sábado",
				"Momingo"};
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



 uint8_t flag_escribir_uart = 0;
float lectura_presion = 0;
float lectura_temperatura = 0;
int32_t temperatura_compensada =0;
uint32_t presion_compensada = 0;
int32_t  t_fine;
uint16_t dig_T1;
int16_t dig_T2, dig_T3;
uint16_t dig_P1;
int16_t dig_P2, dig_P3, dig_P4, dig_P5, dig_P6, dig_P7, dig_P8, dig_P9;



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
  MX_SPI2_Init();
  MX_TIM6_Init();
  MX_USART2_UART_Init();
  MX_I2C2_Init();
  /* USER CODE BEGIN 2 */
  //HAL_I2C_Init(&hi2c2);
  //HAL_I2C_Master_Transmit(&hi2c2, SlaveAddW, RTCConfig, 9, 100);


  HAL_TIM_Base_Start_IT(&htim6); //inicia timer6 cada 1 segundo ocurrirá una interrupcion
  CS_Enable();
  HAL_Delay(100);

  uint8_t tx_id[2] = {0xD0 | 0x80, 0x00}; // 0xD0|0x80 = 0xD0 (la dirección de ID del BME280 es 0xD0, bit7 ya es 1)
  uint8_t rx_id[2] = {0};
  CS_Enable();
  HAL_SPI_TransmitReceive(&hspi2, tx_id, rx_id, 2, 100);
  CS_Disable();
  printf("Chip ID: %h",rx_id[1]);


  Config_BMX280(); // inicializa los oversampling y el modo del bmp
  // consigue los valores de calibracion, para que cada vez que se lea
  // un dato se convierta en el correspondiente

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {




	  if(flag_escribir_uart){ //flag activada en la interrupcion


		  //HAL_I2C_Master_Receive(&hi2c2, SlaveAddR, RTCRead, 7, 100);
		  HAL_I2C_Mem_Read(&hi2c2, SlaveAddR,0x00,1, RTCRead, 7,100);

		  //I2C_HandleTypeDef *hi2c, uint16_t DevAddress, uint16_t MemAddress,
		  //                                   uint16_t MemAddSize, uint8_t *pData, uint16_t Size, uint32_t Timeout
		  seg = RTCRead[0] & 0b00001111;
		  Decseg = (RTCRead[0] & 0b01110000) >> 4;
		  min = RTCRead[1] & 0b00001111;
		  Decmin = (RTCRead[1] & 0b01110000) >> 4 ;
		  hr = RTCRead[2] & 0b00001111;
		  Dechr = (RTCRead[2] & 0b00110000) >> 4 ;
		  DiaSel = RTCRead[3] & 0b00000111;
		  switch (DiaSel) {
			  default : day = semana[0] ; break;
			  case 1: day = semana[1]; break;
			  case 2: day = semana[2]; break;
			  case 3: day = semana[3]; break;
			  case 4: day = semana[4]; break;
			  case 5: day = semana[5]; break;
			  case 6: day = semana[6]; break;

		  }
		  ufecha = RTCRead[4] & 0b00001111;
		  Dechfecha = (RTCRead[4] & 0b00110000) >> 4 ;
		  mehs =  RTCRead[5] & 0b00001111;
		  Decmes = (RTCRead[5] & 0b00010000) >> 4 ;
		  anuel = RTCRead[6] & 0b00001111;
		  AnuelBr = (RTCRead[6] & 0b11110000) >> 4 ;




		  //__disable_irq(); //desactiva la interrupcion
		  lectura_presion = Leer_Presion(); //lee el valor crudo de la presion
		  lectura_temperatura = Leer_Temperatura(); // lee el valor crudo de la temperatura
		  temperatura_compensada =BME280_compensate_T_int32(lectura_temperatura); //Se utiliza los datos de la libreria para
		  //compensar las no linealidades del sensor
		  //El fabricante deja un codigo dentro del datasheet para calcular este ajuste
		  presion_compensada = BME280_compensate_P_int32(lectura_presion);
		  //printf("%.2f	-	%.1f\n\r",(float)(temperatura_compensada/100.0f), (float)(presion_compensada/10.0f));
		  //printf("Presion: %.1f\n\r",(float)(presion_compensada/10.0f));
		  //printf("")

		  //printf("AñoD:%u\nAño:%u\nMesD:%u\nMes:%u\nDiaD:%u\nDia%u\nHoraD%u\nHora%u\nMinutoD:%u\nMinuto:%u\nSegD:%u\nSeg:%u Presion: %.1f\n\r", AnuelBr, anuel,Decmes,mehs,Dechfecha,ufecha,Dechr,hr,Decmin,min,Decseg,seg, (float)presion_compensada);
		  printf("20%u%u-%u%u-%u%u %u%u:%u%u:%u%u Presion [hPa]: %.1f\n\r", AnuelBr, anuel,Decmes,mehs,Dechfecha,ufecha,Dechr,hr,Decmin,min,Decseg,seg, (float)presion_compensada/100.f);//Escribe en la consola serial
		  flag_escribir_uart = 0; //Baja la flag del timer , asi permite que dentro de 1000ms se vuelva a activar
		  //__enable_irq();

	  }

    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */

	  //I2C

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
