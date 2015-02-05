/*
 * IRremote: IRrecvDemo - demonstrates receiving IR codes with IRrecv
 * An IR detector/demodulator must be connected to the input RECV_PIN.
 * Version 0.1 July, 2009
 * Copyright 2009 Ken Shirriff
 * http://arcfn.com
 *
 * For the generic remote kit from Marginally Clever (check actual source)
 * Use 3.3V for the IR receiver NOT 5V
 * Sam Barnum (barnumster@gmail.com)
 */

#include <IRremote.h>
//from https://github.com/shirriff/Arduino-IRremote

int RECV_PIN = 2;

IRrecv irrecv(RECV_PIN);
decode_results results;
byte startPin = 3; //the first pin to start sending data on
byte endPin = 9; //the last pin

byte LCD[10] =  // on/off pattern to show each number. Left most column is the 'a' pin, then 'b' etc. eg for '0' all LEDs are on except the gPin LED
{
  0b011111100,//0
  0b001100000,//1
  0b011011010,//2
  0b011110010,//3
  0b001100110,//4
  0b010110110,//5
  0b010111110,//6
  0b011100000,//7
  0b011111110,//8
  0b011110110,//9
};


int codeToInt(long hex){ //converts hex codes of results.value to integers corresponding with each button. Button codes were manually determined from raw output
/*consider making an array.*/
  int i;
  switch(hex){

  case 0xFF6897:
    i = 0;
    break;
  case 0xFF30CF:
    i = 1;
    break;
  case 0xFF18E7:
    i = 2;
    break;
  case 0xFF7A85:
    i = 3;
    break;
  case 0xFF10EF:
    i = 4;
    break;
  case 0xFF38C7:
    i = 5;
    break;
  case 0xFF5AA5:
    i = 6;
    break;
  case 0xFF42BD:
    i = 7;
    break;
  case 0xFF4AB5:
    i = 8;
    break;
  case 0xFF52AD:
    i = 9;
    break;

  default:
    i = -999;
    Serial.println("Unrecognized code. Please add it to the list of known codes");
    break;
  }

  return i;
}


void printLCD(int n){
  byte code = LCD[n]; //gets the corresponding on/off pattern for each number. LCD[0] holds the on/off code for the number '0', LCD[1] holds
  boolean off; //set == 1 the LED segment is off. set == 0 and LED segment is ON
  int readIndex = 7; //start reading at the *8th* bit of the byte that refers to the 'a' pin. 0th is the first digit
  //an 8th digit exists for a decimal point but is not currently coded in

  for(int i = startPin; i <= endPin ; i++){ //runs through all the digits
    off = bitRead(code,readIndex);
    //    Serial.print("digit :");
    //    Serial.print(i);
    //    Serial.print("/t");
    //    Serial.println(off);
    digitalWrite(i, off-1); //type abuse. if 1, then -> 0. if 0 -> -1 (anything that is not == 0 is "HIGH" for digitalWrite()
    readIndex--;//move to the right one digit /*Hey! Sam! Figure out how to use bitwise operators!!!*/
  }
}



void setup() {
  irrecv.enableIRIn(); // Start the receiver
  for(int j = startPin; j <= endPin; j++){//sets pins all to output
    pinMode(j, OUTPUT);
  }
  digitalWrite(endPin, 1); //turns off the 
  Serial.begin(9600);
}


void loop() {
  long val;
  int p; //digit for LCD output
  if (irrecv.decode(&results)) {
    val = results.value;
    if(val != 0xFFFFFFFF){//
      Serial.println(val, HEX);
      p = codeToInt(val);
      if( p != -999){ //checks that it is not undefined signal being returned
        printLCD(p);
      }
    }
    irrecv.resume(); // Receive the next value
  }
}






