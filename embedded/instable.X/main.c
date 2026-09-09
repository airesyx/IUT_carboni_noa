#include <stdio.h>
#include <stdlib.h>
#include <xc.h>
#include "ChipConfig.h"
#include "IO.h"
#include "timer.h"
#include "PWM.h"
#include "Toolbox.h"

int main (void){
//Initialisation oscillateur
InitOscillator();

//Initialisation des timers
InitTimer1();
InitTimer23();

//Initialisation PWM
InitPWM();

//Configuration des input et output (IO)
InitIO();
LED_BLANCHE_1 = 0;
LED_BLEUE_1 = 0;
LED_ORANGE_1 = 0;
LED_ROUGE_1 = 0;
LED_VERTE_1 = 0;

LED_BLANCHE_2 = 0;
LED_BLEUE_2 = 0;
LED_ORANGE_2 = 0;
LED_ROUGE_2 = 0;
LED_VERTE_2 = 0;

// Boucle Principale
    while(1)
    {
        
    } // fin main
}