#include <arduino.h>
#include <stdio.h>
#include <librobus.h>

void setup() {
  //Initialisation de la plateforme.
  BoardInit();
}

void debugWheels();

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
