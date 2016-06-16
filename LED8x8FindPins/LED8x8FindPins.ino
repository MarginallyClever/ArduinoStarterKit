//-----------------------------------------------------------
// Find all the pins on an 8x8 LED grid
// Dan Royer (dan@marginallyclever.com) 2015-02-21
// Inspired by Sam Barnum
// Plug your LED 8x8 into an Arduino UNO.
// Use pins digital pins 2-13 and analog pins 2-5.
// analog 0-5 can be referenced as digital 14-19.
//-----------------------------------------------------------

// uncomment this line to run this file on it's own.
// comment this line and call find_8x8_pins() to use this in another app.
#define TEST_LED8x8FINDPINS

// Comment this out to remove some messages
//#define VERBOSE


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

  reportAnodes();
  
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

  reportCathodes();
  assertAllPinsAssigned();
  
  // turn off all lights
  for(i=0;i<8;++i) {
    digitalWrite(anodes[i],LOW);
    digitalWrite(cathodes[i],HIGH);
  }
}


void reportAnodes() {
  Serial.print(F("The anodes are "));
  for(int i=0;i<8;++i) {
    if(i>0) Serial.print(", ");
    Serial.print(anodes[i]);
  }
  Serial.println();
}

void reportCathodes() {
  Serial.print(F("The cathodes are "));
  for(int i=0;i<8;++i) {
    if(i>0) Serial.print(", ");
    Serial.print(cathodes[i]);
  }
  Serial.println();
}

void assertAllPinsAssigned() {
  int pins2[20] = {0,0,2,3,4,5,6,7, 8,9,10,11,12,13, 0,0,16,17,18,19};
  int i;
  
  for(i=0;i<8;++i) {
    pins2[cathodes[i]]=0;
    pins2[anodes[i]]=0;
  }
  for(i=0;i<20;++i) {
    if(pins2[i]!=0) break;
  }
  if(i<20) {
    Serial.print(F("ERROR: unassigned pins "));
    char v=0;
    for(i=0;i<20;++i) {
      if(pins2[i]!=0) {
        if(v!=0) Serial.print(", ");
        v=1;
        Serial.print(pins2[i],DEC);
      }
    }
    Serial.println('.');
  }
}

#ifdef TEST_LED8x8FINDPINS
void setup() {
  Serial.begin(9600);
  
  find_8x8_pins();
  setup_complete();
}

//-----------------------------------------------------------
// When setup is done we will know the anodes and cathodes.
// We can now run animations.
//-----------------------------------------------------------
void setup_complete() {
  int i;
 
  // turn everything off.
  for(i=0;i<8;++i) {
    digitalWrite(out(i),LOW);
  }
  for(i=0;i<8;++i) {
    digitalWrite(in(i),HIGH);
  }
}


#define LETTER_W    (6)
#define MESSAGE_LEN (4)
#define MESSAGE_W   (MESSAGE_LEN * LETTER_W)
#define LETTER_H    (8)
#define GRID_W      (8)
#define GRID_H      (8)

#define SCROLL_DELAY (30)


// 1 means on
// 0 means off
// message says "VHS "
const int message[] = {
  1,0,0,0,1,0, 1,0,0,0,1,0, 0,1,1,1,1,0, 0,0,0,0,0,0,
  1,0,0,0,1,0, 1,0,0,0,1,0, 1,0,0,0,0,0, 0,0,0,0,0,0,
  1,0,0,0,1,0, 1,0,0,0,1,0, 1,0,0,0,0,0, 0,0,0,0,0,0,
  1,0,0,0,1,0, 1,1,1,1,1,0, 0,1,1,1,0,0, 0,0,0,0,0,0,
  1,0,0,0,1,0, 1,0,0,0,1,0, 0,0,0,0,1,0, 0,0,0,0,0,0,
  0,1,0,1,0,0, 1,0,0,0,1,0, 0,0,0,0,1,0, 0,0,0,0,0,0,
  0,1,0,1,0,0, 1,0,0,0,1,0, 0,0,0,0,1,0, 0,0,0,0,0,0,
  0,0,1,0,0,0, 1,0,0,0,1,0, 1,1,1,1,0,0, 0,0,0,0,0,0,
};




// I want to turn on the column N from the left.
// this figures out which pin on the LED that is,
// then figures out which pin on the arduino matches that LED pin.
// two translations!
int out(int x) {
  return anodes[x];
}


// I want to turn on the row N from the top.
// this figures out which pin on the LED that is,
// then figures out which pin on the arduino matches that LED pin.
// two translations!
int in(int y) {
  return cathodes[y];
}


// I want to turn on point P(x,y), which is X from the left and Y from the top.
// I might also want to hold it on for ms milliseconds.
int p(int x,int y,int ms) {
  digitalWrite(out(x),HIGH);
  digitalWrite(in(y),LOW);
  delay(ms);
  digitalWrite(in(y),HIGH);
  digitalWrite(out(x),LOW);
}


// called over and over after setup()
void loop() {
  one_cathode_at_a_time();
  one_anode_at_a_time();
  test_p();
  vhs_message();
}


void vhs_message() {
  // for animations
  long clock=millis();
  int scroll_step=0;
  int x,y,nx,ny;

  do {
    // for every dot on the LED grid
    for(x=0;x<GRID_W;++x) {
      for(y=0;y<GRID_H;++y) {
        // find the 1 or 0 in the message to display at this dot
        ny = MESSAGE_W * y;
        nx = ( scroll_step + x ) % MESSAGE_W;
        // if it's a 1, turn on the light for a fraction of a second.
        // persistence of vision will create the illusion thath it's solid.
        if( message[ ny + nx ] == 1 ) {
          p(x,y,0);
        }
      }
    }

    // animation time - adjust scroll_step, which will push the message to the left. 
    if( millis() - clock > SCROLL_DELAY*2) {
      clock=millis();
      ++scroll_step;
    }
  } while(scroll_step < MESSAGE_W * 3);
}


// method to test p(x,y)
// should light one pixel at a time, top to bottom, left to right.
void test_p() {
  int x,y;

  for(x=0;x<GRID_W;++x) {
    for(y=0;y<GRID_H;++y) {
      p(x,y,SCROLL_DELAY);
    }
  }
}


// test the sequence of cathodes
// should light each row top to bottom
void one_cathode_at_a_time() {
  int i,j;

  // 8s could be defined as NUM_INPUTS and NUM_OUTPUTS
  // allow electricity to flow from all anodes
  for(j=0;j<8;++j) {
    digitalWrite(out(j),HIGH);
  }
  // change one cathode at a time
  for(i=0;i<8;++i) {
    digitalWrite(in(i),LOW);
    delay(SCROLL_DELAY);
    digitalWrite(in(i),HIGH);
  }
  // turn off all anodes
  for(j=0;j<8;++j) {
    digitalWrite(out(j),LOW);
  }
}


// test the sequence of anodes
// should light each column left to right
void one_anode_at_a_time() {
  int i,j;

  // 8s could be defined as NUM_INPUTS and NUM_OUTPUTS
  // allow electricity to flow into all cathodes
  for(j=0;j<8;++j) {
    digitalWrite(in(j),LOW);
  }
  // change one anode at a time
  for(i=0;i<8;++i) {
    digitalWrite(out(i),HIGH);
    delay(SCROLL_DELAY);
    digitalWrite(out(i),LOW);
  }
  // block all cathodes
  for(j=0;j<8;++j) {
    digitalWrite(in(j),HIGH);
  }
}
#endif
