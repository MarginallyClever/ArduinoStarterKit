//------------------------------------------------------------------------------
// 8x8 single color LED Breakout for Arduino UNO and a joystick breakout.
// dan@marginallycelver.com 2015-02-21
//------------------------------------------------------------------------------
// Copyright at end of file.
// please see http://www.github.com/MarginallyClever/ArduinoStarterKit for more information.
//
// The LED grid is a red 8x8.
// The Arduino is an UNO.
// The joystick is a xinda ps3/xbox style clickable stick on a breakout board.
// 
// Place the grid so the lights are facing you and the nub on one edge is on the
// bottom.  On the back should be two horizontal rows of pins.  the top row, from
// left to right, connects to arduino digital pins 9-2.  The bottom row of pins,
// from left to right, connect to digital pins 13-10 and analog pins 2-5.
// 
// The joystick has 5 pins.  joystick 5v and GND go to the same pins on arduino.
// VRx goes to A0.  VRy goes to A1.  SW goes to arduino digital pin 1.
// 
// While the SW pin is connected you will not be able to use Serial.*, because
// Serial *also* uses pins 0 and 1.

//--------------------------------------------------------------------------------
// CONSTANTS
//--------------------------------------------------------------------------------

// size of the LED grid
#define GRID_W              (8)
#define GRID_H              (8)

#define JOYSTICK_DEAD_ZONE  (30)
#define JOYSTICK_BUTTON     (1)
#define JOYSTICK_X          (0)
#define JOYSTICK_Y          (1)

#define PADDLE_VX           (0.0002)
#define BALL_V              (2.0)

#define WALL_H              (3)  // layers of bricks

#define _1088BS  // LED type


#ifdef _1088BS
int anodes[] = { 19, 11, 2, 16, 9, 3, 8, 5 };
int cathodes[] = { 10, 7, 6, 18, 4, 17, 12, 13 };
#else
// y values
int anodes[] = { 9,4,19,6,10,18,11,16 };
// x values
int cathodes[] = { 5,12,13,8,17,7,3,2 };
#endif


//--------------------------------------------------------------------------------
// GLOBALS
//--------------------------------------------------------------------------------

// this is how arduino remembers what the button was doing in the past,
// so arduino can tell when it changes.
int old_button=0;

// this is how arduino remembers the falling piece.
float paddle_x;

// ball position
float ball_x;
float ball_y;
// ball velocity
float ball_vx;
float ball_vy;

char ball_glued=1;

// this controls how fast the player can move.
long last_move;
long move_delay=10;  // 10ms = 50 times a second

long last_tick;
long tick_delay=30;  // 30 times a second

// this is how arduino remembers where pieces are on the grid.
char grid[GRID_W*GRID_H];


//--------------------------------------------------------------------------------
// METHODS
//--------------------------------------------------------------------------------


// I want to turn on point P(x,y), which is X from the left and Y from the top.
// I might also want to hold it on for us microseconds.
void p(int x,int y,int us) {
  // don't try to turn on a light that doesn't exist
  //if(x<0 || x>GRID_W) return;
  //if(y<0 || y>GRID_H) return;
  
  // now light it
  digitalWrite(anodes[y],HIGH);
  digitalWrite(cathodes[x],LOW);
  delayMicroseconds(us);
  digitalWrite(cathodes[x],HIGH);
  digitalWrite(anodes[y],LOW);
}


// grid contains the arduino's memory of the game board, including the piece that is falling.
void draw_grid() {
  int x, y;
  for(y=0;y<GRID_H;++y) {
    for(x=0;x<GRID_W;++x) {
      if( grid[y*GRID_W+x] != 0 ) {
        p(x,y,30);
      }
    }
  }
}


void remove_paddle() {
  int x = GRID_W*(GRID_H-1)+floor(paddle_x);
  grid[x]=0;
  grid[x+1]=0;
}


void add_paddle() {
  int x = GRID_W*(GRID_H-1)+floor(paddle_x);
  grid[x]=1;
  grid[x+1]=1;
}



void remove_ball() {
  int x = GRID_W*floor(ball_y)+floor(ball_x);
  if(x<GRID_W*GRID_H && x>=0) {
    grid[x]=0;
  }
}


void add_ball() {
  int x = GRID_W*floor(ball_y)+floor(ball_x);
  if(x<GRID_W*GRID_H && x>=0) {
    grid[x]=1;
  }
}


