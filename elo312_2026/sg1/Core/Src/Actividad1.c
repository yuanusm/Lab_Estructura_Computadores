/*
 * Actividad1.c
 *
 *  Created on: 18 ago 2026
 *      Author: Saile
 */

#include <stdio.h>
#include "Actividad1.h"


int arr1[5] = {1, 2, 3, 4, 5};
char arr2[6] = {'E','L','O','3','1','2'};//
char arr3[7] = {'2','1','3','O','L','E','\0'};
char arr4[7] = "ELO312\0";
char arr5[] = "ELO312\0";

int Actividad1(){
	int elemento_arr1 = arr1[1];
	char elemento_arr2 = arr2[3];
	arr3[0] = 'A';
	arr4[5] = arr5[0];

	int arr6[2][2] = {{1, 2}, {3, 4}};
	arr6[0][0] = 5;

	arr1[5] = 6;
	arr2[-1] = 0xff;
	printf("prueba1: %c",arr3[0]);
	return 0;

}
