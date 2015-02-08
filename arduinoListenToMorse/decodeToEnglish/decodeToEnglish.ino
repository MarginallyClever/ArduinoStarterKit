// use a microphone to listen for morse code, then decypher the message.
#define THRESHOLD    (44)
#define MAX_SAMPLES  (5)

#define BAUD         (100.0)
#define WAIT         (5.0)
#define AVG_LONG     (BAUD*3.0/WAIT)
#define AVG_SHORT    (BAUD*1.0/WAIT)
#define AVG_NEWWORD  (BAUD*7.0/WAIT)
#define MINIMUM      (AVG_SHORT/4.0)


int top=0;

int samples[MAX_SAMPLES];
int si=0;
int mi=0;
int total=0;

int c=0;
int is_on=0;


void setup() {
  Serial.begin(57600);
  pinMode(2,INPUT);
  //digitalWrite(A0,HIGH);
  
  for(int i=0;i<MAX_SAMPLES;++i) {
    samples[i]=0;
  }
}


void loop() {
  findOnOffEvent();
}


void findOnOffEvent() {
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
        // Was the noice a long or a short?
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
