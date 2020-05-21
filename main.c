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
#include "interruption.h"
#include "fonction.h"
#include <p18f2520.h>







void main(void) 
{
    initPIC();
    initINT();
    MI2CInit();
    Etat->START = 0;
    while(Etat->START)
    {
        
    }
    PWM(0);
}

