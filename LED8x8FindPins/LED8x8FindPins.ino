//-----------------------------------------------------------
// Find all the pins on an 8x8 LED grid
// Dan Royer (dan@marginallyclever.com) 2015-02-21
// Inspired by Sam Barnum
// Plug your LED 8x8 into an Arduino UNO.
// Use pins digital pins 2-13 and analog pins 0-5.
// analog 0-5 can be referenced as digital 14-19.
//-----------------------------------------------------------

// uncomment this line to run this file on it's own.
// comment this line and call find_8x8_pins() to use this in another app.
#define TEST_LED8x8FINDPINS

// Comment this out to remove some messages
#define VERBOSE


#ifdef TEST_LED8x8FINDPINS
#define VERBOSE  // must be defined to run solo
#endif

//-----------------------------------------------------------

// anodes will go here
int anodes[8];
// cathodes will go here
int cathodes[8];

//-----------------------------------------------------------


int find_8x8_pins_read_number() {
  char c;
  int p=0;
  
  while(1) {
    while(!Serial.available());
    c = Serial.read();
    if(c=='\n') break;
    
    int x = c-'0';
    //Serial.print("READ ");
    //Serial.println(x);
    p = p*10 + x;
  };
  
  Serial.println(p);
  return p;
}


void find_8x8_pins() {
  // unsorted pins
  int pins[16] = {2,3,4,5, 6,7,8,9, 10,11,12,13, 16,17,18,19};  // 16...19 = A2...A5
  // unsorted cathodes
  int cathodes_unsorted[8];
  int i, j, x;
  
  // set up the pins,
  // unblock all cathodes
  // don't transmit on any anode.
  for(i=0;i<16;++i) {
    pinMode(pins[i],OUTPUT);
    digitalWrite(pins[i],LOW);
  }
  
  // find the anodes
  Serial.println(F("I will try to light rows.  Tell me which row turns on.  The row on the top is one.  Use zero if nothing turns on."));

  j=0;  
  for(i=0;i<16;++i) {
    digitalWrite(pins[i],HIGH);
    Serial.print(F("Which row turns on? "));
    x = find_8x8_pins_read_number();
    digitalWrite(pins[i],LOW);
    
    if( x != 0 ) {
      anodes[x-1]=pins[i];
#ifdef VERBOSE
      Serial.print("row ");
      Serial.print(x-1);
      Serial.print(" is pin ");
      Serial.println(pins[i]);
#endif
    } else {
      cathodes_unsorted[j++]=pins[i];
      if(j==8) break;
    }
  }
  
#ifdef VERBOSE
  Serial.print(F("The anodes are "));
  for(i=0;i<8;++i) {
    if(i>0) Serial.print(", ");
    Serial.print(anodes[i]);
  }
  Serial.println();
#endif  
  
  // send on all anodes and block all cathodes.
  for(i=0;i<16;++i) {
    digitalWrite(pins[i],HIGH);
  }
  
  // find the cathodes
  Serial.println(F("I will try to light columns.  Tell me which column turns on.  The column on the top is zero.  Use zero if nothing turns on."));

  for(i=0;i<8;++i) {
    digitalWrite(cathodes_unsorted[i],LOW);
    Serial.println(F("Which column turns on?"));
    x = find_8x8_pins_read_number()-1;
    cathodes[x]=cathodes_unsorted[i];
    digitalWrite(cathodes_unsorted[i],HIGH);
#ifdef VERBOSE
    Serial.print("column ");
    Serial.print(x);
    Serial.print(" is pin ");
    Serial.println(cathodes_unsorted[i]);
#endif
  }

#ifdef VERBOSE
  Serial.print(F("The cathodes are "));
  for(i=0;i<8;++i) {
    if(i>0) Serial.print(", ");
    Serial.print(cathodes[i]);
  }
  Serial.println();
#endif

  // turn off all lights
  for(i=0;i<8;++i) {
    digitalWrite(anodes[i],LOW);
    digitalWrite(cathodes[i],HIGH);
  }
}


#ifdef TEST_LED8x8FINDPINS
void setup() {
  Serial.begin(9600);
  
  find_8x8_pins();
}


void loop() {
  // play success animation?
}
#endif
