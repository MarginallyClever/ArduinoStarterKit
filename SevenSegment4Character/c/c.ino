// seven segment test
// dan@marginallyclever.com 2016-03-17

// we built a map of the pins in part b.
// now use the map to display real numbers.

#define NUM_CHAR_PINS  4
#define NUM_SEG_PINS   8
#define NUM_DIGITS    10


// thousands, hundreds, tens, ones.
int characterPins[NUM_CHAR_PINS] = {8,11,12,2};


// aaa
//f   b
//f   b
// ggg
//e   c
//e   c
// ddd  h
//                                a, b, c, d, e, f, g, h
int segmentPins[NUM_SEG_PINS] = { 9,13, 4, 6, 7,10, 3, 5};
int        zero[NUM_SEG_PINS] = { 1, 1, 1, 1, 1, 1, 0, 0};
int         one[NUM_SEG_PINS] = { 0, 1, 1, 0, 0, 0, 0, 0};
int         two[NUM_SEG_PINS] = { 1, 1, 0, 1, 1, 0, 1, 0};
int       three[NUM_SEG_PINS] = { 1, 1, 1, 1, 0, 0, 1, 0};
int        four[NUM_SEG_PINS] = { 0, 1, 1, 0, 0, 1, 1, 0};
int        five[NUM_SEG_PINS] = { 1, 0, 1, 1, 0, 0, 1, 0};
int         six[NUM_SEG_PINS] = { 1, 0, 1, 1, 1, 1, 1, 0};
int       seven[NUM_SEG_PINS] = { 1, 1, 1, 0, 0, 0, 0, 0};
int       eight[NUM_SEG_PINS] = { 1, 1, 1, 1, 1, 1, 1, 0};
int        nine[NUM_SEG_PINS] = { 1, 1, 1, 1, 0, 1, 1, 0};


int *digits[NUM_DIGITS] = {one,two,three,four,five,six,seven,eight,nine};


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


void segmentPinsAllOff() {
  int i;
  for(i=0;i<NUM_SEG_PINS;++i) {
    segmentPinOff(i);
  }
}

void displayDigit(int i) {
  int *segmentList = digits[i];

  int j;
  for(j=0;j<NUM_SEG_PINS;++j) {
    if(segmentList[j]==1) {
      segmentPinOn(j);
    }
  }
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
    for(i=0;i<NUM_DIGITS;++i) {
      Serial.print("digit ");
      Serial.println(i);
      
      displayDigit(i);
      delay(100);
      segmentPinsAllOff();
    }
    charPinOff(j);
  }
}


