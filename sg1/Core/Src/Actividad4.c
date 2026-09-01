/*
 * Actividad4.c
 *
 *  Created on: 18 ago 2026
 *      Author: Saile
 */

#include "Actividad4.h"
#include <stdio.h>
typedef struct _estructura{
        int a;
        char b;
    }estructura;


int Actividad4(){



    volatile int a = 5;      // & extrae la direccion de esa variable
    volatile int *p1 = &a;   // no es lo mismo inicializar con *
                    // que usarlo con una variable
                    // al inicializar, p1 queda como puntero
    *p1 = (*p1) + 1;// toma el valor  de p1 y le suma 1
                    // al valor contenido en la direccion.
                    // equivalente a = a + 1;
    p1++;           // p1 tiene el valor de una direccion, por
                    // equivalente a p1 = p1 + sizeof(int)
                    // lo que sumarle 1 avanza a la siguiente direccion
                    // luego de los 4bytes del int

                    // * toma el valor
                    // & toma la direccion
                    // inicializar *hace un puntero


    char nombre[] = "federico"; //9 bytes = 'f','e','d','e','r','i','c','o','\0'
    char *p2;           // puntero de tamaño 1 byte
    p2 = nombre;        // un array siempre devuelve su posicion en memoria
    printf("nombre: %.8s\n",nombre);
    printf("p2: %.8s\n",p2);
    p2[0] = 'F';        //9 bytes = 'F','e','d','e','r','i','c','o','\0'
    printf("nombre: %.8s\n",nombre);
    printf("p2: %.8s\n",p2);
    char letra = *(nombre + 1); // aumentar en 1, aumenta nombre en sizeof(char)
                    // si se parte de la direccion donde se guarda 'F','__e__','d','e','r','i','c','o','\0'
                    // entonces el valor de letra " * " será el caracter 'e'
                    //
    *(p2 + 7) = 61; // la direccion p2, es de el array 'F','e','d','e','r','i','c','=','\0'
                    // se avanzan 7 bytes en la direccion caracter 'e' federic__=__
                    // en esta direccion de memoria se asigna el valor de 61, en ASCII es el signo =


    printf("letra: %c\n",letra);
    printf("nombre: %.8s\n",nombre);
    printf("p2: %.8s\n",p2);


    estructura estr; // inicializa 4 bytes mas 1 byte
                    // 5 bytes  4 + 1
    estructura *pEstr = &estr; // direccion de 4bytes apunta a 5 bytes
                    // se le asigna la direccion de la estructura estr

    (*pEstr).a = 10;
                        // al valor del puntero (la estructura), en la region de tipo int (a)
                        // se asigna el valor 10
    pEstr->b = 'k';     // es una equivalencia de sintaxism
    (*pEstr).b = 'k';
    return 0;
}
