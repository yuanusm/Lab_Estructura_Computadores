/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    spi.c
  * @brief   This file provides code for the configuration
  *          of the SPI instances.
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
#include "spi.h"

/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

SPI_HandleTypeDef hspi2;

/* SPI2 init function */
void MX_SPI2_Init(void)
{

  /* USER CODE BEGIN SPI2_Init 0 */

  /* USER CODE END SPI2_Init 0 */

  /* USER CODE BEGIN SPI2_Init 1 */

  /* USER CODE END SPI2_Init 1 */
  hspi2.Instance = SPI2;
  hspi2.Init.Mode = SPI_MODE_MASTER;
  hspi2.Init.Direction = SPI_DIRECTION_2LINES;
  hspi2.Init.DataSize = SPI_DATASIZE_4BIT;
  hspi2.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi2.Init.CLKPhase = SPI_PHASE_1EDGE;
  hspi2.Init.NSS = SPI_NSS_SOFT;
  hspi2.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_16;
  hspi2.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi2.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi2.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi2.Init.CRCPolynomial = 7;
  hspi2.Init.CRCLength = SPI_CRC_LENGTH_DATASIZE;
  hspi2.Init.NSSPMode = SPI_NSS_PULSE_ENABLE;
  if (HAL_SPI_Init(&hspi2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN SPI2_Init 2 */

  /* USER CODE END SPI2_Init 2 */

}

void HAL_SPI_MspInit(SPI_HandleTypeDef* spiHandle)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};
  if(spiHandle->Instance==SPI2)
  {
  /* USER CODE BEGIN SPI2_MspInit 0 */

  /* USER CODE END SPI2_MspInit 0 */
    /* SPI2 clock enable */
    __HAL_RCC_SPI2_CLK_ENABLE();

    __HAL_RCC_GPIOC_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();
    /**SPI2 GPIO Configuration
    PC2     ------> SPI2_MISO
    PC3     ------> SPI2_MOSI
    PB13     ------> SPI2_SCK
    */
    GPIO_InitStruct.Pin = GPIO_PIN_2|GPIO_PIN_3;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF5_SPI2;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_13;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF5_SPI2;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    /* SPI2 interrupt Init */
    HAL_NVIC_SetPriority(SPI2_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(SPI2_IRQn);
  /* USER CODE BEGIN SPI2_MspInit 1 */

  /* USER CODE END SPI2_MspInit 1 */
  }
}

void HAL_SPI_MspDeInit(SPI_HandleTypeDef* spiHandle)
{

  if(spiHandle->Instance==SPI2)
  {
  /* USER CODE BEGIN SPI2_MspDeInit 0 */

  /* USER CODE END SPI2_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_SPI2_CLK_DISABLE();

    /**SPI2 GPIO Configuration
    PC2     ------> SPI2_MISO
    PC3     ------> SPI2_MOSI
    PB13     ------> SPI2_SCK
    */
    HAL_GPIO_DeInit(GPIOC, GPIO_PIN_2|GPIO_PIN_3);

    HAL_GPIO_DeInit(GPIOB, GPIO_PIN_13);

    /* SPI2 interrupt Deinit */
    HAL_NVIC_DisableIRQ(SPI2_IRQn);
  /* USER CODE BEGIN SPI2_MspDeInit 1 */

  /* USER CODE END SPI2_MspDeInit 1 */
  }
}

/* USER CODE BEGIN 1 */
void CS_Enable(void){
	HAL_GPIO_WritePin(SS_SPI_GPIO_Port, SS_SPI_Pin,0);
}
void CS_Disable(void){
	HAL_GPIO_WritePin(SS_SPI_GPIO_Port, SS_SPI_Pin,1);
}
float Leer_Presion(void){
	uint8_t master_output_slave_input[4] = {0xF7, 0x00, 0x00, 0x00}; //lee con la mascara 0x80 y luego manda dummy para seguir leyendo
	uint8_t txF4[2] = {0x74, 0b00010110}; //oversampling pressure y forced 39ms //manda la escritura de FORCED para que lea el ADC 1 vez
	// se activa el oversampling de presion asi como tambien el modo forzado
	uint8_t rx[4] = {0}; // para los 4 datos respuesta del sensor donde solo los 3 ultimos tendrán informacion
	uint32_t result;
	CS_Enable(); //Activa la comunicacion del sensor
	HAL_SPI_TransmitReceive(&hspi2,
				txF4,
				rx,
				2,
				200);//Asignando un registro de MODE, toma un dato del ADC
	CS_Disable(); //desabilita la comunicacion en espera de la toma de datos

	HAL_Delay(50); // la toma de datos de parte del sensor puede durar 39ms teoricamente(oversampling), a pesar de esto se espera un tiempo adicional

	CS_Enable(); // pasado el tiempo correspondiente se extraen los datos del ADC
	HAL_SPI_TransmitReceive(&hspi2,
			master_output_slave_input,
			rx,
			4,
			200);// se extraen los 4 datos de PRESION
	CS_Disable();
	//1111 0000 -> 0000 0000 1111
	//1111 1111 -> 1111 1111 0000
	// Se ensamblan los datos dado que vienen separados los 20bits
	// Se extraen los 20bits a pesar de que no se usa el iir, unicamente el oversampling
	result = (rx[3]>>4) | (rx[2]<<4) | (rx[1]<<12);
	return (float)result;
}
float Leer_Temperatura(void){
	uint8_t master_output_slave_input[4] = {0xFA, 0x00, 0x00, 0x00};//lee el registro de temperatura en 0xFA los demas son dummy
	uint8_t txF4[2] = {0x74, 0b10100010}; //oversampling temperature y forced 39ms
	//se configura el oversampling de x16 para la temperatura y se activa el modo forzado
	uint8_t rx[4] = {0};
	uint32_t result;
	CS_Enable();
	HAL_SPI_TransmitReceive(&hspi2,
				txF4,
				rx,
				2,
				200);
	CS_Disable();

	HAL_Delay(50);

	CS_Enable();
	HAL_SPI_TransmitReceive(&hspi2,
			master_output_slave_input,
			rx,
			4,
			200);
	CS_Disable();
	//1111 0000 -> 0000 0000 1111
	//1111 1111 -> 1111 1111 0000
	// Se reconstruyen los datos
	result = (rx[3]>>4) | (rx[2]<<4) | (rx[1]<<12);
	return (float)result;
}
void Config_BMX280(){
	uint8_t txF2[2] = {0x72, 0b000}; //oversampling de humedad en 0, no lee la humedad
	uint8_t txF4[2] = {0x74, 0b10110100};
	// oversampling temperatura, overs_presion, mode(sleep)
	uint8_t txF5[2] = {0x75, 0b00000000};
	//time stand by -- iir -- 3wire
	uint8_t rx[4] = {0};
	uint32_t result=0;
	CS_Enable(); // Prepara al chip para tomar datos
	HAL_SPI_TransmitReceive(&hspi2,
			txF2,
			rx,
			2,
			200); //Manda la primera direccion de memoria para asignar la humedad
	CS_Disable();
	CS_Enable();
	HAL_SPI_TransmitReceive(&hspi2,
			txF4,
			rx,
			2,
			200); //Manda el segundo registro de oversampling de temperatura presision y modo
	CS_Disable();
	CS_Enable();
	HAL_SPI_TransmitReceive(&hspi2,
			txF5,
			rx,
			2,
			200);// Configura el standby filtro y tipo de comunicacion
	CS_Disable(); // termina la configuracion de registros
	bme_read_calibration(); //inicia la toma de datos para extraer las calibraciones del fabricante
}

void bme_read_calibration(void)
{
    uint8_t c[25]; // son 24 datos distintos de calibracion de temperatura y presion
    uint8_t tx[25];// son 25 respuestas distintas dado la primera que es un dummy
    tx[0] = 0x88; //en el registro 0x88 empiezan las calibraciones
    //bme_read(REG_CALIB, c, 24);
    CS_Enable();
    HAL_SPI_TransmitReceive(&hspi2,tx,c,25,400);
    CS_Disable(); //dada la configuracion little endian hay que invertir cada byte
    dig_T1 = (uint16_t)(c[2]  << 8 | c[1]);
    dig_T2 = (int16_t) (c[4]  << 8 | c[3]);
    dig_T3 = (int16_t) (c[6]  << 8 | c[5]);
    dig_P1 = (uint16_t)(c[8]  << 8 | c[7]);
    dig_P2 = (int16_t) (c[10]  << 8 | c[9]);
    dig_P3 = (int16_t) (c[12] << 8 | c[11]);
    dig_P4 = (int16_t) (c[14] << 8 | c[13]);
    dig_P5 = (int16_t) (c[16] << 8 | c[15]);
    dig_P6 = (int16_t) (c[18] << 8 | c[17]);
    dig_P7 = (int16_t) (c[20] << 8 | c[19]);
    dig_P8 = (int16_t) (c[22] << 8 | c[21]);
    dig_P9 = (int16_t) (c[24] << 8 | c[23]);
}
/* USER CODE END 1 */

