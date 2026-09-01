#include "stm32l476xx.h"

#include "template_1.h"
/*
 *
 *	#define BIT_SET(reg, n)     ( (reg) |= (1u << (n)) )
	#define BIT_CLEAR(reg, n)   ( (reg) &= ~(1u << (n)) )
	#define BIT_TOGGLE(reg, n)  ( (reg) ^= (1u << (n)) )
	#define BIT_READ(reg, n)    ( ((reg) >> (n)) & (1u) )
 *
 *
 *
 *
 *	typedef struct
{
/*  __IO uint32_t MODER;       /*!< GPIO port mode register,               Address offset: 0x00      */
/*  __IO uint32_t OTYPER;      /*!< GPIO port output type register,        Address offset: 0x04      */
 /* __IO uint32_t OSPEEDR;     /*!< GPIO port output speed register,       Address offset: 0x08      */
  /*__IO uint32_t PUPDR;       /*!< GPIO port pull-up/pull-down register,  Address offset: 0x0C      */
  /*__IO uint32_t IDR;         /*!< GPIO port input data register,         Address offset: 0x10      */
  /*__IO uint32_t ODR;         /*!< GPIO port output data register,        Address offset: 0x14      */
  /*__IO uint32_t BSRR;        /*!< GPIO port bit set/reset  register,     Address offset: 0x18      */
 /* __IO uint32_t LCKR;        /*!< GPIO port configuration lock register, Address offset: 0x1C      */
 /* __IO uint32_t AFR[2];      /*!< GPIO alternate function registers,     Address offset: 0x20-0x24 */
  /* __IO uint32_t BRR;         /*!< GPIO Bit Reset register,               Address offset: 0x28      */
 /* __IO uint32_t ASCR;        /*!< GPIO analog switch control register,   Address offset: 0x2C     */
/*
/*} GPIO_TypeDef;
 *
 *
 *
 *
 * */
void Config_Clock_GPIOA(void)
{
	/*Habilitar reloj para GPIOA*/
    RCC->AHB2ENR |= RCC_AHB2ENR_GPIOAEN;

    /*Modificar registro MODER para configurar puerto A pin 5 como salida*/


    BIT_SET(GPIOA->MODER, 10);
    BIT_CLEAR(GPIOA->MODER, 11); // set 01 en el registro moder por lo que se configura como General Purpose OUTPUT

    BIT_CLEAR(GPIOA->OTYPER, 5); // set 0 PUSH PULL

    BIT_SET(GPIOA->OSPEEDR, 10);
    BIT_CLEAR(GPIOA->OSPEEDR, 11); // set 01 en el registro de speed por lo que queda en medium speed

    BIT_CLEAR(GPIOA->PUPDR, 10);	// clear 00 para que la salida quede como push - pull
    BIT_CLEAR(GPIOA->PUPDR, 11);


    while (1)
    {
    	/*Modificar registro BSRR para levantar el bit 5 del puerto A*/
    	BIT_SET(GPIOA->BSRR, 5); // set bit 5 para setear en 1 la salida 5
    	BIT_SET(GPIOA->BSRR, 21);// set bit 16+ 5 para clear la salida 5

    	/*Modificar registro BSRR para bajar el bit 5 del puerto A*/

    }
}
