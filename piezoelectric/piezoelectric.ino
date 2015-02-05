// Connect the positive leg (long) to pin 6
// Connect the negative leg (short) to GND

#define PIEZO_PIN  (6)

// pause in the song
#define NONE  (0)
// notes based on http://www.phy.mtu.edu/~suits/notefreqs.html
#define C4    (261.625565)
#define D4    (293.7)
#define E4    (329.6)
#define F4    (349.2)

// beats per minute
#define BPM   (120.0)

// list of notes in the song,
float notes[] = { C4,   D4,   E4,   F4,   NONE };
// and the length of each note
float times[] = { 1.00, 0.50, 0.50, 0.25, 0.10 };


void setup() {
  pinMode(PIEZO_PIN,OUTPUT);
}

  
void play_one_note(float frequency,float duration) {
  duration *= 1000000 * (60.0/BPM);

  if(frequency>0) {
    float t=0;
    
    
    int hold_time = 1000000.0/frequency;
    while(t<duration) {
      digitalWrite(PIEZO_PIN,HIGH);
      // half the time on
      delayMicroseconds( hold_time );
      digitalWrite(PIEZO_PIN,LOW);
      // half the time off
      delayMicroseconds( hold_time );
      t += hold_time;
    }
  } else {
    delayMicroseconds( duration );
  }
}


void loop() {
  int note_count = sizeof(notes) / sizeof(float);

  for(int i=0;i<note_count;++i) {
    play_one_note(notes[i],times[i]);
  }
}
