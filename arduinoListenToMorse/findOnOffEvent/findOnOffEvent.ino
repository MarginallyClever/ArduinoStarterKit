// use a microphone to listen for morse code, then decypher the message.
#define THRESHOLD    (44)  // minimum threshold, the sound in a quiet room.
#define MAX_SAMPLES  (5)


// the samples collected so far
int samples[MAX_SAMPLES];
// a running total of all the samples
int total=0;
// how many samples are valid?  first time through the samples list mi<MAX_SAMPLES.  after that mi=MAX_SAMPLES always.
int mi=0;
// which sample is the oldest?  replace it with the newest sample and update the running total.
int si=0;
// the loudest average sound heard so far
int top=0;

int c=0;


void setup() {
  Serial.begin(57600);
  
  for(int i=0;i<MAX_SAMPLES;++i) {
    samples[i]=0;
  }
}


void loop() {
  int volume=analogRead(0);
  
  total -= samples[si];
  samples[si] = volume;
  total += samples[si];
  if( mi < MAX_SAMPLES ) mi++;
  si = (si+1) % MAX_SAMPLES;
  int average = total / mi;
  
  if( top < average ) top = average;
  
  int x = 10.0 * (float)(average-THRESHOLD)/(float)(top-THRESHOLD);
  if(x<0) x=0;
  if(x>10) x=10;
  
  if(x>1) Serial.print('*');
  else    Serial.print(' ');
  
  c++;
  if(c>100) {
    c=0;
    Serial.println();
  }
  
  delay(5);
}
