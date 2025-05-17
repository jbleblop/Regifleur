//Ne marche pas mais idée à prendre

#include "CytronMotorDriver.h"
// Configure the motor driver.
CytronMD motor2(PWM_PWM, 5, 6);    // PWM 1A = Pin 3, PWM 1B = Pin 9.
CytronMD motor1(PWM_PWM, 10, 11);  // PWM 2A = Pin 10, PWM 2B = Pin 11.



void setup() {
  Serial.begin(9600);
}

void robotStop() {
  motor1.setSpeed(0);  // Motor 1 stops.
  motor2.setSpeed(0);  // Motor 2 stops.

}

void robotForward() {
  motor1.setSpeed(60);  // Motor 1 runs forward.
  motor2.setSpeed(60);  // Motor 2 runs forward.
}

void robotReverse() {
  motor1.setSpeed(-150);  // Motor 1 runs backward.
  motor2.setSpeed(-150);  // Motor 2 runs backward.
}

void robotTurnLeft() {
  motor1.setSpeed(150);   // Motor 1 runs forward.
  motor2.setSpeed(-150);  // Motor 2 runs backward.
}

void robotTurnRight() {
  motor1.setSpeed(-150);  // Motor 1 runs backward.
  motor2.setSpeed(150);   // Motor 2 runs forkward.
}
void loop() {

  robotForward();
  delay(1000);}
 

