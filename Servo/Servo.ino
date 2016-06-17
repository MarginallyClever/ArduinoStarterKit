// Makes waves with a servo
// Attach the servo signal wire to Arduino pin 8
// Attach the servo 5v wire to Arduino 5V pin
// Attach the servo ground wire to Arduino GND pin

// Someone already wrote the code to do the PWM stuff.
// Just use their library.
#include <Servo.h>

// This servo is global and can be accessed from anywhere in the sketch.
Servo myServo;


void setup() {
  // get ready to talk to the PC, if any.  The baud rate is 9600.
  Serial.begin(9600);
  // tell the Arduino where the servo is connected.
  myServo.attach(8);
}


void loop() {
  int i;

  // wave back and forth, smaller and smaller
  for(i=90;i>0;i-=10) {
    turnToAngle(90+i);
    turnToAngle(90-i);
  }

  // wave back and forth, bigger and bigger
  for(i=0;i<90;i+=10) {
    turnToAngle(90+i);
    turnToAngle(90-i);
  }
}


void turnToAngle(int angle) {
  Serial.print(F("Turning to angle "));
  Serial.println(angle);
  myServo.write(angle);
  // give the servo some time to turn
  delay(60);
}


