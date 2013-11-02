#include <LiquidCrystal.h>

LiquidCrystal lcd(2,3,4,5,6,7);

void setup(){
  lcd.begin(16,2);
  
  /* print test message */
  lcd.setCursor(0,0);
  lcd.print("Hello");
  
  lcd.setCursor(0,1);
  lcd.print("world !");
}

void loop(){
  
}
