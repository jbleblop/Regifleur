#include "IRSensors.h"
#include "MoteurDouble.h"

// Configuration des capteurs IR (5 capteurs)
int PINLIST[5] = { A0, A1, A2, A3, A4 };  // Capteurs : Extrême gauche, Gauche, Centre, Droit, Extrême droit
int WEIGHTS[5] = { -3, -1, 0, 1, 3 };     // Poids plus précis pour calcul de position
IRSensors ir1(5, PINLIST, WEIGHTS);

// Configuration des moteurs
int AIN1 = 11;
int AIN2 = 10;
int BIN1 = 6;
int BIN2 = 5;
MoteurDouble motorController(AIN1, AIN2, BIN1, BIN2);

// Paramètres de suivi de ligne
int baseSpeed = 175;  // Vitesse de base

void setup() {
  Serial.begin(115200);
}

void loop() {
  int position = ir1.getPosition(); // Lire la position de la ligne

  int leftSpeed = baseSpeed;
  int rightSpeed = baseSpeed;

  // Ajustement des vitesses en fonction de la position
  if (position == 0) {
    // La ligne est au centre, avancer tout droit
    leftSpeed = baseSpeed;
    rightSpeed = baseSpeed;
  } else if (position == -1) {
    // La ligne est légèrement à gauche ou fortement à gauche, tourner à gauche
    leftSpeed = baseSpeed / 1.1;
    rightSpeed = baseSpeed*1.1;
  } else if (position == 1) {
    // La ligne est légèrement à droite ou fortement à droite, tourner à droite
    leftSpeed = baseSpeed*1.1;
    rightSpeed = baseSpeed / 1.1;
  }
  else if (position == -3)
  {
    leftSpeed = baseSpeed/1.4;
    rightSpeed = baseSpeed * 1.4; 
  }
  else if (position == 3){
    leftSpeed = baseSpeed*1.4;
    rightSpeed = baseSpeed / 1.4;
  }

  // Appliquer les vitesses aux moteurs
  motorController.move(1, leftSpeed);
  motorController.move(2, rightSpeed);

  // Affichage des valeurs pour debug
  Serial.print("Position: "); Serial.print(position);
  Serial.print(" | Left Speed: "); Serial.print(leftSpeed);
  Serial.print(" | Right Speed: "); Serial.println(rightSpeed);

  delay(10);  // Petit délai pour stabilité
}
