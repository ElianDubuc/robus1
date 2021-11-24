/*
    Dans ce fichier, vous devez mettre les fonctions du robot qui seront 
    appelées dans le "main.cpp". Veuilliez déclarer les fonctions et les 
    constantes (define) dans le fichier "fonction.h".
*/
//Include
#include "fonction.h"

//Variables globales
Adafruit_TCS34725 tcs = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_50MS, TCS34725_GAIN_4X);

//Fonctions
void afficherNb(int nb, int display)
{
    switch (nb)
    {
    case 0:
        digitalWrite(38,HIGH);
        digitalWrite(39,HIGH);
        digitalWrite(40,HIGH);
        digitalWrite(41,HIGH);
        digitalWrite(42,HIGH);
        digitalWrite(43,HIGH);
        digitalWrite(44,LOW);
        break;
    case 1:
        digitalWrite(38,LOW);
        digitalWrite(39,HIGH);
        digitalWrite(40,HIGH);
        digitalWrite(41,LOW);
        digitalWrite(41,LOW);
        digitalWrite(42,LOW);
        digitalWrite(43,LOW);
        digitalWrite(44,LOW);
        break;
    case 2:
        digitalWrite(38,HIGH);
        digitalWrite(39,HIGH);
        digitalWrite(40,LOW);
        digitalWrite(41,HIGH);
        digitalWrite(42,HIGH);
        digitalWrite(43,LOW);
        digitalWrite(44,HIGH);
        break;
    case 3:
        digitalWrite(38,HIGH);
        digitalWrite(39,HIGH);
        digitalWrite(40,HIGH);
        digitalWrite(41,HIGH);
        digitalWrite(42,LOW);
        digitalWrite(43,LOW);
        digitalWrite(44,HIGH);
        break;
    case 4:
        digitalWrite(38,LOW);
        digitalWrite(39,HIGH);
        digitalWrite(40,HIGH);
        digitalWrite(41,LOW);
        digitalWrite(42,LOW);
        digitalWrite(43,HIGH);
        digitalWrite(44,HIGH);
        break;
    case 5:
        digitalWrite(38,HIGH);
        digitalWrite(39,LOW);
        digitalWrite(40,HIGH);
        digitalWrite(41,HIGH);
        digitalWrite(42,LOW);
        digitalWrite(43,HIGH);
        digitalWrite(44,HIGH);
        break;
    case 6:
        digitalWrite(38,HIGH);
        digitalWrite(39,LOW);
        digitalWrite(40,HIGH);
        digitalWrite(41,HIGH);
        digitalWrite(42,HIGH);
        digitalWrite(43,HIGH);
        digitalWrite(44,HIGH);
        break;
    case 7:
        digitalWrite(38,HIGH);
        digitalWrite(39,HIGH);
        digitalWrite(40,HIGH);
        digitalWrite(41,LOW);
        digitalWrite(42,LOW);
        digitalWrite(43,LOW);
        digitalWrite(44,LOW);
        break;
    case 8:
        digitalWrite(38,HIGH);
        digitalWrite(39,HIGH);
        digitalWrite(40,HIGH);
        digitalWrite(41,HIGH);
        digitalWrite(42,HIGH);
        digitalWrite(43,HIGH);
        digitalWrite(44,HIGH);
        break;
    case 9:
        digitalWrite(38,HIGH);
        digitalWrite(39,HIGH);
        digitalWrite(40,HIGH);
        digitalWrite(41,LOW);
        digitalWrite(42,LOW);
        digitalWrite(43,HIGH);
        digitalWrite(44,HIGH);
        break;
    default:
        break;
    }

    digitalWrite(display, HIGH);
    delay(10);
    
    digitalWrite(display, LOW);
    digitalWrite(38,LOW);
    digitalWrite(39,LOW);
    digitalWrite(40,LOW);
    digitalWrite(41,LOW);
    digitalWrite(41,LOW);
    digitalWrite(42,LOW);
    digitalWrite(43,LOW);
    digitalWrite(44,LOW);
}

void initpins()
{
    pinMode(38, OUTPUT);
    pinMode(39, OUTPUT);
    pinMode(40, OUTPUT);
    pinMode(41, OUTPUT);
    pinMode(42, OUTPUT);
    pinMode(43, OUTPUT);
    pinMode(44, OUTPUT);
}
