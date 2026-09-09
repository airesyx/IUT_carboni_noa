#include <xc.h>
#include "IO.h"
#include "PWM.h"
#include "Toolbox.h"

#define PWMPER 24.0

void InitPWM(void) {
    PTCON2bits.PCLKDIV = 0b000; //Divide by 1
    PTPER = 100 * PWMPER;

    //Reglage PWM moteur 1 sur hacheur 1
    IOCON1bits.PMOD = 0b11; //PWM I/O pin pair is in the True Independent Output mode
    IOCON1bits.PENL = 1;
    IOCON1bits.PENH = 1;
    FCLCON1 = 0x0003; //Desactive la gestion de faults

    IOCON2bits.PMOD = 0b11; //PWM I/O pin pair is in the True Independent Output mode
    IOCON2bits.PENL = 1;
    IOCON2bits.PENH = 1;
    FCLCON2 = 0x0003; //Desactive la gestion des faults
    
    /*Enable PWM Module*/
    PTCONbits.PTEN = 1;
}

double talon = 50;
void PWMSetSpeed(float vitesseEnPourcents, uint8_t selMotor){
    
    if(selMotor == MOTEUR_GAUCHE){
        if(vitesseEnPourcents >= 0){
            PDC1 = vitesseEnPourcents * PWMPER + talon;
            SDC1 = talon; 
        }
        else {
            PDC1 = talon;
            SDC1 = -vitesseEnPourcents * PWMPER + talon;
        }
    }
    else if (selMotor == MOTEUR_DROIT){
        if(vitesseEnPourcents >= 0){
            PDC2 = talon;
            SDC2 = vitesseEnPourcents * PWMPER + talon;
        }
        else {
            PDC2 = -vitesseEnPourcents * PWMPER + talon;
            SDC2 = talon; 
        }
    }
}