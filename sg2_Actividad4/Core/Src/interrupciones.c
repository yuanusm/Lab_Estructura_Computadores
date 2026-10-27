/*
 * interrupciones.c
 *
 *  Created on: 31 ago 2026
 *      Author: Saile
 */
#include "main.h"
#include "interrupciones.h"

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin){
	if(GPIO_Pin == GPIO_PIN_13){
		PartidaToggle();
	}

}
