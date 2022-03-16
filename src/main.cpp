/*
  Titre      : Pompe d'evacuation d'eau
  Auteur     : Yvan Tankeu
  Date       : 16/03/2022
  Description: Evacuation d'eau provenant d'une fuite dans un immeuble
  Version    : 0.0.1
*/

#include <Arduino.h>
#include "WIFIConnector_MKR1000.h"
#include "MQTTConnector.h"

#define Relai 2
#define Pompe 1

const int delai = 5000;

bool StatutDetecteur(0);
bool StatutPompe(0);

// fonction qui s'executera quand l'interrupt sera détecter
void etat(){
  
  // le relai ouvre la pompe quand il y'a detection d'eau
  digitalWrite(Relai, HIGH);
  
  StatutPompe = digitalRead(Pompe);
  StatutDetecteur = digitalRead(Relai);
  
  // envoie du statut
  appendPayload("Detecteur", StatutDetecteur);
  appendPayload("Pompe", StatutPompe);
  sendPayload();

  delay(delai);

}

void setup() {
  Serial.begin(9600);
  
  wifiConnect(); //Branchement au réseau WIFI
  MQTTConnect(); //Branchement au broker MQTT

  pinMode(Pompe, INPUT); // Pompe en mode d'entrée
  pinMode(Relai, OUTPUT); // Relai déclencheur en mode sortie
  
  // Inititalisation pour l'utilisation d'interuption
  attachInterrupt(digitalPinToInterrupt(Pompe), etat, HIGH);
}

void loop() {
  // le relai ne laissera pas passer le courant quand il n'y a pas detection d'eau
  digitalWrite(Relai, LOW);
  
  // sauvegarde la valeur de la pompe 
  StatutPompe = digitalRead(Pompe);

  StatutDetecteur = digitalRead(Relai);

  // envoie du statut
  appendPayload("Detecteur", StatutDetecteur);
  appendPayload("Pompe", StatutPompe);

  sendPayload();
  delay(delai);
}