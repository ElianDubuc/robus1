/*
    Veuilliez déclarer les fonctions et les constantes qui seront utilisées dans 
    le fichier "fonction.h" et appelées dans le "main.cpp".
*/
//Includes
#include <Arduino.h>
#include <stdio.h>
#include <LibRobus.h>
#include <math.h>
#include <Wire.h>
#include "Adafruit_TCS34725.h"


//Déclaration des constantes
#define DISPLAY0 10
#define DISPLAY1 11
#define DISPLAY2 13
#define BUTTON0  3
#define MAX_JOUEURS 6
#define MAX_CARTES 8
#define MAX_JEU 2


//Déclaration des fonctions
void afficherNb(int, int);
void blink(int, int, int, int, int*);
void changedisp();
void changevalue();
void initpins();