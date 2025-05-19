# 🤖 Robot et Contrôle Android (HC-05 & Arduino + App Kotlin)

Ce projet combine un **robot contrôlé par Arduino** via **Bluetooth (HC-05)** avec une **application Android Kotlin** permettant une interface en grille simple, multilingue (français/anglais), et un retour visuel des commandes.

---

## 📆 Contenu du projet

- Code Arduino (Robot)
- Application Android (Bluetooth)
- README explicatif (ce document)

---

## 🔧 1. Code Arduino du robot

### 🔌 Matériel utilisé

- Arduino Mega
- Module Bluetooth HC-05
- Capteurs IR (suivi de ligne)
- Servos (bras & pince)
- Capteurs LDR (luminosité)
- Capteur LASER (VL53L0X)
- Caméra Pixy2
- Écran OLED (SSD1306)
- Maker Drvie

### 📱 Communication Bluetooth

- L'application Android envoie une **lettre unique (char)** via Bluetooth.
- Le robot lit ces caractères et exécute les commandes associées.

### 📋 Commandes disponibles

| Lettre envoyée | Action robot            | Retour                 |
| -------------- | ----------------------- | ---------------------- |
| `0`            | Arrêt                   | `stop`                 |
| `1`            | Avancer                 | `avancer`              |
| `2`            | Tourner droite          | `droite`               |
| `3`            | Tourner gauche          | `gauche`               |
| `4`            | Reculer                 | `retour`               |
| `5`            | Ouvrir pince            | `ouvrir`               |
| `6`            | Fermer pince            | `fermer`               |
| `7`            | Monter bras             | `monter`               |
| `8`            | Descendre bras          | `descendre`            |
| `9`            | Lire couleur via Pixy2  | `couleur`              |
| `A`            | Lire luminosité (LDR)   | `luminosité lue`       |
| `B`            | Activer suivi ligne     | `suivi de ligne`       |
| `C`            | Mémoriser couleur + LDR | `valeurs enregistrées` |
| `D`            | Modifier seuil distance | `seuil mis à jour`     |
| `E`            | Comparer 3 couleurs     | `min/moy/max`          |

---

## 📲 2. Application Android Kotlin

### 💻 Fonctionnalités principales

- Connexion Bluetooth au module HC-05
- Interface en **grille** avec **15 boutons de commande**
- Affichage des réponses Bluetooth (feedback)
- Bouton pour changer la **langue** (Fr / En)
- Popup pour entrer une **valeur de seuil personnalisée**

### ⚖️ Structure des boutons (layout)

```
Ligne 1 : Stop | Avancer | Suivi
Ligne 2 : Gauche | Reculer | Droite
Ligne 3 : Ouvrir | Fermer | Luminosité
Ligne 4 : Monter | Descendre | Lire Couleur
Ligne 5 : Comparer | Sequence | Changer distance
```

### 📊 Retour de l'état

L'application affiche toutes les réponses de l'Arduino via Bluetooth dans un encart de feedback en bas de l'écran.

---

## 🚗 3. Installation de l'application sur smartphone

### Option 1 : via Android Studio (USB)

1. Active **Options développeur** sur ton téléphone (tap 7x sur "Version MIUI")
2. Active **Débogage USB**
3. Branche le téléphone via USB
4. Autorise la connexion USB (popup)
5. Dans Android Studio, clique sur **Run** ▶

### Option 2 : via APK

1. Dans Android Studio : `Build > Build APK`
2. Récupère le fichier `.apk` :
   `app/build/outputs/apk/debug/app-debug.apk`
3. Transfère sur le téléphone (via USB ou Google Drive)
4. Ouvre le fichier et autorise l'installation

---

## 🎓 Auteurs et mentions

- NIOCHET Jean-Baptiste et Baptiste TARGOWSKi
- Application Android : générée et adaptée pour correspondre exactement à la logique du robot
- Langue : FR / EN via mécanisme d'internationalisation Kotlin

---

## 📦 À faire / Suggestions futures

- Gestion d'états mémorisés du robot (position, historique)
