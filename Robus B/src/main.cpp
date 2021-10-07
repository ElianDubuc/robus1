#include <Arduino.h>
#include <stdio.h>
#include <LibRobus.h>

#define PULSES_PAR_TOUR 3200
#define PULSES_PAR_SEC 10000 //7025
#define DELAY_LOOP 50

#define KP 0.00002
#define KI 0.0005



void deplacement(float, float);

void setup() {
  // put your setup code here, to run once: :')
  BoardInit();
  deplacement(5, 0.40);

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
  double PULSE_PER_DELAY = PULSES_PAR_SEC / 20;
  int loopCnt = 0;
  double pulseTh = 0; //Pulse théorique
  double lastPulseTh = 0;


  int pulsePrLeft = 0; //Pulse pratique
  int diffLeft = 0;
  int lastPulseLeft = 0;
  int KPDiffPrLeft = 0;
  int KPDiffLeft = 0;

  int pulsePrRight = 0; //Pulse pratique
  int diffRight = 0;
  int lastPulseRight = 0;
  int KPDiffPrRight = 0;
  int KPDiffRight = 0;

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
    
    pulseTh = loopCnt * PULSE_PER_DELAY;


    pulsePrLeft = ENCODER_Read(LEFT);///Ajst Gauche

    KPDiffPrLeft = pulsePrLeft - lastPulseLeft;
    KPDiffLeft = PULSE_PER_DELAY - KPDiffPrLeft;

    lastPulseLeft = pulsePrLeft;
    diffLeft = pulseTh - pulsePrLeft;


//Ajst Droit
    pulsePrRight = ENCODER_Read(RIGHT);

    KPDiffPrRight = pulsePrRight - lastPulseRight;
    KPDiffRight = PULSE_PER_DELAY - KPDiffPrRight;

    lastPulseRight = pulsePrRight;
    diffRight = pulseTh - pulsePrRight;

    //if(loopCnt > 10){
        MOTOR_SetSpeed(LEFT, v + KPDiffLeft*KP + diffLeft*KI);
        MOTOR_SetSpeed(RIGHT, v + KPDiffRight*KP + diffRight*KI);
      //}
    

    Serial.print(loopCnt); Serial.print("\tThéorique: "); Serial.print(pulseTh); Serial.print("\tPratique G: "); Serial.print(pulsePrLeft); Serial.print("\tPratique D: "); Serial.print(pulsePrRight);Serial.print("\tV Mot G: "); Serial.print(v + diffLeft*KI); Serial.print("\tV Mot D: "); Serial.print(v + diffRight*KI); Serial.print("\tKP Left : "); Serial.print(KPDiffLeft);Serial.print("\tKP Right : "); Serial.println(KPDiffRight);
    delay(DELAY_LOOP);
    loopCnt++;
  }
  MOTOR_SetSpeed(LEFT, 0);
  MOTOR_SetSpeed(RIGHT, 0);
}