void setup() {
  Serial.begin(9600);     // Communication avec le PC (moniteur série)
  Serial1.begin(9600);    // Communication avec HC-05 (pins 18 TX1, 19 RX1)

  Serial.println("Test Arduino Mega + HC-05");
  Serial1.println("Hello HC-05");  // Envoyé au téléphone
}

void loop() {
  // Si un message arrive du téléphone via Bluetooth
  if (Serial1.available()) {
    char c = Serial1.read();
    Serial.print("Reçu depuis HC-05 : ");
    Serial.println(c);  // Affiche dans le moniteur série

    // Écho : renvoyer ce qui est reçu
    Serial1.print("Echo : ");
    Serial1.println(c);
  }

  // Si un message est tapé depuis le PC (moniteur série)
  if (Serial.available()) {
    char c = Serial.read();
    Serial1.print("PC -> HC-05 : ");
    Serial1.println(c);  // Envoyé au téléphone
  }
}
