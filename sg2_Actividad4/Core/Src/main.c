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
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define RETARDO 250
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
void EscribirPines(char LED){
			HAL_GPIO_WritePin(GPIOA, LED1_Pin, BIT_READ(LED, 0));
		  HAL_GPIO_WritePin(GPIOA, LED2_Pin, BIT_READ(LED, 1));
		  HAL_GPIO_WritePin(GPIOB, LED3_Pin, BIT_READ(LED, 2));
		  HAL_GPIO_WritePin(GPIOB, LED4_Pin, BIT_READ(LED, 3));
		  HAL_GPIO_WritePin(GPIOB, LED5_Pin, BIT_READ(LED, 4));
		  HAL_GPIO_WritePin(GPIOB, LED6_Pin, BIT_READ(LED, 5));
		  HAL_GPIO_WritePin(GPIOB, LED7_Pin, BIT_READ(LED, 6));
		  HAL_GPIO_WritePin(GPIOB, LED8_Pin, BIT_READ(LED, 7));

}
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
  /* USER CODE BEGIN 2 */
  volatile enum _State {Parpadear, ShifterR, ShifterL, TodosParpadean};
  volatile enum _State State, State_n;
  State = Parpadear;
  State_n = Parpadear;
  volatile char Partida = 0;
  volatile char Boton1,Boton2,boton_start_previo,boton_start;
  boton_start_previo = 1;
  volatile char LED   = 0b11110000;
  volatile char LED_state = LED;
  EscribirPines(LED);
  /* USER CODE END 2 */

  /* Initialize leds */
  BSP_LED_Init(LED_GREEN);

  /* Initialize USER push-button, will be used to trigger an interrupt each time it's pressed.*/
  BSP_PB_Init(BUTTON_USER, BUTTON_MODE_EXTI);

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
	  boton_start = HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_13);
	  Boton1 = HAL_GPIO_ReadPin(Boton1_GPIO_Port, Boton1_Pin);
	  Boton2 = HAL_GPIO_ReadPin(Boton2_GPIO_Port, Boton2_Pin);



	  if(Partida){

		  switch(State){
			  case Parpadear:
				  LED ^= LED_state;
					  break;
			  case ShifterR:
				  if(BIT_READ(LED_state,0)){
					  LED_state = LED_state >> 1u;
					  BIT_SET(LED_state, 7);
				  }else{
					  LED_state = LED_state >> 1u;
					  BIT_CLEAR(LED_state, 7);
				  }
				  LED = LED_state;
					  break;
			  case ShifterL:

				  if(BIT_READ(LED_state,7)){
					  LED_state = LED_state << 1u;
					  BIT_SET(LED_state, 0);
				  }else{
					  LED_state = LED_state << 1u;
					  BIT_CLEAR(LED_state, 0);
				  }
				  LED = LED_state;
					  break;
			  case TodosParpadean:
				  if(LED & 0xFF){
					  LED = 0x00;
				  }else{
					  LED = 0xFF;
				  }
					  break;
			  default:
				  Partida = 0;
				  break;
		  }
		  HAL_Delay(RETARDO);
		  EscribirPines(LED);

		  if(!Boton1 && !Boton2){
			  State_n = Parpadear;
		  }else if(Boton1 && !Boton2){
			  State_n = ShifterR;
		  }else if(!Boton1 && Boton2){
			  State_n = ShifterL;
		  }else if(Boton1 && Boton2){
			  State_n = TodosParpadean;
		  }
		  if(State_n != State){
			  State = State_n;
			  LED = LED_state;
			  EscribirPines(LED_state);
		  }

	  }





	  if(boton_start_previo && (!boton_start)){
		  BIT_TOGGLE(Partida, 0);
		  //LED = LED_state;
		  //EscribirPines(LED_state);
	  }
	  boton_start_previo = boton_start;








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
