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

void setup() 
{
  BoardInit();
  initpins();
  attachInterrupt(digitalPinToInterrupt(3), changedisp, RISING);
  attachInterrupt(digitalPinToInterrupt(2), changevalue, RISING);
}

void loop()
{
  int compteur;

  for(int x=0; x<200; x++)
  {
    blink(nombrecartes, nombrejoueurs, selectionjeu, displayselect, &compteur);        
  }

  /*
  switch ()
  {
  //Jeu, nb carte, nb joueur
  case 0:
    
    break;
  case 1:
    while(startGame() == false);
    break;
  default:
    break;
  }
  */
}

void changedisp()
{
  if(displayselect==0)
    displayselect=3;
  else
    displayselect--;

  int click = 0;
  while(click<30000)
  {
    click++;
  }

  Serial.print("Display selected: ");
  Serial.println(displayselect);
  
  return;
}

void changevalue()
{
  Serial.print("Changement de valeur: ");
  Serial.println(displayselect);
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

  int tick = 0;
  while(tick<10000)
  {
    tick++;
  }

  return;
}