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
float leftMotorSpeed = 0.0;
float rightMotorSpeed = 0.0;
void OperatingSystemLoop(void){
    switch (stateRobot){
        case STATE_ATTENTE:
            timestamp = 0;
            PWMSetSpeedConsigne(0, MOTEUR_DROIT);
            PWMSetSpeedConsigne(0, MOTEUR_GAUCHE);
            stateRobot = STATE_ATTENTE_EN_COURS;
        break;
        
        case STATE_ATTENTE_EN_COURS:
            if (timestamp > 2000)
                stateRobot = STATE_AVANCE;
        break;
        
        case STATE_AVANCE:
            PWMSetSpeedConsigne(BASE_SPEED_PERCENT, MOTEUR_DROIT);
            PWMSetSpeedConsigne(BASE_SPEED_PERCENT, MOTEUR_GAUCHE);
            stateRobot = STATE_AVANCE_EN_COURS;
        break;
        
        case STATE_AVANCE_EN_COURS:
            SetNextRobotStateInAutomaticMode();
        break;
        
        case STATE_AVANCE_TOURNE:
            PWMSetSpeedConsigne(25, MOTEUR_DROIT);
            PWMSetSpeedConsigne(0, MOTEUR_GAUCHE);
            stateRobot = STATE_AVANCE_TOURNE_EN_COURS;
        break;
        
        case STATE_AVANCE_TOURNE_EN_COURS:
            SetNextRobotStateInAutomaticMode();
        break;
        
        case STATE_ESQUIVE:
            PWMSetSpeedConsigne(rightMotorSpeed, MOTEUR_DROIT);
            PWMSetSpeedConsigne(leftMotorSpeed, MOTEUR_GAUCHE);
            stateRobot = STATE_ESQUIVE_EN_COURS;
        break;
        
        case STATE_ESQUIVE_EN_COURS:
            SetNextRobotStateInAutomaticMode();
        break;
        
        case STATE_FIND_POTENTIAL_EXIT:
            PWMSetSpeedConsigne(rightMotorSpeed, MOTEUR_DROIT);
            PWMSetSpeedConsigne(leftMotorSpeed, MOTEUR_GAUCHE);
            stateRobot = STATE_FIND_POTENTIAL_EXIT_EN_COURS;
        break;
        
        case STATE_FIND_POTENTIAL_EXIT_EN_COURS:
            SetNextRobotStateInAutomaticMode();
        break;
        
        default :
            stateRobot = STATE_ATTENTE;
        break;
    }
}

volatile OBSTACLE obstacle;
const float SENSOR_COS[SENSOR_NB] = {0.0, 0.0, 0.0, 0.0, 0.0};//EGauche, Gauche, Centre, Droit, EDroit
const float SENSOR_SIN[SENSOR_NB] = {0.0, 0.0, 0.0, 0.0, 0.0};//EGauche, Gauche, Centre, Droit, EDroit
unsigned char nextStateRobot = 0;
void SetNextRobotStateInAutomaticMode(void){
    float dist[SENSOR_NB] = {
        robotState.distanceTelemetreEGauche, 
        robotState.distanceTelemetreGauche,
        robotState.distanceTelemetreCentre,
        robotState.distanceTelemetreDroit,
        robotState.distanceTelemetreEDroit,
    };
    float vectorX = 0.0;
    float vectorY = 0.0;
    for(uint8_t i; i < SENSOR_NB; i++){
        float pow = repulsePow(dist[i]);
        vectorX -= SENSOR_COS[i] * pow;
        vectorY -= SENSOR_SIN[i] * pow;
    }
    static unsigned int iterationCount = 0;
    static float minVectorX = 0.0;
    static float lastMinVectorX = 0.0;
    if(stateRobot != STATE_ESQUIVE_EN_COURS){
        if( -vectorX >= ESQUIVE_TH_H){
            iterationCount = 0;
            lastMinVectorX = 0;
            stateRobot = STATE_ESQUIVE;
        }
    }else {
        if (iterationCount < MAX_ESQUIVE_ITERATION){
            if( -vectorX <= ESQUIVE_TH_L){
                stateRobot = STATE_AVANCE;
            }
            iterationCount += 1;
        }
        else if (iterationCount >= MAX_ESQUIVE_ITERATION){
            if(minVectorX > vectorX){
                minVectorX = vectorX;
                iterationCount = MAX_ESQUIVE_ITERATION;
            }
            if(lastMinVectorX == minVectorX){
                iterationCount += 1;
            }
            lastMinVectorX = minVectorX;
        
            if(iterationCount >= MAX_ESQUIVE_CHECK_ITERATION){
                stateRobot = STATE_FIND_POTENTIAL_EXIT;
            }
        }
    }
    
    if(stateRobot == STATE_ESQUIVE || stateRobot == STATE_ESQUIVE_EN_COURS){
        if(vectorY > 0){
            leftMotorSpeed = -ESQUIVE_SPEED_PERCENT;
            rightMotorSpeed = ESQUIVE_SPEED_PERCENT;
        }
        else {
            leftMotorSpeed = ESQUIVE_SPEED_PERCENT;
            rightMotorSpeed = -ESQUIVE_SPEED_PERCENT;
        }
    }
    else{
        
    }
    
    
    if(vectorY > 0 || vectorY < 0){//Steer Left
        stateRobot = STATE_AVANCE_TOURNE;
    }
    else {
        stateRobot = STATE_AVANCE;
    }
    
    //Si l on n est pas dans la transition de l etape en cours
    if (nextStateRobot != stateRobot - 1){
        stateRobot = nextStateRobot;  
    }
    
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
}

float repulsePow(float dist){
    if(dist>= MAX_DIST) dist = MAX_DIST;
    if(dist<= MIN_DIST) dist = MIN_DIST;
    return K_REPULSE / (dist * dist);
}