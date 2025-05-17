#include "CytronMotorDriver.h"

// Capteurs : L2, L1, R1, R2
int irPins[5] = {A0, A1, A2, A3, A4}; // adapter selon ton câblage réel

// Moteurs
CytronMD leftMotor(PWM_PWM, 5, 6);
CytronMD rightMotor(PWM_PWM, 10, 11);

int seuil = 150;
int seuil1= seuil;

void setup() {
  Serial.begin(9600);
  for (int i = 0; i < 4; i++) {
    pinMode(irPins[i], INPUT);
  }
}

void loop() {
  int L2 = analogRead(irPins[0]);
  int L1 = analogRead(irPins[1]);
  int C = analogRead(irPins[2]);
  int R1 = analogRead(irPins[3]);
  int R2 = analogRead(irPins[4]);

  Serial.print("L2: "); Serial.print(L2);
  Serial.print(" | L1: "); Serial.print(L1);
  Serial.print(" | C: "); Serial.print(C);
  Serial.print(" | R1: "); Serial.print(R1);
  Serial.print(" | R2: "); Serial.println(R2);

  // Logique de suivi (à adapter selon la couleur de la ligne)
  if ((L1 > seuil)&&(L2> seuil1) && (R1 > seuil)&&(R2> seuil1)) {
    leftMotor.setSpeed(80);
    rightMotor.setSpeed(94);
    Serial.println("Avancer");
  // Ligne à gauche
  }
  else if (R1 > seuil) {
    leftMotor.setSpeed(0);
    rightMotor.setSpeed(94);
    Serial.println("Tourner gauche");
  }
  else if ((R2> seuil1) && (C< seuil1)) {
    leftMotor.setSpeed(0);
    rightMotor.setSpeed(154);
    Serial.println("Tourner gauche");
  }
  // Ligne à droite
  else if (L1 > seuil) {
    leftMotor.setSpeed(80);
    rightMotor.setSpeed(0);
    Serial.println("Tourner droite");
  }
  else if ((L2> seuil1)&& (C<seuil1)) {
    leftMotor.setSpeed(140);
    rightMotor.setSpeed(0);
    Serial.println("Tourner droite");
  }
  else{
    leftMotor.setSpeed(80);
    rightMotor.setSpeed(94);
  }}