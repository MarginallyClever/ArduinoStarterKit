// Seven segment wire solver
// dan@marginallyclever.com 2016-03-22
//----------------------------------------------
// Figures out how you wired up the 10 pin, 7 segment display and then counts up.
// Connect the ten pins of your display to Arduino UNO pins 2-11, inclusive.
// Open the serial window to Newline+57600.
// Answer the questions.
// Enjoy digits.
// Tweet your success vid to @marginallyc.

//----------------------------------------------
// CONSTANTS
//----------------------------------------------

#define NUM_SEGS 8
#define NUM_PINS 10


// a map of which segment draws which digit.
// segment     1,2,3,4,5,6,7,8
int zero [] = {1,1,1,1,1,1,0,0};
int one  [] = {0,1,1,0,0,0,0,0};
int two  [] = {1,1,0,1,1,0,1,0};
int three[] = {1,1,1,1,0,0,1,0};
int four [] = {0,1,1,0,0,1,1,0};
int five [] = {1,0,1,1,0,1,1,0};
int six  [] = {1,0,1,1,1,1,1,0};
int seven[] = {1,1,1,0,0,0,0,0};
int eight[] = {1,1,1,1,1,1,1,0};
int nine [] = {1,1,1,1,0,1,1,0};

int *digits[] = {zero,one,two,three,four,five,six,seven,eight,nine};


//----------------------------------------------
// GLOBALS
//----------------------------------------------

// filled in by the user
int pins[NUM_SEGS];


//----------------------------------------------
// METHODS
//----------------------------------------------

void setup() {
  Serial.begin(57600);
  Serial.println();
  Serial.println(" 111 ");
  Serial.println("6   2");
  Serial.println("6   2");
  Serial.println(" 777 ");
  Serial.println("5   3");
  Serial.println("5   3");
  Serial.println(" 444  8");
  Serial.println();
  Serial.println("\nWhich light is lit?  Type the digit and press enter.  0 for none.");

  // put your setup code here, to run once:
  int i;
  for(i=2;i<2+NUM_PINS;++i) {
    pinMode(i,OUTPUT);
    digitalWrite(i,LOW);
  }
  
  // put your main code here, to run repeatedly:
  for(i=2;i<12;++i) {
    //Serial.print("I am lighting pin ");
    //Serial.println(i);
    digitalWrite(i,HIGH);
    int index = readPinFromUser();
    if(index>0) {
      pins[index-1] = i;
    }
    digitalWrite(i,LOW);
  }
}

int readPinFromUser() {
  Serial.println("\nWhich light is lit?  (0 for none)");
  
  int done=0;
  int sum=0;
  while(1) {
    if(Serial.available()) {
      char c = Serial.read();
      if(c=='\n') break;
      int d = c-'0';
      if(d<0 || d>(NUM_SEGS+1) ) {
        Serial.print("invalid character '");
        Serial.print(c);
        Serial.println("'");
      }
      sum *= 10;
      sum += d;
      //Serial.print("** ");
      //Serial.println(sum);
    }
  }
//  Serial.print("I heard you say '");
//  Serial.print(sum);
//  Serial.println("'");
  return sum;
}

void loop() {
  int i=0;

  Serial.println("\nDisplaying digits");

  // all ten digits
  for(i=0;i<10;++i) {
    // 300ms each
    long t=millis()+300;
    while(millis()<t) {
      // show!
      displayDigit(i);
    }
  }
}

void displayDigit(int i) {
  int j;
  for(j=0;j<NUM_SEGS;++j) {
    if(digits[i][j]==1) {
      digitalWrite(pins[j],HIGH);
      digitalWrite(pins[j],LOW);
    }
  }
}

