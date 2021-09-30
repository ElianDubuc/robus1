#include <arduino.h>
#include <stdio.h>
#include <librobus.h>

void setup() {
  //Initialisation de la plateforme.
  BoardInit();
}

//Séquence principale.
void loop() {
  Serial.write(ENCODER_Read(0));
  delay(2000);
}