/*
    Veuilliez seulement appeler les fonctions. Ne pas déclarer et coder des 
    fonctions dans ce fichier.
*/
//Include
#include "fonction.h"

//Declaration variables globales
int displayselect=3;
int nombrejoueurs=1;
int nombrecartes=1;
int selectionjeu=1;
int G_state=0;

void setup() 
{
  BoardInit();
  initpins();
  attachInterrupt(digitalPinToInterrupt(3), changedisp, RISING);
  attachInterrupt(digitalPinToInterrupt(2), changevalue, RISING);
}

void loop()
{
  switch (G_state)
  {
  //Jeu, nb carte, nb joueur
  case 0:
    int compteur;
    for(int x=0; x<200; x++)
    {
      blink(nombrecartes, nombrejoueurs, selectionjeu, displayselect, &compteur);        
    }
    if(startGame())
      G_state = 1;
    break;
  case 1:
    for(int y=1; y<100; y++)
    {
      afficherNb(8, DISPLAY0);
      afficherNb(8, DISPLAY1);
      afficherNb(8, DISPLAY2);
    }
    if(startGame())
      G_state = 0;
    break;
  default:
    break;
  }

}

void changedisp()
{
  if(displayselect==0)
    displayselect=3;
  else
    displayselect--;

  //Serial.print("Display selected: ");
  //Serial.println(displayselect);
  
  return;
}

void changevalue()
{
  //Serial.print("Changemet de valeur: ");
  //Serial.println(displayselect);
  switch (displayselect)
  {
  case 0:
    if(nombrecartes<MAX_CARTES)
      nombrecartes++;
    else
      nombrecartes = 1;
    break;

  case 1:
    if(nombrejoueurs<MAX_JOUEURS)
      nombrejoueurs++;
    else
      nombrejoueurs = 1;
    break;

  case 2:
    if(selectionjeu<MAX_JEU)
      selectionjeu++;
    else
      selectionjeu = 0;
    break;

  default:
    break;
  }

  return;
}