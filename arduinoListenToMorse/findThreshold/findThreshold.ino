// use a microphone to listen for morse code, then decypher the message.
// a: find the threshold for this microphone


int bottom=2000;
int top=0;


void setup() {
  Serial.begin(57600);
  pinMode(2,INPUT);
  //digitalWrite(A0,HIGH);
}


void loop() {
  findThreshold();
}


void findThreshold() {
  int volume=analogRead(0);
  if(top<volume) top=volume;
  if(bottom>volume) bottom=volume;
  
  Serial.print(digitalRead(2));
  Serial.print('\t');
  Serial.print(top);
  Serial.print('\t');
  Serial.print(bottom);
  Serial.print('\t');
  Serial.println(volume);
  
  delay(25);
}
