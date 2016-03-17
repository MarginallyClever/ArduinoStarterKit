// seven segment test - write "cool" instead of digits.
// dan@marginallyclever.com 2016-03-17

// we displayed individual digits in part c.
// now we run multi-digit numbers.
// we are turning lights on and off so fast the human eye can't tell they are off
// it's called "persistence of vision"

#define NUM_CHAR_PINS  4
#define NUM_SEG_PINS   8
#define NUM_DIGITS    3


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
int         one[NUM_SEG_PINS] = { 1, 0, 0, 1, 1, 1, 0, 0};
int         two[NUM_SEG_PINS] = { 0, 0, 0, 1, 1, 1, 0, 0};


int *digits[NUM_DIGITS] = {zero,one,two};


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


void displayNumber(int value) {
  int maxValue = pow(10,NUM_CHAR_PINS);
  value %= maxValue;

  int i;
  for(i=0;i<NUM_CHAR_PINS;++i) {
    //if(i>0 && value==0) break;  // uncomment to hide leading zeros
    int digit = value % 10;

    // turn on
    charPinOn(NUM_CHAR_PINS-1-i);
    displayDigit(digit);

    value-=digit;
    value/=10;

    // turn off (clean up for next digit)
    segmentPinsAllOff();
    charPinOff(NUM_CHAR_PINS-1-i);
  }
}


void loop() {
  //cool: zero-one-one-two
  displayNumber(1002);
}


