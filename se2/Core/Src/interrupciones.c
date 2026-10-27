/*
 * interrupciones.c
 *
 *  Created on: 5 oct 2026
 *      Author: Saile
 */

#include "interrupciones.h"
#include "tim.h"
#include "usart.h"

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    if (htim->Instance == TIM6)
    {
    	flag_escribir_uart = 1;

    }else if (htim->Instance == TIM7)
        {
        	flag_segundo_caracter = 1;
        	index1 = 0;
        }
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart){


	if(huart->Instance == USART2){

		HAL_TIM_Base_Start_IT(&htim7);
		flag_segundo_caracter = 0;
		flag_leer_uart = 1;


	}
}
