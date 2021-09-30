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

//--------------------------------------------------
//Fonctions

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