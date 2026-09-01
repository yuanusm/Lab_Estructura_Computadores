/*
 * MaquinaDeEstado.c
 *
 *  Created on: 24 ago 2026
 *      Author: Saile
 */

#include "stm32l476xx.h"
#include "MaquinaDeEstado.h"
#include "stm32l4xx_hal.h"
#define BIT_SET(reg, n)     ( (reg) |= (1u << (n)) )
#define BIT_CLEAR(reg, n)   ( (reg) &= ~(1u << (n)) )
#define BIT_TOGGLE(reg, n)  ( (reg) ^= (1u << (n)) )
#define BIT_READ(reg, n)    ( ((reg) >> (n)) & (1u) )

void MaquinaDeEstado(void){
	enum _State  {Encendido, Parpadeo};
	enum _State State_n;
	enum _State State;
	char Boton, Boton_anterior;
	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOAEN; //Activa clock del GPIO A?
	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOCEN; //Activa clock del GPIO C?
/*
 *	BIT_SET(GPIOA->MODER, 10);
 *	BIT_CLEAR(GPIOA->MODER, 11); // set 01 en el registro moder por lo que se configura como General Purpose OUTPUT
 *	BIT_CLEAR(GPIOA->OTYPER, 5); // set 0 PUSH PULL
 *	BIT_SET(GPIOA->OSPEEDR, 10);
 *	BIT_CLEAR(GPIOA->OSPEEDR, 11); // set 01 en el registro de speed por lo que queda en medium speed
 *	BIT_CLEAR(GPIOA->PUPDR, 10);	// clear 00 para que la salida quede como push - pull
 *	BIT_CLEAR(GPIOA->PUPDR, 11);
 */
	//BIT_SET(GPIOA->BSRR, 5);
	//BIT_SET(GPIOA->BSRR, 21);


	GPIO_InitTypeDef Pin_Led;
	Pin_Led.Pin = GPIO_PIN_5;
	Pin_Led.Mode = GPIO_MODE_OUTPUT_PP;
	Pin_Led.Pull = GPIO_NOPULL;
	Pin_Led.Speed = GPIO_SPEED_FREQ_LOW;

	HAL_GPIO_Init(GPIOA, &Pin_Led);

	HAL_GPIO_WritePin( GPIOA, GPIO_PIN_5, GPIO_PIN_SET);





//	GPIO_InitTypeDef Pin_Led;
//	Pin_Led.Pin = GPIO_PIN_5;
//	Pin_Led.Mode = GPIO_MODE_OUTPUT_PP;
//	Pin_Led.Pull = GPIO_NOPULL;
//	Pin_Led.Speed = GPIO_SPEED_FREQ_LOW;
//
//	HAL_GPIO_Init(GPIOA, &Pin_Led);v

	GPIO_InitTypeDef Pin_Boton;
	Pin_Led.Pin = GPIO_PIN_13;
	Pin_Led.Mode = GPIO_MODE_INPUT;
	Pin_Led.Pull = GPIO_NOPULL;

	HAL_GPIO_Init(GPIOC, &Pin_Boton);

//	BIT_CLEAR(GPIOC->MODER, 26);
//	BIT_CLEAR(GPIOC->MODER, 27); // set 01 en el registro moder por lo que se configura como INPUT
//	BIT_CLEAR(GPIOC->PUPDR, 26);	// clear 00 para que la salida quede como input floating
//	BIT_CLEAR(GPIOC->PUPDR, 27);

	//
	State = Encendido;
	Boton_anterior = 0;

	while(1){
		Boton = BIT_READ(GPIOC->IDR, 13);

		if(State == Encendido){
			//BIT_SET(GPIOA->BSRR, 5);
			HAL_GPIO_WritePin( GPIOA, GPIO_PIN_5, GPIO_PIN_SET);
			if((Boton_anterior) && !Boton){
				State_n = Parpadeo;
			}else{
				State_n = Encendido;
			}
		}else if(State == Parpadeo){
			HAL_GPIO_TogglePin( GPIOA, GPIO_PIN_5);
			HAL_Delay(500);
			if((Boton_anterior) && !Boton){
				State_n = Encendido;
			}else{
				State_n = Parpadeo;
			}
		}else{
			State_n = Encendido;
			break;
		}

		State = State_n;
		Boton_anterior = Boton;
	}
}
