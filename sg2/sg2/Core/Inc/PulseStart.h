/*
 * PulseStart.h
 *
 *  Created on: 24-08-2026
 *      Author: viceo
 */

#ifndef INC_PULSESTART_H_
#define INC_PULSESTART_H_
#define BIT_CLEAR(reg, n)   ( (reg) &= ~(1u << (n)) )
#define BIT_READ(reg, n)    ( ((reg) >> (n)) & (1u) )

void PulseStart(void);

#endif /* INC_PULSESTART_H_ */
