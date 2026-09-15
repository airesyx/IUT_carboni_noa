#ifndef ROBOT_H
#define ROBOT_H
typedef struct robotStateBITS {
    unsigned char taskEnCours;
    
    //Vitesse
    float vitesseGaucheConsigne;
    float vitesseGaucheCommandeCourante;
    float vitesseDroiteConsigne;
    float vitesseDroiteCommandeCourante;
    //Distance
    float distanceTelemetreEDroit;
    float distanceTelemetreDroit;
    float distanceTelemetreCentre;
    float distanceTelemetreGauche;
    float distanceTelemetreEGauche;
} ROBOT_STATE_BITS;

extern volatile ROBOT_STATE_BITS robotState;

#define PWMPER 24.0
#define MOTEUR_DROIT 0
#define MOTEUR_GAUCHE 1

void PWMUpdateSpeed();
void PWMSetSpeedConsigne(float vitesseEnPourcents, char moteur);

#endif /* ROBOT_H */