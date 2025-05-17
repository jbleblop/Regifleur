#include "CytronMotorDriver.h"

// Capteurs : L2, L1, C, R1, R2
int irPins[5] = {A0, A1, A2, A3, A4};

// Moteurs
CytronMD leftMotor(PWM_PWM, 5, 6);
CytronMD rightMotor(PWM_PWM, 10, 11);

int seuil = 150; // seuil général
unsigned long lastTurnTime = 0;
const unsigned long turnCooldown = 500;

void setup() {
  Serial.begin(9600);
  for (int i = 0; i < 5; i++) {
    pinMode(irPins[i], INPUT);
  }
}

void loop() {
  int L2 = analogRead(irPins[0]);
  int L1 = analogRead(irPins[1]);
  int C  = analogRead(irPins[2]);
  int R1 = analogRead(irPins[3]);
  int R2 = analogRead(irPins[4]);

  Serial.print("L2: "); Serial.print(L2);
  Serial.print(" | L1: "); Serial.print(L1);
  Serial.print(" | C: ");  Serial.print(C);
  Serial.print(" | R1: "); Serial.print(R1);
  Serial.print(" | R2: "); Serial.println(R2);

  unsigned long now = millis();

  // === VIRAGE À 90° GAUCHE ===
  if ((R2 > seuil && R1 < seuil && C < seuil && L1 < seuil && L2 < seuil) && (now - lastTurnTime > turnCooldown)) {
    Serial.println("↪️ Virage 90° gauche");
    lastTurnTime = now;

    leftMotor.setSpeed(100);    // gauche avance
    rightMotor.setSpeed(-100);  // droite recule
    delay(300);

    leftMotor.setSpeed(0);
    rightMotor.setSpeed(0);
    delay(100);
    return;
  }

  // === VIRAGE À 90° DROITE ===
  if ((L2 > seuil && L1 < seuil && C < seuil && R1 < seuil && R2 < seuil) && (now - lastTurnTime > turnCooldown)) {
    Serial.println("↩️ Virage 90° droite");
    lastTurnTime = now;

    leftMotor.setSpeed(-100);   // gauche recule
    rightMotor.setSpeed(100);   // droite avance
    delay(300);

    leftMotor.setSpeed(0);
    rightMotor.setSpeed(0);
    delay(100);
    return;
  }

  // === SUIVI CLASSIQUE ===
  if ((L1 > seuil)&&(L2 > seuil) && (R1 > seuil)&&(R2 > seuil)) {
    leftMotor.setSpeed(80);
    rightMotor.setSpeed(94);
    Serial.println("Avancer");
  }
  else if (R1 > seuil) {
    leftMotor.setSpeed(0);
    rightMotor.setSpeed(94);
    Serial.println("Tourner légèrement à gauche");
  }
  else if ((R2 > seuil) && (C < seuil)) {
    leftMotor.setSpeed(0);
    rightMotor.setSpeed(154);
    Serial.println("Tourner fort à gauche");
  }
  else if (L1 > seuil) {
    leftMotor.setSpeed(80);
    rightMotor.setSpeed(0);
    Serial.println("Tourner légèrement à droite");
  }
  else if ((L2 > seuil) && (C < seuil)) {
    leftMotor.setSpeed(0);
    rightMotor.setSpeed(140);
    Serial.println("Tourner fort à droite");
  }
  else {
    leftMotor.setSpeed(70);
    rightMotor.setSpeed(84);
    Serial.println("Avancer doucement - rien détecté");
  }
}
