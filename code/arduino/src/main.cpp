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
  Serial1.begin(9600); //Serial1 : RFID
  initpins();
  attachInterrupt(digitalPinToInterrupt(3), changedisp, RISING);
  attachInterrupt(digitalPinToInterrupt(2), changevalue, RISING);
}

void loop()
{
  int compteur = 0;
  while(1)
  {
    switch (G_state)
    {
    //Jeu, nb carte, nb joueur
    case 0:   //affichage parametres jeu
      for(int x=0; x<200; x++)
      {
        blink(nombrecartes, nombrejoueurs, 10, displayselect, &compteur);        
      }
      if(startGame())
        G_state = 1;
      break;
    case 1:   //distibution initale
      distribution(nombrejoueurs, nombrecartes);
      G_state = 2;
      break;
    case 2:   //affichage distribution secondaire
      for(int x=0; x<200; x++)
      {
        blink(nombrecartes, nombrejoueurs, 11, displayselect, &compteur);        
      }
      if(startGame())
        G_state = 1;
      break;
    case 3:

      break;
    default:
      break;
    }
  }
  return;
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