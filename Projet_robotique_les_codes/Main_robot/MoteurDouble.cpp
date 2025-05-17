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

// Turns the robot in the specified direction (0 for left, 1 for right)
void MoteurDouble::tourner(int direction, int speed) {
    if (direction == 0) {
        // Turn left: slow down left motor or reverse it
        motorA.setSpeed(-speed);
        motorB.setSpeed(speed);
    } else if (direction == 1) {
        // Turn right: slow down right motor or reverse it
        motorA.setSpeed(speed);
        motorB.setSpeed(-speed);
    }
}
