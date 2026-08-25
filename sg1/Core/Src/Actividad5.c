/*
 * Actividad7.c
 *
 *  Created on: 24 ago 2026
 *      Author: Saile
 */

#include "Actividad5.h"

int elevar_cuadrado(int entrada){

	int salida;
	salida = entrada * entrada;
	return salida;

}

////////////////

float area_circulo(int radio){
	return PI * radio * radio;

}

char neg_or_pos(int number){

	if(number < 0){

		return 'n';

	} else if(number >0){

		return 'p';
	} else {

		return 'z';

	}

}


////////////////////////

int funcion_1(int x, int y){

	x = y;
	return x;
}
int funcion_2(int *x, int *y){
	*x = *y;
	return *x;
}


int Actividad3_A5(int *enteros){
    int res = 0;

    for(int i = 0; i <= 5; i++){
        res += *(enteros + i);
    }
    return res;
}
