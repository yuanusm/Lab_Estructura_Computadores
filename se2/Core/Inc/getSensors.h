/*
 * getSensors.h
 *
 *  Created on: 5 oct 2026
 *      Author: Saile
 */

#ifndef INC_GETSENSORS_H_
#define INC_GETSENSORS_H_


#define SlaveAddR  0b11010001
#define SlaveAddW  0b11010000


#include "main.h"
#include <math.h>
#include <stdio.h>
#include <stdint.h>



extern uint8_t comando;




void Config_BMX280(void);
void bme_read_calibration(void);
void Config_Sensor(void);
float get_pressure(void);
char* get_rtc(void);
float get_temperature(void);
float get_ldr(void);
void set_rtc(uint8_t* SetTime);

#endif /* INC_GETSENSORS_H_ */
