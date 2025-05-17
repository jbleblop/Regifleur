#include "IRSensors.h"
#include "MoteurDouble.h"

// PID Constants
float kp = 40;
float ki = 0.5;
float kd = 4;
float previousError = 0;
float integral = 0;
bool tournerProchain = false;

// Capteurs IR et leur configuration
int PINLIST[3] = { A0, A1, A2,A3,A4};
int WEIGHTS[3] = {-3,-1, 0, 1,3};
IRSensors ir1(3, PINLIST, WEIGHTS);

// Configuration des moteurs
int AIN1 = 6;
int AIN2 = 9;
int BIN1 = 10;
int BIN2 = 11;
MoteurDouble motorController(AIN1, AIN2, BIN1, BIN2);

void setup() {
  Serial.begin(115200);
  while (!Serial) {
    delay(1);
  }
}

float PIDCorrector() {
  float error = ir1.getPosition();
  if (error == 0) {
    return 404;  // Aucune ligne détectée
  }
  Serial.print("Position error: ");
  Serial.println(error);

  integral += error;
  float derivee = error - previousError;
  previousError = error;

  return kp * error + ki * integral + kd * derivee;
}

bool detecterLigneTransversale() {
  return ((analogRead(PINLIST[0]) <= 30) || (analogRead(PINLIST[2]) <= 30)||(analogRead(PINLIST[1]) <= 30) || (analogRead(PINLIST[4]) <= 30));
}
bool detecterFinLigne(){
  return (analogRead(PINLIST[2]) >= 30));
}

void loop() {
  int correction = PIDCorrector();

  if (correction == 404) {  // Plus de ligne détectée
    motorController.move(1, 150);  // Ajustement avec le suivi de ligne
    motorController.move(2, -150);
    return;
  }

  // Ajustement de la vitesse avec la correction PID
  int speedA = constrain(150 + correction, 0, 150);
  int speedB = constrain(150 - correction, 0, 150);
  motorController.move(1, speedA);
  motorController.move(2, speedB);

  // Détection des lignes transversales
  if (detecterLigneTransversale()) {
    Serial.println("Ligne transversale détectée, enregistrement de la position et luminosité");
    // Enregistrer position et luminosité ici
    motorController.move(1, speedA);
    motorController.move(2, speedB);
    if(detecterFinLigne()){ //dans le coin
    // tourner à droite ou à gauche
      if(tournerProchain()){ //tourner à droite ou à gauche

      }
      else{
        //tourner dans l'autre sens 
      }
      motorController.move(1, speedA);
      motorController.move(2, speedB);

    }
  }
}
