
#include "init.h"
#include <p18f2520.h>
void initPIC(void)
{
    /* Configuration Oscillateur*/
	OSCCONbits.IRCF0 = 0 ;
    OSCCONbits.IRCF1 = 1 ;
	OSCCONbits.IRCF2 = 1 ;
    /* Configuration E/S*/
	TRISCbits.RC1 = 0;	// RC1 en sortie PWM droite
	TRISCbits.RC2 = 0;	// RC2 en sortie PWM gauche
    
    TRISBbits.RB5 = 0;  //LED test
    TRISAbits.RA0 = 1;  //IRD
    TRISAbits.RA1 = 1;  //IRG
    TRISAbits.RA2 = 1;  //Vbat
    TRISAbits.RA6 = 0;  //Direction D
    TRISAbits.RA7 = 0;  //Direction G
    
    /*ADC*/
    ADCON1bits.VCFG=0;//Vref- = Vss & Vref+ =VDD
    ADCON1bits.PCFG=12;//Port AN2 opened
    ADCON2bits.ADCS=6;//Fosc/8 1
    ADCON2bits.ACQT=7;//4 TAD 3
    ADCON0bits.CHS=2;//Channel 2
    ADCON2bits.ADFM=0;//Format left justified
    ADCON0bits.ADON=1;//Enable converter
    PIR1bits.ADIF=0;//reset flag
    PIE1bits.ADIE=1;//allow interrupt
    INTCON2bits.INTEDG0 = 0;//Falling Edge
    INTCON2bits.INTEDG1 = 0;//Falling Edge
    ADCON0bits.GO=1;//launch converter

    
}

void initINT(void)
{
    /* Configuration TIMER2 */
	T2CONbits.T2CKPS0 = 1; 
	T2CONbits.T2CKPS1 = 0; // CLK /4
	PR2 = 249;				// Reglage periode FPWM = Fosc/(4*(PR2+1)*PRE)
    T2CONbits.T2OUTPS=9; // postscaler = 10
/* Reglage rapport cyclique */
	CCPR1L = 20*2.5;			//20%-->	CCPRxLDCxB1DCxB0 = 0x64 = 0b1100100 --> CCPRxL = 0b1001=0x19 --> DCxB1=DCxB0=0 	
	CCPR2L = 20*2.5;
	CCP1CONbits.DC1B0 = 0;
	CCP1CONbits.DC1B1 = 0;
	CCP2CONbits.DC2B0 = 0;
	CCP2CONbits.DC2B1 = 0;
/* Selection Mode PWM */
	CCP1CONbits.CCP1M3 = 1;
	CCP1CONbits.CCP1M2 = 1;
	CCP2CONbits.CCP2M3 = 1;
	CCP2CONbits.CCP2M2 = 1;
 /* Configuration interruption TMR2*/
    PIE1bits.TMR2IE=1;  // Validation TMR2IF (TMR2IP =1 par défault)
    INTCONbits.PEIE=1;  //Validation des interruptions des périphériques
/* TMR2 marche */
	T2CONbits.TMR2ON = 1;
    INTCON2bits.INTEDG0 = 0;	// INT0 sur front descendant
	INTCONbits.INT0IE = 1;		//Validation interruption sur INT0
	INTCONbits.GIE = 1;
}

