/* This is a kind of comment.  Comments are ignored by the computer when it
 * 'compiles' (translates) your code into pure machine language and copies
 * it into the Arduino chip.
 */
// This is another kind of comment.  It's only one line long.
// To wire up this circuit, connect the following:
// Attach joystick SW to arduino digital pin 2
// Attach joystick SVx to arduino analog pin 0
// Attach joystick SVy to arduino analog pin 1
// Attach joystick 5v to arduino 5v
// Attach joystick GND to arduino GND

void setup() {
  // receiving data on digital pin 2
  pinMode(2,INPUT);
  // turn on internal resistor
  digitalWrite(2,HIGH);
  // get ready to talk to human  
  Serial.begin(9600);
  Serial.println("Ready");  // println(x) will output "x\n".  \n means 'new line'
}


void loop() {
  // print the x axis.  ~512 is center
  Serial.print((int)analogRead(0),DEC);  Serial.print("\t");  // \t means tab
  // print the y axis.  ~512 is center
  Serial.print((int)analogRead(1),DEC);  Serial.print("\t");
  // print the button state.  Notice this is NOT analogRead, because it is attached to a digital pin.
  Serial.print((int)digitalRead(2),DEC);  Serial.print("\n");  // \n is the same as return.
}

/* Now change the wiring and code so that an LED in pins 13 
 * and GND will turn on when you click the joystick button.
 */
