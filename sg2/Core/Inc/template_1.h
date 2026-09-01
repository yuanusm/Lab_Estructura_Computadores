/*
 * template_1.h
 *
 *  Created on: 24 ago 2026
 *      Author: Saile
 */

#ifndef INC_TEMPLATE_1_H_
#define INC_TEMPLATE_1_H_

#define BIT_SET(reg, n)     ( (reg) |= (1u << (n)) )
#define BIT_CLEAR(reg, n)   ( (reg) &= ~(1u << (n)) )
#define BIT_TOGGLE(reg, n)  ( (reg) ^= (1u << (n)) )
#define BIT_READ(reg, n)    ( ((reg) >> (n)) & (1u) )

void Config_Clock_GPIOA(void);

#endif /* INC_TEMPLATE_1_H_ */
