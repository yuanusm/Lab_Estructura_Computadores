/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2021 STMicroelectronics.
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

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "utils.h"
#include "display.h"
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
TIM_HandleTypeDef htim6;
TIM_HandleTypeDef htim7;

UART_HandleTypeDef huart2;

/* USER CODE BEGIN PV */
uint8_t flag_timer_100;
uint8_t flag_timer_data;
uint8_t indice =0;
char buffer[10];
uint8_t flag_refresco_pantalla;
char comando_recibido[10];
char comando;
uint8_t flag_conteo;
uint16_t indice_conteo=0;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_TIM6_Init(void);
static void MX_USART2_UART_Init(void);
static void MX_TIM7_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/*Esto es para redireccionar la funcion de printf a la consola SWV durante la sesion debug*/
//int _write(int file, char *ptr, int len)
//{
//	/* Implement your write code here, this is used by puts and printf for example */
//	int i=0;
//	for(i=0 ; i<len ; i++)
//		ITM_SendChar((*ptr++));
//	return len;
//}

int _write(int fd, char* ptr, int len)
{
    for(int i = 0; i < len ; i++)
    {
      display_putchar(ptr[i]);
    }
    return len;
}

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */
	IKS01A2_MOTION_SENSOR_Axes_t local_mag;
	IKS01A2_MOTION_SENSOR_Axes_t buffer_conteo[1000];

	int32_t xmin=0;
	int32_t ymin=0;
	int32_t zmin=0;
	int32_t xmax=0;
	int32_t ymax=0;
	int32_t zmax=0;

	float calibracionx = 0;
	float calibraciony = 0;
	float calibracionz = 0;
  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  IKS01A2_init();	/*Inicializar los sensores*/

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_TIM6_Init();
  MX_USART2_UART_Init();
  MX_TIM7_Init();
  BSP_I2C1_Init();
  /* USER CODE BEGIN 2 */

 /*  Actividad 2 */

  HAL_TIM_Base_Start_IT(&htim6);



  HAL_UART_Receive_IT(&huart2, (uint8_t*)&comando, 1);

  char mensaje[100];


