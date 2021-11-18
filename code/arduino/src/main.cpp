/*
    Veuilliez seulement appeler les fonctions. Ne pas déclarer et coder des 
    fonctions dans ce fichier.
*/
#include <Arduino.h>
#include <stdio.h>
#include <LibRobus.h>
#include <math.h>
#include <Wire.h>
#include "Adafruit_TCS34725.h"
#include "fonction.h"
Adafruit_TCS34725 tcs = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_50MS, TCS34725_GAIN_4X);

void setup() 
{
  BoardInit();
}

void loop() 
{
  
}
