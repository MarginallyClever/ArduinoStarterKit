//------------------------------------------------------------------------------
// 8x8 single color LED Tetris for Arduino UNO and a joystick breakout.
// dan@marginallycelver.com 2015-01-22
//------------------------------------------------------------------------------
// Copyright at end of file.
// please see http://www.github.com/MarginallyClever/8x8LEDtetris for more information.

/*
 HARDWARE
 ---
 The LED grid is a red 8x8 model 1088BS.
 The Arduino is an UNO.
 The joystick is a xinda ps3/xbox style clickable stick on a breakout board.
 
 WIRING
 ----
 Place the grid so the lights are facing you and the nub on one edge is on the
 bottom.  On the back should be two horizontal rows of pins.  the top row, from
 left to right, connects to arduino digital pins 9-2.  The bottom row of pins,
 from left to right, connect to digital pins 13-10 and analog pins 2-5.
 
 The joystick has 5 pins.  joystick 5v and GND go to the same pins on arduino.
 VRx goes to A0.  VRy goes to A1.  SW goes to arduino digital pin 1.
 
 While the SW pin is connected you will not be able to use Serial.*, because
 Serial *also* uses pins 0 and 1.
*/

//--------------------------------------------------------------------------------
// CONSTANTS
//--------------------------------------------------------------------------------

#define LETTER_W         (6)
#define MESSAGE_LEN      (4)
#define MESSAGE_W        (MESSAGE_LEN * LETTER_W)
#define LETTER_H         (8)
#define GRID_W           (8)
#define GRID_H           (8)
#define PIECE_W          (4)
#define PIECE_H          (4)

#define ON               LOW
#define OFF              HIGH 

#define NUM_PIECE_TYPES  (7)
#define MAX_CHANCES      (9)

// Joysticks often have a small dead zone in the center where nothing registers.
#define JOYSTICK_DEAD_ZONE  (10)


// a list of cathode pins, sorted by top to bottom of the grid
const int cathode[8] = { 8,13,7,11,0,6,1,4 };

// a list of anode pins, sorted by left to right of the grid
const int anode[8] = { 12,2,3,9,5,10,14,15 };


// translate the pins on the LED panel to pins on the Arduino
const int arduino_to_grid[16] = { 13,12,11,10, 16,17,18,19, 2,3,4,5, 6,7,8,9 };

// each piece is max 4 wide, 4 tall, and 4 rotations
const char piece_I[] = {
  0,0,0,0,
  0,0,0,0,
  1,1,1,1,
  0,0,0,0,

  0,1,0,0,
  0,1,0,0,
  0,1,0,0,
  0,1,0,0,
  
  0,0,0,0,
  0,0,0,0,
  1,1,1,1,
  0,0,0,0,

  0,1,0,0,
  0,1,0,0,
  0,1,0,0,
  0,1,0,0,
};

const char piece_L1[] = {
  0,1,0,0,
  0,1,0,0,
  0,1,1,0,
  0,0,0,0,

  0,0,0,0,
  1,1,1,0,
  1,0,0,0,
  0,0,0,0,
  
  1,1,0,0,
  0,1,0,0,
  0,1,0,0,
  0,0,0,0,

  0,0,1,0,
  1,1,1,0,
  0,0,0,0,
  0,0,0,0,
};

const char piece_L2[] = {
  0,1,0,0,
  0,1,0,0,
  1,1,0,0,
  0,0,0,0,

  1,0,0,0,
  1,1,1,0,
  0,0,0,0,
  0,0,0,0,
  
  0,1,1,0,
  0,1,0,0,
  0,1,0,0,
  0,0,0,0,

  0,0,0,0,
  1,1,1,0,
  0,0,1,0,
  0,0,0,0,
};

