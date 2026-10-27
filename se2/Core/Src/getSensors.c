/*
 * getSensors.c
 *
 *  Created on: 5 oct 2026
 *      Author: Saile
 */


#include "getSensors.h"
#include "i2c.h"
#include "spi.h"
#include "usart.h"
#include "adc.h"
#include "Compensada.h"



//I2C | RTC
uint8_t seg;
uint8_t Decseg;
uint8_t	min;
uint8_t	Decmin;
uint8_t	hr;
uint8_t	Dechr;
uint8_t ufecha;
uint8_t	Dechfecha;
uint8_t	mehs;
uint8_t	Decmes;
uint8_t	anuel;
uint8_t	AnuelBr;


uint32_t lecturaADC;
float resultado;

int32_t  t_fine;
uint16_t dig_T1;
int16_t dig_T2, dig_T3;
uint16_t dig_P1;
int16_t dig_P2, dig_P3, dig_P4, dig_P5, dig_P6, dig_P7, dig_P8, dig_P9;



unsigned char RTCRead[7];

char LecRTC[50];



char* get_rtc(void){
	HAL_I2C_Mem_Read(&hi2c2, SlaveAddR,0x00,1, RTCRead, 7,100);

	seg = RTCRead[0] & 0b00001111;
	Decseg = (RTCRead[0] & 0b01110000) >> 4;
	min = RTCRead[1] & 0b00001111;
	Decmin = (RTCRead[1] & 0b01110000) >> 4 ;
    hr = RTCRead[2] & 0b00001111;
	Dechr = (RTCRead[2] & 0b00110000) >> 4 ;
    ufecha = RTCRead[4] & 0b00001111;
    Dechfecha = (RTCRead[4] & 0b00110000) >> 4 ;
    mehs =  RTCRead[5] & 0b00001111;
    Decmes = (RTCRead[5] & 0b00010000) >> 4 ;
    anuel = RTCRead[6] & 0b00001111;
    AnuelBr = (RTCRead[6] & 0b11110000) >> 4 ;

    sprintf(LecRTC, "20%u%u-%u%u-%u%u %u%u:%u%u:%u%u",
				  AnuelBr, anuel,
				  Decmes,mehs,
				  Dechfecha,ufecha,
				  Dechr,hr,
				  Decmin,min,
				  Decseg,seg);

    return LecRTC;
}






float lectura_presion = 0;
float lectura_temperatura = 0;
int32_t temperatura_compensada =0;
uint32_t presion_compensada = 0;


float get_temperature(void){
	uint8_t master_output_slave_input[4] = {0xFA, 0x00, 0x00, 0x00};//lee el registro de temperatura en 0xFA los demas son dummy
	uint8_t txF4[2] = {0x74, 0b10100010}; //oversampling temperature y forced 39ms
	//se configura el oversampling de x16 para la temperatura y se activa el modo forzado
	uint8_t rx[4] = {0};
	uint32_t result;
	CS_Enable();
	HAL_SPI_TransmitReceive(&hspi2,
				txF4,
				rx,
				2,
				200);
	CS_Disable();

	HAL_Delay(50);

	CS_Enable();
	HAL_SPI_TransmitReceive(&hspi2,
			master_output_slave_input,
			rx,
			4,
			200);
	CS_Disable();
	//1111 0000 -> 0000 0000 1111
	//1111 1111 -> 1111 1111 0000
	// Se reconstruyen los datos
	result = (rx[3]>>4) | (rx[2]<<4) | (rx[1]<<12);
	return (float)result;
}

float get_pressure(void){
	uint8_t master_output_slave_input[4] = {0xF7, 0x00, 0x00, 0x00}; //lee con la mascara 0x80 y luego manda dummy para seguir leyendo
	uint8_t txF4[2] = {0x74, 0b00010110}; //oversampling pressure y forced 39ms //manda la escritura de FORCED para que lea el ADC 1 vez
	// se activa el oversampling de presion asi como tambien el modo forzado
	uint8_t rx[4] = {0}; // para los 4 datos respuesta del sensor donde solo los 3 ultimos tendrán informacion
	uint32_t result;
	CS_Enable(); //Activa la comunicacion del sensor
	HAL_SPI_TransmitReceive(&hspi2,
				txF4,
				rx,
				2,
				200);//Asignando un registro de MODE, toma un dato del ADC
	CS_Disable(); //desabilita la comunicacion en espera de la toma de datos

	HAL_Delay(50); // la toma de datos de parte del sensor puede durar 39ms teoricamente(oversampling), a pesar de esto se espera un tiempo adicional

	CS_Enable(); // pasado el tiempo correspondiente se extraen los datos del ADC
	HAL_SPI_TransmitReceive(&hspi2,
			master_output_slave_input,
			rx,
			4,
			200);// se extraen los 4 datos de PRESION
	CS_Disable();
	//1111 0000 -> 0000 0000 1111
	//1111 1111 -> 1111 1111 0000
	// Se ensamblan los datos dado que vienen separados los 20bits
	// Se extraen los 20bits a pesar de que no se usa el iir, unicamente el oversampling
	result = (rx[3]>>4) | (rx[2]<<4) | (rx[1]<<12);


	lectura_presion = (float)result; //lee el valor crudo de la presion
	lectura_temperatura = get_temperature(); // lee el valor crudo de la temperatura
	temperatura_compensada =BME280_compensate_T_int32(lectura_temperatura); //Se utiliza los datos de la libreria para
	//compensar las no linealidades del sensor
	//El fabricante deja un codigo dentro del datasheet para calcular este ajuste
	presion_compensada = BME280_compensate_P_int32(lectura_presion);

	return (float)presion_compensada;
}


