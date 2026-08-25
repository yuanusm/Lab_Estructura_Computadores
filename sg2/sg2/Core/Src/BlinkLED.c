/*
 * BlinkLED.c
 *
 *  Created on: 24-08-2026
 *      Author: viceo
 */

#include "BlinkLED.h"
#include "stm32l476xx.h"






void StartBlinkLED(void){
/*Habilitar reloj para GPIOA*/
    RCC->AHB2ENR |= RCC_AHB2ENR_GPIOAEN;

    //Reset GPIOA
    //GPIOA -> MODER[10] = 0xABFFFFFF;


    /*Modificar registro MODER para configurar puerto A pin 5 como salida*/

    //#define GPIOA               ((GPIO_TypeDef *) GPIOA_BASE)
    /*
     *
     *IO uint32_t MODER;       /*!< GPIO port mode register,               Address offset: 0x00
    _*IO uint32_t OTYPER;      !< GPIO port output type register,        Address offset: 0x04
     *IO uint32_t OSPEEDR;     /*!< GPIO port output speed register,       Address offset: 0x08
  __ *IO uint32_t PUPDR;       /*!< GPIO port pull-up/pull-down register,  Address offset: 0x0C
     */

    //Se configuran los registros(de configuración)de GPIO



}
