#include "IRSensors.h"

IRSensors::IRSensors(int nbPin, int* pins, int* weights){
  _nbPin = nbPin;
  _pins = new int[nbPin];
    for (int i = 0; i < nbPin; ++i) {
        _pins[i] = pins[i];
    }
  _weights = new int[nbPin];
  for (int i = 0; i < nbPin; ++i) {
        _weights[i] = weights[i];
    }
};

IRSensors::~IRSensors() {
    delete[] _pins;
};

void IRSensors::init(){
  for (int i = 0; i < _nbPin; ++i) {
    pinMode(_pins[i], INPUT);
  }
};

int IRSensors::getSensorInfo(int pin){
  if (analogRead(pin)>650){
    return 1;
  }
  else{
    return 0;
  }
};

int IRSensors::getPosition(){
  int score = 0;
  int captorOn = 0;

  for (int i = 0; i < _nbPin; i++) {
    if (getSensorInfo(i) == 1) {
      score += _weights[i];
      captorOn++;
    }
  }
  if (captorOn){
    return score;
  }
  else{
    return 404;
  } 
}

