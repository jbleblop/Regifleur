#include "CytronMotorDriver.h"
#include <Servo.h>
#include "VL53L0X_RangeSensor.h"
#include <Pixy2.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

unsigned long dernierCheckVL53 = 0;
const unsigned long intervalVL53 = 100; // ms

CytronMD MG(PWM_PWM, 5, 6);
CytronMD MD(PWM_PWM, 10, 11);

int lumBleu = -1;
int lumOrange = -1;
int lumJaune = -1;

int irPins[5] = {A0, A1, A2, A3, A4};
int seuil = 150;
int seuil1 = seuil;

const int photoRes1 = A15;
const int photoRes2 = A14;
const int nbMesures = 10;
int mesures1[nbMesures];
int mesures2[nbMesures];
int i = 0;

VL53L0X_RangeSensor rangeSensor;
int distanceSeuil = 150;

Pixy2 pixy;
Servo servoMotor1;
Servo servoMotor2;
Servo servoMotor3;

char val;
bool modeSuivi = false;

int luminositesObjets[3] = {-1, -1, -1};
int signaturesObjets[3] = {-1, -1, -1};
int indexObjet = 0;

String couleurDepuisSignature(int sig) {
  switch (sig) {
    case 1: return "Bleu";
    case 2: return "Orange";
    case 3: return "Jaune";
    default: return "Inconnue";
  }
}

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

  if (distance == -1) {
    Serial.println("Erreur de mesure VL53L0X !");
    return;
  }

  if (distance < distanceSeuil) {
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

  if ((L1 > seuil) && (L2 > seuil1) && (R1 > seuil) && (R2 > seuil1)) {
    MG.setSpeed(80);
    MD.setSpeed(94);
  } else if (R1 > seuil) {
    MG.setSpeed(0);
    MD.setSpeed(94);
  } else if ((R2 > seuil1) && (C < seuil1)) {
    MG.setSpeed(0);
    MD.setSpeed(154);
  } else if (L1 > seuil) {
    MG.setSpeed(80);
    MD.setSpeed(0);
  } else if ((L2 > seuil1) && (C < seuil1)) {
    MG.setSpeed(140);
    MD.setSpeed(0);
  } else {
    MG.setSpeed(80);
    MD.setSpeed(94);
  }
}

void setup() {
  Serial.begin(9600);
  Serial1.begin(9600); // HC-05 branché sur RX1/TX1

  for (int i = 0; i < 5; i++) pinMode(irPins[i], INPUT);

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

  if (!rangeSensor.begin()) {
    afficherOLED("Erreur capteur", "VL53L0X");
    while (true);
  }
  afficherOLED("VL53L0X", "Initialisé");

  servoMotor1.attach(8);
  servoMotor2.attach(13);
  servoMotor3.attach(2);

  pixy.init();
}

