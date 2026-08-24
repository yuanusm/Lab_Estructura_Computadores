#include <stdio.h>

#define BIT_SET(reg, n)     ( (reg) |= (1u << (n)) )
#define BIT_CLEAR(reg, n)   ( (reg) &= ~(1u << (n)) )
#define BIT_TOGGLE(reg, n)  ( (reg) ^= (1u << (n)) )
#define BIT_READ(reg, n)    ( ((reg) >> (n)) & (1u) )

#define N 5
int a;
int b = 10;

void Actividad1(){

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

int Actividad2(){
    int a = 0;
    for(int i = 0; i <= N; i = i + 1){
        printf("a: %d \n",a);
        a = a + i;
    }
    return 0;
}

int Actividad3(){
    int b = 0,c = 0;

    while(b < 5){
        b++;
        c = c + 2;
        printf("b = %d ; c = %d\n",b,c);
    }
    return 0;
}

int Actividad4(){
    int b = 0,c = 0;

    do{
        b++;
        c = c + 2;
        printf("b = %d ; c = %d\n",b,c);
    }
    while(b < 5);

    return 0;
}

int Actividad8(){
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

int Actividad9(){
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


int main()
{   
    //Actividad1();
    //Actividad2();
    //Actividad3();
    //Actividad4();
    //Actividad8();
    //Actividad9();
}

