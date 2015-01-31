//------------------------------------------------------------------------------
// Move a dot on an 8x8 single color LED with Arduino UNO and a joystick breakout.
// dan@marginallycelver.com 2015-01-24
//------------------------------------------------------------------------------
// Copyright at end of file.
// please see http://www.github.com/MarginallyClever/ArduinoStarterKit for more information.


//--------------------------------------------------------------------------------
// CONSTANTS
//--------------------------------------------------------------------------------

// size of the LED grid
#define GRID_W           (8)
#define GRID_H           (8)

#define JOYSTICK_DEAD_ZONE  (10)

// a list of anode pins, sorted by top to bottom of the grid
const int anode[8] = { 8,13,7,11,0,6,1,4 };
// a list of cathode pins, sorted by left to right of the grid
const int cathode[8] = { 12,2,3,9,5,10,14,15 };
// translate the pins on the LED panel to pins on the Arduino
const int arduino_to_grid[16] = { 13,12,11,10, 16,17,18,19, 2,3,4,5, 6,7,8,9 };

//--------------------------------------------------------------------------------
// GLOBALS
//--------------------------------------------------------------------------------

// this remembers the last time the dot moved.  Controls the speed of movement.
long clock;

// this remembers where the dot is on the grid.
float px, py;


//--------------------------------------------------------------------------------
// METHODS
//--------------------------------------------------------------------------------

void setup() {
  int i;
  // set all the pins to output.
  for(i=0;i<16;++i) {
    pinMode(arduino_to_grid[i],OUTPUT);
  }
  // turn on all resistors, should produce no light
  for(i=0;i<8;++i) {
    digitalWrite(out(i),LOW);
    digitalWrite(in(i),HIGH);
  }
  
  // set the start to the top left corner
  px=0;
  py=0;
  
  clock=millis();
}



// I want to turn on the column N from the left.
// this figures out which pin on the LED that is,
// then figures out which pin on the arduino matches that LED pin.
// two translations!
int out(int x) {
  return arduino_to_grid[cathode[x]];
}


// I want to turn on the row N from the top.
// this figures out which pin on the LED that is,
// then figures out which pin on the arduino matches that LED pin.
// two translations!
int in(int y) {
  return arduino_to_grid[anode[y]];
}


// I want to turn on point P(x,y), which is X from the left and Y from the top.
// I might also want to hold it on for us microseconds.
void p(int x,int y,int us) {
  // don't try to turn on a light that doesn't exist
  //if(x<0 || x>GRID_W) return;
  //if(y<0 || y>GRID_H) return;
  
  // now light it
  digitalWrite(out(x),HIGH);
  digitalWrite(in(y),LOW);
  delayMicroseconds(us);
  // turn it off again
  digitalWrite(in(y),HIGH);
  digitalWrite(out(x),LOW);
}


void loop() {
  //one_at_a_time();
  //test_p();
  move_dot();
}


void move_dot() {
  // draw the dot
  p(px,py,0);
  
  // 10 times a second,
  if(millis()-clock>100) {
    // get ready to wait another 1/10th of a second
    clock=millis();

    // read the joystick
    float dx=map(analogRead(0),0,1023,-500,500);
    float dy=map(analogRead(1),0,1023,-500,500);
  
    // move the dot
    if(dx> JOYSTICK_DEAD_ZONE) px++;
    if(dx<-JOYSTICK_DEAD_ZONE) px--;
    if(dy> JOYSTICK_DEAD_ZONE) py++;
    if(dy<-JOYSTICK_DEAD_ZONE) py--;

    // don't let the dot go off the side of the grid
    if(px>GRID_W-1) px=GRID_W-1;
    if(px<0       ) px=0;
    if(py>GRID_H-1) py=GRID_H-1;
    if(py<0       ) py=0;
  }
}


// light the whole grid
void test_p() {
  int x,y;

  for(x=0;x<GRID_W;++x) {
    for(y=0;y<GRID_H;++y) {
      p(x,y,5);
    }
  }
}


// light one dot at a time
void one_at_a_time() {
  int i,j;

  for(j=0;j<8;++j) {
    digitalWrite(out(j),HIGH);
    for(i=0;i<8;++i) {
      digitalWrite(in(i),LOW);
      delay(100);
      digitalWrite(in(i),HIGH);
    }
    digitalWrite(out(j),LOW);
  }
}


/**
* This file is part of ArduinoStarterKit.
*
* ArduinoStarterKit is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* ArduinoStarterKit is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with Arduino Timer Interrupt. If not, see <http://www.gnu.org/licenses/>.
*/