const char piece_T[] = {
  0,0,0,0,
  1,1,1,0,
  0,1,0,0,
  0,0,0,0,

  0,1,0,0,
  1,1,0,0,
  0,1,0,0,
  0,0,0,0,

  0,1,0,0,
  1,1,1,0,
  0,0,0,0,
  0,0,0,0,

  0,1,0,0,
  0,1,1,0,
  0,1,0,0,
  0,0,0,0,
};

const char piece_S1[] = {
  1,0,0,0,
  1,1,0,0,
  0,1,0,0,
  0,0,0,0,

  0,1,1,0,
  1,1,0,0,
  0,0,0,0,
  0,0,0,0,

  1,0,0,0,
  1,1,0,0,
  0,1,0,0,
  0,0,0,0,

  0,1,1,0,
  1,1,0,0,
  0,0,0,0,
  0,0,0,0,
};

const char piece_S2[] = {
  0,1,0,0,
  1,1,0,0,
  1,0,0,0,
  0,0,0,0,

  1,1,0,0,
  0,1,1,0,
  0,0,0,0,
  0,0,0,0,
  
  0,1,0,0,
  1,1,0,0,
  1,0,0,0,
  0,0,0,0,

  1,1,0,0,
  0,1,1,0,
  0,0,0,0,
  0,0,0,0,
  0,0,0,0,
};

const char piece_O[] = {
  1,1,0,0,
  1,1,0,0,
  0,0,0,0,
  0,0,0,0,
  
  1,1,0,0,
  1,1,0,0,
  0,0,0,0,
  0,0,0,0,
  
  1,1,0,0,
  1,1,0,0,
  0,0,0,0,
  0,0,0,0,
  
  1,1,0,0,
  1,1,0,0,
  0,0,0,0,
  0,0,0,0,
};


const char *pieces[] = {
  piece_S1,
  piece_S2,
  piece_L1,
  piece_L2,
  piece_O,
  piece_T,
  piece_I,
};

const char chances[MAX_CHANCES] = {
  0,
  1,
  2,
  3,
  4,
  4,
  5,
  5,
  6,
};

int old_button=0;
int piece_id;
int piece_rotation;
int piece_x;
int piece_y;

long last_move;
long move_delay=100;

long last_drop;
long drop_delay=500;

char grid[8*8];


//--------------------------------------------------------------------------------
// METHODS
//--------------------------------------------------------------------------------



// I want to turn on the column N from the left.
// this figures out which pin on the LED that is,
// then figures out which pin on the arduino matches that LED pin.
// two translations!
int out(int x) {
  return arduino_to_grid[anode[x]];
}


// I want to turn on the row N from the top.
// this figures out which pin on the LED that is,
// then figures out which pin on the arduino matches that LED pin.
// two translations!
int in(int y) {
  return arduino_to_grid[cathode[y]];
}


// I want to turn on point P(x,y), which is X from the left and Y from the top.
// I might also want to hold it on for us microseconds.
void p(int x,int y,int us) {
  // don't try to turn on a light that doesn't exist
  //if(x<0 || x>GRID_W) return;
  //if(y<0 || y>GRID_H) return;
  
  // now light it
  digitalWrite(out(x),ON);
  digitalWrite(in(y),OFF);
  delayMicroseconds(us);
  digitalWrite(in(y),ON);
  digitalWrite(out(x),OFF);
}


// draw a piece from (px,py) to (px+x,py+y) on the grid
void draw_piece(int px,int py) {
  int x, y;
  
  const char *piece = pieces[piece_id] + (piece_rotation * PIECE_H * PIECE_W);
  
  for(y=0;y<PIECE_H;++y) {
    for(x=0;x<PIECE_W;++x) {
      if( piece[y*PIECE_W+x] == 1 ) {
        p(px+x,py+y,150);
      }
    }
  }
}


// grid contains the arduino's memory of the game board, including the piece that is falling.
void draw_grid() {
  int x, y;
  for(y=0;y<GRID_H;++y) {
    for(x=0;x<GRID_W;++x) {
      if( grid[y*GRID_W+x] != 0 ) {
        p(x,y,150);
      }
    }
  }
}


