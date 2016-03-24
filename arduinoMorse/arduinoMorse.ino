//------------------------------------------------------------------------------
// Morse Code Blink Example.
// dan@marginallyclever.com 2014-11-02
//
// Type an upper case message in the arduino serial window and this program
// will blink the built in LED on pin 13 in the matching Morse code.
//
// Must have "send new line" on in the Arduino serial window.
// Only works with upper case letters.
//
// Try this demo with a piezo speaker instead of an LED!
//
// More codes obtained from http://en.wikipedia.org/wiki/Morse_code
// Please support Wikipedia.
//------------------------------------------------------------------------------
// Copyright at end of file.  Please see
// please see http://www.github.com/MarginallyClever/ArduinoStarterKit for more information.


//------------------------------------------------------------------------------
// constants
//------------------------------------------------------------------------------
#define ONCE     (100)
#define LED      (13)
#define MAX_BUF  (64)
#define BAUD     (57600)

//                            0        10        20        30        40        50
//                            0123456789012345678901234567890123456789012345678901234
static const char *letters = "ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789.,?'!/()&:;=+-_\"$@";

// each code represents a character.
// 1 represents a dot and 0 represents a dash.
// the order of these codes matches the order of the characters in *letters.
static const char *codes[] = {
  "10",      // A, codes[0]
  "0111",    // B, codes[1]
  "0101",    // C
  "011",     // D
  "1",       // E
  "1101",    // F
  "001",     // G
  "1111",    // H
  "11",      // I
  "1000",    // J
  "010",     // K
  "1011",    // L
  "00",      // M
  "01",      // N
  "000",     // O
  "1001",    // P
  "0010",    // Q
  "101",     // R
  "111",     // S
  "0",       // T
  "110",     // U
  "1110",    // V
  "100",     // w
  "0110",    // x
  "0100",    // y
  "0011",    // z
  "00000",   // 0
  "10000",   // 1
  "11000",   // 2
  "11100",   // 3
  "11110",   // 4
  "11111",   // 5
  "01111",   // 6
  "00111",   // 7
  "00011",   // 8
  "00001",   // 9
  "101010",  // .
  "001100",  // ,
  "110011",  // ?
  "100001",  // '
  "010100",  // !
  "01101",   // /
  "01001",   // (
  "010010",  // )
  "10111",   // &
  "000111",  // :
  "010101",  // ;
  "01110",   // =
  "10101",   // +
  "01110",   // -
  "110010",  // _
  "101101",  // "
  "1110110", // $
  "100101",  // @, codes[54]
};

//------------------------------------------------------------------------------
// global variables
//------------------------------------------------------------------------------

char buffer[MAX_BUF];
int soFar;



//------------------------------------------------------------------------------
// methods
//------------------------------------------------------------------------------

// turn the light on
void on() {
  digitalWrite(LED,HIGH);
}

// turn the light off
void off() {
  digitalWrite(LED,LOW);
}


// short mark, dot or "dit" (·) — "dot duration" is one time unit long
void dot() {
  on();
  Serial.print('.');
  delay(ONCE);
}


// longer mark, dash or "dah" (–) — three time units long
void dash() {
  on();
  Serial.print('-');
  delay(ONCE*3);
}


// short gap between any combination of dots and dashes — one time unit long
void nextElement() {
  off();
  delay(ONCE);
}


// short gap (between letters) — three time units long
void nextLetter() {
  off();
  //Serial.println("  ***");
  delay(ONCE*3);
}


// medium gap (between words) — seven time units long
void nextWord() {
  off();
  //Serial.println("  *******");
  delay(ONCE*7);
}


void ready() {
  soFar=0;
  Serial.println("> ");
}


void setup() {
  // set up LED
  pinMode(LED,OUTPUT);
  off();
  
  // set up to talk with the PC.
  Serial.begin(BAUD);
  Serial.println("** Start **");
  ready();
}


void loop() {
  // listen for serial commands
  while( Serial.available() > 0 ) {
    char c = Serial.read();
    if( c == '\n' && soFar > 0 ) {
      buffer[soFar]=0;
      processMessage(buffer);
      ready();
    } else if( soFar < MAX_BUF ) {
      buffer[ soFar ] = c;
      soFar++;
    }
  }
}


void processMessage(char *message) {
  int i, first;

  // is this the first letter of word?
  first = 1;
  // go through the characters sent by the PC user one by one
  for( i=0; i<strlen(message); ++i ) {
    // find the character in the list of *letters
    char c = message[i];

    if( c == ' ' ) {
      Serial.print("  ");
      nextWord();
      first = 1;
      Serial.print("\n");  // new line
      continue;
    }
    if( first != 1 ) {
      nextLetter();
    }
      
    Serial.print(c);
    Serial.print(' ');
    // try to turn it into morse.
    morsifyLetter(c);
    off();
    first = 0;
    Serial.print("\n");  // new line
  }
}


// find j such that codes[j] is the sequence for character c
void morsifyLetter(char c) {
  int j;
  
  for( j=0; j<strlen(letters); ++j ) {
    if( letters[j] == c ) {
      // letters[j] ia a match!  That means codes[j] is the code to use.
      blinkCodeSequence(j);
      // stop looping, quit morsifyLetter() right now
      return;
    }
  }
  // didn't find it...
  Serial.print("** Not found **");
}


// blink sequence codes[j]
void blinkCodeSequence(int j) {
  int i;

  for( i=0; i < strlen(codes[j]); ++i ) {
    // put a gap between dots and dashes.
    if( i > 0 ) nextElement();

    // pulse the light
    if( codes[j][i] == '0' ) dash();
    if( codes[j][i] == '1' ) dot();
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

