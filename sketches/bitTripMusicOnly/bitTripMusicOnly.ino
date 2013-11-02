#define toggleFreq 40
// buzzer pin 8

const int octave[7] = {262, 294, 330, 349, 392, 440, 494};
const int duration = 100;
const int pin_buzzer = 8;
const int led1 = 12;
const int led2 = 13;
const int led3 = 2;
const int led4 = 4;
const int led5 = 5;
const int led6 = 6;
const int led7 = 7;
int toggle;
int i;
const int ledInd1 = 9;
const int ledInd2 = 10;

void setup(){
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
  pinMode(led3, OUTPUT);
  pinMode(led4, OUTPUT);
  pinMode(led5, OUTPUT);
  pinMode(led6, OUTPUT);
  pinMode(led7, OUTPUT);
  pinMode(ledInd1, OUTPUT);
  pinMode(ledInd2, OUTPUT);
  
}


void loop(){
  
  
  playNote(octave[4]  * 2, duration, duration * 2, led6);
  playNote(octave[0]  * 2, duration, duration * 2, led3);
  playNote(octave[3]  * 2, duration, duration * 2, led5);
  playNote(octave[2]  * 2, duration, duration * 2, led4);
  
  delay(duration * 24);
  
  playNote(octave[4] , duration, duration * 4, led1);
  playNote(octave[0]  * 2, duration, duration * 2, led3);
  playNote(octave[4] , duration, duration * 2, led1);
  playNote(octave[3]  * 2, duration, duration * 2, led5);
  playNote(octave[2]  * 2, duration, duration * 2, led4);
  playNote(octave[0]  * 2, duration, duration * 2, led3);

  delay(duration * 18);
  
  playNote(octave[0]  * 4, duration, duration * 6, led7);
  playNote(octave[4]  * 2, duration, duration * 6, led6);
  playNote(octave[3]  * 2, duration, duration * 4, led5);
  playNote(octave[2]  * 2, duration, duration * 4, led4);
  playNote(octave[0]  * 2, duration, duration * 4, led3);

  delay(duration * 8);
  
  playNote(octave[4] , duration, duration * 2, led1);
  playNote(octave[6] , duration, duration * 2, led2);
  playNote(octave[0]  * 2, duration, duration * 2, led3);
  playNote(octave[1]  * 2, duration, duration * 2, led4);
  playNote(octave[0]  * 2, duration, duration * 2, led3);

  delay(duration * 24);


}

void playNote(int freq, int note_duration, int delay_duration, int led){
 toggle = note_duration / (toggleFreq << 1); 

 for(i = 0; i < toggle; ++i ){
   tone(pin_buzzer, freq, toggleFreq);
   delay(toggleFreq);
   tone(pin_buzzer, freq<<1, toggleFreq);
   delay(toggleFreq);
 }
 
 digitalWrite(led, HIGH);
 

 delay(delay_duration - (toggle * (toggleFreq << 1)));

 digitalWrite(led, LOW);
  
}
