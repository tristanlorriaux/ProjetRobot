#include "init.h"
#include "MI2C.h"
#include "fonction.h"
#include <p18f2520.h>

void initPIC(void)
{
    /* Configuration Oscillateur*/
	OSCCONbits.IRCF0 = 0 ;//Horloge à 8Mhz
    	OSCCONbits.IRCF1 = 1 ;
	OSCCONbits.IRCF2 = 1 ;
    
    /* Configuration E/S*/
	TRISCbits.RC1 = 0;	// RC1 en sortie PWM droite
	TRISCbits.RC2 = 0;	// RC2 en sortie PWM gauche
    
    TRISBbits.RB5 = 0;  //LED vie test

    TRISAbits.RA0 = 1;  //IRD
    TRISAbits.RA1 = 1;  //IRG

    TRISAbits.RA2 = 1;  //Vbat

    TRISBbits.RB0=1; //Interuption télécommande en entrée
    
    /* Initialisation I2C*/
    MI2CInit();         
    
    /*Configuration Uart*/
    
    TXSTAbits.SYNC=0;//Mode asynchrone 
    TXSTAbits.BRGH=1;//Vitesse élevée
    BAUDCONbits.BRG16=0;//Baud rate 8 bits
    SPBRG=51;//Réglage du baud rate
    RCSTAbits.SPEN=1;
    TXSTAbits.TXEN=1;//Autorisation des transmissions
    //PIR1bits.TXIF=0;
    //PIE1bits.TXIE=0;
    //RCSTAbits.CREN=1;//enables receiver
   	TRISCbits.RC6 = 0; //TX en sortie
    TRISCbits.RC7=1;//RX en entrée - récéption
	
   /*ADC*/
	
    ADCON1bits.VCFG=0;//Vref- = Vss & Vref+ =VDD
    ADCON1bits.PCFG=12;//Port AN2 opened
    ADCON2bits.ADCS=6;//Fosc/8 1
    ADCON2bits.ACQT=4;//4 TAD 3
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
    PWM(0);
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
    /* Configuration interruption TMR0*/
    T0CONbits.TMR0ON=1; 
    T0CONbits.T08BIT=0;//16 bits timer 
    T0CONbits.T0CS=0;//Clock source internal 
    T0CONbits.PSA=0;//prescaler is assigned 
    T0CONbits.T0PS=7;//Prescalar selection 1:256 
    INTCONbits.TMR0IE=1;//enable it 
    TMR0H=0xE1;//Start time (to overflow at 1s) 
    TMR0L=0x7B; 
}