//  for(uint8_t address = 0x08; address <= 0x77; address++){
//
//      if(HAL_I2C_IsDeviceReady(&hi2c1, address << 1, 2, 200) == HAL_OK){
//
//          sprintf(mensaje, "Dispositivo encontrado: 0x%02X\r\n", address);
//
//          HAL_UART_Transmit(&huart2,
//                            (uint8_t*)mensaje,
//                            strlen(mensaje),
//                            HAL_MAX_DELAY);
//      }
//  }


  uint8_t who_i;
  BSP_I2C1_ReadReg(0x1E<<1, 0x4F, (uint8_t*)&who_i, 1);


  //int32_t  BSP_I2C1_ReadReg(uint16_t DevAddr, uint16_t Reg, uint8_t *pData, uint16_t Length)
  sprintf(mensaje, "WHO_I_AM: %X\r\n", who_i);

  HAL_UART_Transmit(&huart2,
                    (uint8_t*)mensaje,
                    strlen(mensaje),
                    HAL_MAX_DELAY);


  uint8_t contador_segundos = 0;
  flag_refresco_pantalla = 0;
  flag_timer_100 = 0;
  flag_conteo = 0;



  setvbuf(stdout, (char*)NULL, _IONBF, 0);
  display_init();
  display_set_pos(0x00);
  //printf("Hola mundo");
  display_clear();



  //printf("HOla%u\n\r",3);

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
	while (1)
	{
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */

		if(flag_timer_data){
					if(comando=='\n' || comando=='\r' ||indice == 10){

						for(uint8_t i = 0; i<10 ;i++){
							comando_recibido[i] = buffer[i];
							buffer[i] = 0;
						}

						indice = 0;

					}
					flag_timer_data = 0;
				}

		if(flag_timer_100){
			++contador_segundos;
			local_mag = Magneto_Sensor_Handler(IKS01A2_LSM303AGR_MAG_0);
			if(contador_segundos==10){
				flag_refresco_pantalla = 1;
				contador_segundos = 0;
			}
			flag_timer_100 = 0;

//			comando_recibido[0]=='A' ||
			if(!strcmp(comando_recibido,"B\n")){
				//char mensaje[100];
				//sprintf(mensaje, "mx:%f.2 my:%f.2 mz:%f.2\n\r", (float)local_mag.x,(float)local_mag.y,(float)local_mag.z);
				//sprintf(mensaje, "Comando: %c \n\r",comando_recibido[0]==0?1:comando_recibido[0]);
				//HAL_UART_Transmit(&huart2,(uint8_t*)mensaje,strlen(mensaje),HAL_MAX_DELAY);
				flag_conteo = 1;
				indice_conteo = 0;

				for (int i = 0; i < sizeof(comando_recibido); i++) {
					comando_recibido[i] = '\0';
				}
			}else if(!strcmp(comando_recibido,"A\n")){
				char mensaje[100];
				sprintf(mensaje, "mx:%.2f my:%.2f mz:%.2f\n\r", (float)local_mag.x-calibracionx,(float)local_mag.y-calibraciony,(float)local_mag.z-calibracionz);
				//sprintf(mensaje, "Comando: %c \n\r",comando_recibido[0]==0?1:comando_recibido[0]);
				HAL_UART_Transmit(&huart2,(uint8_t*)mensaje,strlen(mensaje),HAL_MAX_DELAY);
				for (int i = 0; i < sizeof(comando_recibido); i++) {
					comando_recibido[i] = '\0';
				}
			}else if(strcmp(comando_recibido,"\0\0\0\0") ){
				char mensaje[100];
				//sprintf(mensaje, "mx:%f.2 my:%f.2 mz:%f.2\n\r", (float)local_mag.x-calibracionx,(float)local_mag.y-calibraciony,(float)local_mag.z-calibracionz);
				sprintf(mensaje, "Mensaje Desconocido \n\r%c",0);
				HAL_UART_Transmit(&huart2,(uint8_t*)mensaje,strlen(mensaje),HAL_MAX_DELAY);
				for (int i = 0; i < sizeof(comando_recibido); i++) {
					comando_recibido[i] = '\0';
				}
			}

			if(flag_conteo){
				buffer_conteo[indice_conteo] = local_mag;
				if(indice_conteo!=0){

					xmin = buffer_conteo[indice_conteo].x>=buffer_conteo[indice_conteo-1].x?buffer_conteo[indice_conteo-1].x:buffer_conteo[indice_conteo].x;
					ymin = buffer_conteo[indice_conteo].y>=buffer_conteo[indice_conteo-1].y?buffer_conteo[indice_conteo-1].y:buffer_conteo[indice_conteo].y;
					zmin = buffer_conteo[indice_conteo].z>=buffer_conteo[indice_conteo-1].z?buffer_conteo[indice_conteo-1].z:buffer_conteo[indice_conteo].z;
					xmax = buffer_conteo[indice_conteo].x<buffer_conteo[indice_conteo-1].x?buffer_conteo[indice_conteo-1].x:buffer_conteo[indice_conteo].x;
					ymax = buffer_conteo[indice_conteo].y<buffer_conteo[indice_conteo-1].y?buffer_conteo[indice_conteo-1].y:buffer_conteo[indice_conteo].y;
					zmax = buffer_conteo[indice_conteo].z<buffer_conteo[indice_conteo-1].z?buffer_conteo[indice_conteo-1].z:buffer_conteo[indice_conteo].z;

				}
				indice_conteo++;
				if(indice_conteo == 999){
					flag_conteo = 0;
					indice_conteo=0;

					for(indice_conteo = 0; indice_conteo!=999; indice_conteo++){
						char mensaje[100];
						sprintf(mensaje, "mx:%1.2f my:%1.2f mz:%1.2f\n\r", (float)buffer_conteo[indice_conteo].x,(float)buffer_conteo[indice_conteo].y,(float)buffer_conteo[indice_conteo].z);
						HAL_UART_Transmit(&huart2,(uint8_t*)mensaje,strlen(mensaje),HAL_MAX_DELAY);


					}

					char mensaje[100];
					//sprintf(mensaje, "xmin:%d\nymin:%d\nzmin:%d\nxmax:%d\nymax:%d\nzmax:%d\n\r", xmin,ymin,zmin,xmax,ymax,zmax);
					//HAL_UART_Transmit(&huart2,(uint8_t*)mensaje,strlen(mensaje),HAL_MAX_DELAY);

					calibracionx = (xmin+xmax)/2.0f;
					calibraciony = (ymin+ymax)/2.0f;
					calibracionz = (zmin+zmax)/2.0f;

					//sprintf(mensaje, "x:%f\ny:%f\nz:%f\n\r", calibracionx,calibraciony,calibracionz);
					//HAL_UART_Transmit(&huart2,(uint8_t*)mensaje,strlen(mensaje),HAL_MAX_DELAY);

				}
			}
		}
		//printf("Hol%u\n",comando_recibido[0]);
		if(flag_refresco_pantalla){

			//			printf("Magnetometro: x = %d y = %d z = %d\n",(int)local_mag.x,(int)local_mag.y,(int)local_mag.z);

			//char mensaje[100];
			//sprintf(mensaje, "mx:%1.2f my:%1.2f mz:%1.2f\n\r", (float)local_mag.x,(float)local_mag.y,(float)local_mag.z);
			//HAL_UART_Transmit(&huart2,(uint8_t*)mensaje,strlen(mensaje),HAL_MAX_DELAY);

			display_clear();
			//HAL_Delay(20);
			display_set_pos(0x40);
			printf( "MagField mx:%1.0f\nmy:%1.0f mz:%1.0f", (float)local_mag.x-calibracionx,(float)local_mag.y-calibraciony,(float)local_mag.z-calibracionz);


			flag_refresco_pantalla = 0;
		}

//
//		printf("----------------Mediciones Actuales----------------------\n");
//
//
//		HAL_Delay(1000);	//solo para visualizar mejor los datos
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

/**
  * @brief TIM6 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM6_Init(void)
{

  /* USER CODE BEGIN TIM6_Init 0 */

  /* USER CODE END TIM6_Init 0 */

  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM6_Init 1 */

  /* USER CODE END TIM6_Init 1 */
  htim6.Instance = TIM6;
  htim6.Init.Prescaler = 8000-1;
  htim6.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim6.Init.Period = 1000-1;
  htim6.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim6) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim6, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM6_Init 2 */

  /* USER CODE END TIM6_Init 2 */

}

