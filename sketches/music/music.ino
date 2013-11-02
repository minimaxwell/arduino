
// buzzer pin 8

const int octave[7] = {262, 294, 330, 349, 392, 440, 494};
const int duration = 50;

void setup(){
  
}


void loop(){
 

  
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
