#include "CytronMotorDriver.h"
#include <Servo.h>
#include "VL53L0X_RangeSensor.h"
#include <Pixy2.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// OLED
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET    -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// Capteurs IR
int irPins[5] = {A0, A1, A2, A3, A4};
int seuil = 150;
int seuil1 = seuil;

// Moteurs
CytronMD MG(PWM_PWM, 5, 6);       
CytronMD MD(PWM_PWM, 10, 11);    

// Capteur distance
VL53L0X_RangeSensor rangeSensor;

// Autres composants
Pixy2 pixy;
Servo servoMotor1, servoMotor2, servoMotor3;

// États
char val;
bool suiviLigneActif = false;

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

void arreterRobot() {
  MG.setSpeed(0);
  MD.setSpeed(0);
  afficherOLED("ARRET", "");
  Serial.println("Robot arrêté");
}

void suiviLigne() {
  int L2 = analogRead(irPins[0]);
  int L1 = analogRead(irPins[1]);
  int C  = analogRead(irPins[2]);
  int R1 = analogRead(irPins[3]);
  int R2 = analogRead(irPins[4]);

  delay(10); // Réduit les perturbations I2C
  int distance = rangeSensor.getDistance();

  if (distance != -1 && distance < 100) {
    arreterRobot();
    afficherOLED("Obstacle", String(distance) + "mm");
    suiviLigneActif = false;
    return;
  }

  if ((L1 > seuil)&&(L2 > seuil1) && (R1 > seuil)&&(R2 > seuil1)) {
    MG.setSpeed(80);
    MD.setSpeed(94);
    Serial.println("Avancer");
  }
  else if (R1 > seuil) {
    MG.setSpeed(0);
    MD.setSpeed(94);
    Serial.println("Tourner gauche");
  }
  else if ((R2 > seuil1) && (C < seuil1)) {
    MG.setSpeed(0);
    MD.setSpeed(154);
    Serial.println("Tourner gauche");
  }
  else if (L1 > seuil) {
    MG.setSpeed(80);
    MD.setSpeed(0);
    Serial.println("Tourner droite");
  }
  else if ((L2 > seuil1) && (C < seuil1)) {
    MG.setSpeed(140);
    MD.setSpeed(0);
    Serial.println("Tourner droite");
  }
  else {
    MG.setSpeed(80);
    MD.setSpeed(94);
    Serial.println("Correction");
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
    Serial.println(F("Erreur écran OLED"));
    while (true);
  }
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.println("OLED OK");
  display.display();

  // Capteur distance
  if (!rangeSensor.begin()) {
    afficherOLED("Erreur capteur", "VL53L0X");
    while (true);
  }
  afficherOLED("VL53L0X prêt");

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
    Serial.print("Reçu : ");
    Serial.println(val);

    switch (val) {
      case '0':
        suiviLigneActif = false;
        arreterRobot();
        break;

      case '1':
        MG.setSpeed(80);
        MD.setSpeed(97);
        afficherOLED("Commande", "Avancer");
        break;

      case '2':
        MG.setSpeed(115);
        MD.setSpeed(0);
        afficherOLED("Commande", "Droite");
        break;

      case '3':
        MG.setSpeed(0);
        MD.setSpeed(120);
        afficherOLED("Commande", "Gauche");
        break;

      case '4':
        MG.setSpeed(-80);
        MD.setSpeed(-97);
        afficherOLED("Commande", "Retour");
        break;

      case '5':
        servoMotor1.write(90);
        afficherOLED("Servo", "Ouvrir");
        break;

      case '6':
        servoMotor1.write(35);
        afficherOLED("Servo", "Fermer");
        break;

      case '7':
        servoMotor2.write(180);
        servoMotor3.write(0);
        afficherOLED("Bras", "Monter");
        break;

      case '8':
        servoMotor2.write(10);
        servoMotor3.write(170);
        afficherOLED("Bras", "Descendre");
        break;

      case '9':
        pixy.ccc.getBlocks();
        if (pixy.ccc.numBlocks) {
          int signature = pixy.ccc.blocks[0].m_signature;
          afficherOLED("Pixy2", "Sig: " + String(signature));
        } else {
          afficherOLED("Pixy2", "Aucun objet");
        }
        break;

      case 'B':
        suiviLigneActif = true;
        afficherOLED("Mode", "Suivi ligne");
        break;

      default:
        afficherOLED("Commande", "Inconnue");
        break;
    }
  }

  if (suiviLigneActif) {
    suiviLigne();
  }
}
