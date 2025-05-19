const int photoRes1 = A14;  // Photo-résistance 1 sur A0
const int photoRes2 = A15;  // Photo-résistance 2 sur A1
const int nbMesures = 20;  // Nombre de mesures à stocker

int mesures1[nbMesures];  // Tableau pour stocker les mesures de la photo-résistance 1
int mesures2[nbMesures];  // Tableau pour stocker les mesures de la photo-résistance 2

void setup() {
  Serial.begin(9600);
  
}

void loop() {
  for (int i = 0; i < nbMesures; i++) {
    mesures1[i] = analogRead(photoRes1);
    mesures2[i] = analogRead(photoRes2);

    Serial.print("Mesure ");
    Serial.print(i);
    Serial.print(" - PhotoR 1: ");
    Serial.print(mesures1[i]);
    Serial.print(" | PhotoR 2: ");
    Serial.println(mesures2[i]);

    delay(1000); // Attendre 1 seconde entre chaque mesure
  }

  // Exemple d'utilisation des données stockées
  Serial.println("\n--- Fin des mesures ---");
  int somme1 = 0;
  int somme2 = 0;

  for (int i = 0; i < nbMesures; i++) {
    somme1 += mesures1[i];
    somme2 += mesures2[i];
  }

  Serial.print("Moyenne PhotoR 1: ");
  Serial.println(somme1 / nbMesures);
  Serial.print("Moyenne PhotoR 2: ");
  Serial.println(somme2 / nbMesures);

  // Fin de la boucle pour éviter de recommencer
  while (true); // Boucle infinie pour arrêter ici
}
