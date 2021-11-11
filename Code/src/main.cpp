#include <Arduino.h>
#include <stdio.h>
#include <LibRobus.h>
#include <math.h>
#include <Wire.h>
#include "Adafruit_TCS34725.h"

Adafruit_TCS34725 tcs = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_50MS, TCS34725_GAIN_4X);

#define PULSES_PAR_TOUR 3200
#define PULSES_PAR_SEC 5000 //7025
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
#define MILIEU 2
#define BAISSER 3

#define Vmax 0.25
#define INC 0.08
#define bingShillingSpeed 0.10


#define BLEU 1
#define ROSE 2
#define JAUNE 3

void deplacement(float, bool);
void tourner(float);
void tournerSurLui(float);
int detectionsifflet();
void suiveurLignes(bool);
void capteurIR();
void detectionQuille();
int capteurSonor();
int detectquille();
int couleur();
void bonerAlerte(int);
void bing_shilling(int);
void colormatter(int);

//---Variables du suiveur de ligne---
//Variables des pins
int ls = 45;
int cs = 46;
int rs = 47;
int lineS = 48;
int ledVerte = 42;
int ledJaune = 41;
int ledRouge = 40;
int ledBleu = 43;
float const SPEED_LINE = 0.25;
// defines pins numbers
const int trigPin = 23;
const int echoPin = 22;
// defines variables
long duration;
float VLeft = 0;
float Vright = 0;
int nbMesure = 0;
//int distance;

//Variable loop
int etat = 1;
bool haveTurned = false;
int couleurASeRendre = 0;
//-----------------------------------

enum etats{
  e_suiveur = 0,
  e_dect_quille,
  e_suiveurSortie,
  e_detectCouleur,
  e_vaChercher,
  e_vaPorter,
  e_retour
};



void setup() {

  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  Serial.begin(115200);
  //---Déclaration des pins pour les capteurs de ligne---
  pinMode(ls, INPUT);
  pinMode(cs, INPUT);
  pinMode(rs, INPUT);
  pinMode(lineS, INPUT);
  //-----------------------------------------------------
  pinMode(ledVerte, OUTPUT);
  pinMode(ledRouge, OUTPUT);
  pinMode(ledBleu, OUTPUT);
  pinMode(ledJaune, OUTPUT);
  BoardInit();
  SERVO_Enable(0);//Active les deux moteurs avec les ressources de la librairie
  SERVO_Enable(1);
  SERVO_SetAngle(0,  125);//Position haute pour les deux moteurs
  SERVO_SetAngle(1, 75);
  delay(1000);
  etat = 1;
}

void loop() {
Serial.print("Mode: ");Serial.println(etat);
  switch (etat)
  {
  case 1:
    if(detectionsifflet() == 1)
    {
      etat = 2;
      break;
    }
    else
    {
      suiveurLignes(false);
    }
    break;
  
  case 2:
    detectionQuille();
    break;
  case 3:
    digitalWrite(ledRouge, LOW);
    digitalWrite(ledJaune, LOW);
    digitalWrite(ledBleu, LOW);
    suiveurLignes(true);
    break;
  case 4:
    Serial.println(couleur());
    couleurASeRendre = couleur();
    if(couleurASeRendre != 0)
    {
      bing_shilling(8);
      etat = 5;
    }
    break;
  case 5:
    //Va chercher la balle et enlignement corridor
    deplacement(7, false);
    bonerAlerte(BAISSER);
    delay(1000);
    colormatter(couleurASeRendre);
    
    etat = 7;
    break;
  case 6:
    //Avance jusqu'au fond du corridor
    break;
  case 7:
    //retourne sur la ligne blanche
    digitalWrite(ledRouge, LOW);
    digitalWrite(ledJaune, LOW);
    digitalWrite(ledBleu, LOW);
    haveTurned = false;
    etat = 1;
    //suiveurLignes(true);
    break;
  default:
    break;
  }
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

    //Serial.print(loopCnt); Serial.print("\tThéorique: "); Serial.print(pulseTh); Serial.print("\tPratique G: "); Serial.print(pulsePrLeft); Serial.print("\tPratique D: "); Serial.print(pulsePrRight);Serial.print("\tV Mot G: "); Serial.print(VITESSE_DEPART + diffLeft*KI); Serial.print("\tV Mot D: "); Serial.print(VITESSE_DEPART + diffRight*KI); Serial.print("\tKP Left : "); Serial.print(KPDiffLeft);Serial.print("\tKP Right : "); Serial.println(KPDiffRight);
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
  /*Serial.print("\nSignal: ");
  Serial.print(analogRead(A1));
  Serial.print(" Bruit ambiant: ");
  Serial.print(analogRead(A0));*/
  int sifflet = analogRead(A1) - analogRead(A0);
  //Serial.print(" Delta: ");
  Serial.println(sifflet);
  if(sifflet>0)
  {
    digitalWrite(ledRouge, HIGH);
    digitalWrite(ledJaune, HIGH);
    digitalWrite(ledBleu, HIGH);
    return 1;
  }
  else
    return 0;
}

