/*
 * Actividad7.c
 *
 *  Created on: 24 ago 2026
 *      Author: Saile
 */

#include "Actividad5.h"
int Actividad5(int *enteros){
    int res = 0;
    for(int i = 0; i <= 5; i++){
        res += *(enteros + i);
    }
    return res;
}
