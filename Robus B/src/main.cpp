#include <Arduino.h>
#include <stdio.h>
#include <LibRobus.h>
#include <math.h>

#define PULSES_PAR_TOUR 3200
#define PULSES_PAR_SEC 15000 //7025
#define DELAY_LOOP 50
#define VITESSE_DEPART 0.2

#define CONVERTION_PULSE 133.67
#define CONVERTION_ANGLE 1.8

#define MIN_SPEED 0.1
#define NORMAL_SPEED 0.38
#define DECELERATION 0.01
#define POURCENT_DECEL 0.9

#define KP 0.00005
#define KI 0.0002

#define DIAMETRE_ROBOT 18.6

void deplacement(float, bool);
void tourner(float);
void tournerSurLui(float);
float ajustementAngle(float, float);

void setup() {
  // put your setup code here, to run once: :')

  //Alle du parcours
  BoardInit();
  deplacement(210, true);
  tourner(-90);
  deplacement(30, true);
  tourner(90);
  deplacement(28, true);
  tourner(90);
  deplacement(30, true);
  tourner(-45);
  /*deplacement(50, true);
  tourner(-90);
  deplacement(60, true);
  tourner(45);
  deplacement(40, true);
  tourner(23);
  deplacement(80, true);
  tournerSurLui(180);*/

  //Retour du parcours
 /*deplacement(80, true);
  tourner(-18);
  deplacement(40, true);
  tourner(-45);
  deplacement(60, true);
  tourner(90);
  deplacement(45, true);
  tourner(45);
  deplacement(35, true);
  tourner(-90);
  deplacement(28, true);
  tourner(-90);
  deplacement(33, true);
  tourner(90);
  deplacement(210, true);*/

  /*
  ENCODER_Reset(LEFT);
  MOTOR_SetSpeed(LEFT, 0.65);
  delay(1000);
  Serial.println(ENCODER_Read(0));
  MOTOR_SetSpeed(LEFT, 0);
*/
}

void loop() {
  // put your main code here, to run repeatedly:
  //Serial.print(ENCODER_Read(0));
}

void deplacement(float d, bool decel)
{
  double PULSE_PER_DELAY = PULSES_PAR_SEC / 20;
  int loopCnt = 0;
  int preDecec = 0;
  double pulseTh = 0; //Pulse théorique
  double speed = NORMAL_SPEED;

  int pulsePrLeft = 0; //Pulse pratique
  int diffLeft = 0;
  int lastPulseLeft = 0;
  int KPDiffPrLeft = 0;
  int KPDiffLeft = 0;

  int pulsePrRight = 0; //Pulse pratique
  int diffRight = 0;
  int lastPulseRight = 0;
  int KPDiffPrRight = 0;
  int KPDiffRight = 0;

  ENCODER_Reset(LEFT);
  ENCODER_Reset(RIGHT);

  if(d > 0)
  {
    MOTOR_SetSpeed(LEFT, VITESSE_DEPART);
    MOTOR_SetSpeed(RIGHT, VITESSE_DEPART);
  }
  else if(d < 0)
  {
    MOTOR_SetSpeed(LEFT, -VITESSE_DEPART);
    MOTOR_SetSpeed(RIGHT, -VITESSE_DEPART);
  }

  while(ENCODER_Read(LEFT) <= d*CONVERTION_PULSE)
  {
    
    pulseTh = loopCnt * PULSE_PER_DELAY;


    pulsePrLeft = ENCODER_Read(LEFT);//Ajst Gauche

    KPDiffPrLeft = pulsePrLeft - lastPulseLeft;
    KPDiffLeft = PULSE_PER_DELAY - KPDiffPrLeft;

    lastPulseLeft = pulsePrLeft;
    diffLeft = pulseTh - pulsePrLeft;

    pulsePrRight = ENCODER_Read(RIGHT);//Ajst Droit

    KPDiffPrRight = pulsePrRight - lastPulseRight;
    KPDiffRight = PULSE_PER_DELAY - KPDiffPrRight;

    lastPulseRight = pulsePrRight;
    diffRight = pulseTh - pulsePrRight;

    if(ENCODER_Read(LEFT) > d*CONVERTION_PULSE*POURCENT_DECEL && decel == true){
      speed = speed - (loopCnt - preDecec)*DECELERATION;
      if(speed < MIN_SPEED){
        speed = MIN_SPEED;
      }
      Serial.println(speed);
      MOTOR_SetSpeed(LEFT, speed);
      MOTOR_SetSpeed(RIGHT, speed);
    }else{
      MOTOR_SetSpeed(LEFT, VITESSE_DEPART + KPDiffLeft*KP + diffLeft*KI);
      MOTOR_SetSpeed(RIGHT, VITESSE_DEPART + KPDiffRight*KP + diffRight*KI);
      preDecec = loopCnt;
    }

    Serial.print(loopCnt); Serial.print("\tThéorique: "); Serial.print(pulseTh); Serial.print("\tPratique G: "); Serial.print(pulsePrLeft); Serial.print("\tPratique D: "); Serial.print(pulsePrRight);Serial.print("\tV Mot G: "); Serial.print(VITESSE_DEPART + diffLeft*KI); Serial.print("\tV Mot D: "); Serial.print(VITESSE_DEPART + diffRight*KI); Serial.print("\tKP Left : "); Serial.print(KPDiffLeft);Serial.print("\tKP Right : "); Serial.println(KPDiffRight);
    delay(DELAY_LOOP);
    loopCnt++;
  }
  MOTOR_SetSpeed(LEFT, 0);
  MOTOR_SetSpeed(RIGHT, 0);
}

void tourner(float angle)
{
  delay(50);
  ENCODER_Reset(LEFT);
  ENCODER_Reset(RIGHT);
  float arc =  2 * PI * DIAMETRE_ROBOT * (abs(angle)/360);
  float dist_pulse = arc * CONVERTION_PULSE;

  if(angle > 0)
  {
    while(ENCODER_Read(LEFT) <= dist_pulse)
    {
      MOTOR_SetSpeed(LEFT, 0.4);
    }
    MOTOR_SetSpeed(LEFT, 0);
  }
  else
  {
    while(ENCODER_Read(RIGHT) <= dist_pulse)
    {
      MOTOR_SetSpeed(RIGHT, 0.4);
    }
    MOTOR_SetSpeed(RIGHT, 0);
  }
}

void tournerSurLui(float angle)
{
  ENCODER_Reset(LEFT);
  ENCODER_Reset(RIGHT);
  float arc = PI * DIAMETRE_ROBOT * (abs(angle)/360);
  float dist_pulse = arc * CONVERTION_PULSE;

  if(angle > 0)
  {
    while(ENCODER_Read(LEFT) <= dist_pulse)
    {
      MOTOR_SetSpeed(LEFT, 0.4);
      MOTOR_SetSpeed(RIGHT, -0.4);
    }
    MOTOR_SetSpeed(LEFT, 0);
    MOTOR_SetSpeed(RIGHT, 0);
  }
  else
  {
    while(ENCODER_Read(RIGHT) <= dist_pulse)
    {
      MOTOR_SetSpeed(RIGHT, 0.4);
      MOTOR_SetSpeed(LEFT, -0.4);
    }
    MOTOR_SetSpeed(RIGHT, 0);
    MOTOR_SetSpeed(LEFT, 0);
  }
}

float ajustementAngle(float angle, float dist)
{
  float ajustement = angle * CONVERTION_ANGLE;
  return dist - ajustement;
}