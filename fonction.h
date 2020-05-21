/* 
 * File:   fonction.h
 * Author: maiso
 *
 * Created on 20 mai 2020, 18:16
 */

#ifndef FONCTION_H
#define	FONCTION_H

void PWM(int r_cyclique);       //Fixe le rapport cyclique du PWM à r_cyclique(%)

int detectionObjet(void);       //retourne 1 s'il y a un objet entre 40cm et 150cm

#endif	/* FONCTION_H */

