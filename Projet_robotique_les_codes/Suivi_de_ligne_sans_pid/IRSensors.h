#ifndef IRSensors_H
#define IRSensors_H

#include <Arduino.h>

class IRSensors{
  public : 
  IRSensors(int nbPin, int* pins, int* weights);
  ~IRSensors();
  void init();
  int getSensorInfo(int pin);
  int getPosition();
  int getpin(int i);

  private : 
  int _nbPin;
  int* _pins;
  int* _weights;
};


#endif