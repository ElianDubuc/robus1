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
    case 1:
        
        break;
    case 2:
        
        break;
    case 3:
        
        break;
    case 4:
        
        break;
    case 5:
        
        break;
    case 6:
        
        break;
    case 7:
        
        break;
    case 8:
        digitalWrite(20,HIGH);
        digitalWrite(22,HIGH);
        digitalWrite(24,HIGH);
        digitalWrite(26,HIGH);
        digitalWrite(28,HIGH);
        digitalWrite(30,HIGH);
        digitalWrite(32,HIGH);
        break;
    case 9:
        
        break;
    default:
        break;
    }
}

