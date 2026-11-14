/*
 * interrupciones.c
 *
 *  Created on: 3 nov 2026
 *      Author: Saile
 */
#include "main.h"
#include "interrupciones.h"
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    if (htim->Instance == TIM6)
    {
    	flag_timer_100 = 1;
    }
//
//
//    HAL_TIM_Base_Start_IT(&htim6);
}


void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart){
	if(huart->Instance == USART2){
		flag_timer_data = 1;
		buffer[indice++]=comando;
		if(indice==10){
			indice=0;
		}
		HAL_UART_Receive_IT(&huart2, (uint8_t*)&comando, 1);
	}
}


