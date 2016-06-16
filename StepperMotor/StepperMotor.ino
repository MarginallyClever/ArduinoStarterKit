
/*
  Stepper Motor Control - one revolution

  This program drives a unipolar or bipolar stepper motor.
  The motor is attached to digital pins 8 - 11 of the Arduino.

  The motor should revolve one revolution in one direction, then
  one revolution in the other direction.


  Created 11 Mar. 2007
  Modified 30 Nov. 2009
  by Tom Igoe

  Modified 2016-06-15 Dan Royer for Marginally Clever tutorial.
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
  
  // set the speed at 60 rpm:
  myStepper.setSpeed(8);
}

void loop() {
  // step one revolution  in one direction:
  Serial.println("clockwise");
  myStepper.step(stepsPerRevolution);
  delay(500);

  // step one revolution in the other direction:
  Serial.println("counterclockwise");
  myStepper.step(-stepsPerRevolution);
  delay(500);
}

