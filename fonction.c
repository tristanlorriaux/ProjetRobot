/* 
 * File:   fonction.c
 * Author: maiso
 *
 * Created on 20 mai 2020, 18:16
 */

#include <stdio.h>
#include <stdlib.h>
#include "p18f2520.h"
#include "fonction.h"
#include "init.h"
#include "MI2C.h"

void PWM(int r_cyclique)
{
    CCPR1L = r_cyclique*2.5;
	CCPR2L = r_cyclique*2.5;
}

int detectionObjet(void)
{
    int IRDmes = 0;
    int IRGmes = 0;
    ADCON0bits.CHS = 0; //Channel sur IRD
    ADCON0bits.GO = 1;   
    while(PIR1bits.ADIF == 0);
    PIR1bits.ADIF = 0;
    ADCON0bits.GO = 0; 
    IRDmes = ADRESH*256+ADRESL;
    ADCON0bits.CHS = 1; //Channel sur IRG
    ADCON0bits.GO = 1;
    while(PIR1bits.ADIF == 0);
    PIR1bits.ADIF = 0;
    IRGmes = ADRESH*256+ADRESL;
    ADCON0bits.GO = 0;
    ADCON0bits.CHS = 2; //Channel sur Vbat    
    return((40<IRDmes && IRDmes<150) || (40<IRGmes && IRGmes<150)); //Les valeurs sont à changer / 40cm : 0.75V, 150cm : 0.30V
}



void affichageLED(struct Statut *etat)
{
    int led = 0b11111111;
    if(etat->START)
        led = led&0b01111111;
    else if(etat->Moteurs)
        led = led&0b10111111;
    else if(etat->Timer0)
        led = led&0b11011111;
    else if(etat->Objet)
        led = led&0b11101111;
    else if(etat->nbMesure == 3)
        led = led&0b11110011;
    else if(etat->nbMesure == 2)
        led = led&0b11110111;
    else if(etat->nbMesure == 1)
        led = led&0b11111011;
    Write_PCF8574(0x40, led);
}