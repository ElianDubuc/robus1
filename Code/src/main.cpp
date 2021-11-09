#include <Arduino.h>
#include <stdio.h>
#include <LibRobus.h>
#include <math.h>
#include <Wire.h>
#include "Adafruit_TCS34725.h"

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

#define MONTER 1
#define BAISSER 0

void deplacement(float, bool);
void tourner(float);
void tournerSurLui(float);
int detectionsifflet();
void suiveurLignes();
void capteurIR();

//---Variables du suiveur de ligne---
//Variables des pins
int ls = 48;
int cs = 47;
int rs = 46;
int lineS = 49;
//Étape de scan de couleur/quille
bool goToQuille = false;
bool goToColorSample = false;
float const SPEED_LINE = 0.25;
//-----------------------------------

void setup() {
  // put your setup code here, to run once: :')
  //Serial.begin(9600);
  Serial.begin(115200);
  //---Déclaration des pins pour les capteurs de ligne---
  pinMode(lineS, INPUT);
  //-----------------------------------------------------
  BoardInit();
}

void loop() {
  // put your main code here, to run repeatedly:
  //Serial.print(ENCODER_Read(0));
  //Serial.println(detectionsifflet());
  //detectionsifflet();
  //suiveurlignes();
  capteurIR();
  delay(1000);
}

void deplacement(float d, bool decel)
{
  double PULSE_PER_DELAY = PULSES_PAR_SEC / 20;
  int loopCnt = 0;
  int preDecec = 0;
  double pulseTh = 0; //Pulse théorique
  //double lastPulseTh = 0;
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

int detectionsifflet()
{

  //Serial.println(analogRead(A0));
  //Serial.println(analogRead(A1));
  int sifflet = analogRead(A1) - analogRead(A0);
  Serial.println(sifflet);
  if(sifflet>50)
    return 1;
  else
    return 0;
}

void suiveurLignes()
{
  int trame = 0;
  trame = digitalRead(lineS);
  trame = (trame << 1) + digitalRead(ls);
  trame = (trame << 1) + digitalRead(cs);
  trame = (trame << 1) + digitalRead(rs);
  Serial.println(trame);
  if(ROBUS_IsBumper(3)) //Bumper est temporaire, à remplacer par le capteur de son
  {
    goToQuille = true;
    if(ROBUS_IsBumper(3) && goToQuille == true)
    {
      goToColorSample = true;
    }    
  }
  switch (trame)
  {
  case 0: //Aucune ligne captée
    MOTOR_SetSpeed(RIGHT, SPEED_LINE);
    MOTOR_SetSpeed(LEFT, SPEED_LINE);
    break;
  case 1: //Ligne sur capteur de droit
    MOTOR_SetSpeed(RIGHT, -SPEED_LINE);
    MOTOR_SetSpeed(LEFT, SPEED_LINE);
    break;
  case 2: //Ligne sur capteur central
    MOTOR_SetSpeed(RIGHT, SPEED_LINE);
    MOTOR_SetSpeed(LEFT, SPEED_LINE);
  break;
  case 3: //Ligne sur capteur central et droit
    MOTOR_SetSpeed(RIGHT, -SPEED_LINE);
    MOTOR_SetSpeed(LEFT, SPEED_LINE);
    break;
  case 4: //Ligne sur capteur de droite
    MOTOR_SetSpeed(RIGHT, SPEED_LINE);
    MOTOR_SetSpeed(LEFT, -SPEED_LINE);
    break;
  case 6: //Ligne sur capteur central et gauche
    MOTOR_SetSpeed(RIGHT, SPEED_LINE);
    MOTOR_SetSpeed(LEFT, -SPEED_LINE);
    break;
  case 8: //Condition pour tourner à droite pour scan de couleur
    if(goToColorSample == true)
    {
      MOTOR_SetSpeed(RIGHT, SPEED_LINE);
      MOTOR_SetSpeed(LEFT, -SPEED_LINE);
    }
  break;
  case 12: //Condition pour tourner à droite pour scan de couleur
    if(goToColorSample == true)
    {
      MOTOR_SetSpeed(RIGHT, SPEED_LINE);
      MOTOR_SetSpeed(LEFT, -SPEED_LINE);
    }
    break;
  case 15: //Condition pour tourner à droite pour scan de couleur
    if(goToColorSample == true)
    {
      MOTOR_SetSpeed(RIGHT, SPEED_LINE);
      MOTOR_SetSpeed(LEFT, -SPEED_LINE);
    }
    break;
  case 17: //Ligne sur tous les capteurs, arrêt
  if(goToColorSample == true)
    {
      MOTOR_SetSpeed(RIGHT, 0);
      MOTOR_SetSpeed(LEFT, 0);
    }
    break;
  default:
    Serial.println(" ");
    break;
  }
}

void capteurIR()
{
  float capteur0 = ROBUS_ReadIR(0);
  float capteur1 = ROBUS_ReadIR(1);
  bool capt = false;

  Serial.println(capteur0);
  //Serial.println(capteur1);
  /*if(capteur0 < 380 && capteur0 > 320)
  {
    while(capt == false)
    {
      MOTOR_SetSpeed(0, -0.1);
      MOTOR_SetSpeed(1, 0.1);

      if(capteur1 < 380 && capteur1 > 320)
      {
        //Serial.println();
        capt = true;
      }
    }
    MOTOR_SetSpeed(0, 0);
    MOTOR_SetSpeed(1, 0);
  }*/
}
