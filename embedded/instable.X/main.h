#ifndef MAIN_H
#define	MAIN_H

#define FCY 60000000 //Timer Clock Frequency

#define SENSOR_NB 5
#define MAX_DIST 50
#define MIN_DIST 10

#define K_REPULSE 350
#define K_CORRECTION 5

#define BASE_SPEED_PERCENT 25
#define ESQUIVE_SPEED_PERCENT 15

#define ESQUIVE_TH_H 5 //Engage Esquive 
#define ESQUIVE_TH_L 4 //Disengage Esquive
#define MAX_ESQUIVE_ITERATION 5000// 1kHz -> 1000/s
#define MAX_ESQUIVE_CHECK_ITERATION MAX_ESQUIVE_ITERATION*2

#define STATE_ATTENTE 0
#define STATE_ATTENTE_EN_COURS 1
#define STATE_AVANCE 2
#define STATE_AVANCE_EN_COURS 3
#define STATE_AVANCE_TOURNE 4
#define STATE_AVANCE_TOURNE_EN_COURS 5
#define STATE_ESQUIVE 6
#define STATE_ESQUIVE_EN_COURS 7
#define STATE_ARRET 8
#define STATE_ARRET_EN_COURS 9
#define STATE_FIND_POTENTIAL_EXIT 10
#define STATE_FIND_POTENTIAL_EXIT_EN_COURS 11

typedef struct Obstacle {
    uint8_t front;
    uint8_t eright;
    uint8_t eleft;
    uint8_t right;
    uint8_t left;
    
} OBSTACLE;
void OperatingSystemLoop(void);
void SetNextRobotStateInAutomaticMode(void);
float repulsePow(float dist);

#endif	/* MAIN_H */

