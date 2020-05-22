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
    unsigned int Moteurs;           //Statut des moteurs (1 : marche, 0 : arrêt)
    unsigned int Timer0;            //Statit de Timer0    
    unsigned int Objet;             //1 si un objet est détecté    
    volatile int nbMesure;
    unsigned int Vbat;              //Statut de la batterie      
    unsigned int SommeMesures;      //Somme des mesures de Vbat (on fait une moyenne sur 4 mesures)
};

void initINT(void);          //Initialise les interruptions
int initDirection(void);    //Initialise la direction
void initPIC(void);           //Initialise le PIC avec les ports E/S, I2C, horloge
void initStatut(struct Statut *Etat);

#endif	/* INIT_H */

