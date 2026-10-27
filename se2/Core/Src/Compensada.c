/*
 * Compensada.h
 *
 *  Created on: 6 oct 2026
 *      Author: Saile
 */


/*
 * Compensada.c
 *
 *  Created on: 29 sept 2026
 *      Author: Saile
 */
#include "Compensada.h"

#include <stdint.h>

BME280_S32_t BME280_compensate_T_int32(BME280_S32_t adc_T){
	BME280_S32_t var1, var2, T;
	var1 = ((((adc_T>>3) - ((BME280_S32_t)dig_T1<<1))) * ((BME280_S32_t)dig_T2)) >> 11;
	var2 = (((((adc_T>>4) - ((BME280_S32_t)dig_T1)) * ((adc_T>>4) - ((BME280_S32_t)dig_T1)))
	>> 12) *
	((BME280_S32_t)dig_T3)) >> 14;
	t_fine = var1 + var2;
	T = (t_fine * 5 + 128) >> 8;
	return T;
}

// Returns pressure in Pa as unsigned 32 bit integer. Output value of “96386” equals 96386 Pa= 963.86 hPa

BME280_U32_t BME280_compensate_P_int32(BME280_S32_t adc_P){
	BME280_S32_t var1, var2;
	BME280_U32_t p;

	var1 = (((BME280_S32_t)t_fine)>>1) - (BME280_S32_t) 64000;
	var2 = (((var1>>2) * (var1>>2)) >> 11 ) * ((BME280_S32_t)dig_P6);
	var2 = var2 + ((var1*((BME280_S32_t)dig_P5))<<1);
	var2 = (var2>>2)+(((BME280_S32_t)dig_P4)<<16);
	var1 = (((dig_P3 * (((var1>>2) * (var1>>2)) >> 13 )) >> 3) + ((((BME280_S32_t)dig_P2) *
	var1)>>1))>>18;
	var1 =((((32768+var1))*((BME280_S32_t)dig_P1))>>15);

	if (var1 == 0){
		return 0; // avoid exception caused by division by zero
	}

	p = (((BME280_U32_t)(((BME280_S32_t)1048576)-adc_P)-(var2>>12)))*3125;

	if (p < 0x80000000){
		p = (p << 1) / ((BME280_U32_t)var1);
	}
	else{
		p = (p / (BME280_U32_t)var1) * 2;
	}

	var1 = (((BME280_S32_t)dig_P9) * ((BME280_S32_t)(((p>>3) * (p>>3))>>13)))>>12;
	var2 = (((BME280_S32_t)(p>>2)) * ((BME280_S32_t)dig_P8))>>13;
	p = (BME280_U32_t)((BME280_S32_t)p + ((var1 + var2 + dig_P7) >> 4));

	return p;
}
