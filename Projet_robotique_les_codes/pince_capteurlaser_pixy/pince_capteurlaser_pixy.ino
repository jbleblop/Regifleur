#include "VL53L0X_RangeSensor.h"
#include "Servo.h"
#include <Pixy2.h>

// Initialisation des objets
Pixy2 pixy;
Servo servoMotor1;  // Servo de la pince
VL53L0X_RangeSensor rangeSensor;  // Capteur laser

// Variables de timer
unsigned long startTime = 0;  
bool timerActive = false;    

void setup() {
  Serial.begin(9600);

  while (!Serial) {
    delay(1);
  }

  Serial.println("Initialisation...");

  if (!rangeSensor.begin()) {
    Serial.println(F("Erreur: VL53L0X non détecté"));
    while (1);
  }

  Serial.println(F("VL53L0X initialisé"));

  servoMotor1.attach(11);  // Broche du servo
  servoMotor1.write(45);  // Ouvrir la pince

  pixy.init();
}

void loop() {
  // Vérifier si le timer est actif et si 5 sec sont écoulées
  if (timerActive && millis() - startTime >= 5000) {
    servoMotor1.write(45);  // Ouvrir la pince après 5 secondes
    timerActive = false;  // Réinitialiser le timer pour la prochaine détection
  }

  // Récupération des blocs détectés par Pixy
  pixy.ccc.getBlocks();

  if (pixy.ccc.numBlocks) {  // Si Pixy détecte des objets
    Serial.print("Objets détectés : ");
    Serial.println(pixy.ccc.numBlocks);
    
    for (int i = 0; i < pixy.ccc.numBlocks; i++) {
      pixy.ccc.blocks[i].print();
      // Vérifier si c'est l'objet recherché (bleu, signature 3)
      if (pixy.ccc.blocks[i].m_signature == 1) {
        
        int distance = rangeSensor.getDistance();
        Serial.print("Distance : ");
        Serial.println(distance);

        // Vérifier si la pince n'est PAS déjà fermée et que l'objet est à bonne distance
        if (!timerActive && distance >= 10 && distance <= 30) {
          servoMotor1.write(90);  // Fermer la pince
          startTime = millis();  // Démarrer le timer
          timerActive = true;    // Empêcher toute autre détection jusqu'à l'ouverture
        }

        break;  // Sortir de la boucle après avoir traité un objet valide
      }
    }
  }
}