void react_to_player() {
  float vx = map(analogRead(0),0,1023,512,-512);
  float vy = map(analogRead(1),0,1023,-512,512);
  
  Serial.println(vx);
  
  remove_paddle();
  
  if( abs(vx) > JOYSTICK_DEAD_ZONE ) {
    paddle_x += PADDLE_VX*vx;
    if(paddle_x<0) paddle_x=0;
    if(paddle_x>GRID_W-2) paddle_x=GRID_W-2;
  }
  
  add_paddle();
  
  remove_ball();
  
  if(ball_glued) {
    ball_x = paddle_x;
    ball_y = GRID_H-2;
    //fix_ball_speed();
    if(vy > JOYSTICK_DEAD_ZONE) {
      ball_glued=0;
    }
  }
  
  add_ball();
}


void game_over() {
  while(1) {
    if(digitalRead(1)==0) {
      setup();
      return;
    }
  }
}


void fix_ball_speed() {
  float len = sqrt(ball_vx*ball_vx+ball_vy*ball_vy);
  len *= BALL_V;
  ball_vx/=len;
  ball_vy/=len;
}


void move_game() {
  if(ball_glued) return;

  remove_ball();
    
  ball_x += ball_vx;
  ball_y += ball_vy;
  
  // hit walls?
  if( ball_x < 0 ) {
    ball_x = -ball_x;
    ball_vx= -ball_vx;
  }
  if( ball_x >= GRID_W ) {
    ball_x = 2.0f*(GRID_W) - ball_x;
    ball_vx= -ball_vx;
  }
  
  if( ball_y < 0 ) {
    ball_y = -ball_y;
    ball_vy= -ball_vy;
  } else if( ball_y > GRID_H ) {
//    game_over();
    ball_y = 2.0f*GRID_H -ball_y;
    ball_vy= -ball_vy;
  }
  
  // hit paddle?
  if(ball_x>=paddle_x && ball_x < paddle_x+2)
  {
    if(ball_y > GRID_H) {
      // bounce
      ball_y = 2.0f*(GRID_H) - ball_y;
      ball_vy= -ball_vy;
    }
  }
  
  // hit bricks?
  int x,y;
  float dx,dy;
  for(y=0;y<WALL_H;++y) {
    for(x=0;x<GRID_W;++x) {
      if(grid[GRID_W*y+x]==0) continue;
      float brick_x = x+0.5f;
      float brick_y = y+0.5f;
      
      dx = ball_x - brick_x;
      dy = ball_y - brick_y;
      if(dx*dx+dy*dy < 1) {
        // hit!
        // kill brick
        grid[GRID_W*y+x]=0;
        // bounce ball
        float len = sqrt(dx*dx+dy*dy);
        dx/=len;
        dy/=len;
        float dotproduct = dx * ball_vx + dy * ball_vy;
        ball_vx += -2.0f*dotproduct * dx;
        ball_vy += -2.0f*dotproduct * dy;
        fix_ball_speed();
      }
    }
  }
  
  add_ball();
}


// called once when arduino reboots
void setup() {
  Serial.begin(9600);
  
  int i;
  for(i=0;i<8;++i) {
    // set all the pins to output.
    pinMode(anodes[i],OUTPUT);
    pinMode(cathodes[i],OUTPUT);
    // turn on all resistors, should produce no light
    digitalWrite(anodes[i],LOW);
    digitalWrite(cathodes[i],HIGH);
  }
  
  // set up joystick button
  pinMode(1,INPUT);
  digitalWrite(1,HIGH);
  
  // make sure arduino knows the grid is empty.
  for(i=0;i<GRID_W*GRID_H;++i) {
    grid[i]=0;
  }
  // light top three rows on.
  for(i=0;i<GRID_W*WALL_H;++i) {
    grid[i]=1;
  }
  // center paddle  
  paddle_x = 3;
  ball_x = paddle_x;
  ball_y = GRID_H-2;
  ball_vx = BALL_V;
  ball_vy = -BALL_V;
  fix_ball_speed();
  
  // start the game clock after everything else is ready.
  last_move = millis();
  last_tick = last_move;
}


// called over and over after setup()
void loop() {
  // the game plays at one speed,
  if(millis() - last_move > move_delay ) {
    last_move = millis();
    react_to_player();
  }
  
  // ...and drops the falling block at a different speed.
  if(millis() - last_tick > tick_delay ) {
    last_tick = millis();
    move_game();
  }
  
  // when it isn't doing those two things, it's redrawing the grid.
  draw_grid();
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


