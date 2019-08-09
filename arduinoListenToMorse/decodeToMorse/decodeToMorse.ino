// use a microphone to listen for morse code, then decypher the message.
// see https://en.wikipedia.org/wiki/Morse_code#Representation,_timing,_and_speeds
#define THRESHOLD    (44)  // microphone volume reported in a quiet room.
#define MAX_SAMPLES  (5)

#define BAUD         (100.0)
#define WAIT         (5.0)
#define AVG_LONG     (BAUD*3.0/WAIT)
#define AVG_SHORT    (BAUD*1.0/WAIT)
#define AVG_NEWWORD  (BAUD*7.0/WAIT)
#define MINIMUM      (AVG_SHORT/4.0)


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
// track how long the sound has been on or off.
int c=0;
// is it currently on?
int is_on=0;


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
  
  if(x>1) {
    // noise!
    if(is_on==0) {
      // noise has just started.
      if( c > MINIMUM ) {
        // Was the silence a new word or a new letter?
        if( c > (AVG_NEWWORD+AVG_SHORT)/2.0 ) {
          Serial.println();
        } else if( c > (AVG_LONG+AVG_SHORT)/2.0 ) {
          Serial.print(' ');
        }
      }
      // remember noise started
      is_on=1;
      c=0;
    }
  } else {
    // silence!
    if(is_on==1) {
      // silence is new
      if( c > MINIMUM ) {
        // Was the noise a long or a short?
        if( c > (AVG_LONG + AVG_SHORT)/2.0 ) {
          Serial.print('-');
        } else {
          Serial.print('.');
        }
      }
      // remember silence started
      is_on=0;
      c=0;
    }
  }

  c++;
  
  delay(WAIT);
}
