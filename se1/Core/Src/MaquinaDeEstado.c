/*
 * MaquinaDeEstado.c
 *
 *  Created on: 7 sept 2026
 *      Author: Saile
 */


#include "MaquinaDeEstado.h"
#include "main.h"
#include <stdlib.h>
#include <stddef.h>
#include "tim.h"

State State1;
State* PntState;
uint8_t *Patron;
uint8_t indexPeriodo = 0;
uint8_t size = 6;
uint8_t indexPatron = 0;
uint16_t semillarda;
uint8_t ArrPatron1[6] 	= {
	0b100000,
	0b010000,
	0b001000,
	0b000100,
	0b000010,
	0b000001,
	};
uint8_t ArrPatron2[10] 	= {
	0b100000,
	0b110000,
	0b111000,
	0b111100,
	0b111110,
	0b111111,
	0b111110,
	0b111100,
	0b111000,
	0b110000,
	};
uint8_t ArrPatron3[6] 	= {
	0b000000,
	0b100001,
	0b110011,
	0b111111,
	0b110011,
	0b100001,
	};
uint8_t ArrPatron4[1];

void Maquinon(void){
	PntState = &State1;

	__disable_irq();
	switch(State1){
		case Patron1:
			size = 6;
			Patron = ArrPatron1;

			break;
		case Patron2:
			size = 10;
			Patron = ArrPatron2;

			break;
		case Patron3:
			size = 6;
			Patron = ArrPatron3;

			break;
		case Patron4:
			size = 1;
			Patron = ArrPatron4;

			break;
		default:
			State1 = Patron1;
			break;
	}
	__enable_irq();

}

void WriteLedPinPattern(uint32_t seed){

	Patron == ArrPatron4 ? Patron[0] = rand() : 1;
	indexPatron %= (size);
	HAL_GPIO_WritePin(Led0_GPIO_Port, Led0_Pin, BIT_READ(Patron[indexPatron],0));
	HAL_GPIO_WritePin(Led1_GPIO_Port, Led1_Pin, BIT_READ(Patron[indexPatron],1));
	HAL_GPIO_WritePin(Led2_GPIO_Port, Led2_Pin, BIT_READ(Patron[indexPatron],2));
	HAL_GPIO_WritePin(Led3_GPIO_Port, Led3_Pin, BIT_READ(Patron[indexPatron],3));
	HAL_GPIO_WritePin(Led4_GPIO_Port, Led4_Pin, BIT_READ(Patron[indexPatron],4));
	HAL_GPIO_WritePin(Led5_GPIO_Port, Led5_Pin, BIT_READ(Patron[indexPatron],5));
	indexPatron = indexPatron+1u;
}
