/*
 * interrupciones.c
 *
 *  Created on: 7 sept 2026
 *      Author: Saile
 */

#include "my_it_callback.h"
#include "tim.h"
#include "usart.h"
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    if (htim->Instance == TIM2)
    {
    	HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5);

    }
}
