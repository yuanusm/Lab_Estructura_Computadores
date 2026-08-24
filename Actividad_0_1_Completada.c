#include <stdio.h>
#include "funciones.h"
#define BIT_SET(reg, n)     ( (reg) |= (1u << (n)) )
#define BIT_CLEAR(reg, n)   ( (reg) &= ~(1u << (n)) )
#define BIT_TOGGLE(reg, n)  ( (reg) ^= (1u << (n)) )
#define BIT_READ(reg, n)    ( ((reg) >> (n)) & (1u) )

#define N 5
int a;
int b = 10;

void Actividad1_1(){

    b = 62;
    a = b;

    int res1            = a * b;    // 62*62, int es de 4 bytes signed -2_147_483_648 +2_147_483_648 
    int res2            = a - 72;   // -10 en hexadecimal, int signed 4 bytes, con signo 2^31 -2_147_483_648 +2_147_483_648
    unsigned int res3   = a - 72;   // 4_294_967_286, 4 bytes sin signo 2^32, se guarda asi pero se expresa en valor entero con signo
    unsigned int res4 = a - 72;   // 65526, 2 bytes 65536
    int res5            = res3 + 72;// 62
    int res6            = res4 + 72;// 65598
    int res7            = 5 / 2;    // 2
    float res8          = 5 / 2;    // 2
    float res9          = 5 / 2.0;  // 2.5f
    int res10           = 5 % 2;    // 1
    char res11          = b;        // > 2_bytes
    char res12          = 0xFF + 1; // 0? 2 bytes //El programa advierte de overflow


    printf("r1: %d\n", res1);
    printf("sizeof(r2) = %d\n",sizeof(res2));
    printf("r2: %x\n", res2);   // -10 en hexadecimal con 4 bytes, cada byte ff_ff_ff_f6
    printf("r3: %d\n", res3);   //A pesar que la variable es unsigned, los caracteres de salida
                                //estan en tipo entero con signo %d 
    printf("r4: %d\n", res4);   //65526 bien predicho %d entero con signo
    printf("r5: %u\n", res5);   //62 en %u entero sin signo
    printf("r6: %X\n", res6);   //%X Hexadecimal mayuscula   1_003E
    printf("r7: %d\n", res7);   //%d entero con signo 2
    printf("r8: %f\n", res8);   //%f flotante precision simple 2.000000 %lf es double
    printf("r9: %f\n", res9);   //%f flotante precision simple 2.500000 %lf es double
    printf("r10: %d\n", res10); //%d entero con signo 1
    printf("r11: %c\n", res11); //%c Caracter ascii > 
    printf("r12: %d\n", res12); //%d el calculo da 0 en 2 bytes, salida es 0
}

int Actividad2_1(){
    int a = 0;
    for(int i = 0; i <= N; i = i + 1){
        printf("a: %d \n",a);
        a = a + i;
    }
    return 0;
}

int Actividad3_1(){
    int b = 0,c = 0;

    while(b < 5){
        b++;
        c = c + 2;
        printf("b = %d ; c = %d\n",b,c);
    }
    return 0;
}

int Actividad4_1(){
    int b = 0,c = 0;

    do{
        b++;
        c = c + 2;
        printf("b = %d ; c = %d\n",b,c);
    }
    while(b < 5);

    return 0;
}

int Actividad8_1(){
    unsigned int op1 = 0x00FF, op2 = 0x0F0A, op3 = 4;
    unsigned int res1,res2,res3,res4,res5,
                res6,res7,res8,res9,res10;

    res1 = op1 && op2; //operador booleano de 1 bit, cualquier cosa no nula es 1
                        // op1 = 1, op2 =1 ---> 1
    res2 = op1 & op2; // operador booleano bit a bit 
                    //op1     = 11111111 
                    //op2 = 111100001010 solo sobrevive el 1010 
    res3 = op1 || op2; // operador booleano de 1 bit
                    //op1     = 11111111 = 1
                    //op2 = 111100001010 = 1  --->op1||op2 = 1
    res4 = op1 | op2; // operador bit a bit
                    //op1     = 11111111 
                    //op2 = 111100001010  ---> 1111_1111_1111
    res5 = op1 ^ op2;//operador xor bit a bit
                    //op1 =         11111111
                    //op2 =     111100001010
                    //op1^op2 = 111111110101
    res6 = !op1;    // operador booleano 1 bitand op1 = 1 ---> !op1 = 0
    res7 = ~op2;    // operador booleano bit a bit 
                    //int op2(4byte) = 00000000_00000000_00001111_00001010 
                    //     ---> ~op2 = 11111111_11111111_11110000_11110101
    res8 = op3 << 2;// shift register
                    //  op3 =      00000000_00000000_00000000_00000100
                    //  op3 << 2 = 00000000_00000000_00000000_00010000
    res9 = op3 >> 2;//  op3 >> 2 = 00000000_00000000_00000000_00000001
    res10 = op2 << 4;// op2 << 4 = 00000000_00000000_11110000_10100000
    printf("%b\n",op1);
    printf("%b\n",op2);
    printf("%b\n",op3);
    printf("r1: %b\n", res1);
    printf("r2: %b\n", res2);   
    printf("r3: %b\n", res3);   //
    printf("r4: %b\n", res4);   //
    printf("r5: %b\n", res5);   //
    printf("r6: %b\n", res6);   //
    printf("r7: %b\n", res7);   //
    printf("r8: %b\n", res8);   //
    printf("r9: %b\n", res9);   //
    printf("r10: %b\n", res10); //
}

