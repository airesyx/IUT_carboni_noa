#include <stdio.h>
#include <stdlib.h>
#include <xc.h>
#include "main.h"
#include "ChipConfig.h"
#include "IO.h"
#include "timer.h"
#include "PWM.h"
#include "Robot.h"
#include "Toolbox.h"
#include "ADC.h"

int main (void){
//Initialisation oscillateur
InitOscillator();

//Configuration des input et output (IO)
InitIO();

//Initialisation des timers
InitTimer1();
InitTimer23();
InitTimer4();

//Initialisation PWM
InitPWM();

//Initialisation ADC1
InitADC1();

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
        if(ADCIsConversionFinished()){
            ADCClearConversionFinishedFlag();
            unsigned int * result = ADCGetResult();
            float volts = ((float) result [0])* 3.3 / 4096;
            robotState.distanceTelemetreEGauche = 34 / volts - 5;
            volts = ((float) result [1])* 3.3 / 4096;
            robotState.distanceTelemetreGauche = 34 / volts - 5;
            volts = ((float) result [2])* 3.3 / 4096;
            robotState.distanceTelemetreCentre = 34 / volts - 5;
            volts = ((float) result [3])* 3.3 / 4096;
            robotState.distanceTelemetreDroit = 34 / volts - 5;
            volts = ((float) result [4])* 3.3 / 4096;
            robotState.distanceTelemetreEDroit = 34 / volts - 5;
        }
    } // fin main
}

unsigned char stateRobot;
void OperatingSystemLoop(void){
    SetNextRobotStateInAutomaticMode();
    /*
    switch (stateRobot){
        case STATE_ATTENTE:
            timestamp = 0;
            PWMSetSpeedConsigne(0, MOTEUR_DROIT);
            PWMSetSpeedConsigne(0, MOTEUR_GAUCHE);
            stateRobot = STATE_ATTENTE_EN_COURS;
        break;
        
        case STATE_ATTENTE_EN_COURS:
            if (timestamp > 1000)
                stateRobot = STATE_AVANCE;
        break;
        
        case STATE_AVANCE:
            PWMSetSpeedConsigne(25, MOTEUR_DROIT);
            PWMSetSpeedConsigne(25, MOTEUR_GAUCHE);
            stateRobot = STATE_AVANCE_EN_COURS;
        break;
        
        case STATE_AVANCE_EN_COURS:
            SetNextRobotStateInAutomaticMode();
        break;
        
        case STATE_TOURNE_GAUCHE:
            PWMSetSpeedConsigne(25, MOTEUR_DROIT);
            PWMSetSpeedConsigne(0, MOTEUR_GAUCHE);
            stateRobot = STATE_TOURNE_GAUCHE_EN_COURS;
        break;
        
        case STATE_TOURNE_GAUCHE_EN_COURS:
            SetNextRobotStateInAutomaticMode();
        break;
        
        case STATE_TOURNE_DROITE:
            PWMSetSpeedConsigne(0, MOTEUR_DROIT);
            PWMSetSpeedConsigne(25, MOTEUR_GAUCHE);
            stateRobot = STATE_TOURNE_DROITE_EN_COURS;
        break;
        
        case STATE_TOURNE_DROITE_EN_COURS:
            SetNextRobotStateInAutomaticMode();
        break;
        
        case STATE_TOURNE_SUR_PLACE_GAUCHE:
            PWMSetSpeedConsigne(10, MOTEUR_DROIT);
            PWMSetSpeedConsigne(-10, MOTEUR_GAUCHE);
            stateRobot = STATE_TOURNE_SUR_PLACE_GAUCHE_EN_COURS;
        break;
        
        case STATE_TOURNE_SUR_PLACE_GAUCHE_EN_COURS:
            SetNextRobotStateInAutomaticMode();
        break;
        
        case STATE_TOURNE_SUR_PLACE_DROITE:
            PWMSetSpeedConsigne(-10, MOTEUR_DROIT);
            PWMSetSpeedConsigne(10, MOTEUR_GAUCHE);
            stateRobot = STATE_TOURNE_SUR_PLACE_DROITE_EN_COURS;
        break;
        
        case STATE_TOURNE_SUR_PLACE_DROITE_EN_COURS:
            SetNextRobotStateInAutomaticMode();
        break;
        
        default :
            stateRobot = STATE_ATTENTE;
        break;
    }
     */
}

volatile OBSTACLE obstacle;

unsigned char nextStateRobot=0;
void SetNextRobotStateInAutomaticMode(void){
    
    obstacle.front = 0;
    obstacle.left = 0;
    obstacle.right = 0;
    obstacle.eleft = 0;
    obstacle.eright = 0;
    
    obstacle.front += robotState.distanceTelemetreCentre < 60;
    obstacle.left += robotState.distanceTelemetreGauche < 20;
    obstacle.right += robotState.distanceTelemetreDroit < 20;
    obstacle.eleft += robotState.distanceTelemetreEGauche < 20;
    obstacle.eright += robotState.distanceTelemetreEDroit< 20;
    
    obstacle.front += robotState.distanceTelemetreCentre < 50;
    obstacle.left += robotState.distanceTelemetreGauche < 20;
    obstacle.right += robotState.distanceTelemetreDroit < 20;
    obstacle.eleft += robotState.distanceTelemetreEGauche < 20;
    obstacle.eright += robotState.distanceTelemetreEDroit< 20;
    
    obstacle.front += robotState.distanceTelemetreCentre < 40;
    obstacle.left += robotState.distanceTelemetreGauche < 30;
    obstacle.right += robotState.distanceTelemetreDroit < 30;
    obstacle.eleft += robotState.distanceTelemetreEGauche < 30;
    obstacle.eright += robotState.distanceTelemetreEDroit< 30;
    
    obstacle.front += robotState.distanceTelemetreCentre < 30;
    obstacle.left += robotState.distanceTelemetreGauche < 20;
    obstacle.right += robotState.distanceTelemetreDroit < 20;
    obstacle.eleft += robotState.distanceTelemetreEGauche < 20;
    obstacle.eright += robotState.distanceTelemetreEDroit< 20;
    
    obstacle.front += robotState.distanceTelemetreCentre < 20;

    LED_BLANCHE_1 = obstacle.eleft >= 1;
    LED_BLEUE_1 = obstacle.left >= 1;
    LED_ORANGE_1 = obstacle.front >= 1;
    LED_ROUGE_1 = obstacle.right >= 1;
    LED_VERTE_1 = obstacle.eright >= 1;
    
    LED_VERTE_2 = obstacle.front >= 1;
    LED_ROUGE_2 = obstacle.front >= 2;
    LED_ORANGE_2 = obstacle.front >= 3;
    LED_BLEUE_2 = obstacle.front >= 4;
    LED_BLANCHE_2 = obstacle.front >= 5;
    
    /*
    //Determination de l etat a venir du robot
    if (positionObstacle == PAS_D_OBSTACLE)
        nextStateRobot = STATE_AVANCE;
    else if (positionObstacle == OBSTACLE_A_DROITE)
        nextStateRobot = STATE_TOURNE_GAUCHE;
    else if (positionObstacle == OBSTACLE_A_GAUCHE)
        nextStateRobot = STATE_TOURNE_DROITE;
    else if (positionObstacle == OBSTACLE_EN_FACE)*
        nextStateRobot = STATE_TOURNE_SUR_PLACE_GAUCHE;
        */

    //Si l on n est pas dans la transition de l etape en cours
    if (nextStateRobot != stateRobot - 1)
        stateRobot = nextStateRobot;
}