/**
  * @brief TIM7 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM7_Init(void)
{

  /* USER CODE BEGIN TIM7_Init 0 */

  /* USER CODE END TIM7_Init 0 */

  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM7_Init 1 */

  /* USER CODE END TIM7_Init 1 */
  htim7.Instance = TIM7;
  htim7.Init.Prescaler = 8000-1;
  htim7.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim7.Init.Period = 10000-1;
  htim7.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim7) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_OnePulse_Init(&htim7, TIM_OPMODE_SINGLE) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim7, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM7_Init 2 */

  /* USER CODE END TIM7_Init 2 */

}

/**
  * @brief USART2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART2_UART_Init(void)
{

  /* USER CODE BEGIN USART2_Init 0 */

  /* USER CODE END USART2_Init 0 */

  /* USER CODE BEGIN USART2_Init 1 */

  /* USER CODE END USART2_Init 1 */
  huart2.Instance = USART2;
  huart2.Init.BaudRate = 115200;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  huart2.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart2.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART2_Init 2 */

  /* USER CODE END USART2_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  /* USER CODE BEGIN MX_GPIO_Init_1 */

  /* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, LD2_Pin|D7_Pin|D6_Pin|D2_Pin
                          |D3_Pin|RS_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, D1_Pin|RW_Pin|D0_Pin|E_Pin
                          |D5_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(D4_GPIO_Port, D4_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : B1_Pin */
  GPIO_InitStruct.Pin = B1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(B1_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : LD2_Pin D7_Pin D6_Pin D2_Pin
                           D3_Pin RS_Pin */
  GPIO_InitStruct.Pin = LD2_Pin|D7_Pin|D6_Pin|D2_Pin
                          |D3_Pin|RS_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : D1_Pin RW_Pin D0_Pin E_Pin
                           D5_Pin */
  GPIO_InitStruct.Pin = D1_Pin|RW_Pin|D0_Pin|E_Pin
                          |D5_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pin : D4_Pin */
  GPIO_InitStruct.Pin = D4_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(D4_GPIO_Port, &GPIO_InitStruct);

  /* EXTI interrupt init*/
  HAL_NVIC_SetPriority(EXTI15_10_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);

  /* USER CODE BEGIN MX_GPIO_Init_2 */

  /* USER CODE END MX_GPIO_Init_2 */
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
