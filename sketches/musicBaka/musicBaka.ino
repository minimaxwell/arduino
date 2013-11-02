#include<LiquidCrystal.h>
LiquidCrystal lcd(13,12,6,5,4,2);

// buzzer pin 8



const int octave[7] = {262, 294, 330, 349, 392, 440, 494};
const int duration = 50;
int msg = 0;

void setup(){
  lcd.begin(16,2);
  lcd.print("baka ! ! !");
}


void loop(){
  
  lcd.setCursor(0, 0);
  lcd.clear();
  lcd.print("baka ! ! !");
  lcd.setCursor(0, 1);
  
  msg = random(4);
  switch(msg){
    case 0 : lcd.print(":3"); break;
    case 1 : lcd.print("o_O"); break;
    case 2 : lcd.print("*_*"); break;
    case 3 : lcd.print(":D"); break;    
  }

  
  tone(8, octave[4] * 4,duration);
  delay(duration * 2);
  tone(8, octave[1] * 4,duration);
  delay(duration * 2);
  tone(8, octave[1] * 4, duration);
  delay(duration * 4);
  tone(8, octave[1] * 4, duration);
  delay(duration * 4);
  tone(8, octave[0] * 4, duration);
  delay(duration * 4);
  tone(8, octave[6] * 2, duration);
  delay(duration * 2);
  tone(8, octave[0] * 4, duration);
  delay(duration * 2);
  tone(8, octave[1] * 4, duration);
  delay(duration * 4);
  tone(8, octave[1] * 4, duration);
  delay(duration * 4);
  tone(8, octave[4] * 2, duration);
  delay(duration * 4);
}
