#include "IRSensors.h"

IRSensors::IRSensors(int nbPin, int* pins, int* weights) {
  _nbPin = nbPin;
  _pins = new int[nbPin];
  for (int i = 0; i < nbPin; ++i) {
    _pins[i] = pins[i];
  }
  _weights = new int[nbPin];
  for (int i = 0; i < nbPin; ++i) {
    _weights[i] = weights[i];
  }
}

IRSensors::~IRSensors() {
  delete[] _pins;
  delete[] _weights; 
}

void IRSensors::init() {
  for (int i = 0; i < _nbPin; ++i) {
    pinMode(_pins[i], INPUT);
  }
}

int IRSensors::getSensorInfo(int pin) {
  if (analogRead(pin) > 55) {
    return 1;
  } else {
    return 0;
  }
}

int IRSensors::getPosition() {
  int score = 0;
  int captorOn = 0;

  for (int i = 0; i < _nbPin; i++) {
    if (getSensorInfo(_pins[i]) == 1) {  // Utilisation des _pins[i]
      score += _weights[i];
      captorOn++;
    }
  }
    return score;
 
}

int IRSensors::getpin(int i) {
  return _pins[i];  // Renvoie le pin à l'index i
}
