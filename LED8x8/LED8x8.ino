//------------------------------------------------------------------------------
// 8x8 single color LED grid controlled by Arduino
// dan@marginallycelver.com 2014-11-05
//------------------------------------------------------------------------------
// Copyright at end of file.
// please see http://www.github.com/MarginallyClever/8x8LED for more information.

//--------------------------------------------------------------------------------
// CONSTANTS
//--------------------------------------------------------------------------------

#define LETTER_W    (6)
#define MESSAGE_LEN (4)
#define MESSAGE_W   (MESSAGE_LEN * LETTER_W)
#define LETTER_H    (8)
#define GRID_W      (8)
#define GRID_H      (8)

#define SCROLL_DELAY (30)


// these pin numbers start at 1, because I was stupid.
// a list of cathode pins, unsorted
//const int cathode[8] = { 1,2,5,7,8,9,12,14 };
// a list of cathode pins, sorted by top to bottom
const int cathode[8] = { 9,14,8,12,1,7,2,5 };

// these pin numbers start at 1, because I was stupid.
// a list of anode pins, unsorted
//const int anode[8] = { 3,4,6,10,11,13,15,16 };
// a list of anode pins, sorted by left to right
const int anode[8] = { 13,3,4,10,6,11,15,16 };

// translate the pins on the LED panel to pins on the Arduino
const int arduino_to_grid[16] = { 13,12,11,10, 16,17,18,19, 2,3,4,5, 6,7,8,9 };

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



//--------------------------------------------------------------------------------
// METHODS
//--------------------------------------------------------------------------------



// I want to turn on the column N from the left.
// this figures out which pin on the LED that is,
// then figures out which pin on the arduino matches that LED pin.
// two translations!
int out(int x) {
  return arduino_to_grid[anode[x]-1];
}


// I want to turn on the row N from the top.
// this figures out which pin on the LED that is,
// then figures out which pin on the arduino matches that LED pin.
// two translations!
int in(int y) {
  return arduino_to_grid[cathode[y]-1];
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


// called once when arduino reboots
void setup() {
  int i;
 
  // set all the pins to output.
  // 16 could be defined as NUM_INPUTS+NUM_OUTPUTS
  for(i=0;i<16;++i) {
    pinMode(arduino_to_grid[i],OUTPUT);
  }

  // turn everything off.
  for(i=0;i<8;++i) {
    digitalWrite(out(i),LOW);
  }
  for(i=0;i<8;++i) {
    digitalWrite(in(i),HIGH);
  }
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


/**
* This file is part of LED8x8.
*
* LED8x8 is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* LED8x8 is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with Arduino Timer Interrupt. If not, see <http://www.gnu.org/licenses/>.
*/

