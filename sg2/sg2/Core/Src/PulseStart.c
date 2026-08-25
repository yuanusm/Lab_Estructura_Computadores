/*
 * PulseStart.c
 *
 *  Created on: 24-08-2026
 *      Author: viceo
 */

#include "PulseStart.h"
#include "stm32l476xx.h"

void PulseStart(void){
	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOCEN;

	BIT_CLEAR(GPIOC -> MODER, 26); // Coloca un 0 en el bit N°26 del registro MODER en la estructura GPIOC
	BIT_CLEAR(GPIOC -> MODER, 27);   // Coloca un 1 en el bit N°27 del registro MODER en la estructura GPIOC


	BIT_CLEAR(GPIOC -> PUPDR, 10);
	BIT_CLEAR(GPIOC -> PUPDR, 11);


}
