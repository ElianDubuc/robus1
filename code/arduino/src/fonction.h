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

//Déclaration des constantes
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

String playerID[6] = {"0414E50DCF37","041514AF2288","041514A9B31F","0E008E46E026","0E008E9E736D","0F027D734043"}; //Liste des codes des différentes puces
int joueursTotals[6][6] = {{1},{2,5},{1,3,5},{1,3,4,6},{1,2,3,4,5},{1,2,3,4,5,6}}; //Liste des joueurs actifs dépendant du nombre de joueur sur le jeu

//Déclaration des fonctions
void afficherNb(int, int);
void blink(int, int, int, int, int*);
void changedisp();
void changevalue();
void initpins();
int startGame();