void choose_new_piece() {
  // make the chances array longer to change the odds of different pieces appearing.
  piece_id = chances[rand() % MAX_CHANCES];
  // always start the piece top center.
  piece_y=-4;  // -4 squares off the top of the screen.
  piece_x=3;
  // always start in the same orientation.
  piece_rotation=0;
}


void erase_piece_from_grid() {
  int x, y;
  
  const char *piece = pieces[piece_id] + (piece_rotation * PIECE_H * PIECE_W);
  
  for(y=0;y<PIECE_H;++y) {
    for(x=0;x<PIECE_W;++x) {
      int nx=piece_x+x;
      int ny=piece_y+y;
      if(ny<0 || ny>GRID_H) continue;
      if(nx<0 || nx>GRID_W) continue;
      if(piece[y*PIECE_W+x]==1) {
        grid[ny*GRID_W+nx]=0;  // zero erases the grid location.
      }
    }
  }
}


void add_piece_to_grid() {
  int x, y;
  
  const char *piece = pieces[piece_id] + (piece_rotation * PIECE_H * PIECE_W);
  
  for(y=0;y<PIECE_H;++y) {
    for(x=0;x<PIECE_W;++x) {
      int nx=piece_x+x;
      int ny=piece_y+y;
      if(ny<0 || ny>GRID_H) continue;
      if(nx<0 || nx>GRID_W) continue;
      if(piece[y*PIECE_W+x]==1) {
        grid[ny*GRID_W+nx]=1;  // zero erases the grid location.
      }
    }
  }
}


// Move everything down 1 space, destroying the old row number y in the process.
void delete_row(int y) {
  int x;
  for(;y>0;--y) {
    for(x=0;x<GRID_W;++x) {
      grid[y*GRID_W+x] = grid[(y-1)*GRID_W+x];
    }
  }
  // everything moved down 1, so the top row must be empty or the game would be over.
  for(x=0;x<GRID_W;++x) {
    grid[x]=0;
  }
}


void check_remove_lines() {
  int x, y, c;
  for(y=0;y<GRID_H;++y) {
    // count the full spaces in this row
    c = 0;
    for(x=0;x<GRID_W;++x) {
      if( grid[y*GRID_W+x] > 0 ) c++;
    }
    if(c==GRID_W) {
      // row full!
      delete_row(y);
    }
  }  
}


void try_to_move_piece_sideways() {
  // what does the joystick angle say
  int dx = map(analogRead(0),0,1023,500,-500);
  
  int new_px = piece_x;
  // is the joystick really being pushed?
  if(dx> JOYSTICK_DEAD_ZONE) new_px++;
  if(dx<-JOYSTICK_DEAD_ZONE) new_px--;

  if(can_piece_fit(new_px,piece_y,piece_rotation)) {
    piece_x=new_px;
  }
}


void try_to_rotate_piece() {
  // what does the joystick button say
  int new_button = digitalRead(1);
  // if the button state has just changed AND it is being let go,
  if( new_button > 0 && old_button != new_button ) {
    // figure out what it will look like at that new angle
    int new_pr = ( piece_rotation + 1 ) % 4;
    // if it can fit at that new angle (doesn't bump anything)
    if(can_piece_fit(piece_x,piece_y,new_pr)) {
      // then make the turn.
      piece_rotation = new_pr;
    }
  }
  old_button = new_button;
}


int can_piece_drop() {
  // can the piece drop lower?
  int x,y;
  for(y=0;y<GRID_H;++y) {
    for(x=0;x<GRID_W;++x) {
      //Serial.print(grid[y*GRID_W+x]);
      if(grid[y*GRID_W+x] == 2) {
        if(y==7) return 0;  // no: on bottom row
        if(grid[(y+1)*GRID_W+x] == 1) return 0;  // no: on a piece
      }
    }
    //Serial.print('\n');
  }
  return 1;  // can drop
}


