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
#include <SoftwareSerial.h>
#include <string.h>

//Déclaration des constantes pour l'affichage
#define DISPLAY0 10
#define DISPLAY1 11
#define DISPLAY2 13
#define PLAY 8
#define BUTTON0  3
#define MAX_JOUEURS 6
#define MAX_CARTES 8
#define MAX_JEU 2

//Constantes suiveur de ligne
#define Vmax 0.19
#define INC 0.20
#define ls 45
#define cs 46
#define rs 47
#define lineS 48

//Constantes pour la distibution de cartes
#define FIRST 29
#define SECOND 28
#define THIRD 26
#define FOURTH 27

//Déclaration des fonctions
void afficherNb(int, int);
void blink(int, int, int, int, int*);
void changedisp();
void changevalue();
void initpins();
int startGame();
void distribution(byte, int);
void donnerCarte(byte, int);
char * readRFID();
void suiveurLignes();
void distributecards();