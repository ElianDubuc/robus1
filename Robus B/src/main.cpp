#include <Arduino.h>
#include <stdio.h>
#include <LibRobus.h>

#define PULSES_PAR_TOUR 3200

void deplacement(float, float);

void setup() {
  // put your setup code here, to run once: :')
  BoardInit();
  deplacement(1, 1);
  ENCODER_Reset(LEFT);
  ENCODER_Reset(RIGHT);
}

void loop() {
  // put your main code here, to run repeatedly: Allo Isa!!!
  Serial.print(ENCODER_Read(0));
}

void deplacement(float d, float v)
{
  ENCODER_Reset(LEFT);
  ENCODER_Reset(RIGHT);

  if(d > 0)
  {
    MOTOR_SetSpeed(LEFT, v);
    MOTOR_SetSpeed(RIGHT, v);
  }
  else if(d < 0)
  {
    MOTOR_SetSpeed(LEFT, -v);
    MOTOR_SetSpeed(RIGHT, -v);
  }

  while(ENCODER_Read(0) <= d*PULSES_PAR_TOUR)
  {
    Serial.println(d - ENCODER_Read(0));
    if((d*PULSES_PAR_TOUR) - ENCODER_Read(0) < (PULSES_PAR_TOUR)/4)
    {
      
      MOTOR_SetSpeed(LEFT, 0.2);
      MOTOR_SetSpeed(RIGHT, 0.2);
    }
  }
  MOTOR_SetSpeed(LEFT, 0);
  MOTOR_SetSpeed(RIGHT, 0);
}