/*
 * interrupciones.c
 *
 *  Created on: 1 sept 2026
 *      Author: Saile
 */

#include <stdint.h>
//#include "interrupciones.h"
//#include "stm32l4xx_hal_gpio.h"
#include "Actividad3.h"
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin){
	if(GPIO_Pin == ((uint16_t)0x2000) ){
		presionarboton();
	}

}
