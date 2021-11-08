#include <Arduino.h>
#include <stdio.h>
#include <LibRobus.h>
#include <math.h>

void setup() {

  SERVO_Enable(0);
  SERVO_Enable(1);
  //SGauche.write(40);  //Position base
  //SDroit.write(165);
  SERVO_SetAngle(0,  40);
  SERVO_SetAngle(1, 165);

  delay(2000);
  //SGauche.write(125);  //position haute
  //SDroit.write(75);
  SERVO_SetAngle(0,  125);
  SERVO_SetAngle(1, 75);

  }
  

  void loop() {/*
  // put your main code here, to run repeatedly:
  SGauche.write(90);  // set servo to mid-point
  SDroit.write(90);
  delay(2000);
  SGauche.write(0);  // set servo to mid-point
  SDroit.write(180);
  delay(2000);*/
}