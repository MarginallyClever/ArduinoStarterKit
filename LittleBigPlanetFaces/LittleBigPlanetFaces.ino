//-----------------------------------------------------------
// little big planet emotion system
// 8x8 led grid, joystick, Arduino UNO
// Dan Royer (dan@marginallyclever.com) 2015-02-21
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
//-----------------------------------------------------------

#define SCREEN_W  (8)
#define SCREEN_H  (8)
#define DEADZONE  (30)

#define NEUTRAL 0
#define HAPPY   1
#define SAD     2
#define ANGRY   3
#define SCARED  4

// screen types
#define SCREEN_1088BS (1)
#define SCREEN_1088AS (2)

#define SCREEN_TYPE SCREEN_1088AS

//-----------------------------------------------------------

//#if SCREEN_TYPE == SCREEN_1088BS
//int anodes[] = { 19, 11, 2, 16, 9, 3, 8, 5 };
//int cathodes[] = { 10, 7, 6, 18, 4, 17, 12, 13 };
//#endif

//#if SCREEN_TYPE == SCREEN_1088AS
int anodes[] = { 10, 11, 18, 7, 17, 5, 4, 13 };
int cathodes[] = { 6, 3, 8, 2, 19, 9, 12, 16 };
//#endif

char face_neutral[] = {
  0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,
  0,1,0,0,0,0,1,0,
  0,1,0,0,0,0,1,0,
  0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,
  0,1,1,1,1,1,1,0,
  0,0,0,0,0,0,0,0,
};

char face_happy[] = {
  0,0,0,0,0,0,0,0,
  0,1,0,0,0,0,1,0,
  0,1,0,0,0,0,1,0,
  0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,
  0,1,0,0,0,0,1,0,
  0,0,1,1,1,1,0,0,
  0,0,0,0,0,0,0,0,
};
char face_sad[] = {
  0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,
  0,0,1,0,0,1,0,0,
  0,1,0,0,0,0,1,0,
  0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,
  0,0,1,1,1,1,0,0,
  0,1,0,0,0,0,1,0,
};
char face_angry[] = {
  0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,
  0,1,0,0,0,0,1,0,
  0,0,1,0,0,1,0,0,
  0,0,0,0,0,0,0,0,
  0,0,0,1,1,0,0,0,
  0,0,1,0,0,1,0,0,
  0,1,0,0,0,0,1,0,
};
char face_scared[] = {
  0,0,0,0,0,0,0,0,
  0,1,1,0,0,1,1,0,
  0,1,0,0,0,0,1,0,
  0,0,0,0,0,0,0,0,
  0,0,0,1,1,0,0,0,
  0,0,1,0,0,1,0,0,
  0,0,1,0,0,1,0,0,
  0,0,0,0,0,0,0,0,
};

char *faces[] = {
  face_neutral,
  face_happy,
  face_sad,
  face_angry,
  face_scared,
};

//-----------------------------------------------------------

long t=0;
int old_joystick=0;
int mood=NEUTRAL;

//-----------------------------------------------------------



void draw_dot_at(int x,int y) {
  digitalWrite(cathodes[y],LOW);
  digitalWrite(anodes[x],HIGH);
  digitalWrite(anodes[x],LOW);
  digitalWrite(cathodes[y],HIGH);
}


void draw_face() {
  char *f = faces[mood];
  
  int x,y;
  
  for(y=0;y<SCREEN_H;++y) {
    for(x=0;x<SCREEN_W;++x) {
      if(f[y*SCREEN_W+x]==1) {
        draw_dot_at(x,y);
      }
    }
  }
}


void setup() {
  int i;
  for(i=0;i<8;++i) {
    // send data on all pins
    pinMode(anodes[i],OUTPUT);
    pinMode(cathodes[i],OUTPUT);
    
    digitalWrite(anodes[i],LOW);
    digitalWrite(cathodes[i],HIGH);
  }
  t=millis();
}


void loop() {
  int vx = analogRead(0) - 512;
  int vy = analogRead(1) - 512;

  if(old_joystick==0) {
    if( abs(vx) > DEADZONE || abs(vy) > DEADZONE ) {
      old_joystick=1;
      
      if( abs(vx) > abs(vy) ) {
        // left/right => happy/sad
        if(vx>0) {
          // left
          if(mood==NEUTRAL) mood = SAD;
          else if(mood!=NEUTRAL) mood = NEUTRAL;
        } else {
          // right
          if(mood==NEUTRAL) mood = HAPPY;
          else if(mood!=NEUTRAL) mood = NEUTRAL;
        }
      } else {
        // up/down => angry/scared
        if(vy>0) {
          // up
          if(mood==NEUTRAL) mood = ANGRY;
          else if(mood!=NEUTRAL) mood = NEUTRAL;
        } else {
          // down
          if(mood==NEUTRAL) mood = SCARED;
          else if(mood!=NEUTRAL) mood = NEUTRAL;
        }
      }
    }
  } else {
    if( abs(vx) < DEADZONE && abs(vy) < DEADZONE ) {
      old_joystick=0;
    }
  }

  draw_face();
}