void suiveurLignes(bool goToColorSample)
{
  int trame = 0;
  trame = digitalRead(lineS);
  trame = (trame << 1) + digitalRead(ls);
  trame = (trame << 1) + digitalRead(cs);
  trame = (trame << 1) + digitalRead(rs);
  //Serial.println(trame);
  switch (trame)
  {
  case 0: //Aucune ligne captée
    //MOTOR_SetSpeed(RIGHT, SPEED_LINE);
    //MOTOR_SetSpeed(LEFT, SPEED_LINE);
    VLeft += INC;
    Vright += INC;
    break;
  case 1: //Ligne sur capteur de droit
    //MOTOR_SetSpeed(RIGHT, -SPEED_LINE);
    //MOTOR_SetSpeed(LEFT, SPEED_LINE);
    VLeft += INC;
    Vright -= INC;
    break;
  case 2: //Ligne sur capteur central
    //MOTOR_SetSpeed(RIGHT, SPEED_LINE);
    //MOTOR_SetSpeed(LEFT, SPEED_LINE);
    VLeft += INC;
    Vright += INC;
  break;
  case 3: //Ligne sur capteur central et droit
    //MOTOR_SetSpeed(RIGHT, SPEED_LINE);
    //MOTOR_SetSpeed(LEFT, SPEED_LINE);
    VLeft += INC;
    Vright += INC;
    break;
  case 4: //Ligne sur capteur de gauche
    //MOTOR_SetSpeed(RIGHT, SPEED_LINE);
    //MOTOR_SetSpeed(LEFT, -SPEED_LINE);
    VLeft -= INC;
    Vright += INC;
    break;
  case 6: //Ligne sur capteur central et gauche
    //MOTOR_SetSpeed(RIGHT, SPEED_LINE);
    //MOTOR_SetSpeed(LEFT, SPEED_LINE);
    VLeft += INC;
    Vright += INC;
    break;
  break;
  case 8 ... 14: //Condition pour tourner à droite pour scan de couleur
    if(goToColorSample == true)
    {
      if(haveTurned == false)
      {
        MOTOR_SetSpeed(RIGHT, 0);
        MOTOR_SetSpeed(LEFT, 0);
        tourner(-110);
        deplacement(5, false);
        haveTurned = true;
        
      }
      else
      {
        MOTOR_SetSpeed(RIGHT, SPEED_LINE);
        MOTOR_SetSpeed(LEFT, -SPEED_LINE);
      }
    }
    break;
    case 15:
    {
      if(goToColorSample == true)
      {
        if(haveTurned == false)
        {
          MOTOR_SetSpeed(RIGHT, 0);
          MOTOR_SetSpeed(LEFT, 0);
          delay(500);
          MOTOR_SetSpeed(RIGHT, -0.2);
          MOTOR_SetSpeed(LEFT, -0.2);
          delay(500);
          MOTOR_SetSpeed(RIGHT, 0);
          MOTOR_SetSpeed(LEFT, 0);
          tourner(-100);
          deplacement(5, false);
          haveTurned = true;
        }
        else
        {
          MOTOR_SetSpeed(RIGHT, 0);
          MOTOR_SetSpeed(LEFT, 0);
          Serial.println("a l'aide");
          delay(1000);
          MOTOR_SetSpeed(LEFT, 0.25);
          MOTOR_SetSpeed(RIGHT, 0.25);
          do{
              trame = digitalRead(lineS);
              trame = (trame << 1) + digitalRead(ls);
              trame = (trame << 1) + digitalRead(cs);
              trame = (trame << 1) + digitalRead(rs);
              Serial.println("testing");
          }
          while (trame != 0);

          etat = 4;
          MOTOR_SetSpeed(LEFT, 0);
          MOTOR_SetSpeed(RIGHT, 0);
        }
      }
    }
  default:
    Serial.println(" ");
    break;
  }

    if(VLeft > Vmax){
      VLeft = Vmax;
    }
    if(Vright > Vmax){
      Vright = Vmax;
    }
    if(VLeft < -Vmax){
      VLeft = -Vmax;
    }
    if(Vright < -Vmax){
      Vright = -Vmax;
    }
    MOTOR_SetSpeed(RIGHT, Vright);
    MOTOR_SetSpeed(LEFT, VLeft);
    delay(10);
}

