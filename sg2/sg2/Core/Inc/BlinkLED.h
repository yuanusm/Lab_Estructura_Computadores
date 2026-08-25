/*
 * BlinkLED.h
 *
 *  Created on: 24-08-2026
 *      Author: viceo
 */

#ifndef INC_BLINKLED_H_
#define INC_BLINKLED_H_
#define BIT_SET(reg, n)     ( (reg) |= (1u << (n)) )
#define BIT_CLEAR(reg, n)   ( (reg) &= ~(1u << (n)) )


void StartBlinkLED(void);

#endif /* INC_BLINKLED_H_ */
