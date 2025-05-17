#include "CytronMotorDriver.h"

// Capteurs : L1, R1 seulement
int irL1 = A1; // Ajuste selon ton câblage
int irR1 = A3;

// Moteurs
CytronMD leftMotor(PWM_PWM, 5, 6);
CytronMD rightMotor(PWM_PWM, 10, 11);

void setup() {
  Serial.begin(9600);
  pinMode(irL1, INPUT);
  pinMode(irR1, INPUT);
}

void loop() {
  int L1 = analogRead(irL1);
  int R1 = analogRead(irR1);

  Serial.print("L1: "); Serial.print(L1);
  Serial.print(" | R1: "); Serial.println(R1);

  // Seuil à adapter selon la couleur/fond (500 est un exemple)
  int seuil = 70;


  // Cas de ligne au centre (L1 et R1 détectent)
    leftMotor.setSpeed(80);
    rightMotor.setSpeed(94);
    Serial.println("Avancer");
  // Ligne à gauche
  if ((R1 > seuil)&&(R2> seuil1) {
    leftMotor.setSpeed(0);
    rightMotor.setSpeed(74);
    Serial.println("Tourner gauche");
  }
  // Ligne à droite
  else if ((L1 > seuil)&&(L2> seuil1) {
    leftMotor.setSpeed(60);
    rightMotor.setSpeed(0);
    Serial.println("Tourner droite");
  }
}
