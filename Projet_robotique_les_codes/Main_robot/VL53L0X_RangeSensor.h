#ifndef VL53L0X_RangeSensor_H
#define VL53L0X_RangeSensor_H

#include <Adafruit_VL53L0X.h>

class VL53L0X_RangeSensor {
public:
    VL53L0X_RangeSensor();
    bool begin(); // Initialisation du capteur
    int getDistance(); // Mesure de distance en millimètres

private:
    Adafruit_VL53L0X lox;
};

#endif
