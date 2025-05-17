#include "CytronMotorDriver.h"
#include <Servo.h>
#include "VL53L0X_RangeSensor.h"
#include <Pixy2.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// === OLED ===
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// === Moteurs ===
CytronMD MG(PWM_PWM, 5, 6);        // Moteur gauche
CytronMD MD(PWM_PWM, 10, 11);      // Moteur droit

// === Capteurs de suivi de ligne ===
int irPins[5] = {A0, A1, A2, A3, A4}; // L2, L1, C, R1, R2
int seuil = 40;
int seuil1 = seuil;

// === Capteurs de luminosité ===
const int photoRes1 = A15;
const int photoRes2 = A14;
const int nbMesures = 10;
int mesures1[nbMesures];
int mesures2[nbMesures];
int i = 0;

// === Capteur de distance ===
VL53L0X_RangeSensor rangeSensor;
int distanceSeuil = 150;

// === Pixy2 & servos ===
Pixy2 pixy;
Servo servoMotor1;
Servo servoMotor2;
Servo servoMotor3;

// === Contrôle Bluetooth ===
char val;
bool modeSuivi = false;

void afficherOLED(const char* message, const String& valeur = "") {
  display.clearDisplay();
  display.setCursor(0, 0);
  display.setTextSize(1);
  display.println(message);
  if (valeur != "") {
    display.setCursor(0, 20);
    display.println(valeur);
  }
  display.display();
}

int calculerMoyenne(int* mesures, int taille) {
  long somme = 0;
  for (int j = 0; j < taille; j++) {
    somme += mesures[j];
  }
  return somme / taille;
}

void arreter() {
  MG.setSpeed(0);
  MD.setSpeed(0);
  afficherOLED("Commande", "Arrêt");
  Serial.println("Robot arrêté");
}

void suiviLigne() {
  int distance = rangeSensor.getDistance();
  if (distance > 0 && distance < distanceSeuil) {
    Serial.print("Obstacle à "); Serial.println(distance);
    arreter();
    modeSuivi = false;
    return;
  }

  int L2 = analogRead(irPins[0]);
  int L1 = analogRead(irPins[1]);
  int C  = analogRead(irPins[2]);
  int R1 = analogRead(irPins[3]);
  int R2 = analogRead(irPins[4]);

  Serial.print("L2: "); Serial.print(L2);
  Serial.print(" L1: "); Serial.print(L1);
  Serial.print(" C: "); Serial.print(C);
  Serial.print(" R1: "); Serial.print(R1);
  Serial.print(" R2: "); Serial.println(R2);

  if ((L1 > seuil) && (L2 > seuil1) && (R1 > seuil) && (R2 > seuil1)) {
    MG.setSpeed(80);
    MD.setSpeed(94);
    Serial.println("Avancer");
  } else if (R1 > seuil) {
    MG.setSpeed(0);
    MD.setSpeed(94);
    Serial.println("Tourner gauche");
  } else if ((R2 > seuil1) && (C < seuil1)) {
    MG.setSpeed(0);
    MD.setSpeed(154);
    Serial.println("Tourner gauche fort");
  } else if (L1 > seuil) {
    MG.setSpeed(80);
    MD.setSpeed(0);
    Serial.println("Tourner droite");
  } else if ((L2 > seuil1) && (C < seuil1)) {
    MG.setSpeed(140);
    MD.setSpeed(0);
    Serial.println("Tourner droite fort");
  } else {
    MG.setSpeed(80);
    MD.setSpeed(94);
  }
}

void setup() {
  Serial.begin(9600);
  Serial1.begin(9600);

  for (int i = 0; i < 5; i++) {
    pinMode(irPins[i], INPUT);
  }

  // OLED
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println("Erreur : OLED non détecté");
    while (true);
  }
  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.println("OLED OK");
  display.display();
  delay(1000);

  // Capteur distance
  if (!rangeSensor.begin()) {
    afficherOLED("Erreur capteur", "VL53L0X");
    while (true);
  }
  afficherOLED("VL53L0X", "Initialisé");

  // Servos
  servoMotor1.attach(8);
  servoMotor2.attach(13);
  servoMotor3.attach(2);

  // Pixy2
  pixy.init();
}

