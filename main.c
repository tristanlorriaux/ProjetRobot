/* 
 * File:   main.c
 * Author: maiso
 *
 * Created on 18 mai 2020, 16:38
 */

#include <stdio.h>
#include <stdlib.h>
#include "init.h"
#include "MI2C.h"
//#include "interruption.h"
#include "fonction.h"
#include <p18f2520.h>
#pragma config OSC = INTIO67
#pragma config PBADEN = OFF, WDT = OFF, LVP = OFF, DEBUG = ON

struct Statut *Etat;
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
        PIR1bits.ADIF=0;
        if(ADCON0bits.CHS == 2) //On vérifie que le channel est sur Vbat pour éviter de mesurer des valeurs de IRD/G
        {
            ADCON0bits.GO=0;
            Etat->SommeMesures += ADRESH*256+ADRESL; //&0x0000FFFF
            Etat->nbMesure++;
            if(Etat->nbMesure == 4)
            {
                Etat->Vbat = Etat->SommeMesures/4;
                if(Etat->Vbat < 759)    //759 = 10V à vérifier
                    Etat->START = 0;
                Etat->SommeMesures = 0;
                Etat->nbMesure = 0;
            }
        }
    }
    if(INTCONbits.TMR0IF)       //Timer0 qui contrôle la fréquence des mesures batterie
    {
        INTCONbits.TMR0IF=0; 
        TMR0H=0xE1; 
        TMR0L=0x7B;             //si le flag d'interruption est ON 
        ADCON0bits.GO=1;        //On autorise une mesure
    }
}


void main(void) 
{
    initPIC();
    initINT();
    while(!Etat->START);
    while(1)
    {
        while(Etat->START)
        {
          if(detectionObjet())
              PWM(20);
          else
              PWM(0);
        }
        /*Séquence d'arrêt (à compléter)*/
        PWM(0);
        
    }
}

