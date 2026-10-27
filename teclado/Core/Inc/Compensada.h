/*
 * Compensada.h
 *
 *  Created on: 29 sept 2026
 *      Author: Saile
 */

#ifndef INC_COMPENSADA_H_
#define INC_COMPENSADA_H_
#define BME280_S32_t int32_t
#define BME280_U32_t uint32_t

extern int32_t  t_fine;
extern uint16_t dig_T1;
extern int16_t dig_T2, dig_T3;
extern uint16_t dig_P1;
extern int16_t dig_P2, dig_P3, dig_P4, dig_P5, dig_P6, dig_P7, dig_P8, dig_P9;

BME280_S32_t BME280_compensate_T_int32(BME280_S32_t adc_T);
BME280_U32_t BME280_compensate_P_int32(BME280_S32_t adc_P);
#endif /* INC_COMPENSADA_H_ */
