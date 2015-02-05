// Based on http://www.instructables.com/id/BYJ48-Stepper-Motor/step4/Modify-Code/

#define IN1  8
#define IN2  9
#define IN3  10
#define IN4  11

#define CLOCKWISE        (1)  // -1 to reverse
#define COUNTERCLOCKWISE (-CLOCKWISE)


int stepper_steps = 0;
char stepper_IN1;
char stepper_IN2;
char stepper_IN3;
char stepper_IN4;


void stepper_attach(int a,int b,int c,int d) {
  stepper_IN1=a;
  stepper_IN2=b;
  stepper_IN3=c;
  stepper_IN4=d;

  pinMode(stepper_IN1, OUTPUT); 
  pinMode(stepper_IN2, OUTPUT); 
  pinMode(stepper_IN3, OUTPUT); 
  pinMode(stepper_IN4, OUTPUT); 
}


void setup() {
  stepper_attach(8,9,10,11);
}


void loop() {
  unsigned long last_time;
  unsigned long currentMillis ;
  int steps_left=4095;
  long time;

  while(steps_left>0){
    currentMillis = micros();
    if(currentMillis-last_time>=1000){
      stepper(1,CLOCKWISE); 
      time=time+micros()-last_time;
      last_time=micros();
      steps_left--;
    }
  }
  Serial.println(time);
  Serial.println("Wait...!");
  delay(2000);
  steps_left=4095;

  while(steps_left>0){
    currentMillis = micros();
    if(currentMillis-last_time>=1000){
      stepper(1,COUNTERCLOCKWISE); 
      time=time+micros()-last_time;
      last_time=micros();
      steps_left--;
    }
  }
  Serial.println(time);
  Serial.println("Wait...!");
  delay(2000);
}


// stepper(number of steps, CLOCKWISE);
// stepper(number of steps, COUNTERCLOCKWISE);
void stepper(int step_count,int direction) {
  for (int x=0;x<xw;x++){
    switch(stepper_steps){
       case 0:
         digitalWrite(stepper_IN1, LOW); 
         digitalWrite(stepper_IN2, LOW);
         digitalWrite(stepper_IN3, LOW);
         digitalWrite(stepper_IN4, HIGH);
       break; 
       case 1:
         digitalWrite(stepper_IN1, LOW); 
         digitalWrite(stepper_IN2, LOW);
         digitalWrite(stepper_IN3, HIGH);
         digitalWrite(stepper_IN4, HIGH);
       break; 
       case 2:
         digitalWrite(stepper_IN1, LOW); 
         digitalWrite(stepper_IN2, LOW);
         digitalWrite(stepper_IN3, HIGH);
         digitalWrite(stepper_IN4, LOW);
       break; 
       case 3:
         digitalWrite(stepper_IN1, LOW); 
         digitalWrite(stepper_IN2, HIGH);
         digitalWrite(stepper_IN3, HIGH);
         digitalWrite(stepper_IN4, LOW);
       break; 
       case 4:
         digitalWrite(stepper_IN1, LOW); 
         digitalWrite(stepper_IN2, HIGH);
         digitalWrite(stepper_IN3, LOW);
         digitalWrite(stepper_IN4, LOW);
       break; 
       case 5:
         digitalWrite(stepper_IN1, HIGH); 
         digitalWrite(stepper_IN2, HIGH);
         digitalWrite(stepper_IN3, LOW);
         digitalWrite(stepper_IN4, LOW);
       break; 
         case 6:
         digitalWrite(stepper_IN1, HIGH); 
         digitalWrite(stepper_IN2, LOW);
         digitalWrite(stepper_IN3, LOW);
         digitalWrite(stepper_IN4, LOW);
       break; 
       case 7:
         digitalWrite(stepper_IN1, HIGH); 
         digitalWrite(stepper_IN2, LOW);
         digitalWrite(stepper_IN3, LOW);
         digitalWrite(stepper_IN4, HIGH);
       break; 
       default:
         digitalWrite(stepper_IN1, LOW); 
         digitalWrite(stepper_IN2, LOW);
         digitalWrite(stepper_IN3, LOW);
         digitalWrite(stepper_IN4, LOW);
       break; 
    }

    stepper_steps = ( stepper_steps + 7 + direction ) % 7;
  }
}