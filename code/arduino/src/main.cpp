/*
    Veuilliez seulement appeler les fonctions. Ne pas déclarer et coder des 
    fonctions dans ce fichier.
*/
//Include
#include "fonction.h"

void setup() 
{
  BoardInit();
  initpins();
  
}

void loop()
{
  int i;
  for(i=0; i<10; i++)
    {
      for(int x=0; x<50; x++)
      {
        afficherNb(9-i, DISPLAY0);
        afficherNb(i, DISPLAY1);
      }
    }
    i=0;
}
