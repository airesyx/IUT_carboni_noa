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
        
        
        case STATE_WAIT:
            timestamp = 0;
            PWMSetSpeedConsigne(0, MOTEUR_DROIT);
            PWMSetSpeedConsigne(0, MOTEUR_GAUCHE);
            stateRobot = STATE_WAIT_ONGOING;
        break;
        
        case STATE_WAIT_ONGOING:
            if (timestamp > 2000)
                stateRobot = STATE_FORWARD;
        break;
        
        case STATE_FORWARD:
            PWMSetSpeedConsigne(BASE_SPEED_PERCENT, MOTEUR_DROIT);
            PWMSetSpeedConsigne(BASE_SPEED_PERCENT, MOTEUR_GAUCHE);
            stateRobot = STATE_FORWARD_ONGOING;
        break;
        
        case STATE_FORWARD_ONGOING:
            SetNextRobotStateInAutomaticMode();
        break;
        
        case STATE_FORWARD_CONTROLED:
            PWMSetSpeedConsigne(rightMotorSpeed, MOTEUR_DROIT);
            PWMSetSpeedConsigne(leftMotorSpeed, MOTEUR_GAUCHE);
            stateRobot = STATE_FORWARD_CONTROLED_ONGOING;
        break;
        
        case STATE_FORWARD_CONTROLED_ONGOING:
            SetNextRobotStateInAutomaticMode();
        break;
        
        case STATE_EVADE:
            PWMSetSpeedConsigne(rightMotorSpeed, MOTEUR_DROIT);
            PWMSetSpeedConsigne(leftMotorSpeed, MOTEUR_GAUCHE);
            stateRobot = STATE_EVADE_ONGOING;
        break;
        
        case STATE_EVADE_ONGOING:
            SetNextRobotStateInAutomaticMode();
        break;
        
        case STATE_FIND_EXIT:
            PWMSetSpeedConsigne(rightMotorSpeed, MOTEUR_DROIT);
            PWMSetSpeedConsigne(leftMotorSpeed, MOTEUR_GAUCHE);
            stateRobot = STATE_FIND_EXIT_ONGOING;
        break;
        
        case STATE_FIND_EXIT_ONGOING:
            SetNextRobotStateInAutomaticMode();
        break;
        
        default :
            stateRobot = STATE_WAIT;
        break;
        
    }
}

const float SENSOR_COS[SENSOR_NB] = {0.642787609, 0.906307787, 1.0, 0.906307787, 0.642787609};//EGauche, Gauche, Centre, Droit, EDroit
const float SENSOR_SIN[SENSOR_NB] = {0.766044443, 0.422618261, 1.0, -0.422618261, -0.766044443};//EGauche, Gauche, Centre, Droit, EDroit
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
    for(uint8_t i = 0; i < SENSOR_NB; i++){
        float pow = repulsePow(dist[i]);
        vectorX -= SENSOR_COS[i] * pow;
        vectorY -= SENSOR_SIN[i] * pow;
    }
    float correction = LimitToInterval(vectorY * K_CORRECTION, -BASE_SPEED_PERCENT, BASE_SPEED_PERCENT);
    //Control
    static unsigned int iterationCount = 0;
    static float minVectorX = 0.0;
    static float lastMinVectorX = 0.0;
    if(stateRobot != STATE_EVADE_ONGOING){
        if( -vectorX >= ESQUIVE_TH_H){
            iterationCount = 0;
            lastMinVectorX = 0;
            stateRobot = STATE_EVADE;
        }
        else if (vectorY > 0 || vectorY < 0){//Steer Left
            stateRobot = STATE_FORWARD_CONTROLED;
        }
        else {
            stateRobot = STATE_FORWARD;
        }
    }
    else if(stateRobot == STATE_EVADE_ONGOING){
        if(iterationCount < MAX_EVADE_ITERATION){
            if( -vectorX <= ESQUIVE_TH_L){
                stateRobot = STATE_FORWARD;
            }
            if(minVectorX > vectorX){
                minVectorX = vectorX;
            }
            iterationCount += 1;
        }
        else if(iterationCount >= MAX_EVADE_ITERATION){
            if(minVectorX > vectorX){
                minVectorX = vectorX;
                iterationCount = MAX_EVADE_ITERATION;
            }
            if( Abs(lastMinVectorX - minVectorX ) < EXIT_DELTA){
                iterationCount += 1;
            }
            lastMinVectorX = minVectorX;
        
            if(iterationCount >= MAX_EVADE_DIST_CHECK_ITERATION){
                iterationCount = 0;
                leftMotorSpeed = leftMotorSpeed > 0 ? -EXIT_SPEED_PERCENT : EXIT_SPEED_PERCENT;
                rightMotorSpeed = -leftMotorSpeed;
                stateRobot = STATE_FIND_EXIT;
            }
        }
    }
    else if (stateRobot == STATE_FIND_EXIT_ONGOING){
        if( (lastMinVectorX - vectorY) < EXIT_DELTA){
            stateRobot = STATE_TRY_EXIT;
        }
        iterationCount += 1;
    }
    
    if(stateRobot == STATE_EVADE || stateRobot == STATE_EVADE_ONGOING){
        
        leftMotorSpeed = vectorY > 0 ? -EVADE_SPEED_PERCENT : EVADE_SPEED_PERCENT;
        rightMotorSpeed = -leftMotorSpeed;
        
    }
    else if(stateRobot == STATE_FORWARD_CONTROLED || stateRobot == STATE_FORWARD_CONTROLED_ONGOING){
        leftMotorSpeed = LimitToInterval(BASE_SPEED_PERCENT - correction, 0, BASE_SPEED_PERCENT);
        rightMotorSpeed = LimitToInterval(BASE_SPEED_PERCENT + correction, 0, BASE_SPEED_PERCENT);
    }
    
    
    
    LED_BLANCHE_1 = robotState.distanceTelemetreEGauche <=30;
    LED_BLEUE_1 = robotState.distanceTelemetreGauche <=30;
    LED_ORANGE_1 = robotState.distanceTelemetreCentre <=30;
    LED_ROUGE_1 = robotState.distanceTelemetreDroit <=30;
    LED_VERTE_1 = robotState.distanceTelemetreEDroit <=30;
    
    LED_VERTE_2 = robotState.distanceTelemetreCentre <=60;
    LED_ROUGE_2 = robotState.distanceTelemetreCentre <=50;
    LED_ORANGE_2 = robotState.distanceTelemetreCentre <=40;
    LED_BLEUE_2 = robotState.distanceTelemetreCentre <=30;
    LED_BLANCHE_2 = robotState.distanceTelemetreCentre <=20;
}

float repulsePow(float dist){
    if(dist>= MAX_DIST) dist = MAX_DIST;
    if(dist<= MIN_DIST) dist = MIN_DIST;
    return K_REPULSE / (dist * dist);
}