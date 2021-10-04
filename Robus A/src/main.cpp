#include <arduino.h>
#include <stdio.h>
#include <librobus.h>

void setup() {
  //Initialisation de la plateforme.
  BoardInit();
}
//--------------------------------------------------
//Prototypes des fonctions
void movement(int);
void debugWheels();

//--------------------------------------------------
//Fonctions

//Circ. 23,9389cm
//Séquence principale.
void loop() {

    MOTOR_SetSpeed(LEFT, 1);
    MOTOR_SetSpeed(RIGHT, 0.25);

  debugWheels();
}

void debugWheels() {
  Serial.println(ENCODER_Read(0));
  Serial.println(ENCODER_Read(1));
  Serial.println("--------------------");
  delay(1000);
}

//Séquence principale.
void loop() {
  movement(5);
  Serial.write(ENCODER_Read(0));
  delay(2000);
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

//Cette fonction convertis une distance en pulses
//L'entrée est une distance en mm et sort le nombre de pulses
//pour accomplir cette distance
int distopulse(int dist)
{
  float nbpulses = 0;

  nbpulses = dist/0.0748;
}