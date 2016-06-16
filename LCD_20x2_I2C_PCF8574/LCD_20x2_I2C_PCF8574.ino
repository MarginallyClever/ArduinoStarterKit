#define LCD_WIDTH  20
#define LCD_HEIGHT 2
#include <Wire.h> // necessary if using LiquidCrystalNew_TWI
#include <LiquidCrystalNew_TWI.h>   // import library

LiquidCrystalNew_TWI lcd(0x27,  // The I2C address
                         0,
                         PCF8574);  // The shift register chip on the I2C backpack

char *url = "http://marginallyclever.com - ";
char msg[21];
int urlLen;
int i;


void setup() {
  Serial.begin(57600);
  lcd.begin(LCD_WIDTH,LCD_HEIGHT); // Wire will be inited here by library so don't need to do manually
  lcd.backlight(0);

  // how long is URL?
  urlLen=0;
  while(url[urlLen]!=0) urlLen++;
  //Serial.print("URL=");
  //Serial.println(urlLen);
  i=0;
  
  lcd.backlight(0);
  lcd.setCursor(0,0);
  lcd.print("Read how to at");
}

void loop() {
  if(++i>=urlLen) i=0;

  int j;
  for(j=0;j<LCD_WIDTH-1;++j) {
    msg[j]=url[(i+j)%urlLen];
  }
  msg[j]=0;
  Serial.println(msg);
  
  lcd.setCursor(0,1);
  lcd.print(msg);
  
  delay(400);
}
