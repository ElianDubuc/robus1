#include <arduino.h>
#include <stdio.h>
#include <librobus.h>

#define circonference = 23.9389;

void setup() {
  //Initialisation de la plateforme.
  BoardInit();
}
//--------------------------------------------------
//Prototypes des fonctions
void movement(int);
void debugWheels();
void ajustementPID();
void avancer(int);

//--------------------------------------------------
//Fonctions

//Circ. 23,9389cm
//Séquence principale.
void loop() {
  avancer(1);
  debugWheels();
}

//Permet d'avancer tout droit. Unité en cm
void avancer(int dist)
{
  MOTOR_SetSpeed(LEFT, 0.25);
  MOTOR_SetSpeed(RIGHT, 0.24);
  ajustementPID();
}

//Affichage des valeurs des encodeurs
void debugWheels() {
  ENCODER_ReadReset(0);
  ENCODER_ReadReset(1);
  Serial.println(ENCODER_Read(0));
  Serial.println(ENCODER_Read(1));
  Serial.println("--------------------");
  delay(1000);
}

//
void ajustementPID()
{
  int leftWheelRotation = ENCODER_Read(0);
  int rightWheelRotation = ENCODER_Read(1);

  while(leftWheelRotation > rightWheelRotation)
  {
    MOTOR_SetSpeed(LEFT, 0.49);
    MOTOR_SetSpeed(RIGHT, 0.5);
  }
}

//Séquence de mouvement
void movement(int dist)
{
  int x;
  MOTOR_SetSpeed(RIGHT, 0.25);
  MOTOR_SetSpeed(LEFT, 0.24);
  for(x = 0; x < dist; x++)
  {
    delay(2000);
  }
  MOTOR_SetSpeed(RIGHT, 0);
  MOTOR_SetSpeed(LEFT, 0);
  
}

