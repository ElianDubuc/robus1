#include <Arduino.h>
#include <LibRobus.h>

void mouvement(int);

void setup() {
  //Initialisation de la plateforme.
  BoardInit();
}

//Séquence principale.
void loop() {
  mouvement();
}

//La fonction mouvement prend une valeur de distnace en cm et
//la convertis en pulses qui seront envoyés au encodeurs de moteur
void mouvement(int dist);
{

}