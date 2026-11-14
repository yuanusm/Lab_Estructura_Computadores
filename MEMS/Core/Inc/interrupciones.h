/*
 * interrupciones.h
 *
 *  Created on: 3 nov 2026
 *      Author: Saile
 */

#ifndef INC_INTERRUPCIONES_H_
#define INC_INTERRUPCIONES_H_

extern uint8_t flag_timer_100;
extern uint8_t flag_timer_data;
extern char comando;
extern char comando_recibido[10];
extern UART_HandleTypeDef huart2;
extern uint8_t indice;
extern char buffer[10];

#endif /* INC_INTERRUPCIONES_H_ */
