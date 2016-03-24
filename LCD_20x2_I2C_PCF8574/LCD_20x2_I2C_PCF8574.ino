#include <Wire.h> // necessary if using LiquidCrystalNew_TWI
#include <LiquidCrystalNew_TWI.h>   // import library

LiquidCrystalNew_TWI lcd(0x27,0,PCF8574);

void setup() {
  lcd.begin(20,2); // Wire will be inited here by library so don't need to do manually
  lcd.backlight(0);
  lcd.print("initialized");
}

void loop() {
  long t;
  t=millis()+1000;
  while(millis()<t) { 
    lcd.setCursor(0,1);
    lcd.print("marginally");
  }

  t=millis()+1000;
  while(millis()<t) { 
    lcd.setCursor(0,1);
    lcd.print("  clever  ");
  }
}
