/* 
 * File:   interruption.c
 * Author: maiso
 *
 * Created on 20 mai 2020, 18:15
 */

#include <stdio.h>
#include <stdlib.h>
#include "init.h"
#include "p18f2520.h"
#include "interruption.h"

#pragma code HighVector=0x08
void IntHighVector(void)
{
    _asm goto HighISR _endasm
}
#pragma code

#pragma interrupt HighISR 
void HighISR(void)
{
    //Etat->MDG = 0;
    if(PIR1bits.TMR2IF)     //PWM Timer2
    {
        PIR1bits.TMR2IF=0;
    }
    if(INTCONbits.INT0IF)   //Télécommande
    {
        INTCONbits.INT0IF = 0;
        Etat->START = ~Etat->START;
    }
    if(PIR1bits.ADIF==1)    //Batterie
    {
        ADCON0bits.GO=0;
        PIR1bits.ADIF=0; 
        Etat->SommeMesures += ADRESH*256+ADRESL; //&0x0000FFFF
        Etat->nbMesure++; 
        if(Etat->nbMesure == 4)
        {
            Etat->Vbat = Etat->SommeMesures/4;
            if(Etat->Vbat < 853)    //853 = 10V à vérifier
                Etat->START = 0;
            Etat->Vbat = 0;
            Etat->nbMesure = 0;
        }
    }
}

