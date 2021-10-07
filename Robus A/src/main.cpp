#include <arduino.h>
#include <stdio.h>
#include <librobus.h>

#define circonference = 23.9389;
#define DIAMETREROB 190  //défini le rayon de rotation du robot quand il tourne

void setup() {
  //Initialisation de la plateforme.
  BoardInit();
}
//--------------------------------------------------
//Prototypes des fonctions
void movefwd(int);
float distopulse(int);
void debugWheels();
void ajustementPID();
void movement(int dist);

void rotate2wheels(int);
//--------------------------------------------------
//Fonctions

//Circ. 23,9389cm
bool initial = true;
//Séquence principale.
void loop() {
  if(initial == true)
  {
    MOTOR_SetSpeed(LEFT, 0.25);
    MOTOR_SetSpeed(RIGHT, 0.25);
    initial = false;
  }

  while(1)
  {
    ajustementPID();
    delay(100);
  }
}

//Affichage des valeurs des encodeurs
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
  Serial.print("The length is: ");
  Serial.print(length);
  Serial.print("\n");

  ENCODER_Reset(0);
  do
  {
    MOTOR_SetSpeed(RIGHT, 0.25);
    MOTOR_SetSpeed(LEFT, 0.25);
    ajustementPID();
  }
  while(x < length);
  /*for(x = 0; x < length/2; x++)
  
  {
    MOTOR_SetSpeed(RIGHT, 0.25);
    MOTOR_SetSpeed(LEFT, 0.24);
  }*/

  MOTOR_SetSpeed(RIGHT, 0);
  MOTOR_SetSpeed(LEFT, 0);
  
}

//Cette fonction convertis une distance en pulses
//L'entrée est une distance en mm et sort le nombre de pulses
//pour accomplir cette distance
float distopulse(int dist)
{
  int nbpulses = 0;
  nbpulses = dist/0.0748;
  return nbpulses;
}

void rotate1wheel(int roue, int angle)
{
  int arc = PI*DIAMETREROB*(angle/360);
  int x;
  
  ENCODER_Reset(0);
  do
  {
    MOTOR_SetSpeed(roue, 0.25);
    x = ENCODER_Read(roue);
    Serial.println(x);
    delay(50);
  }while(x < arc);
}

void rotate2wheels(int angle)
{ 
  int dist = 0;
  int arcrot = PI*DIAMETREROB*(angle/360);
  ENCODER_Reset(0);
  ENCODER_Reset(1);
  while(dist != arcrot)
  {
    MOTOR_SetSpeed(RIGHT, 0.25);
    /*MOTOR_SetSpeed(LEFT, 0.24);*/
    dist = ENCODER_Read(RIGHT);
    dist = ENCODER_Read(LEFT);
    Serial.println(dist);
    delay(100);
    dist++;
    }
}