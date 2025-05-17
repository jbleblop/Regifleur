#include "MoteurDouble.h"

// Constructor to initialize motor driver pins
MoteurDouble::MoteurDouble(int pwma, int dira, int pwmb, int dirb) 
    : motorA(PWM_DIR, pwma, dira), motorB(PWM_DIR, pwmb, dirb) {}

// Initializes motor drivers (if needed)
void MoteurDouble::begin() {
    // Add initialization if necessary
}

// Move a specific motor at a given speed (positive for forward, negative for reverse)
void MoteurDouble::move(int motor, int speed) {
    if (motor == 1) {
        motorA.setSpeed(speed);
    } else if (motor == 2) {
        motorB.setSpeed(speed);
    }
}

// Stop both motors
void MoteurDouble::stop() {
    motorA.setSpeed(0);
    motorB.setSpeed(0);
}

// Moves the robot forward at the specified speed
void MoteurDouble::avancer(int speed) {
    motorA.setSpeed(speed);
    motorB.setSpeed(speed);
}

// Moves the robot backward at the specified speed
void MoteurDouble::reculer(int speed) {
    motorA.setSpeed(-speed);
    motorB.setSpeed(-speed);
}


