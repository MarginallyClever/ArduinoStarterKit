int x=0;  // this number doesn't get reset when loop() restarts.


void setup() {
  // get ready to talk with the PC at 9600 baud
  Serial.begin(9600);
}


void loop() {
  x++;
  // print a number
  Serial.print(x);
  // print a string
  Serial.print(" BRAVEST ");
  // print a string with a \n (newline) on the end
  Serial.println("WARRIORS");
}
