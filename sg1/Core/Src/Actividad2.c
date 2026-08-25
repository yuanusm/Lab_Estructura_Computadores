/*
 * Actividad2.c
 *
 *  Created on: 18 ago 2026
 *      Author: Saile
 */


#include <stdio.h>
#include "Actividad2.h"

typedef struct {
        char raza[10];
        char nombre[10];
        int edad;
        float peso;
    } tipo_mascota;

tipo_mascota m1;

int Actividad2(){

    m1.raza[0] = 'P';
    m1.raza[1] = 'e';
    m1.raza[2] = 'r';
    m1.raza[3] = 'r';
    m1.raza[4] = 'o';
    m1.raza[5] = '\0';

    m1.nombre[0] = 'R';
    m1.nombre[1] = 'o';
    m1.nombre[2] = 'c';
    m1.nombre[3] = 0;

    m1.edad = 2;
    m1.peso = 10.5f;

    tipo_mascota m2;


    char n_raza[] = "Pastor"; //ya se define el siguiente byte para \0 , no sobrepasa de 10 bytes, tiene 8
    for(int index = 0; index<=sizeof(n_raza); index++){
        m2.raza[index] = n_raza[index];
    }


    char n_nombre[] = "Juan"; //ya se define el siguiente byte para \0
    for(int index = 0; index<=sizeof(n_nombre); index++){
        m2.nombre[index] = n_nombre[index];
    }


    m2.edad = 21;
    m2.peso = 75.5345f;

    printf("Raza: %s\n",m1.raza);
    printf("Nombre: %s\n",m1.nombre);
    printf("Edad: %u\n",m1.edad);
    //printf("Peso: %f\n",m1.peso);

    printf("\nRaza: %s\n",m2.raza);
    printf("Nombre: %s\n",m2.nombre);
    printf("Edad: %u\n",m2.edad);
    //printf("Peso: %f\n",m2.peso);
    return 0;
}
