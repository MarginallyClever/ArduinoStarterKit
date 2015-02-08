// use a microphone to listen for morse code, then decypher the message.
// Find on/off signals + time of event
#define THRESHOLD    (44)
#define MAX_SAMPLES  (5)


int top=0;

int samples[MAX_SAMPLES];
int si=0;
int mi=0;
int total=0;

char *graph[11] = {
  "----------",
  "*---------",
  "**--------",
  "***-------",
  "****------",
  "*****-----",
  "******----",
  "*******---",
  "********--",
  "*********-",
  "**********",
};  


void setup() {
  Serial.begin(57600);
  pinMode(2,INPUT);
  //digitalWrite(A0,HIGH);
  
  for(int i=0;i<MAX_SAMPLES;++i) {
    samples[i]=0;
  }
}


void loop() {
  graphVolume();
}


void graphVolume() {
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
  
  
  Serial.print(millis());
  Serial.print('\t');
  Serial.print(top);
  Serial.print('\t');
  Serial.println(graph[x]);

  delay(25);
}
