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
void initpins()
{
    //pinMode(BUTTON0, INPUT);
    pinMode(8, INPUT);
    pinMode(10, OUTPUT);
    pinMode(11, OUTPUT);
    pinMode(13, INPUT);
    pinMode(38, OUTPUT);
    pinMode(39, OUTPUT);
    pinMode(40, OUTPUT);
    pinMode(41, OUTPUT);
    pinMode(42, OUTPUT);
    pinMode(43, OUTPUT);
    pinMode(44, OUTPUT);
    return;
}

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
    delay(5);
    
    digitalWrite(display, LOW);
    digitalWrite(38,LOW);
    digitalWrite(39,LOW);
    digitalWrite(40,LOW);
    digitalWrite(41,LOW);
    digitalWrite(41,LOW);
    digitalWrite(42,LOW);
    digitalWrite(43,LOW);
    digitalWrite(44,LOW);
    return;
}

void blink(int nb1, int nb2, int nb3, int dispblk, int *compteur)
{
    switch(dispblk)
    {
        case 0:
            if(*compteur<20)
                afficherNb(nb1, DISPLAY0);
            afficherNb(nb2, DISPLAY1);
            afficherNb(nb3, DISPLAY2);     
            break;
        case 1:
            afficherNb(nb1, DISPLAY0);
            if(*compteur<20)
                afficherNb(nb2, DISPLAY1);  
            afficherNb(nb3, DISPLAY2); 
            break;
        case 2:
            afficherNb(nb1, DISPLAY0);
            afficherNb(nb2, DISPLAY1); 
            if(*compteur<20)
                afficherNb(nb3, DISPLAY2);
            break;
        case 3:
            afficherNb(nb1, DISPLAY0);
            afficherNb(nb2, DISPLAY1);  
            afficherNb(nb3, DISPLAY2); 
            break;
    }
    if(*compteur>50)
        *compteur = 0;
    else
        *compteur+=1;
}

//Appeler la fonction pour vérifier si le jeu peut commencer
//
int startGame()
{
    if(digitalRead(PLAY) == HIGH)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

void distribution(byte nombreJoueur, int nombreCarte) //Fonction pour donner au nombre x de joueurs un nombre de carte y à chacun
{
  for (byte i = 0; i < nombreCarte*6; i++)
  {
    while(!(Serial1.available() > 0))//Suivre la ligne tant qu'aucune puce n'est détectée
    {
      suiveurLignes();
    }
    MOTOR_SetSpeed(LEFT, 0); //Arrêt des moteurs
    MOTOR_SetSpeed(RIGHT, 0);
    AX_BuzzerON(); //Témoin sonore lors de la détection de la puce
    char * test = readRFID(); //Lecture de la puce
    String str(test); //Formattage en char
    for (byte i2 = 0; i2 < 6; i2++) //Identifier le joueur et prise de décision sur la distribution des cartes
    {
      if(playerID[i2] == str) //Vérification des joueurs valides à la table
      {
        for (byte i3 = 0; i3 < 6; i3++)
        {
          if(joueursTotals[nombreJoueur - 1][i3] == (i2 + 1))
          {
            Serial.print("Joueur "); Serial.println(i2 + 1);
            Serial.println("Donner une carte");
          }
        }
        
      }
    }
    delay(50);
    AX_BuzzerOFF();
    delay(50);
  }
  while(digitalRead(lineS) == 0) //Arrêt du robot au point de départ à la fin de la distribution
  {
    suiveurLignes();
  }
  MOTOR_SetSpeed(LEFT, 0); //Arrêt des moteurs
  MOTOR_SetSpeed(RIGHT, 0);
  Serial.println("Tour terminé");
}

void donnerCarte(byte noJoueur, int nbCartes) //Fonction pour donner un nombre de carte x au joueur y
{
  bool taskComplete = false;
  while(taskComplete == false)
  {
    while (!(Serial1.available() > 0))
    {
      suiveurLignes();
    }
    AX_BuzzerON();
    char * code = readRFID(); //Lecture de la puce
    String str(code); //Formattage en string
    delay(50);
    AX_BuzzerOFF();
    for (byte i = 0; i < 6; i++)
    {
      if(playerID[i] == str && noJoueur == (i + 1))
      {
        for (byte i2 = 0; i2 < nbCartes; i2++)
        {
          MOTOR_SetSpeed(LEFT, 0);
          MOTOR_SetSpeed(RIGHT, 0);
          Serial.println("Donner une carte");
          delay(20);
        }
        taskComplete = true;
      }
    }
  }
  while(digitalRead(lineS) == 0) //Arrêt du robot au point de départ à la fin de la distribution
  {
    suiveurLignes();
  }
  MOTOR_SetSpeed(LEFT, 0); //Arrêt des moteurs
  MOTOR_SetSpeed(RIGHT, 0);
  Serial.println("Tour terminé");
}

char * readRFID() //Fonction pour récupérer le code de la pastille du joueur
{
  char receivedChars[15]; //Table servant à prendre les données reçus 
  char * chipCode; //Pointeur de la table du code du joueur
  chipCode = new char[13]; //Instantiation de la table
  byte i = 0;
  while(Serial1.available() > 0) //Copie/conversion des données dans la table
  {
    char rc = Serial1.read();
    receivedChars[i] = rc;
    delay(10);
    i++;
  }
  receivedChars[14] = '\0';
  for (byte i = 0; i < 12; i++) //Transfert de la table et élimination des caractères non désirables
  {
    chipCode[i] = receivedChars[i + 1];
  }
  //Serial.print("receivedChar "); Serial.println(receivedChars);
  //Serial.print("chipCode "); Serial.println(chipCode);
  chipCode[12] = '\0'; //Ajout du caractères pour la fin de la table
  return chipCode; //Retour du pointeur de la table
}

void suiveurLignes()
{
  int trame = 0;
  trame = digitalRead(ls);
  trame = (trame << 1) + digitalRead(cs);
  trame = (trame << 1) + digitalRead(rs);
  float VLeft = 0;
  float Vright = 0;
  //Serial.println(trame);
  switch (trame)
  {
  case 0: //Aucune ligne captée
    VLeft += INC;
    Vright += INC;
    break;
  case 1: //Ligne sur capteur de droit
    VLeft += INC;
    Vright -= INC;
    break;
  case 2: //Ligne sur capteur central
    VLeft += INC;
    Vright += INC;
  break;
  case 3: //Ligne sur capteur central et droit
    VLeft += INC;
    Vright += INC;
    break;
  case 4: //Ligne sur capteur de gauche
    VLeft -= INC;
    Vright += INC;
    break;
  case 5: //Ligne sur capteur de gauche et droite
    VLeft += INC;
    Vright += INC;
  case 6: //Ligne sur capteur central et gauche
    VLeft += INC;
    Vright += INC;
    break;
  }
    if(VLeft > Vmax){
      VLeft = Vmax;
    }
    if(Vright > Vmax){
      Vright = Vmax;
    }
    if(VLeft < -Vmax){
      VLeft = -Vmax;
    }
    if(Vright < -Vmax){
      Vright = -Vmax;
    }
    MOTOR_SetSpeed(RIGHT, Vright);
    MOTOR_SetSpeed(LEFT, VLeft);
}