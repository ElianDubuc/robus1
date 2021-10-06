#include <Arduino.h>
#include <stdio.h>
#include <LibRobus.h>

#define PULSES_PAR_TOUR 3200
#define PULSES_PAR_SEC 7025
#define DELAY_LOOP 50

#define KI 0.0005

int PULSE_PER_DELAY = PULSES_PAR_SEC / 20;
int loopCnt = 0;
int pulseThLeft = 0; //Pulse théorique
int pulsePrLeft = 0; //Pulse pratique
int diffLeft = 0;

int pulseThRight = 0; //Pulse théorique
int pulsePrRight = 0; //Pulse pratique
int diffRight = 0;

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
  // put your main code here, to run repeatedly:
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

  while(ENCODER_Read(LEFT) <= d*PULSES_PAR_TOUR)
  {
    
    pulseThLeft = loopCnt * PULSE_PER_DELAY;///Ajst Gauche
    pulsePrLeft = ENCODER_Read(LEFT);
    diffLeft = pulseThLeft - pulsePrLeft;

    pulseThRight = loopCnt * PULSE_PER_DELAY;//Ajst Droit
    pulsePrRight = ENCODER_Read(RIGHT);
    diffRight = pulseThRight - pulsePrRight;

    if(loopCnt > 10){
        MOTOR_SetSpeed(LEFT, v + diffLeft*KI);
        MOTOR_SetSpeed(RIGHT, v + diffRight*KI);
      }
    

    Serial.print(millis()); Serial.print("Théorique: "); Serial.print(pulseThLeft); Serial.print("\tPratique G: "); Serial.print(pulsePrLeft); Serial.print("\tPratique D: "); Serial.print(pulsePrRight);Serial.print("\tV Mot G: "); Serial.print(v + diffLeft*KI); Serial.print("\tV Mot D: "); Serial.println(v + diffRight*KI);
    delay(DELAY_LOOP);
    loopCnt++;
  }
  MOTOR_SetSpeed(LEFT, 0);
  MOTOR_SetSpeed(RIGHT, 0);
}