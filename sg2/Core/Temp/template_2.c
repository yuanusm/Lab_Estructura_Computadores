#include "stm32l476xx.h"

#include "template_2.h"

void Read_GPIO_C_13(void){
	/*Habilitar reloj para GPIOC*/
    RCC->AHB2ENR |= RCC_AHB2ENR_GPIOCEN;

    /*Modificar registro MODER para configurar puerto C pin 13 como entrada*/
    //GPIOC->... fill in
    
    /*crear variable para guardar estado del pulsador*/

    char Boton = 0;

    BIT_CLEAR(GPIOC->MODER, 26);
	BIT_CLEAR(GPIOC->MODER, 27); // set 01 en el registro moder por lo que se configura como INPUT

	BIT_CLEAR(GPIOC->PUPDR, 26);	// clear 00 para que la salida quede como input floating
	BIT_CLEAR(GPIOC->PUPDR, 27);

    while (1){
    	/*Leer registro IDR para obtener el estado del bit 13 del puerto C 
    	 * y guardarlo en la variable creada*/
    	Boton = BIT_READ(GPIOC->IDR, 13);

    }
}

