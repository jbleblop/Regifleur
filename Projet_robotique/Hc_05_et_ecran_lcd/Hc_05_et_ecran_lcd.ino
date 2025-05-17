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
#define OLED_RESET    -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// === Matériel ===
CytronMD MG(PWM_PWM, 5, 6);       
CytronMD MD(PWM_PWM, 10, 11);    

int irPins[5] = {A0, A1, A2, A3, A4};

int seuil = 150;
int seuil1= seuil;

const int photoRes1 = A15;
const int photoRes2 = A14;
const int nbMesures = 10;
int mesures1[nbMesures];
int mesures2[nbMesures];
int i = 0;

Pixy2 pixy;
Servo servoMotor1;
Servo servoMotor2;
Servo servoMotor3;

VL53L0X_RangeSensor rangeSensor;

char val;
int signature = 0;

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

void setup() {
  Serial.begin(9600);
  Serial1.begin(9600);

  for (int i = 0; i < 4; i++) {
    pinMode(irPins[i], INPUT);
  }

  // Initialisation OLED
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("Erreur : écran OLED non détecté"));
    while (true);
  }

  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.println("OLED OK");
  display.display();
  delay(1000);

  // Initialisation capteur distance
  if (!rangeSensor.begin()) {
    afficherOLED("Erreur capteur", "VL53L0X");
    while (true);
  }

  afficherOLED("Capteur VL53L0X", "initialisé");

  servoMotor1.attach(8);
  servoMotor2.attach(13);
  servoMotor3.attach(2);

  pixy.init();
}



void loop() {
  if (Serial1.available()) {
    val = Serial1.read();
    Serial.print("Reçu Bluetooth : ");
    Serial.println((int)val);

    switch (val) {
      case '0':
        MG.setSpeed(0);
        MD.setSpeed(0);
        afficherOLED("Commande", "Stop");
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
        servoMotor1.write(35);
        afficherOLED("Servo", "Fermer");
        Serial1.println("fermer");
        break;

      case '7':
        servoMotor2.write(180);
        servoMotor3.write(0);
        afficherOLED("Bras", "Monter");
        Serial1.println("monter");
        break;

      case '8':
        servoMotor2.write(10);
        servoMotor3.write(170);
        afficherOLED("Bras", "Descendre");
        Serial1.println("descendre");
        break;

      case '9':
        pixy.ccc.getBlocks();
        if (pixy.ccc.numBlocks) {
          signature = pixy.ccc.blocks[0].m_signature;
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
      case 'B': {
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
    MG.setSpeed(80);
    MD.setSpeed(94);
    Serial.println("Avancer");
  // Ligne à gauche
  }
  else if (R1 > seuil) {
    MG.setSpeed(0);
    MD.setSpeed(94);
    Serial.println("Tourner gauche");
  }
  else if ((R2> seuil1) && (C< seuil1)) {
    MG.setSpeed(0);
    MD.setSpeed(154);
    Serial.println("Tourner gauche");
  }
  // Ligne à droite
  else if (L1 > seuil) {
    MG.setSpeed(80);
    MD.setSpeed(0);
    Serial.println("Tourner droite");
  }
  else if ((L2> seuil1)&& (C<seuil1)) {
    MG.setSpeed(140);
    MD.setSpeed(0);
    Serial.println("Tourner droite");
  }
  else{
    MG.setSpeed(80);
    MD.setSpeed(94);
  }
      }

      default:
        afficherOLED("Commande", "Inconnue");
        Serial1.println("Commande inconnue");
        break;
    }
  }
}
