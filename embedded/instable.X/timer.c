#include <xc.h>
#include "timer.h"
#include "IO.h"
#include "PWM.h"
//Initialisation d?un timer 16 bits

void InitTimer1(void) {
    //Timer1 pour horodater les mesures (1ms)
    T1CONbits.TON = 0; // Disable Timer
    T1CONbits.TCKPS = 0b10; //Prescaler
    //11 = 1:256 prescale value
    //10 = 1:64 prescale value
    //01 = 1:8 prescale value
    //00 = 1:1 prescale value
    T1CONbits.TCS = 0; //clock source = internal clock
    PR1 = 0x0249F;
    IFS0bits.T1IF = 0; // Clear Timer Interrupt Flag
    IEC0bits.T1IE = 1; // Enable Timer interrupt
    T1CONbits.TON = 1; // Enable Timer
}
//Interruption du timer 1

void __attribute__((interrupt, no_auto_psv)) _T1Interrupt(void) {
    IFS0bits.T1IF = 0;
    //LED_BLANCHE_1 = !LED_BLANCHE_1;
}
//Initialisation d?un timer 32 bits

void InitTimer23(void) {
    T3CONbits.TON = 0; // Stop any 16-bit Timer3 operation
    T2CONbits.TON = 0; // Stop any 16/32-bit Timer3 operation
    T2CONbits.T32 = 1; // Enable 32-bit Timer mode
    T2CONbits.TCS = 0; // Select internal instruction cycle clock
    T2CONbits.TCKPS = 0b00; // Select 1:1 Prescaler
    TMR3 = 0x00; // Clear 32-bit Timer (msw)
    TMR2 = 0x00; // Clear 32-bit Timer (lsw)
    PR3 = 0x0393; // Load 32-bit period value (msw)
    PR2 = 0x8700; // Load 32-bit period value (lsw)
    IPC2bits.T3IP = 0x01; // Set Timer3 Interrupt Priority Level
    IFS0bits.T3IF = 0; // Clear Timer3 Interrupt Flag
    IEC0bits.T3IE = 1; // Enable Timer3 interrupt
    T2CONbits.TON = 1; // Start 32-bit Timer
}
//Interruption du timer 32 bits sur 2-3

void __attribute__((interrupt, no_auto_psv)) _T3Interrupt(void) {
    IFS0bits.T3IF = 0; // Clear Timer3 Interrupt Flag
    LED_ORANGE_1 = !LED_ORANGE_1;
#if(0)
    static uint8_t cs_count = 0;
    
    switch(cs_count){
        case 0 : 
            PWMSetSpeed(10, MOTEUR_DROIT);
            PWMSetSpeed(-10, MOTEUR_GAUCHE);
            cs_count += 1;
        break;
        
        case 1 : 
            PWMSetSpeed(15, MOTEUR_DROIT);
            PWMSetSpeed(-15, MOTEUR_GAUCHE);
            cs_count += 1;
        break;
        
        case 2 : 
            PWMSetSpeed(20, MOTEUR_DROIT);
            PWMSetSpeed(-20, MOTEUR_GAUCHE);
            cs_count += 1;
        break;
        
        case 3 : 
            PWMSetSpeed(25, MOTEUR_DROIT);
            PWMSetSpeed(-25, MOTEUR_GAUCHE);
            cs_count += 1;
        break;
        
        case 4 : 
            PWMSetSpeed(0, MOTEUR_DROIT);
            PWMSetSpeed(0, MOTEUR_GAUCHE);
            cs_count += 1;
        break;
        
        case 5 :
            PWMSetSpeed(-10, MOTEUR_DROIT);
            PWMSetSpeed(10, MOTEUR_GAUCHE);
            cs_count += 1;
        break;
        case 6 :
            PWMSetSpeed(-15, MOTEUR_DROIT);
            PWMSetSpeed(15, MOTEUR_GAUCHE);
            cs_count += 1;
        break;
        case 7 :
            PWMSetSpeed(-20, MOTEUR_DROIT);
            PWMSetSpeed(20, MOTEUR_GAUCHE);
            cs_count += 1;
        break;
        case 8 :
            PWMSetSpeed(-25, MOTEUR_DROIT);
            PWMSetSpeed(25, MOTEUR_GAUCHE);
            cs_count += 1;
        break;
        case 9 :
            PWMSetSpeed(0, MOTEUR_DROIT);
            PWMSetSpeed(0, MOTEUR_GAUCHE);
            cs_count = 0;
        break;
        
    }
    
#endif
}