/*int detectquilleIR()
{
  float capteur0 = ROBUS_ReadIR(0);
  float capteur1 = ROBUS_ReadIR(1);
  float capteur, old0, old1, old;
  int detection = 0;

  while(detection == 0)
  {
    old0 = capteur0;
    old1 = capteur1;
    old = (old0 + old1)/2;
    delay(300);
    capteur0 = ROBUS_ReadIR(0);
    capteur1 = ROBUS_ReadIR(1);
    capteur = (capteur0 + capteur1)/2;
    Serial.println(capteur);
    if(capteur>old+50)
      {
        detection = 1;
        Serial.println("Quille trouve");
      }
  }
  return 1;
}*/


void detectionQuille()
{
  int dist_quille = capteurSonor();
  if(dist_quille < 40 && dist_quille > 5 && dist_quille != 0)
  {
    nbMesure++;
  }
  else
  {
    suiveurLignes(false);
    nbMesure = 0;
  }

  if (nbMesure >= 2)
  {
    MOTOR_SetSpeed(0,0);
    MOTOR_SetSpeed(1,0);
    digitalWrite(ledVerte, HIGH);
    tourner(90);
    deplacement(dist_quille, false);
    digitalWrite(ledVerte, LOW);
    tournerSurLui(180);
    deplacement(dist_quille*0.95, true);
    tourner(90);
    delay(1000);
    etat = 3;
  }
  
  
}

void capteurIR()
{
  float capteur0 = ROBUS_ReadIR(0);
  float capteur1 = ROBUS_ReadIR(1);
}

int capteurSonor()
{    
  int distance;
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  
  duration = pulseIn(echoPin, HIGH);
  
  distance= duration*0.034/2;
  Serial.print("Distance: ");
  Serial.println(distance);
  return distance;
}

int couleur()
{
  delay(50);
  MOTOR_SetSpeed(RIGHT, 0);
  MOTOR_SetSpeed(LEFT, 0);
  
   uint16_t clear, red, green, blue;
   tcs.getRawData(&red, &green, &blue, &clear);
    /*Serial.print("C:\t"); Serial.println(clear);
    Serial.print("R:\t"); Serial.println(red);
    Serial.print("G:\t"); Serial.println(green);
    Serial.print("B:\t"); Serial.println(blue); Serial.print("\n");*/
  if( 403>red && red>303 && 568>blue && blue>485 &&  525>green && green>440)
  {
    Serial.print("bleu");
    digitalWrite(ledBleu, HIGH);
    return BLEU;
  }
  else if(516>red && red>416 && 463>green && green>393 && 528>blue && blue>428  )
  {
    digitalWrite(ledRouge, HIGH);
    return ROSE;
  }
  else if(548>red && red>448 && 567>green && green>467 && 480>blue && blue>380 )
  {
    digitalWrite(ledJaune, HIGH);
    return JAUNE;
  }
  else
  {
    return 0;
  }
}
/*  delay(1000);  // takes 50ms to read

    tcs.getRawData(&red, &green, &blue, &clear);
    Serial.print("C:\t"); Serial.println(clear);
    Serial.print("R:\t"); Serial.println(red);
    Serial.print("G:\t"); Serial.println(green);
    Serial.print("B:\t"); Serial.println(blue); Serial.print("\n");*/

