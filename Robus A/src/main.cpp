#include <arduino.h>
#include <stdio.h>
#include <librobus.h>
#include <math.h>

#define circonference = 23.9389;
#define DIAMETREROB 380  //défini le rayon de rotation du robot quand il tourne

void setup() {
  //Initialisation de la plateforme.
  BoardInit();
}

//--------------------------------------------------
//Prototypes des fonctions
void movefwd(int);
int distopulse(float);
void debugWheels();
void ajustementPID();
void movement(int dist);

void rotate1wheel(int, float);
void rotate2wheels(int);
//--------------------------------------------------
//Fonctions

//Circ. 23,9389cm
bool initial = true;
//Séquence principale.
void loop() {
  //movefwd(300);
  rotate1wheel(1, 90);
  
  delay(5000);
    //MOTOR_SetSpeed(LEFT, 1);
    //MOTOR_SetSpeed(RIGHT, 0.25);
  //debugWheels();

  //rotate2wheels(90);
}

//--------------------------------------------------
//Fonctions secondaires
void debugWheels() {
  //ENCODER_ReadReset(LEFT);
  //ENCODER_ReadReset(RIGHT);
  Serial.println(ENCODER_Read(LEFT));
  Serial.println(ENCODER_Read(RIGHT));
  Serial.println("--------------------");
}


void ajustementPID()
{
  double leftWheelRotation = ENCODER_ReadReset(LEFT);
  double rightWheelRotation = ENCODER_ReadReset(RIGHT);

  if(leftWheelRotation > rightWheelRotation)
  {
    double rapport = leftWheelRotation / rightWheelRotation;
    Serial.println(rapport);
    MOTOR_SetSpeed(LEFT, 0.24);
    MOTOR_SetSpeed(RIGHT, 0.25 * rapport);
    Serial.println("Ajustement gauche");
    Serial.println(leftWheelRotation);
    Serial.println(rightWheelRotation);
    Serial.println("----------------");
  }
  else if (leftWheelRotation < rightWheelRotation)
  {
    double rapport = rightWheelRotation / leftWheelRotation;
    Serial.println(rapport);
    MOTOR_SetSpeed(LEFT, 0.24 * rapport);
    MOTOR_SetSpeed(RIGHT, 0.25);
    Serial.println("Ajustement droit");
    Serial.println(leftWheelRotation);
    Serial.println(rightWheelRotation);
    Serial.println("----------------");
  }
}

//Séquence pour avancer
//La fonction prend comme entrée, la distance désirée en mm
void movefwd(int dist)
{
  int x = 0;
  int length = distopulse(dist);
  //Serial.print("The length is: ");
  //Serial.print(length);
  //Serial.print("\n");

  ENCODER_Reset(0);
  do
  {
    MOTOR_SetSpeed(RIGHT, 0.25);
    MOTOR_SetSpeed(LEFT, 0.24);
    x = ENCODER_Read(0);
    Serial.println(x);
    delay(50);
  }while(x < length);
 
  MOTOR_SetSpeed(RIGHT, 0);
  MOTOR_SetSpeed(LEFT, 0);
  
}

//Cette fonction convertis une distance en pulses
//L'entrée est une distance en mm et sort le nombre de pulses
//pour accomplir cette distance
int distopulse(float dist)
{
  int nbpulses = 0;
  nbpulses = dist*13.367;
  //nbpulses = dist/0.0748;
  return nbpulses;
}

//Fonction qui fait tourner le robot en utilisant seulement une roue.
//Première entrée est la direction (0 pour gauche, 1 pour droite) et la
//Deuxieme entrée est l'angle de rotation désirée.
void rotate1wheel(int dir, float angle)
{
  int arc = distopulse(PI*DIAMETREROB*(angle/360));
  //Serial.print("La distance de rotation est:");
  //Serial.print(arc);
  //Serial.print("\n");
  int x, moteur;
  if(dir == 0)
    moteur = 1;
  if(dir == 1 )
    moteur = 0;

  //Serial.println("La valeur de l'encodeur est:");
  ENCODER_Reset(moteur);
  x = ENCODER_Read(moteur);
  Serial.println(x);
  while(x < arc)
  {
    MOTOR_SetSpeed(moteur, 0.15);
    x = ENCODER_Read(moteur);
  }

  MOTOR_SetSpeed(moteur, 0);
}

/*
void rotate2wheels(int angle)
{ 
  int dist = 0;
  int arcrot = PI*DIAMETREROB*(angle/360);
  ENCODER_Reset(0);
  ENCODER_Reset(1);
  while(dist != arcrot)
  {
    MOTOR_SetSpeed(RIGHT, 0.25);
    MOTOR_SetSpeed(LEFT, 0.24);
    dist = ENCODER_Read(RIGHT);
    dist = ENCODER_Read(LEFT);
    Serial.println(dist);
    delay(100);
    dist++;
    }
}
*/