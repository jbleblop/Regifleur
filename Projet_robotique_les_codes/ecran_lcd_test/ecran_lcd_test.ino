#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

// Adresse I2C par défaut pour les SSD1306 : 0x3C
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

void setup() {
  // Initialisation de l'écran OLED
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("Échec de l'initialisation de l'écran OLED"));
    for (;;); // Arrêt en cas d'erreur
  }

  display.clearDisplay();               // Efface l'écran
  display.setTextSize(1);               // Taille du texte (1 à 3)
  display.setTextColor(SSD1306_WHITE);  // Couleur du texte
  display.setCursor(0, 0);              // Position en haut à gauche
  display.println("Bonjour, monde !");
  display.println("OLED I2C :)");
  display.display();                    // Affiche le contenu
}

void loop() {
  // Rien pour l'instant
}
