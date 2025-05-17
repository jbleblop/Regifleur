#include "VL53L0X_RangeSensor.h"

VL53L0X_RangeSensor::VL53L0X_RangeSensor() : lox(Adafruit_VL53L0X()) {}

bool VL53L0X_RangeSensor::begin() {
    return lox.begin();
}

int VL53L0X_RangeSensor::getDistance() {
    VL53L0X_RangingMeasurementData_t measure;
    lox.rangingTest(&measure, false);

    if (measure.RangeStatus != 4) { // Pas d'erreur de mesure
        return measure.RangeMilliMeter;
    } else {
        return -1; // Valeur pour "hors de portée"
    }
}
