#include "VL53L0X_RangeSensor.h"

VL53L0X_RangeSensor::VL53L0X_RangeSensor() : lox(Adafruit_VL53L0X()) {}

bool VL53L0X_RangeSensor::begin() {
    return lox.begin();
}

int VL53L0X_RangeSensor::getDistance() {
    VL53L0X_RangingMeasurementData_t measure;
    
    for (int i = 0; i < 3; i++) {
        lox.rangingTest(&measure, false);
        if (measure.RangeStatus != 4) {
            return measure.RangeMilliMeter;
        }
        delay(10); // petit délai entre les tentatives
    }

    return -1; // mesure invalide
}
