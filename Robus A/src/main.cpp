#include <arduino.h>
#include <stdio.h>
#include <librobus.h>

#define PI 3.141592
#define DIAMETREROB 190  //défini le rayon de rotation du robot quand il tourne

void setup() {
  //Initialisation de la plateforme.
  BoardInit();
}
//--------------------------------------------------
//Prototypes des fonctions
void movefwd(int);
float distopulse(int);
void debugWheels();

//--------------------------------------------------
//Fonctions

//Circ. 23,9389cm
//Séquence principale.
void loop() {
  Serial.println("The length is ");
  movefwd(300);
  
  delay(5000);
    //MOTOR_SetSpeed(LEFT, 1);
    //MOTOR_SetSpeed(RIGHT, 0.25);

  //debugWheels();
}

void debugWheels() {
  Serial.println(ENCODER_Read(0));
  Serial.println(ENCODER_Read(1));
  Serial.println("--------------------");
  delay(1000);
}

//Séquence pour avancer
//La fonction prend comme entrée, la distance désirée en mm
void movefwd(int dist)
{
  int x = 0;
  int length = distopulse(dist);
  Serial.print("The length is: ");
  Serial.print(length);
  Serial.print("\n");

  ENCODER_Reset(0);
  do
  {
    MOTOR_SetSpeed(RIGHT, 0.25);
    MOTOR_SetSpeed(LEFT, 0.24);
    x = ENCODER_Read(0);
    Serial.println(x);
    delay(50);
  }while(x < length);
  /*for(x = 0; x < length/2; x++)
  
  {
    MOTOR_SetSpeed(RIGHT, 0.25);
    MOTOR_SetSpeed(LEFT, 0.24);
  }*/

  MOTOR_SetSpeed(RIGHT, 0);
  MOTOR_SetSpeed(LEFT, 0);
  
}

//Cette fonction convertis une distance en pulses
//L'entrée est une distance en mm et sort le nombre de pulses
//pour accomplir cette distance
float distopulse(int dist)
{
  int nbpulses = 0;
  nbpulses = dist/0.0748;
  return nbpulses;
}

void rotate1wheel(int roue, int angle)
{
  int arc = PI*DIAMETREROB*(angle/360);
  int x;
  
  ENCODER_Reset(0);
  do
  {
    MOTOR_SetSpeed(roue, 0.25);
    x = ENCODER_Read(roue);
    Serial.println(x);
    delay(50);
  }while(x < arc);
}

void rotate2wheels(int angle)
{ 
  int dist;
  int arcrot = PI*DIAMETREROB*(angle/360);
  while(dist != arcrot)
  {
    ;
  }
  
}