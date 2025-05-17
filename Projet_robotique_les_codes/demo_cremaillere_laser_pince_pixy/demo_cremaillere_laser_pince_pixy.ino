#include "VL53L0X_RangeSensor.h"
#include "Servo.h"
#include <Pixy2.h>

// Initialisation des objets
Pixy2 pixy;
Servo servoMotor1;  // Servo de la pince
Servo servoMotor2;  // Servo cremaillère
Servo servoMotor3;

VL53L0X_RangeSensor rangeSensor;  // Capteur laser

// Variables de timer
unsigned long startTime = 0;  
bool timerActive = false;    

void setup() {
  Serial.begin(115200);

  while (!Serial) {
    delay(1);
  }

  Serial.println("Initialisation...");

  if (!rangeSensor.begin()) {
    Serial.println(F("Erreur: VL53L0X non détecté"));
    while (1);
  }

  Serial.println(F("VL53L0X initialisé"));

  servoMotor1.attach(8);  // Broche du servo
  servoMotor1.write(90);  // Ouvrir la pince
  servoMotor2.attach(13);  // Broche du servo cremaillère
  servoMotor2.write(0);
  servoMotor3.attach(2);  // Broche du servo cremaillère
  servoMotor3.write(180);

  pixy.init();
}

void loop() {
  // Vérifier si le timer est actif et si 5 sec sont écoulées
  if (timerActive && millis() - startTime >= 5000) {
    servoMotor2.write(0); // Envoyez le servo à la position de 0 degré
    servoMotor3.write(180); // Envoyez le servo à la position de 0 degré
    delay(1000);
      Serial.println("Ouverture DE LA PINCE !");
    servoMotor1.write(90);  // Ouvrir la pince après 5 secondes
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
        if (!timerActive && distance >= 10 && distance <= 25) {
            Serial.println("FERMETURE DE LA PINCE !");
          servoMotor1.write(35);  // Fermer la pince
          delay(1000);
          servoMotor2.write(180); 
          /* Circonference=π×1.6≈3.14×1.6=5.024 cm
Une rotation de 180° (demi-tour) fera avancer la crémaillère de :
  5.024/2 =2.512 cm
Donc, avec un pignon de 1.6 cm de diamètre, ta crémaillère montera d'environ 2.51 cm pour un demi-tour (180°). */
          servoMotor3.write(0); 
          startTime = millis();  // Démarrer le timer
          timerActive = true;    // Empêcher toute autre détection jusqu'à l'ouverture
        }

        break;  // Sortir de la boucle après avoir traité un objet valide
      }
    }
  }
}