void loop() {
  if (Serial1.available()) {
    val = Serial1.read();
    Serial.print("Commande reçue : ");
    Serial.println(val);

    switch (val) {
      case '0': 
        modeSuivi = false; 
        arreter(); 
        Serial.println("stop"); 
        break;
      case '1': 
        MG.setSpeed(80); 
        MD.setSpeed(97); 
        afficherOLED("Commande", "Avancer"); 
        Serial.println("avancer"); 
        break;
      case '2': 
        MG.setSpeed(115); 
        MD.setSpeed(0); 
        afficherOLED("Commande", "Droite"); 
        Serial.println("droite"); 
        break;
      case '3': 
        MG.setSpeed(0); 
        MD.setSpeed(120); 
        afficherOLED("Commande", "Gauche"); 
        Serial.println("gauche"); 
        break;
      case '4': 
        MG.setSpeed(-80); 
        MD.setSpeed(-97); 
        afficherOLED("Commande", "Retour"); 
        Serial.println("retour"); 
        break;

      case '5': 
        servoMotor1.write(90); 
        afficherOLED("Servo", "Ouvrir"); 
        Serial.println("ouvrir"); 
        break;
      case '6': 
        servoMotor1.write(35); 
        afficherOLED("Servo", "Fermer"); 
        Serial.println("fermer"); 
        break;
      case '7': 
        servoMotor2.write(180); 
        servoMotor3.write(0); 
        afficherOLED("Bras", "Monter"); 
        Serial.println("monter"); 
        break;
      case '8': 
        servoMotor2.write(10); 
        servoMotor3.write(170); 
        afficherOLED("Bras", "Descendre"); 
        Serial.println("descendre"); 
        break;

      case '9': {
        pixy.ccc.getBlocks();
        if (pixy.ccc.numBlocks) {
          int sig = pixy.ccc.blocks[0].m_signature;
          String couleur = (sig == 1) ? "Bleu" : (sig == 2) ? "Orange" : (sig == 3) ? "Jaune" : "Inconnue";
          String msg = "Objet : " + couleur;
          afficherOLED("Pixy2", msg.c_str());
          Serial.println(msg);
        } else {
          afficherOLED("Pixy2", "Aucun objet");
        }
        Serial.println("couleur");
        break;
      }

      case 'A': {
        mesures1[i] = analogRead(photoRes1);
        mesures2[i] = analogRead(photoRes2);
        i = (i + 1) % nbMesures;
        int moy1 = calculerMoyenne(mesures1, nbMesures);
        int moy2 = calculerMoyenne(mesures2, nbMesures);
        afficherOLED("Luminosité", "LDR1: " + String(moy1) + "  LDR2: " + String(moy2));
        Serial.println("luminosité lue");
        break;
      }

      case 'B': 
        modeSuivi = true; 
        afficherOLED("Mode", "Suivi de ligne"); 
        Serial.println("suivi de ligne"); 
        break;

      case 'C': {
        servoMotor1.write(90);  // Ouvrir la pince
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
      MD.setSpeed(135);
      delay(1250);
      MG.setSpeed(80);
      MD.setSpeed(95);
      delay(1000);
      arreter();
    } else if (signature == 2) {
      // Déplacement à droite
      MG.setSpeed(120);
      MD.setSpeed(0);
      delay(1250);
      MG.setSpeed(80);
      MD.setSpeed(95);
      delay(1000);
      arreter();
    } else if (signature == 3) {
      // Avancer
      MG.setSpeed(80);
      MD.setSpeed(95);
      delay(1250);
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

     delay(500);
        int lum1 = analogRead(photoRes1);
        int lum2 = analogRead(photoRes2);
        int moyenne = (lum1 + lum2) / 2;
        pixy.ccc.getBlocks();

        if (pixy.ccc.numBlocks) {
          int sig = pixy.ccc.blocks[0].m_signature;
          String couleur = (sig == 1) ? "Bleu" : (sig == 2) ? "Orange" : (sig == 3) ? "Jaune" : "Inconnue";

          if (sig == 1) lumBleu = moyenne;
          else if (sig == 2) lumOrange = moyenne;
          else if (sig == 3) lumJaune = moyenne;

          String msg = couleur + " : " + String(moyenne);
          afficherOLED("Luminosité", msg.c_str());
          Serial.println(msg);
        } else {
          afficherOLED("Erreur", "Pas d'objet");
          Serial.println("Erreur : aucun objet détecté");
        }

    // Étape 6 : Monter la pince
    servoMotor2.write(100);
    servoMotor3.write(80);
    delay(1000);

    // Revenir à la position initiale
    if (signature == 1) {
      // Retour depuis gauche
      MG.setSpeed(-80);
      MD.setSpeed(-95);
      delay(1000);
      MG.setSpeed(0);
      MD.setSpeed(-135);
      delay(1250);
      
    } else if (signature == 2) {
      // Retour depuis droite
      MG.setSpeed(-80);
      MD.setSpeed(-95);
      delay(1000);
      MG.setSpeed(-120);
      MD.setSpeed(0);
      delay(1250);
      
    } else if (signature == 3) {
      // Retour en arrière
      MG.setSpeed(-80);
      MD.setSpeed(-95);
      delay(1250);
    }

    arreter();
    afficherOLED("Séquence", "Terminée");
    Serial1.println("sequence terminee");

  } else {
    afficherOLED("Pixy2", "Aucun objet");
    Serial1.println("aucun objet détecté");
  }
  break;}


      case 'D': {
        while (!Serial1.available());
        int d1 = Serial1.parseInt();
        if (d1 > 10 && d1 < 1000) {
          distanceSeuil = d1;
          afficherOLED("Nouveau seuil", "Seuil: " + String(d1) + "U");
        }
        break;
      }

      case 'E': {
        struct Donnee {
          const char* couleur;
          int lum;
        } donnees[3] = {
          {"Bleu", lumBleu},
          {"Orange", lumOrange},
          {"Jaune", lumJaune}
        };

        bool donneesValides = true;
        String msgErreur = "";

        for (int j = 0; j < 3; j++) {
          if (donnees[j].lum == -1) {
            msgErreur += String(donnees[j].couleur) + " : pas de donnée\n";
            donneesValides = false;
          }
        }

        if (!donneesValides) {
          afficherOLED("Erreur", msgErreur);
          Serial.println("Erreur données manquantes");
          Serial.println(msgErreur);
          break;
        }

        // Tri par luminosité croissante
        for (int j = 0; j < 2; j++) {
          for (int k = j + 1; k < 3; k++) {
            if (donnees[j].lum > donnees[k].lum) {
              Donnee tmp = donnees[j];
              donnees[j] = donnees[k];
              donnees[k] = tmp;
            }
          }
        }

        String msg = "Min: " + String(donnees[0].couleur) + "\n" +
                     "Moy: " + String(donnees[1].couleur) + "\n" +
                     "Max: " + String(donnees[2].couleur);

        afficherOLED("Comparaison", msg);
        Serial.println("Résultat luminosité :");
        Serial.println(msg);
        break;
      }

      default:
        afficherOLED("Commande", "Inconnue");
        Serial.println("commande inconnue");
        break;
    }
  }

  if (modeSuivi) {
      suiviLigne();
  }
}
