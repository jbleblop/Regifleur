#include "Servo.h"

Servo servoMotor2; //  Créez un objet servo
Servo servoMotor3; 
Servo servoMotor1;
void setup() {
  servoMotor2.attach(2); // Indiquez la broche à laquelle le servo est connecté
  servoMotor3.attach(13);
  servoMotor1.attach(8);  // Broche du servo
  servoMotor1.write(90);  // Ouvrir la pince
}

void loop() {
  servoMotor2.write(10); // Envoyez le servo à la position de 0 degré
  servoMotor3.write(170);
  delay(100);
  servoMotor1.write(40);  // Ouvrir la pince
  delay(1000); // Attendez 1 seconde
  servoMotor2.write(170); // Envoyez le servo à la position de 90 degrés
  servoMotor3.write(10);
  delay(100);
  servoMotor1.write(90);  // Ouvrir la pince
  delay(1000); // Attendez 1 seconde
}