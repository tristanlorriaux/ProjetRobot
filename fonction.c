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

void PWM(int r_cyclique)
{
    CCPR1L = r_cyclique*2.5;
	CCPR2L = r_cyclique*2.5;
}

int detectionObjet(void)
{
    int IRDmes = 0;
    int IRGmes = 0;
    ADCON0bits.GO = 1;
    ADCON0bits.CHS = 0; //Channel sur IRD
    while(!PIR1bits.ADIF);
    PIR1bits.ADIF = 0;
    IRDmes = ADRESH*256+ADRESL;
    ADCON0bits.CHS = 1; //Channel sur IRG
    while(!PIR1bits.ADIF);
    PIR1bits.ADIF = 0;
    IRGmes = ADRESH*256+ADRESL;
    ADCON0bits.CHS = 2; //Channel sur Vbat
    ADCON0bits.GO = 0;
    return((40<IRDmes && IRDmes<150) || (40<IRGmes && IRGmes<150)); //Les valeurs sont à changer
}