/*
 * Actividad7.c
 *
 *  Created on: 24 ago 2026
 *      Author: Saile
 */

#include "Actividad5.h"

int elevar_cuadrado(int entrada){
	return entrada * entrada;

}

float area_circulo(int radio){
	return PI * elevar_cuadrado(radio);
	//return PI * radio * radio;
}

char neg_or_pos(int number){
	if(number < 0){
		return 'n';
	}else if(number > 0){
		return 'p';
	}else {
		return 'z';
	}
}
