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
            robotState.distanceTelemetreEGauche = LimitToInterval(34 / volts - 5, MIN_DIST, MAX_DIST);
            volts = ((float) result [1])* 3.3 / 4096;
            robotState.distanceTelemetreGauche = LimitToInterval(34 / volts - 5, MIN_DIST, MAX_DIST);
            volts = ((float) result [2])* 3.3 / 4096;
            robotState.distanceTelemetreCentre = LimitToInterval(34 / volts - 5, MIN_DIST, MAX_DIST);
            volts = ((float) result [3])* 3.3 / 4096;
            robotState.distanceTelemetreDroit = LimitToInterval(34 / volts - 5, MIN_DIST, MAX_DIST);
            volts = ((float) result [4])* 3.3 / 4096;
            robotState.distanceTelemetreEDroit = LimitToInterval(34 / volts - 5, MIN_DIST, MAX_DIST);
        }
    } // fin main
}

unsigned char stateRobot;
float leftMotorSpeed = 0.0;
float rightMotorSpeed = 0.0;
void OperatingSystemLoop(void){
#ifndef __STOP_MOTORS__ 
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
#else
    SetNextRobotStateInAutomaticMode();
#endif
}

const float SENSOR_Kx[SENSOR_NB] = {0.50000000000000, 0.86602540378444, 4.50000000000000, 0.86602540378444, 0.50000000000000};//EGauche, Gauche, Centre, Droit, EDroit
const float SENSOR_Ky[SENSOR_NB] = {0.86602540378444, 0.50000000000000, 0.00000000000000, -0.50000000000000, -0.86602540378444};//EGauche, Gauche, Centre, Droit, EDroit
unsigned char nextStateRobot = 0;
float vectorX = 0.0;
float vectorY = 0.0;
void SetNextRobotStateInAutomaticMode(void){
    float dist[SENSOR_NB] = {
        robotState.distanceTelemetreEGauche, 
        robotState.distanceTelemetreGauche,
        robotState.distanceTelemetreCentre,
        robotState.distanceTelemetreDroit,
        robotState.distanceTelemetreEDroit,
    };
    vectorX = 0.0;
    vectorY = 0.0;
    for(uint8_t i = 0; i < SENSOR_NB; i++){
        float pow = K_REPULSE / (dist[i] * dist[i]);
        vectorX -= SENSOR_Kx[i] * pow;
        vectorY -= SENSOR_Ky[i] * pow;
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
    else if (stateRobot == STATE_FIND_EXIT_ONGOING){ //Needs Work (After finding potential exit try going forward, if vectorX rising : stop until vectorX goes down or try again after x amount of time)
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
    
#if 1
    LED_BLANCHE_1 = vectorY >= 10;
    LED_BLEUE_1 = vectorY >= 5;
    LED_ORANGE_1 = vectorY == 0;
    LED_ROUGE_1 = vectorY <= -5;
    LED_VERTE_1 = vectorY <= -10;
#else
    LED_BLANCHE_1 = -vectorX >= 0;
    LED_BLEUE_1 = -vectorX >= 5;
    LED_ORANGE_1 = -vectorX >= 10;
    LED_ROUGE_1 = -vectorX >= 15;
    LED_VERTE_1 = -vectorX >= 20;
#endif
    
    LED_BLANCHE_2 = stateRobot == STATE_TRY_EXIT || stateRobot == STATE_TRY_EXIT_ONGOING;
    LED_BLEUE_2 = stateRobot == STATE_FIND_EXIT || stateRobot == STATE_FIND_EXIT_ONGOING;
    LED_ORANGE_2 = stateRobot == STATE_EVADE || stateRobot == STATE_EVADE_ONGOING;
    LED_ROUGE_2 = stateRobot == STATE_FORWARD_CONTROLED || stateRobot == STATE_FORWARD_CONTROLED_ONGOING;
    LED_VERTE_2 = stateRobot == STATE_FORWARD || stateRobot == STATE_FORWARD_ONGOING;
    /*
    LED_VERTE_2 = robotState.distanceTelemetreCentre <=60;
    LED_ROUGE_2 = robotState.distanceTelemetreCentre <=50;
    LED_ORANGE_2 = robotState.distanceTelemetreCentre <=40;
    LED_BLEUE_2 = robotState.distanceTelemetreCentre <=30;
    LED_BLANCHE_2 = robotState.distanceTelemetreCentre <=20;
     */
}