void loop() {
  if (Serial1.available()) {
    val = Serial1.read();
    Serial.print("Commande reçue : "); Serial.println(val);

    switch (val) {
      case '0':
        modeSuivi = false;
        arreter();
        Serial1.println("stop");
        break;

      case '1':
        MG.setSpeed(80);
        MD.setSpeed(97);
        afficherOLED("Commande", "Avancer");
        Serial1.println("avancer");
        break;

      case '2':
        MG.setSpeed(115);
        MD.setSpeed(0);
        afficherOLED("Commande", "Droite");
        Serial1.println("droite");
        break;

      case '3':
        MG.setSpeed(0);
        MD.setSpeed(120);
        afficherOLED("Commande", "Gauche");
        Serial1.println("gauche");
        break;

      case '4':
        MG.setSpeed(-80);
        MD.setSpeed(-97);
        afficherOLED("Commande", "Retour");
        Serial1.println("retour");
        break;

      case '5':
        servoMotor1.write(90);
        afficherOLED("Servo", "Ouvrir");
        Serial1.println("ouvrir");
        break;

      case '6':
        servoMotor1.write(37);
        afficherOLED("Servo", "Fermer");
        Serial1.println("fermer");
        break;

      case '7':
        servoMotor2.write(100);
        servoMotor3.write(80);
        afficherOLED("Bras", "Monter");
        Serial1.println("monter");
        break;

      case '8':
        servoMotor2.write(15);
        servoMotor3.write(165);
        afficherOLED("Bras", "Descendre");
        Serial1.println("descendre");
        break;

      case '9':
        pixy.ccc.getBlocks();
        if (pixy.ccc.numBlocks) {
          int signature = pixy.ccc.blocks[0].m_signature;
          String msg = "Signature: " + String(signature);
          afficherOLED("Pixy2", msg);
          Serial.println(msg);
        } else {
          afficherOLED("Pixy2", "Aucun objet");
        }
        Serial1.println("couleur");
        break;

      case 'A': {
        mesures1[i] = analogRead(photoRes1);
        mesures2[i] = analogRead(photoRes2);
        i = (i + 1) % nbMesures;
        int moy1 = calculerMoyenne(mesures1, nbMesures);
        int moy2 = calculerMoyenne(mesures2, nbMesures);
        String luminosite = "LDR1: " + String(moy1) + "  LDR2: " + String(moy2);
        afficherOLED("Luminosité", luminosite);
        Serial.println(luminosite);
        Serial1.println("luminosité lue");
        break;
      }

      case 'B':
        afficherOLED("Mode", "Suivi de ligne");
        Serial1.println("suivi de ligne");
        modeSuivi = true;
        break;

      case 'C': // Séquence automatisée selon la couleur détectée
      // Étape 1 : Descendre la pince
    servoMotor2.write(15);
    servoMotor3.write(165);
    delay(1000);
  pixy.ccc.getBlocks();
  if (pixy.ccc.numBlocks) {
    int signature = pixy.ccc.blocks[0].m_signature;
    String msg = "Signature: " + String(signature);
    afficherOLED("Pixy2", msg);
    Serial.println(msg);

    

    // Étape 2 : Fermer la pince
    servoMotor1.write(37);
    delay(1000);

    // Étape 3 : Monter la pince
    servoMotor2.write(100);
    servoMotor3.write(80);
    delay(1000);

    if (signature == 1) {
      // Déplacement à gauche
      MG.setSpeed(0);
      MD.setSpeed(137);
      delay(1500);
      MG.setSpeed(80);
      MD.setSpeed(97);
      delay(1000);
      arreter();
    } else if (signature == 2) {
      // Déplacement à droite
      MG.setSpeed(120);
      MD.setSpeed(0);
      delay(1500);
      MG.setSpeed(80);
      MD.setSpeed(97);
      delay(1000);
      arreter();
    } else if (signature == 3) {
      // Avancer
      MG.setSpeed(80);
      MD.setSpeed(97);
      delay(1500);
      arreter();
    } else {
      afficherOLED("Signature", "Non reconnue");
      Serial.println("Signature non reconnue");
      break;
    }

    // Étape 4 : Descendre la pince
    servoMotor2.write(15);
    servoMotor3.write(165);
    delay(1000);

    // Étape 5 : Ouvrir la pince
    servoMotor1.write(90);
    delay(1000);

    // Étape 6 : Monter la pince
    servoMotor2.write(100);
    servoMotor3.write(80);
    delay(1000);

    // Revenir à la position initiale
    if (signature == 1) {
      // Retour depuis gauche
      MG.setSpeed(-80);
      MD.setSpeed(-97);
      delay(1000);
      MG.setSpeed(0);
      MD.setSpeed(-137);
      delay(1500);
      
    } else if (signature == 2) {
      // Retour depuis droite
      MG.setSpeed(-80);
      MD.setSpeed(-97);
      delay(1000);
      MG.setSpeed(-120);
      MD.setSpeed(0);
      delay(1500);
      
    } else if (signature == 3) {
      // Retour en arrière
      MG.setSpeed(-80);
      MD.setSpeed(-97);
      delay(1500);
    }

    arreter();
    afficherOLED("Séquence", "Terminée");
    Serial1.println("sequence terminee");

  } else {
    afficherOLED("Pixy2", "Aucun objet");
    Serial1.println("aucun objet détecté");
  }
  break;



      case 'D': {
        while (!Serial1.available());
        int d1 = Serial1.parseInt();
        if (d1 > 10 && d1 < 1000) {
          distanceSeuil = d1;
          String s = "Seuil: " + String(d1) + "mm";
          afficherOLED("Nouveau seuil", s);
          Serial.println(s);
        }
        break;
      }

      default:
        afficherOLED("Commande", "Inconnue");
        Serial1.println("commande inconnue");
        break;
    }
  }

  if (modeSuivi) {
    suiviLigne();
  }
}
