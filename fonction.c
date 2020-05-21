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