float conversion = 0.000811233211233;
float x,y;
float get_ldr(void){
	HAL_ADC_Start_IT(&hadc1);
	lecturaADC = HAL_ADC_GetValue(&hadc1);
	resultado = (double)lecturaADC * conversion;
	x = 7.5f / resultado;
	y = expf((10.0f / 7.0f) * ( logf(x) + 4));
	return y;
}



void Config_Sensor(){
	uint8_t txF2[2] = {0x72, 0b000}; //oversampling de humedad en 0, no lee la humedad
	uint8_t txF4[2] = {0x74, 0b10110100};
	// oversampling temperatura, overs_presion, mode(sleep)
	uint8_t txF5[2] = {0x75, 0b00000000};
	//time stand by -- iir -- 3wire
	uint8_t rx[4] = {0};
	//uint32_t result=0;
	CS_Enable(); // Prepara al chip para tomar datos
	HAL_SPI_TransmitReceive(&hspi2,
			txF2,
			rx,
			2,
			200); //Manda la primera direccion de memoria para asignar la humedad
	CS_Disable();
	CS_Enable();
	HAL_SPI_TransmitReceive(&hspi2,
			txF4,
			rx,
			2,
			200); //Manda el segundo registro de oversampling de temperatura presision y modo
	CS_Disable();
	CS_Enable();
	HAL_SPI_TransmitReceive(&hspi2,
			txF5,
			rx,
			2,
			200);// Configura el standby filtro y tipo de comunicacion
	CS_Disable(); // termina la configuracion de registros
	bme_read_calibration(); //inicia la toma de datos para extraer las calibraciones del fabricante
}


void bme_read_calibration(void)
{
    uint8_t c[25]; // son 24 datos distintos de calibracion de temperatura y presion
    uint8_t tx[25];// son 25 respuestas distintas dado la primera que es un dummy
    tx[0] = 0x88; //en el registro 0x88 empiezan las calibraciones
    //bme_read(REG_CALIB, c, 24);
    CS_Enable();
    HAL_SPI_TransmitReceive(&hspi2,tx,c,25,400);
    CS_Disable(); //dada la configuracion little endian hay que invertir cada byte
    dig_T1 = (uint16_t)(c[2]  << 8 | c[1]);
    dig_T2 = (int16_t) (c[4]  << 8 | c[3]);
    dig_T3 = (int16_t) (c[6]  << 8 | c[5]);
    dig_P1 = (uint16_t)(c[8]  << 8 | c[7]);
    dig_P2 = (int16_t) (c[10]  << 8 | c[9]);
    dig_P3 = (int16_t) (c[12] << 8 | c[11]);
    dig_P4 = (int16_t) (c[14] << 8 | c[13]);
    dig_P5 = (int16_t) (c[16] << 8 | c[15]);
    dig_P6 = (int16_t) (c[18] << 8 | c[17]);
    dig_P7 = (int16_t) (c[20] << 8 | c[19]);
    dig_P8 = (int16_t) (c[22] << 8 | c[21]);
    dig_P9 = (int16_t) (c[24] << 8 | c[23]);
}

#define Reg0 (0<<7u | 3u<<4u | 3u)//00110011   /// CH [7] | Decseg[6-4] | Seg [3-0]
#define Reg1 ( 1u<<4u | 6u)	  /// x[7] | DecMin[6-4] | Min[3-0]
#define Reg2 ( 0<<6u | 1u<<4u | 5)	  /// x[7] | 12-24 hrs_set [6] (24low) | DecHora[5-4] AM-PM[5] | Hora[3-0]
#define Reg3 ( 0b1)	  /// X[7-3] | Dia [2-0]
#define Reg4 ( 2<<4 | 9)  /// X[7-6] | DecFecha [5-4] | Fecha[3-0]
#define Reg5 (0<<4 | 9)   /// X[7-5] | DecMes [4] | Mes[3-0]
#define Reg6 ( 2<<4 | 6)   /// DecAño [7:4] | Año[3-0]
#define Reg7 0b00000000   /// Square/out config



//void set_rtc(uint8_t* SetTime){
//
//char errormsgYear[35] = "error año invalido (2000-2099)\n";
//char errormsgMes[35] = "error mes invalido (0-12)\n";
//char errormsgDia[35] = "error dia invalido (0-12)\n";
//
//char pano[4] = &SetTime;
//uint8_t ano = (uint8_t)strtoul(*SetTime, NULL, 10);
//
//char pmes[2] = &(SetTime+5);
//uint8_t mes = (uint8_t)strtoul(pmes, NULL, 10);
//
//char pdia[2] = &(SetTime+8);
//uint8_t dia = (uint8_t)strtoul(pdia, NULL, 10);
//
//if ((pano[0]!= '2') ||
//		(pano[1] != '0') ||
//		(pano[2] < 48) ||
//		(71 < pano[2]) ||
//		(pano[3]) < 48 ||
//		(71 < pano[3]) ){
//
//	return HAL_UART_Transmit(&huart2, errormsgYear, 32, 1000);
//
//}else if((pmes[0] != 48) &&
//		((pmes[1]) < 48 ||
//		(71 < pmes[1])) ||
//
//		(49 != pmes[0]) &&
//		((pmes[1]) < 48 ||
//		(50 < pmes[1]))){
//	return HAL_UART_Transmit(&huart2, errormsgMes, 32, 1000);
//
//}else if((pdia[0] < 48) ||
//		(51 < pdia[0]) ||
//		(pdia[1]) < 48 ||
//		(71 < pdia[1])){
//		return HAL_UART_Transmit(&huart2, errormsgDia, 32, 1000);
//
//
//}
//
//
//
//HAL_I2C_Master_Transmit(&hi2c2, SlaveAddW, RTCConfig, 9, 100);
//
//
//}
