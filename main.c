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

struct Statut Etat;
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
        INTCONbits.INT0IF = 0 ;
        Etat.START = ~Etat.START;
        if(Etat.START)
            printf("Démarrage du système\r\n");
        else
            printf("Arrêt du système\r\n");
    }
    if(PIR1bits.ADIF==1)    //Batterie
    {
        PIR1bits.ADIF=0;
        if(ADCON0bits.CHS == 2) //On vérifie que le channel est sur Vbat pour éviter de mesurer des valeurs de IRD/G
        {
            ADCON0bits.GO=0;
            Etat.SommeMesures += ADRESH*256+ADRESL&0x0000FFFF; //&0x0000FFFF
            //printf("SommeMesures : %ld\r\n",Etat.SommeMesures);
            Etat.nbMesure++;
            affichageLED(&Etat);
            if(Etat.nbMesure == 4)
            {
                Etat.Vbat = Etat.SommeMesures/4;
                
                if(Etat.Vbat < 43200)    //43 200 = 10V
                {  
                    Etat.START = 0;
                    printf("Batterie faible\r\n",Etat.Vbat);
                }
                Etat.SommeMesures = 0;
                Etat.nbMesure = 0;
                affichageLED(&Etat);
            }
            
        }
    }
    if(INTCONbits.TMR0IF)       //Timer0 qui contrôle la fréquence des mesures batterie
    {
        INTCONbits.TMR0IF=0; 
        TMR0H=0x0B;             //Rechargement
        TMR0L=0xDC;            
        ADCON0bits.GO=1;        //On autorise une mesure
        Etat.Timer0 = ~Etat.Timer0;
        affichageLED(&Etat);
    }
}


void main(void) 
{
    initPIC();
    initINT();
    initStatut(&Etat);
    while(1)
    {
        while(Etat.START == 0)
        {
            INTCONbits.TMR0IE=0;    //disable it
            PIE1bits.TMR2IE=0;
            PIE1bits.ADIE=0;
        }
        INTCONbits.TMR0IE=1;        //enable it 
        PIE1bits.TMR2IE=1;
        PIE1bits.ADIE=1;
        while(INTCONbits.TMR0IF == 0);  //On fait une détection toute les quarts de seconde pour économiser la batterie
        if(detectionObjet())
        {
            Etat.Objet = 1;
            PWM(20);
            Etat.Moteurs = 1;
        }
        else
        {
            Etat.Objet = 0;
            PWM(0);
            Etat.Moteurs = 0;
        }
        /*Séquence d'arrêt*/
        if(Etat.START == 0)
        {
            PWM(0);
            initStatut(&Etat);
        }
        affichageLED(&Etat);
    }
}