void bonerAlerte(int position)
{
  if(position == MONTER)
  {
    SERVO_SetAngle(0, 125);
    SERVO_SetAngle(1, 75);
  }
  else if(position == BAISSER)
  {
    SERVO_SetAngle(0, 90);
    SERVO_SetAngle(1, 150);
  }
  else if(position == MILIEU)
  {
    SERVO_SetAngle(0, 83);
    SERVO_SetAngle(1, 120);
  }
}

void bing_shilling(int bing)
{
  bool m1, m2;
  int trame = 0;
  for (int i = 0; i < bing; i++)
  {
    if(i%2 == 0)
    {
      MOTOR_SetSpeed(LEFT, -bingShillingSpeed);
      MOTOR_SetSpeed(RIGHT, -bingShillingSpeed);
      m1 = m2 = true;
      while(m1 || m2)
      {
        Serial.println("Recule ");
        trame = digitalRead(lineS);
        trame = (trame << 1) + digitalRead(ls);
        trame = (trame << 1) + digitalRead(cs);
        trame = (trame << 1) + digitalRead(rs);
        if(digitalRead(ls))
        {
          Serial.println("Left ");
          MOTOR_SetSpeed(LEFT, 0);
          m1 = false;
        }
        if(digitalRead(rs))
        {
          Serial.println("Right ");
          MOTOR_SetSpeed(RIGHT, 0);
          m2 = false;
        }
        delay(50);
      }
    }
    else
    {
      MOTOR_SetSpeed(LEFT, bingShillingSpeed);
      MOTOR_SetSpeed(RIGHT, bingShillingSpeed);
      m1 = m2 = true;
      while(m1 || m2)
      {
        Serial.println("Avance ");
        trame = digitalRead(lineS);
        trame = (trame << 1) + digitalRead(ls);
        trame = (trame << 1) + digitalRead(cs);
        trame = (trame << 1) + digitalRead(rs);
        if(!digitalRead(ls))
        {
          Serial.println("Left ");
          MOTOR_SetSpeed(LEFT, 0);
          m1 = false;
        }
        if(!digitalRead(rs))
        {
          Serial.println("Right ");
          MOTOR_SetSpeed(RIGHT, 0);
          m2 = false;
        }
        delay(50);
      }
    }
    
    MOTOR_SetSpeed(LEFT, 0);
    MOTOR_SetSpeed(RIGHT, 0);
    delay(50);
  }
}

 void colormatter(int couleurCaptee)
 {
   delay(50);
   //Serial.println("dddddddddddddddddddddddddddddddddddddddddddddddddd");
   //Serial.println(couleurCaptee);
   if(couleurCaptee == BLEU)
   {
     Serial.println("bleu");
    tournerSurLui(-85);
     delay(1000);
     deplacement(45,true);
     delay(1000);
     tournerSurLui(84);
     delay(1000);
     deplacement(217,true);
     delay(1500);
     bonerAlerte(MONTER);
     delay(1500);
     tournerSurLui(170);
     deplacement(215,true);
     delay(1000);
     //tournerSurLui(-25);
   }
   else if(couleurCaptee == ROSE)
   {
     Serial.println("rose");
     tournerSurLui(-1);
     deplacement(217,true);
     delay(1500);
     bonerAlerte(MONTER);
     delay(1500);
     tournerSurLui(170);
     deplacement(215,true);
     delay(1000);
     tournerSurLui(-10);
     deplacement(30, false);
   }
   else if (couleurCaptee == JAUNE)
   {
    Serial.println("Jaune");
     tournerSurLui(85);
     delay(1000);
     deplacement(45,true);
     delay(1000);
     tournerSurLui(-86);
     delay(1000);
     deplacement(215,true);
     delay(1500);
     bonerAlerte(MONTER);
     delay(1500);
     tournerSurLui(171);
     deplacement(220,true);
     delay(1000);
     tourner(20);
   }
   Serial.println("Fuck");
 }