int Actividad9_1(){
    unsigned int reg = 0xECEE678E;
    unsigned int n = 5;
    printf("Inicial:                0b%016b\n", reg);

    reg |= (1u<<4); //Se posiciona un bit en la posicion 6

    printf("(set):                  0b%016b\n", reg);

    reg &= ~(1u<<4); //Se resetea el de la posicion 6

    printf("(reset):                0b%016b\n", reg);

    reg ^= (1u<<4); //Se mantienen todos los bits y el de la posicion 6 se invierte 

    printf("(invertir):             0b%016b\n", reg);
    
    reg |= (0b10001<<2); //Se posicionan 2 bit en la posicion 3 y 7

    printf("(set 3 y 7):            0b%016b\n", reg);

    reg &= ~(0xF<<7);

    printf("(clear 8 - 11):         0b%016b\n", reg);

    reg |= (0x9<<7);

    printf("(set 8 - 11):           0b%016b\n", reg);

    printf("(Lectura del valor entre 8 - 11):          %u\n", (reg>>7) & 0xF);//Es un comparador para leer un bit
    return 0;
}

int Actividad1_2(){
    int arr1[5] = {1, 2, 3, 4, 5}; //array de 5 elementos de 4 byte
    char arr2[6] = {'E','L','O','3','1','2'}; // Array de 6 elementos de 2 byte que no se puede
                                    //interpretar como un string valido por no tener \0
                                    //si se lee como string, se sigue leyendo la memoria hasta hayar \0

    char arr3[7] = {'2','1','3','O','L','E','\0'}; //Array de 7 elementos de 2byte
                                    //es valido como string
    char arr4[7] = "ELO312\0"; //array de 8 elementos de 2byte String mal definido
                                    //incluye 2 veces el \0 por lo que gasta memoria de forma innecesaria
    char arr5[] = "ELO312";     //array de 7 elementos de 2 byte, con tamaño implicito. El \0 automaticamente 
                                //se coloca al final del array 


	int elemento_arr1 = arr1[1];
	char elemento_arr2 = arr2[3];
	arr3[0] = 'A';
	arr4[5] = '3';

	int arr6[2][2] = {{1, 2}, {3, 4}};
	arr6[0][0] = 5;

	return 0;

}


int Actividad2_2(){
    typedef struct _tipo_mascota{
        char raza[10];
        char nombre[10];
        int edad;
        float peso;
    } tipo_mascota;

    tipo_mascota m1;

    m1.raza[0] = 'P';
    m1.raza[1] = 'e';
    m1.raza[2] = 'r';
    m1.raza[3] = 'r';
    m1.raza[4] = 'o';
    m1.raza[5] = '\0';

    m1.nombre[0] = 'R';
    m1.nombre[1] = 'o';
    m1.nombre[2] = 'c';
    m1.nombre[3] = '\0';

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
    printf("Peso: %f\n",m1.peso);

    printf("\nRaza: %s\n",m2.raza);
    printf("Nombre: %s\n",m2.nombre);
    printf("Edad: %u\n",m2.edad);
    printf("Peso: %f\n",m2.peso);
    return 0;
}

int Actividad3_2(){
    typedef union _tipo_ejemplo{ // Usa la memoria mas grande apra guardar todos los datos
                    // no asigna memoria por cada uno, usa una sola memoria grande para interpretar cada datos
        long long largo; //64
        int entero[2]; //64
        unsigned char caracter[8]; // char puede ser negativo, con uchar uno se asegura que si sean de 1 byye
        float flotante[2];
    } tipo_ejemplo;

    tipo_ejemplo ej1;

    ej1.largo = 0x123456789ABCDEF0;
    //Endianness viene desde interpretar los datos desde el 
    //mas significativo o desde el menos significativo
    //Big-Endian pone en primer lugar al mas significativo
    //little-Endian pone en primer lugar al menos significativo
    printf("Interpretacion c: %X", ej1.entero[0]);
    return 0;
}

int Actividad4_2(){

    typedef struct _estructura{
        int a;
        char b;
    }estructura;

    int a = 5;      // & extrae la direccion de esa variable
    int *p1 = &a;   // no es lo mismo inicializar con *
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
int Actividad5_2(){
    printf("Actividad de funciones separadas en archivos");
    return 0;
}

int Actividad6_2(int *enteros){
    int res = 0;
    for(int i = 0; i <= 5; i++){
        res += *(enteros + i);
    }
    return res;
}

int main()
{   
    //Actividad1();
    //Actividad2();
    //Actividad3();
    //Actividad4();
    //Actividad8();
    //Actividad9();

    //Actividad1_2();
    //Actividad2_2();
    //Actividad3_2();
    //Actividad4_2();
    int enteros[5] = {1,3,4,5,6};
    int res = 0;
    res = Actividad6_2(enteros);
    printf("Resultado de sumar el array: %u\n",res);
}

