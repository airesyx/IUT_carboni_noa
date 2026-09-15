#ifndef ROBOT_H
#define ROBOT_H
typedef struct robotStateBITS {
    unsigned char taskEnCours;
    float vitesseGaucheConsigne;
    float vitesseGaucheCommandeCourante;
    float vitesseDroiteConsigne;
    float vitesseDroiteCommandeCourante;
} ROBOT_STATE_BITS;

extern volatile ROBOT_STATE_BITS robotState;

#define PWMPER 24.0

void PWMUpdateSpeed();
void PWMSetSpeedConsigne(float vitesseEnPourcents, char moteur);

#endif /* ROBOT_H */