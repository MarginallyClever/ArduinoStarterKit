// seven segment test
// dan@marginallyclever.com 2016-03-17

// we found which are anodes and which are cathodes in part a.
// I separate the two groups.  
// Anodes select character, cathodes select segement.
// then I watch the serial output and the lights to map which pins go to which segments.

#define NUM_CHAR_PINS 4
#define NUM_SEG_PINS  8

// anodes: thousands, hundreds, tens, ones.
int characterPins[NUM_CHAR_PINS] = {8,11,12,2};

// aaa
//f   b
//f   b
// ggg
//e   c
//e   c
// ddd  h
// cathodes:                      a, b, c, d, e, f, g, h
int segmentPins[NUM_SEG_PINS] = { 9,13, 4, 6, 7,10, 3, 5};


void setup() {
  Serial.begin(57600);
  
  int i;
  for(i=0;i<NUM_SEG_PINS;++i) {
    pinMode(segmentPins[i],OUTPUT);
    segmentPinOff(i);
  }
  
  for(i=0;i<NUM_CHAR_PINS;++i) {
    pinMode(characterPins[i],OUTPUT);
    charPinOff(i);
  }
}


void segmentPinOn(int i) {
  digitalWrite(segmentPins[i],HIGH);
}


void segmentPinOff(int i) {
  digitalWrite(segmentPins[i],LOW);
}


void charPinOn(int i) {
  digitalWrite(characterPins[i],LOW);
}


void charPinOff(int i) {
  digitalWrite(characterPins[i],HIGH);
}


void loop() {
  int i,j;
  for(j=0;j<NUM_CHAR_PINS;++j) {
    charPinOn(j);
    for(i=0;i<NUM_SEG_PINS;++i) {
      Serial.print("seg pin ");
      Serial.println(i);
      
      segmentPinOn(i);
      delay(100);
      segmentPinOff(i);
    }
    charPinOff(j);
  }
}


