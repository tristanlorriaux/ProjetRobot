/* 
 * File:   init.h
 * Author: maiso
 *
 * Created on 16 mai 2020, 21:59
 */

#ifndef INIT_H
#define	INIT_H

enum I2C {INIT, SEND_MESSAGE, RECEIVE_MESSAGE, STOP, ERREUR}; 

struct Statut
{
    volatile int START;
    volatile int MDG;
    unsigned int Vbat;              //Statut de la batterie
    unsigned int SommeMesures;      //Somme des mesures de Vbat (on fait une moyenne sur 4 mesures)
    volatile int nbMesure;      
    enum I2C i2c;                   //Satut de l'i2c
};

void initINT(void);          //Initialise les interruptions
int initDirection(void);    //Initialise la direction
void initPIC(void);           //Initialise le PIC avec les ports E/S, I2C, horloge

#endif	/* INIT_H */

