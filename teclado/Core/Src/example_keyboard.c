/***************************************************
 * Nombre del modulo: example_keyboard.h
 *
 * Modulo creado para la asignatura Elo312
 * Laboratorio de Estructura de Computadores
 * del departamento de Electrónica de la Universidad
 * Técnica Federico Santa María. El uso o copia
 * esté permitido y se agracede mantener el nombre
 * de los creadores.
 *
 * Escrito inicialmente el 30/03/2020 Por Mauricio Solís
 *
 * Descripción del módulo:
 * Ejemplo simple de utilización del driver del teclado.
 *
 ***************************************************/

/*<--------------- Include section ------------------->*/

#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <math.h>
#include "keyboard.h"
#include "display.h"
#include "example_keyboard.h"
#include "adc.h"
#include "i2c.h"
#include "spi.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"





uint8_t RTCConfig[9] = {0x00,
						Reg0,
						Reg1,
						Reg2,
						Reg3,
						Reg4,
						Reg5,
						Reg6,
						Reg7
};


uint8_t RTCRead[7];

uint8_t segundos;
uint8_t minutos;
uint8_t seg;
uint8_t Decseg;
uint8_t	min;
uint8_t	Decmin;
uint8_t	hr;
uint8_t	Dechr;
uint8_t DiaSel;

uint8_t ufecha;
uint8_t	Dechfecha;
uint8_t	mehs;
uint8_t	Decmes;
uint8_t	anuel;
uint8_t	AnuelBr;

char *day;
char semana[7][9]= {"Lunes",
				"Martes",
				"Miercoles",
				"Jueves",
				"Viernes",
				"Sábado",
				"Momingo"};
uint8_t flag_escribir_uart = 0;
float lectura_presion = 0;
float lectura_temperatura = 0;
int32_t temperatura_compensada =0;
uint32_t presion_compensada = 0;
int32_t  t_fine;
uint16_t dig_T1;
int16_t dig_T2, dig_T3;
uint16_t dig_P1;
int16_t dig_P2, dig_P3, dig_P4, dig_P5, dig_P6, dig_P7, dig_P8, dig_P9;


uint32_t lecturaADC;
float resultado;
/*--->> Private Constants <<--------------------------------------------------*/


/*--->> Private Data Types <<-------------------------------------------------*/


/*--->> Private Data <<-------------------------------------------------------*/


/*--->> Private Prototypes <<-------------------------------------------------*/


/*--->> API Implementation <<-------------------------------------------------*/

void example_keyboard_1(uint8_t key)
{
  static uint8_t tx_buffer[20];
  if(key < 4)
  {
    HAL_GPIO_TogglePin(LD2_GPIO_Port, LD2_Pin);
  }






  switch(key){
  case 1:
	  	  display_set_pos(0x00);;
  	  	  display_clear();
  	  	printf("%u%u-%u%u-%u%u %u%u:%u%u",
  	  	       Dechfecha, ufecha,
  	  	       Decmes, mehs,
  	  	       AnuelBr, anuel,
  	  	       Dechr, hr,
  	  	       Decmin, min);
	  break;
  case 2:
	  	  display_set_pos(0x00);;
	  	  display_clear();
	  	  printf("%u%u-%u%u-%u%u %u%u:%u%u\n\r%u hPa",
	  		  	  	  	       Dechfecha, ufecha,
	  		  	  	  	       Decmes, mehs,
	  		  	  	  	       AnuelBr, anuel,
	  		  	  	  	       Dechr, hr,
	  		  	  	  	       Decmin, min,
							   presion_compensada);
	  break;
  case 3:
	  	  display_set_pos(0x00);;
	  	  display_clear();
	  	  printf("%u%u-%u%u-%u%u %u%u:%u%u\n\r%u lux",
	  		  	  	  	       Dechfecha, ufecha,
	  		  	  	  	       Decmes, mehs,
	  		  	  	  	       AnuelBr, anuel,
	  		  	  	  	       Dechr, hr,
	  		  	  	  	       Decmin, min, (uint32_t)get_ldr());
	  break;
  case 5:
	  	  display_set_pos(0x00);;
	  	  display_clear();
	  	printf("%u%u-%u%u-%u%u %u%u:%u%u\n\r%u hPa %u lux",
	  		  		  	  	  	       Dechfecha, ufecha,
	  		  		  	  	  	       Decmes, mehs,
	  		  		  	  	  	       AnuelBr, anuel,
	  		  		  	  	  	       Dechr, hr,
	  		  		  	  	  	       Decmin, min,
									   presion_compensada,
									   (uint32_t)get_ldr());
	  	break;
  default:
	  break;
  }























  //snprintf((char*)tx_buffer, 20, "Tecla: %d\n", key);

  //HAL_UART_Transmit(&huart2, tx_buffer, strlen((const char*)tx_buffer),1000);

}

