#include <Arduino.h>
#include <stdio.h>
#include <LibRobus.h>

#define PULSES_PAR_TOUR 3200
#define PULSES_PAR_SEC 7025
#define DELAY_LOOP 50

int PULSE_PER_DELAY = PULSES_PAR_SEC / 20;
int loopCnt = 0;
int pulseTh = 0; //Pulse théorique

void deplacement(float, float);

void setup() {
  // put your setup code here, to run once: :')
  BoardInit();
  deplacement(10, 0.65);

  /*
  ENCODER_Reset(LEFT);
  MOTOR_SetSpeed(LEFT, 0.65);
  delay(1000);
  Serial.println(ENCODER_Read(0));
  MOTOR_SetSpeed(LEFT, 0);
*/
}

void loop() {
  // put your main code here, to run repeatedly: Allo Isa!!!
  //Serial.print(ENCODER_Read(0));
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

  loopCnt = 0;

  while(ENCODER_Read(0) <= d*PULSES_PAR_TOUR)
  {
    pulseTh = loopCnt * PULSE_PER_DELAY;
    Serial.print("Théorique: "); Serial.print(pulseTh); Serial.print("\tPratique: "); Serial.println(ENCODER_Read(0));
    delay(DELAY_LOOP);
    loopCnt++;
  }
  MOTOR_SetSpeed(LEFT, 0);
  MOTOR_SetSpeed(RIGHT, 0);
}