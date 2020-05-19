/* 
 * File:   main.c
 * Author: maiso
 *
 * Created on 18 mai 2020, 16:38
 */

#include <stdio.h>
#include <stdlib.h>
#include "init.h"
#include "i2c.h"
#include <p18f2520.h>


//volatile struct Statut *Etat;
volatile int START;
volatile int MDG;
void HighISR(void);


#pragma code HighVector=0x08
void IntHighVector(void)
{
    _asm goto HighISR _endasm
}
#pragma code

#pragma interrupt HighISR 
void HighISR(void)
{
    MDG = 0;
    if(PIR1bits.TMR2IF)     //PWM Timer2
    {
        PIR1bits.TMR2IF=0;
    }
    if(INTCONbits.INT0IF)   //Télécommande
    {
        INTCONbits.INT0IF = 0;
        START = 0;
    }
}

void main(void) 
{
    initPIC();
    initINT();
    MI2Cinit();
    printf("ici\n");
    while(1)
    {
        
    }
}