// can the piece fit in this new location?
int can_piece_fit(int px,int py,int pr) {
  int x,y;
  const char *piece = pieces[piece_id] + (pr * PIECE_H * PIECE_W);
  
  for(y=0;y<PIECE_H;++y) {
    for(x=0;x<PIECE_W;++x) {      
      int ny=py+y;
      int nx=px+x;
      if(piece[y*PIECE_W+x]>0) {
        if(nx<0) return 0;  // no: off left side
        if(nx>=GRID_W ) return 0;  // no: off right side
        if(ny>=GRID_H ) return 0;  // no: goes off bottom of grid      
        if(grid[ny*GRID_W+nx]==1 ) return 0;  // no: grid already full in this space
      }
    }
  }
  
  return 1;  // can fit
}


void game_over() {
  while(1) {
    // Your homework: add a 'game over' animation here, then film it and tweet it to @marginallyc.
  }
}


void try_to_drop_piece() {
  if(can_piece_drop()==1) {
    erase_piece_from_grid();
    piece_y++;  // move piece down
    add_piece_to_grid();
  } else {
    // hit something!
    check_remove_lines();
    if(check_game_over()) {
      game_over();
    }
    choose_new_piece();
  }
}


void try_to_drop_faster() {
  int y = map(analogRead(1),0,1023,-500,500);
  if(y>JOYSTICK_DEAD_ZONE) {
    // player is holding joystick down, drop a little faster.
    try_to_drop_piece();
  }
}


void react_to_player() {
  erase_piece_from_grid();
  try_to_move_piece_sideways();
  try_to_rotate_piece();
  add_piece_to_grid();
  
  try_to_drop_faster();
}


int check_game_over() {
  // can the piece fit in this new location
  int x,y;
  const char *piece = pieces[piece_id] + (piece_rotation * PIECE_H * PIECE_W);
  
  for(y=0;y<PIECE_H;++y) {
    for(x=0;x<PIECE_W;++x) {      
      int ny=piece_y+y;
      int nx=piece_x+x;
      if(piece[y*PIECE_W+x]>0) {
        if(ny<0) return 1;  // yes: off the top!
      }
    }
  }
  
  return 0;  // not over yet...
}


// called once when arduino reboots
void setup() {
  int i;
  // set all the pins to output.
  // 16 could be defined as NUM_INPUTS+NUM_OUTPUTS
  for(i=0;i<16;++i) {
    pinMode(arduino_to_grid[i],OUTPUT);
  }

  // turn every light off.
  for(i=0;i<8;++i) {
    digitalWrite(out(i),OFF);
  }
  for(i=0;i<8;++i) {
    digitalWrite(in(i),ON);
  }
  
  // set up joystick click
  pinMode(1,INPUT);
  digitalWrite(1,HIGH);
  
  // clear the grid
  for(i=0;i<8*8;++i) {
    grid[i]=0;
  }
  
  // make the game a bit more random - pull a number from space and use it to 'seed' a crop of random numbers.
  randomSeed(analogRead(1));
  
  // get ready to start the game.
  choose_new_piece();
  
  // start the game clock after everything else is ready.
  last_move = millis();
  last_drop = last_move;
}


// called over and over after setup()
void loop() {
  // the game plays at one speed,
  if(millis() - last_move > move_delay ) {
    last_move = millis();
    react_to_player();
  }
  
  // ...and drops the falling block at a different speed.
  if(millis() - last_drop > drop_delay ) {
    last_drop = millis();
    try_to_drop_piece();
  }
  
  // when it isn't doing those two things, it's redrawing the grid.
  draw_grid();
}


/**
* This file is part of LED8x8tetris.
*
* LED8x8tetris is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* LED8x8tetris is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with Arduino Timer Interrupt. If not, see <http://www.gnu.org/licenses/>.
*/

