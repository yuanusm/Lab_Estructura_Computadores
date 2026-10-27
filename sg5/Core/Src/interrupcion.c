/*
 * interrupcion.c
 *
 *  Created on: 28 sept 2026
 *      Author: Saile
 */

#include "interrupcion.h"
#include "tim.h"

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    if (htim->Instance == TIM6)
    {
    	flag_escribir_uart = 1;

    }
}
