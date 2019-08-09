// use a microphone to listen for morse code, then decypher the message.

#define THRESHOLD    (44)
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


// an easy way to show the volume
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
  
  for(int i=0;i<MAX_SAMPLES;++i) {
    samples[i]=0;
  }
}


void loop() {
  int volume=analogRead(0);
  
  // average the samples
  total -= samples[si];
  samples[si] = volume;
  total += samples[si];
  if( mi < MAX_SAMPLES ) mi++;
  si = (si+1) % MAX_SAMPLES;
  int average = total / mi;
  
  // remember the loudest sound, for scaling the graph
  if( top < average ) top = average;
  
  // scale the volume from threshold to top, inclusive.
  int x = 10.0 * (float)(average-THRESHOLD)/(float)(top-THRESHOLD);
  if(x<0) x=0;
  if(x>10) x=10;
  
  // print it out
  Serial.print(millis());
  Serial.print('\t');
  Serial.print(top);
  Serial.print('\t');
  Serial.println(graph[x]);

  delay(25);
}
