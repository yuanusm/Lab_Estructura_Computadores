/*
 * interrupciones.c
 *
 *  Created on: 7 sept 2026
 *      Author: Saile
 */


#include <my_it_callback.h>
#include "main.h"
#include <stdlib.h>
#include "tim.h"
#include "MaquinaDeEstado.h"

uint16_t Periodo[4] = {100-1u, 200-1u, 400-1u, 4000-1u};

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    if (htim->Instance == TIM6)
    {

    	WriteLedPinPattern(semillarda);
    }
}
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin){
	semillarda = __HAL_TIM_GET_COUNTER(&htim2);
	srand(semillarda);
	if(GPIO_Pin == Pulsador0_Pin){
		__HAL_TIM_SET_AUTORELOAD(&htim6, Periodo[indexPeriodo]);
		__HAL_TIM_SET_COUNTER(&htim6, 0);

		indexPeriodo += 1u ;
		indexPeriodo = indexPeriodo == 4 ? 0 : indexPeriodo;

	}else if(GPIO_Pin == Interruptor0_Pin || GPIO_Pin == Interruptor1_Pin){
		HAL_GPIO_ReadPin(Interruptor0_GPIO_Port, Interruptor0_Pin) ? BIT_SET(*PntState, 0) : BIT_CLEAR(*PntState, 0) ;
		HAL_GPIO_ReadPin(Interruptor1_GPIO_Port, Interruptor1_Pin) ? BIT_SET(*PntState, 1) : BIT_CLEAR(*PntState, 1) ;
	}
}
