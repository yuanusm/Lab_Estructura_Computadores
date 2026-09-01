/*
 * Actividad3.c
 *
 *  Created on: 25 ago 2026
 *      Author: Saile
 */
#include "stm32l4xx_hal.h"

#include "stm32l4xx_nucleo.h"
#include "Actividad3.h"

void Actividad3(void){



	GPIO_InitTypeDef Pin_Led;
	Pin_Led.Pin = GPIO_PIN_5;
	Pin_Led.Mode = GPIO_MODE_OUTPUT_PP;
	Pin_Led.Pull = GPIO_NOPULL;
	Pin_Led.Speed = GPIO_SPEED_FREQ_LOW;

	HAL_GPIO_Init(GPIOA, &Pin_Led);

	GPIO_InitTypeDef Pin_Boton;
	Pin_Led.Pin = GPIO_PIN_13;
	Pin_Led.Mode = GPIO_MODE_INPUT;
	Pin_Led.Pull = GPIO_NOPULL;

	HAL_GPIO_Init(GPIOC, &Pin_Boton);
	uint32_t retardos[] = {1000, 500, 200};
	uint8_t indice = 0;
	int8_t direccion = 1;
	GPIO_PinState boton_actual;
	GPIO_PinState boton_anterior = GPIO_PIN_SET;

	while (1)
	{
		/* USER CODE END WHILE */


		/* USER CODE BEGIN 3 */
		boton_actual = HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_13);

		if (boton_anterior == GPIO_PIN_SET && boton_actual == GPIO_PIN_RESET)
		{
			indice += direccion;

			if (indice == 2)
			{
				direccion = -1;
			}
			else if (indice == 0)
			{
				direccion = 1;
			}

		}
		boton_actual = boton_anterior;

		HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5);

		HAL_Delay(retardos[indice]);
		/* USER CODE END 3 */
	}
}
