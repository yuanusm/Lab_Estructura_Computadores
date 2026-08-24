/*
 * Actividad3.c
 *
 *  Created on: 18 ago 2026
 *      Author: Saile
 */


#include "Actividad3.h"
#include <stdio.h>
typedef union _tipo_ejemplo{ // Usa la memoria mas grande apra guardar todos los datos
                    // no asigna memoria por cada uno, usa una sola memoria grande para interpretar cada datos
        long long largo; //64
        int entero[2]; //64
        unsigned char caracter[8]; // char puede ser negativo, con uchar uno se asegura que si sean de 1 byye
        float flotante[2];
    } tipo_ejemplo;

int Actividad3(){


    volatile tipo_ejemplo ej1;

    ej1.largo = 0x123456789ABCDEF0;
    //Endianness viene desde interpretar los datos desde el
    //mas significativo o desde el menos significativo
    //Big-Endian pone en primer lugar al mas significativo
    //little-Endian pone en primer lugar al menos significativo
    printf("Interpretacion c: %X", ej1.entero[0]);
    return 0;
}
