/*
 * MaquinaDeEstado.h
 *
 *  Created on: 7 sept 2026
 *      Author: Saile
 */

#ifndef INC_MAQUINADEESTADO_H_
#define INC_MAQUINADEESTADO_H_

#include <stdint.h>

#define BIT_SET(reg, n)     ( (reg) |= (1u << (n)) )
#define BIT_CLEAR(reg, n)   ( (reg) &= ~(1u << (n)) )
#define BIT_TOGGLE(reg, n)  ( (reg) ^= (1u << (n)) )
#define BIT_READ(reg, n)    ( ((reg) >> (n)) & (1u) )


void Maquinon(void);
void WriteLedPinPattern(uint32_t seed);

typedef enum _State {
	  Patron1,
	  Patron2,
	  Patron3,
	  Patron4}
  	  State;

extern uint8_t indexPeriodo;
extern State State1;
extern State* PntState;
extern uint16_t semillarda;

#endif /* INC_MAQUINADEESTADO_H_ */
