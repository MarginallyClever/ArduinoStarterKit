// seven segment test
// dan@marginallyclever.com 2016-03-17
// wire the twelve pins clockwise starting bottom right to arduino pins 2-13, inclusive.
// light one pin at a time.
// watch the serial output and the lights to find which pins are anodes and which are cathodes.

#define NUM_PINS 12

int pins[NUM_PINS] = {2,3,4,5,6,7,8,9,10,11,12,13};


void setup() {
  Serial.begin(57600);
  
  int i;
  for(i=0;i<NUM_PINS;++i) {
    pinMode(pins[i],OUTPUT);
    pinOff(i);
  }
}


void pinOn(int i) {
  digitalWrite(pins[i],HIGH);
}


void pinOff(int i) {
  digitalWrite(pins[i],LOW);
}


void loop() {
  int i;
  
  for(i=0;i<NUM_PINS;++i) {
    Serial.print("pin ");
    Serial.println(pins[i]);
    
    pinOn(i);
    delay(2000);
    pinOff(i);
  }
}


