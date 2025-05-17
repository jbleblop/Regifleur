#include "IRSensors.h"
#include "MoteurDouble.h"

// Configuration des capteurs IR (3 capteurs)
int PINLIST[5] = { A0, A1, A2, A3, A4 };  // Capteurs : Extrême gauche, Gauche, Centre, Droit, Extrême droit
int WEIGHTS[5] = { -3, -1, 0, 1, 3 };     // Poids plus précis pour calcul de position
IRSensors ir1(5, PINLIST, WEIGHTS);

// Configuration des moteurs
int AIN1 = 10;
int AIN2 = 11;
int BIN1 = 6;
int BIN2 = 5;
MoteurDouble motorController(AIN1, AIN2, BIN1, BIN2);

// Paramètres de suivi de ligne
int baseSpeed = 100;  // Vitesse de base
float Kp = 15.0;      // Coefficient proportionnel pour correction

void setup() {
  Serial.begin(115200);
}

void loop() {
  int position = ir1.getPosition(); // Lire la position de la ligne
  int error = position;          // L'erreur est la position de la ligne (idéalement 0)
  
  int correction = Kp * error;   // Correction proportionnelle

  int leftSpeed = baseSpeed - correction;
  int rightSpeed = baseSpeed + correction;

  // Limiter la vitesse entre 0 et 150
  leftSpeed = constrain(leftSpeed, 0, 255);
  rightSpeed = constrain(rightSpeed, 0, 255);

  // Appliquer la correction aux moteurs
  motorController.move(1, leftSpeed);
  motorController.move(2, rightSpeed);

  // Affichage des valeurs pour debug
  for(int i=0; i<5; i++)
  {Serial.print("Ir "); Serial.print(i); Serial.print(" :"); Serial.println(ir1.getSensorInfo(i));
  }
  delay(1000);
  Serial.println(ir1.getPosition());
  delay(1000);
  Serial.print("Position: "); Serial.print(position);
  Serial.print(" | Correction: "); Serial.print(correction);
  Serial.print(" | Left Speed: "); Serial.print(leftSpeed);
  Serial.print(" | Right Speed: "); Serial.println(rightSpeed);

}
