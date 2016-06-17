/*
  Stepper Motor Control - one revolution

  This program drives a unipolar or bipolar stepper motor.
  The motor is attached to digital pins 8 - 11 of the Arduino.

  The motor should revolve one revolution in one direction, then
  one revolution in the other direction.

  2007-03-11 Created by Tom Igoe
  2009-11-30 Modified by Tom Igoe
  2016-06-15 Modified Dan Royer for Marginally Clever tutorial.
*/
#include <Stepper.h>

const int stepsPerTurn = 64;
const int gearReduction = 32;
const int stepsPerRevolution = stepsPerTurn*gearReduction;


// initialize the stepper library on pins 8 through 11:
Stepper myStepper(stepsPerRevolution, 8, 10, 9, 11);


void setup() {
  // initialize the serial port:
  Serial.begin(9600);
  Serial.print(stepsPerRevolution);
  myStepper.setSpeed(8);
}

void loop() {
  // one full turn
  Serial.println("clockwise");
  myStepper.step(stepsPerRevolution);
  // half second delay
  delay(500);

  // one full turn the other way
  Serial.println("counterclockwise");
  myStepper.step(-stepsPerRevolution);
  // half second delay
  delay(500);
}

