// 8x8 single color LED grid

//const int input[8] = { 1,2,5,7,8,9,12,14 };
const int input[8] = { 9,14,8,12,1,7,2,5 };

//const int output[8] = { 3,4,6,10,11,13,15,16 };
const int output[8] = { 13, 3, 4,10,6,11,15,16 };

const int arduino_to_grid[16] = { 13,12,11,10, 16,17,18,19, 2,3,4,5, 6,7,8,9 };


void setup() {
  int i;
  
  for(i=0;i<16;++i) {
    pinMode(arduino_to_grid[i],OUTPUT);
  }
  // turn on all resistors, should produce no light
  for(i=0;i<8;++i) {
    digitalWrite(out(i),LOW);
    digitalWrite(in(i),HIGH);
  }
}


int out(int pin) {
  return arduino_to_grid[output[pin]-1];
}


int in(int pin) {
  return arduino_to_grid[input[pin]-1];
}


int p(int x,int y,int ms) {
  digitalWrite(out(x),HIGH);
  digitalWrite(in(y),LOW);
  delay(ms);
  digitalWrite(in(y),HIGH);
  digitalWrite(out(x),LOW);
}


long clock=millis();
float px=0, py=0;


void loop() {
  //one_at_a_time();
  //test_p();
  float dx=(float)(analogRead(0)-512)/512.0f;
  float dy=(float)(analogRead(1)-512)/512.0f;
  
  // draw
  p(px,py,0);
  
  if(millis()-clock>100) {
    clock=millis();
    px+=dx;
    py+=dy;
    if(px>7) px=7;
    if(px<0) px=0;
    if(py>7) py=7;
    if(py<0) py=0;
  }
}


void test_p() {
  int x,y;

  for(x=0;x<8;++x) {
    for(y=0;y<8;++y) {
      p(x,y,5);
    }
  }
}


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



