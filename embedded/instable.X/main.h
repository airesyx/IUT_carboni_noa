#ifndef MAIN_H
#define	MAIN_H

#define FCY 60000000 //Timer Clock Frequency

#define SENSOR_NB 5
#define MAX_DIST 50
#define MIN_DIST 10

#define K_REPULSE 370
#define K_CORRECTION 10
#define EXIT_DELTA 10

#define BASE_SPEED_PERCENT 25
#define EVADE_SPEED_PERCENT 10
#define EXIT_SPEED_PERCENT 10

#define ESQUIVE_TH_H 5 //Engage Esquive 
#define ESQUIVE_TH_L 4 //Disengage Esquive
#define MAX_EVADE_ITERATION 3000 //Nb of iteration before ineterpretting as stuck (1kHz)
#define MAX_EVADE_DIST_CHECK_ITERATION MAX_EVADE_ITERATION + 500 //Nb of iteration before engaging exit finding

#define STATE_WAIT 0
#define STATE_WAIT_ONGOING 1
#define STATE_FORWARD 2
#define STATE_FORWARD_ONGOING 3
#define STATE_FORWARD_CONTROLED 4
#define STATE_FORWARD_CONTROLED_ONGOING 5
#define STATE_EVADE 6
#define STATE_EVADE_ONGOING 7
#define STATE_STOP 8
#define STATE_STOP_ONGOING 9
#define STATE_FIND_EXIT 10
#define STATE_FIND_EXIT_ONGOING 11
#define STATE_TRY_EXIT 10
#define STATE_TRY_EXIT_ONGOING 11

void OperatingSystemLoop(void);
void SetNextRobotStateInAutomaticMode(void);
float repulsePow(float dist);

#endif	/* MAIN_H */

