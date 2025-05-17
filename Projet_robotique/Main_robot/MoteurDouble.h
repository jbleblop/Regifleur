#ifndef MoteurDouble_H
#define MoteurDouble_H

#include <Arduino.h>
#include "CytronMotorDriver.h"

class MoteurDouble {
private:
    // Motor A and B driver configuration
    CytronMD motorA;
    CytronMD motorB;

public:
    // Constructor to initialize motor driver pins
    MoteurDouble(int pwma, int dira, int pwmb, int dirb);

    // Initializes motor drivers (if needed)
    void begin();

    // Move a specific motor at a given speed (positive for forward, negative for reverse)
    void move(int motor, int speed);

    // Stop both motors
    void stop();

    // Moves the robot forward at the specified speed
    void avancer(int speed);

    // Moves the robot backward at the specified speed
    void reculer(int speed);

    // Turns the robot in the specified direction (0 for left, 1 for right)
    void tourner(int direction, int speed);
};

#endif