void example_keyboard_check_key_isr()
{
  static uint8_t but_tic = 0;

  if(but_tic == 9)
  {
    keyboard_tec_matrix();
    but_tic = 0;
  }
  else
  {
    but_tic += 1;
  }
}






float conversion = 0.000811233211233;

float get_ldr(void){
	float x,y;
	HAL_ADC_Start_IT(&hadc3);
	lecturaADC = HAL_ADC_GetValue(&hadc3);
	resultado = (double)lecturaADC * conversion;
	x = 7.5f / resultado;
	y = expf((10.0f / 7.0f) * ( logf(x) + 4));
	return y;
}







void Actualizar_Display(void){
	HAL_I2C_Mem_Read(&hi2c2, SlaveAddR,0x00,1, RTCRead, 7,100);

			  //I2C_HandleTypeDef *hi2c, uint16_t DevAddress, uint16_t MemAddress,
			  //                                   uint16_t MemAddSize, uint8_t *pData, uint16_t Size, uint32_t Timeout
			  seg = RTCRead[0] & 0b00001111;
			  Decseg = (RTCRead[0] & 0b01110000) >> 4;
			  min = RTCRead[1] & 0b00001111;
			  Decmin = (RTCRead[1] & 0b01110000) >> 4 ;
			  hr = RTCRead[2] & 0b00001111;
			  Dechr = (RTCRead[2] & 0b00110000) >> 4 ;
			  DiaSel = RTCRead[3] & 0b00000111;
			  switch (DiaSel) {
				  default : day = semana[0] ; break;
				  case 1: day = semana[1]; break;
				  case 2: day = semana[2]; break;
				  case 3: day = semana[3]; break;
				  case 4: day = semana[4]; break;
				  case 5: day = semana[5]; break;
				  case 6: day = semana[6]; break;

			  }
			  ufecha = RTCRead[4] & 0b00001111;
			  Dechfecha = (RTCRead[4] & 0b00110000) >> 4 ;
			  mehs =  RTCRead[5] & 0b00001111;
			  Decmes = (RTCRead[5] & 0b00010000) >> 4 ;
			  anuel = RTCRead[6] & 0b00001111;
			  AnuelBr = (RTCRead[6] & 0b11110000) >> 4 ;




			  //__disable_irq(); //desactiva la interrupcion
			  lectura_presion = Leer_Presion(); //lee el valor crudo de la presion
			  lectura_temperatura = Leer_Temperatura(); // lee el valor crudo de la temperatura
			  temperatura_compensada =BME280_compensate_T_int32(lectura_temperatura); //Se utiliza los datos de la libreria para
			  //compensar las no linealidades del sensor
			  //El fabricante deja un codigo dentro del datasheet para calcular este ajuste
			  presion_compensada = BME280_compensate_P_int32(lectura_presion);
			  //printf("%.2f	-	%.1f\n\r",(float)(temperatura_compensada/100.0f), (float)(presion_compensada/10.0f));
			  //printf("Presion: %.1f\n\r",(float)(presion_compensada/10.0f));
			  //printf("")

			  //printf("AñoD:%u\nAño:%u\nMesD:%u\nMes:%u\nDiaD:%u\nDia%u\nHoraD%u\nHora%u\nMinutoD:%u\nMinuto:%u\nSegD:%u\nSeg:%u Presion: %.1f\n\r", AnuelBr, anuel,Decmes,mehs,Dechfecha,ufecha,Dechr,hr,Decmin,min,Decseg,seg, (float)presion_compensada);
			  //printf("20%u%u-%u%u-%u%u %u%u:%u%u:%u%u Presion [hPa]: %.1f\n\r", AnuelBr, anuel,Decmes,mehs,Dechfecha,ufecha,Dechr,hr,Decmin,min,Decseg,seg, (float)presion_compensada/100.f);//Escribe en la consola serial
			   //Baja la flag del timer , asi permite que dentro de 1000ms se vuelva a activar
			  //__enable_irq();
}
