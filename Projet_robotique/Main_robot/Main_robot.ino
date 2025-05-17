#include "IRSensors.h"
#include "VL53L0X_RangeSensor.h"
#include "MoteurDouble.h"
#include "Servo.h"
#include <Pixy2.h>
#include <DIYables_IRcontroller.h>  // DIYables_IRcontroller library


// This is the main Pixy object 
Pixy2 pixy;

float kp = 40; 
float ki = 0.5;  
float kd = 4;  
float previousError = 0;
float integral = 0;

int PINLIST[5] = {IR_1,IR_2,IR_3,IR_4,IR_5};
int WEIGHTS[5] = {-3, -1, 0, 1, 3};

// Configure le suiveur de ligne
int pins[MAX] = { A0, A1, A2, A3, A4 };  // Tableau des entrées analogiques pour le suivi de ligne
IRSensors ir1(5, PINLIST, WEIGHTS);

// Configure les moteurs des roues
int AIN1 = 9;
int AIN2 = 8;
int BIN1 = 11;
int BIN2 = 12;
MoteurDouble motorController(AIN1, AIN2, BIN1, BIN2);

// Configure les moteurs de la pince
Servo servoMotor1;  //  Créez un objet servo : la pince
Servo servoMotor2;  //
Servo servoMotor3;  // les deux crémaillere


VL53L0X_RangeSensor rangeSensor;  // le capteur laser

int lum = 0;
int maxLum = 500;

void setup() {
  Serial.begin(115200);

  while (!Serial) {
    delay(1);
  }

  Serial.println("Adafruit VL53L0X test");

  if (!rangeSensor.begin()) {
    Serial.println(F("Failed to boot VL53L0X"));
    while (1)
      ;  // Bloque si l'initialisation échoue
  }

  Serial.println(F("VL53L0X API Simple Ranging example\n\n"));
//  while (irController.getKey() == Key17::NONE)
  //  ;
  servoMotor1.attach(9);  // Indiquez la broche à laquelle le servo est connecté (ici : broche 9)

   pixy.init();
}


float PIDCorrector(){
  float error = ir1.getPosition();
  if (error==404){
    return 4004;
  }
  Serial.print("position error");
  Serial.println(error);
  integral += error;     
  float derivee = error - previousError;
  previousError = error;

  float correction = kp * error + ki * integral + kd * derivee;

  return correction;
}

void loop() {
  //Je veux que la pince soit tout le temps (fermée = 40 ° ou ouvert = 90 °) quand le robot se déplace
 int correction = PIDCorrector();

  if (correction == 4004){   // si aucune ligne n'est détéctée 
    motorController.tourner(0, 255)
  }

  int speedA = 255 + correction;
  int speedB = 255 - correction;

  speedA = constrain(speedA, 0, 255);   // pour ne pas saturer le moteur
  speedB = constrain(speedB, 0, 255);
 /*
  {
      int in1_val = digitalRead(IN1);
  int in4_val = digitalRead(IN4);


if ((in1_val) && (!in4_val)) //S'il y a du noir à gauche et du blanc à droite, tourner à gauche
    {
      // direction == 0 Turn left: slow down left motor
      Serial.println("Tourner à gauche");
      motorController.tourner(0,"speed");
    }
    else if ((!in1_val) && (in4_val)) //S'il y a du blanc à gauche et du noir à droite, tourner à droite
    {
      // direction == 1 Turn right: slow down right motor
      Serial.println("Tourner à droite");
      motorController.tourner(1,"speed");
    }
    else //Si les conditions plus haut ne s'appliquent pas, continuer tout droit
    {
      Serial.println("Continuer tout droit");
      motorController.avancer();
    }
}*/
  //Repérage des zones lumineuses
 /* Premier tour sans pot : à chaque croisement, enregistrement de la luminosité dans un tableau qui garde la position (quel est le croisement) et la luminosité */
  
  //Mise à jour du tableau des zones lumineuses : la plus limineuse, la moyennne, la moins
 
 


  //Déplacement du pot
 /* suivi de ligne*/

  //Depôt du pot
 
 /* En fonction de la plante, le robot va la déposer à l'endroit le plus, moins ou moyen lumineux.*/
  if (lum == maxLum) {
    servoMotor1.write(40);    // Envoyez le servo à la position de 40 degré (fermé)
    delay(1000);            // Attendez 1 seconde
    servoMotor1.write(90);   // Envoyez le servo à la position de 90 degrés (ouvert)
    delay(1000);            // Attendez 1 seconde
  }

  //Repérage du pot avec pixy

  int i; 
  // grab blocks!
  pixy.ccc.getBlocks();
  
  // If there are detect blocks, print them!
  if (pixy.ccc.numBlocks)
  {
    Serial.print("Detected ");
    Serial.println(pixy.ccc.numBlocks);
    for (i=0; i<pixy.ccc.numBlocks; i++)
    {
      Serial.print("  block ");
      Serial.print(i);
      Serial.print(": ");
      pixy.ccc.blocks[i].print();
    }
  }  
/* Les paramètres des blocks : regarder la taille et la signature du pot
  m_signature; (1 = rose ; 2 = orange ; 3 = bleu)
  m_x;
  m_y;
  m_width; (prendre la plus grande aire = w*h)
  m_height;
  m_angle;
  m_index;
  m_age;
*/

  //Prise du pot

  if ((rangeSensor.getDistance() <= 30) && (rangeSensor.getDistance() >= 10)) {
    servoMotor1.write(40);    // Envoyez le servo à la position de 40 degré (fermé)
    delay(1000);            // Attendez 1 seconde
    servoMotor1.write(90);   // Envoyez le servo à la position de 90 degrés (ouvert)
    delay(1000);            // Attendez 1 seconde           
  }
  Serial.println(rangeSensor.getDistance());
}
