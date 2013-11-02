#include<LiquidCrystal.h>

#define PIN_RED 9
#define PIN_YELLOW 10
#define PIN_GREEN 11
#define PIN_BLUE 12
#define DURATION 120
#define MODE_BUTTON 13

#define CDE A0
#define FGA A1

#define STATE_PERIOD 50
#define LED_TIME 100
// buzzer pin 8

const int octave[7] = {262, 294, 330, 349, 392, 440, 494};
const int octaveGrave[7] = {131, 147, 165, 175, 196, 220, 247 };
const int pin_buzzer = 8;
int read1 = 0;
int read2 = 0 ;
int prev = -1;
int curr_time = 0;
int buff_millis = 0;
int stop_tune = -1;
int state[6] = {0,0,0,0,0,0};
int nextState[6] = {-1,-1,-1,-1,-1,-1};
int hasChanged[6] = {0,0,0,0,0,0};
const int glyphPos[6] = {1,4,7,10,13,15}; 
int score = 0;
int nextOff = 0;

const byte mus_button[64] = {3,4,5,4,3,4,5,4,2,3,4,3,2,3,4,3,1,2,3,2,1,2,3,2,0,1,0,1,0-1};
const int mus_note[64] = {262,330,392,330,262,330,392,330,220,262,330,262,220,262,330,262,196,247,294,247,196,247,294,247,196,196,196,196,196,-1};
const int mus_duration[64] = {200,200,200,200,200,100,400,200,   200,200,200,200,200,100,400,200, 200,200,200,200,200,100,400,200, 400,400,400,400, 200,-1};
const int mus_total_duration[64] = {400,400,400,400,400,200,600,400,  400,400,400,400,400,200,600,400, 400,400,400,400,400,200,600,400, 600,600,600,600, 400  -1};




int current_timestamp=0;
int next_timestamp = 0;
int note[6] = {0,0,0,0,0,0};


LiquidCrystal lcd(2, 3, 7, 6, 5, 4);

// custom chars

byte char1[8] = {
  B01110,
  B01110,
  B01110,
  B00000,
  B00000,
  B00000,
  B00000,
  B00000
};

byte char2[8] = {
  B00000,
  B01110,
  B01110,
  B01110,
  B00000,
  B00000,
  B00000,
  B00000
};

byte char3[8] = {
  B00000,
  B00000,
  B01110,
  B01110,
  B01110,
  B00000,
  B00000,
  B00000
};

byte char4[8] = {
  B00000,
  B00000,
  B00000,
  B01110,
  B01110,
  B01110,
  B00000,
  B00000
};

byte char5[8] = {
  B00000,
  B00000,
  B00000,
  B00000,
  B01110,
  B01110,
  B01110,
  B00000
};

byte char6[8] = {
  B00000,
  B00000,
  B00000,
  B00000,
  B00000,
  B01110,
  B01110,
  B01110
};

byte char7[8] = {
  B00000,
  B00000,
  B00000,
  B00000,
  B00000,
  B11111,
  B10001,
  B10101
};

void setup(){
  lcd.begin(16,2);

  lcd.createChar(0,char1);
  lcd.createChar(1,char2);
  lcd.createChar(2,char3);
  lcd.createChar(3,char4);
  lcd.createChar(4,char5);
  lcd.createChar(5,char6);
  lcd.createChar(6,char7);
  
  pinMode(PIN_RED, OUTPUT);
  pinMode(PIN_YELLOW, OUTPUT);
  pinMode(PIN_GREEN, OUTPUT);
  pinMode(PIN_BLUE, OUTPUT);
  pinMode(MODE_BUTTON, INPUT);
 
 
  //state[mus_button[0]] = 1;
  //nextState[mus_button[0]] = STATE_PERIOD;
  //hasChanged[mus_button[0]] = 1;
  //note[mus_button[0]] = 0;
  //next_timestamp = mus_total_duration[0];
   
   playKeyboardCatSolo();
   endGame();
//  playback(); 
  
}

void playback(){
      state[mus_button[0]] = 1;
  while(curr_time < 10000){
    buff_millis = millis();
    if(buff_millis > curr_time){
      curr_time = buff_millis;
      refreshGlyph();
    }
    
  }
}

void endGame(){
  noTone(pin_buzzer);
  bad();
  while(1){
    ; 
  }
}

void loop(){
  if(next_timestamp == -1){
   endGame(); 
  }
  
  
  buff_millis = millis();

  if(buff_millis > curr_time){
    curr_time = buff_millis;
    
    if((stop_tune != -1) && (stop_tune <= curr_time)){
      noTone(pin_buzzer); 
    }
    
    if(nextOff <= curr_time){
     off(); 
    }
    
    
    hero();

    
    refreshGlyph();
    
    
  }

}

void displayScore(int sc){
  if(score == -1){
   bad(); 
  }else if (score <= 1){
    excellent();
  }else if (score <= 5){
     good(); 
  }else if(score <= 10){
     ok(); 
  }
  
  nextOff = curr_time + LED_TIME;
}

void hero(){
 int i; 
 
 for(i=0;i<6;++i){
   
   if(state[i] == 11){
     state[i] = 12;
     score = beAHero(i);
     displayScore(score);
     hasChanged[i] = 1;
   }else{
     if(nextState[i] <= curr_time){
       hasChanged[i] = 1;
       if(state[i] == 12){
         state[i] = 0;
         hasChanged[i] = 0;
       }else if(state[i] == 0){
         ;
       }else{
         state[i] += 1;
       }
         nextState[i] = nextState[i] + STATE_PERIOD;
       
     }
   }
 }
  
  if(curr_time > next_timestamp){
    current_timestamp++;
    note[mus_button[current_timestamp]] = current_timestamp;
    state[mus_button[current_timestamp]] = 1;
    nextState[mus_button[current_timestamp]] = curr_time + STATE_PERIOD;
    hasChanged[mus_button[current_timestamp]] = 1;
    
    if(mus_note[current_timestamp + 1] == -1){
      next_timestamp = -1;
    }else{
      next_timestamp = curr_time + mus_total_duration[current_timestamp]; 
    }

  }
  
}

int beAHero(int butt){
  int isplaying=0;
  int score = -1;
  int mil = millis();
  while(millis() <= (mil + 10)){
    if(!isplaying && isPlayed(mus_button[note[butt]])){
      score = 10-(millis() - mil);
      tone(pin_buzzer,mus_note[note[butt]]);
      isplaying=1;
    }
  }
  if(!isplaying){
     tone(pin_buzzer,mus_note[note[butt]]);
  }
  stop_tune = mil + mus_duration[note[butt]];
  while(millis() < mil + 20){
    if(!isplaying && isPlayed(mus_button[note[butt]])){
      score = 21-(millis() - mil);
      tone(pin_buzzer,mus_note[note[butt]]);
      isplaying=1;
    }
  }
  
  if(!isplaying){
    noTone(pin_buzzer);
  }
  
  return score;
}

void refreshGlyph(){
   int i; 
  
  for(i = 0; i < 6; ++i){
    refreshGlyph(i);
  }
}

void refreshGlyph(int glyph){
  if(hasChanged[glyph]){
    if(state[glyph] < 7){
      lcd.setCursor(glyphPos[glyph],0);
    }else{
      lcd.setCursor(glyphPos[glyph],1);
    }
    
    switch(state[glyph]){
      case 0 : lcd.setCursor(glyphPos[glyph], 1); lcd.write(' '); break;
      case 1 : lcd.write(byte(0)); break;
      case 2 : lcd.write(byte(1)); break;
      case 3 : lcd.write(byte(2)); break;
      case 4 : lcd.write(byte(3)); break;
      case 5 : lcd.write(byte(4)); break;
      case 6 : lcd.write(' '); lcd.setCursor(glyphPos[glyph],1); lcd.write(byte(0)); break;
      case 7 : lcd.write(byte(1)); break;
      case 8 : lcd.write(byte(2)); break;
      case 9 : lcd.write(byte(3)); break;
      case 10 : lcd.write(byte(4)); break;
      case 11 : lcd.write(byte(6)); break;
      case 12 : lcd.write(byte(6)); break;
    }
    hasChanged[glyph] = 0;
  }
}

int isPlayed(byte touch){
  switch(touch){
    case 0 : return C(); 
    case 1 : return D(); 
    case 2 : return E(); 
    case 3 : return Fa(); 
    case 4 : return G(); 
    case 5 : return A(); 
  }
}

int C(){
  if(analogRead(FGA < 10)){
    read1 = analogRead(CDE); 
    if(read1 > 900){
     return 1; 
    }else{
     return 0; 
    }
  }else{
   return 0; 
  }
}

int D(){
  if(analogRead(FGA < 10)){
    read1 = analogRead(CDE); 
    if(read1 < 900 && read1 > 400){
     return 1; 
    }else{
     return 0; 
    }
  }else{
   return 0; 
  }  
}

int E(){
    if(analogRead(FGA < 10)){
    read1 = analogRead(CDE); 
    if(read1 < 400 && read1 > 300){
     return 1; 
    }else{
     return 0; 
    }
  }else{
   return 0; 
  }
}

int Fa(){
    if(analogRead(CDE) < 10){
    read2 = analogRead(FGA); 
    if(read2 > 900){
     return 1; 
    }else{
     return 0; 
    }
  }else{
   return 0; 
  }
}

int G(){
    if(analogRead(CDE) < 10){
    read2 = analogRead(FGA); 
    if(read2 < 900 && read2 > 400 ){
     return 1; 
    }else{
     return 0; 
    }
  }else{
   return 0; 
  }
}

int A(){
    if(analogRead(CDE) < 10){
    read2 = analogRead(FGA); 
    if(read2 < 400 && read2 > 300){
     return 1; 
    }else{
     return 0; 
    }
  }else{
   return 0; 
  }
}


void bad(){
 digitalWrite(PIN_YELLOW, LOW);
 digitalWrite(PIN_GREEN, LOW); 
 digitalWrite(PIN_BLUE, LOW); 
 digitalWrite(PIN_RED, HIGH); 

}

void ok(){
 digitalWrite(PIN_RED, LOW); 
 digitalWrite(PIN_GREEN, LOW); 
 digitalWrite(PIN_BLUE, LOW); 
 digitalWrite(PIN_YELLOW, HIGH);
}

void good(){
 digitalWrite(PIN_RED, LOW); 
 digitalWrite(PIN_YELLOW, LOW);
 digitalWrite(PIN_BLUE, LOW); 
 digitalWrite(PIN_GREEN, HIGH); 
}

void excellent(){
 digitalWrite(PIN_RED, LOW); 
 digitalWrite(PIN_YELLOW, LOW);
 digitalWrite(PIN_GREEN, LOW); 
 digitalWrite(PIN_BLUE, HIGH); 
}

void off(){
   digitalWrite(PIN_RED, LOW); 
 digitalWrite(PIN_YELLOW, LOW);
 digitalWrite(PIN_GREEN, LOW); 
 digitalWrite(PIN_BLUE, LOW);
}


void playKeyboardCatSolo(){
  
  playNote(octave[0] * 2, DURATION * 10, DURATION * 10 );
  playNote(octave[6] , DURATION *2, DURATION * 2 );
  playNote(octave[1] * 2, DURATION *2, DURATION * 2 );
  playNote(octave[0] * 2, DURATION *2, DURATION * 2 );
  
  playNote(octave[4] , DURATION * 10, DURATION * 10 );
  playNote(octave[2] , DURATION * 2, DURATION * 2 );
  playNote(octave[3] , DURATION * 2, DURATION * 2 );
  playNote(octave[4] , DURATION * 2, DURATION * 2 );
  
  
  playNote(octave[4] , DURATION * 4, DURATION * 6 );
  playNote(octave[3] , DURATION * 10, DURATION * 26 );
  
  
  playNote(octave[1] * 2, DURATION * 10, DURATION * 10 );
  playNote(554, DURATION * 2, DURATION * 2 );
  playNote(octave[2] * 2, DURATION * 2, DURATION * 2 );
  playNote(octave[1] * 2, DURATION * 2, DURATION * 2 );
  
  playNote(octave[0] * 2, DURATION * 6, DURATION * 6 );
  playNote(octave[6] , DURATION * 4, DURATION * 4 );
  playNote(octave[5] , DURATION * 4, DURATION * 4 );
  playNote(octave[6] , DURATION * 2, DURATION * 2 );
  
  playNote(octave[5] , DURATION * 4, DURATION * 6 );
  playNote(octave[4] , DURATION * 10, DURATION * 26 );


  playNote(octave[4] * 2, DURATION * 10, DURATION * 10 );
  playNote(740, DURATION * 2, DURATION * 2 );
  playNote(octave[5] * 2, DURATION * 2, DURATION * 2 );
  playNote(octave[4] * 2, DURATION * 2, DURATION * 2 );
  
  playNote(octave[3] * 2, DURATION * 4, DURATION * 6 );
  playNote(octave[2] * 2, DURATION * 6, DURATION * 10 );

  playNote(octave[3] * 2, DURATION * 10, DURATION * 10 );
  playNote(octave[2] * 2, DURATION * 2, DURATION * 2 );
  playNote(octave[4] * 2, DURATION * 2, DURATION * 2 );
  playNote(octave[3] * 2, DURATION * 2, DURATION * 2 );
  
  playNote(octave[2] * 2, DURATION * 4, DURATION * 6 );
  playNote(octave[1] * 2, DURATION * 6, DURATION * 10 );

  playNote(octave[2] * 2, DURATION * 10, DURATION * 10 );
  playNote(622, DURATION * 2, DURATION * 2 );
  playNote(octave[3] * 2, DURATION * 2, DURATION * 2 );
  playNote(octave[2] * 2, DURATION * 2, DURATION * 2 );
  
  playNote(octave[1] * 2, DURATION * 4, DURATION * 6 );
  playNote(554, DURATION * 2, DURATION * 4 );
  playNote(octave[2] * 2, DURATION * 2, DURATION * 4 ); 
  playNote(octave[1] * 2, DURATION * 2, DURATION * 2 );
  
  playNote(octave[0] * 2, DURATION * 4, DURATION * 6 );
  playNote(octave[6] , DURATION * 6, DURATION * 10 );
      
  playNote(octave[0] * 2, DURATION *4, DURATION * 6 );
  playNote(octave[1] * 2, DURATION * 6, DURATION * 6 );
  playNote(octave[4] , DURATION * 4, DURATION * 4 );
  
  playNote(octave[1] * 2, DURATION * 6, DURATION * 6 );
  playNote(octave[0] * 2, DURATION * 6, DURATION * 6 );
  playNote(octave[4] , DURATION * 4, DURATION * 4 );
  
  playNote(octave[1] * 2, DURATION * 6, DURATION * 6 );
  playNote(octave[0] * 2, DURATION * 6, DURATION * 6 );
  playNote(octave[4] , DURATION * 4, DURATION * 4 );
  
  playNote(octave[2] * 2, DURATION * 6, DURATION * 6 );
  playNote(octave[1] * 2, DURATION * 10, DURATION * 14 );
  
  playNote(octave[1] * 2, DURATION * 4, DURATION * 4 );
  playNote(octave[2] * 2, DURATION * 4, DURATION * 4 );
  playNote(octave[3] *2 , DURATION * 4, DURATION * 4 );
  
  playNote(octave[4] * 2, DURATION * 8, DURATION * 10 );
  playNote(octave[4] * 2, DURATION * 2, DURATION * 2 );
  playNote(octave[5] *2 , DURATION * 2, DURATION * 2 );
  
  
  playNote(octave[2] *2, DURATION * 10, DURATION * 12 );
  playNote(octave[1] *2, DURATION * 2, DURATION * 2 );

  playNote(octave[2] *2, DURATION * 2, DURATION * 2 ); 
  playNote(octave[3] *2, DURATION * 10, DURATION * 12 );
  playNote(octave[4] *2, DURATION * 2, DURATION * 2 ); 
  playNote(octave[3] *2, DURATION * 2, DURATION * 2 ); 
  
  playNote(octave[2] *2, DURATION * 4, DURATION * 4 ); 
  playNote(octave[0] *2, DURATION * 2, DURATION * 2 ); 
  playNote(octave[2] *2, DURATION * 2, DURATION * 2 ); 
  playNote(octave[1] *2, DURATION * 6, DURATION * 6); 
  playNote(octave[4] , DURATION * 4, DURATION * 4 );
 
  playNote(octave[1] * 2, DURATION * 6, DURATION * 6 );
  playNote(octave[0] * 2, DURATION * 6, DURATION * 6 );
  playNote(octave[4] , DURATION * 4, DURATION * 4 );
  
  playNote(octave[1] * 2, DURATION * 6, DURATION * 6 );
  playNote(octave[0] * 2, DURATION * 6, DURATION * 6 );
  playNote(octave[4] , DURATION * 4, DURATION * 4 );
  
  playNote(octave[2] * 2, DURATION * 6, DURATION * 6 );
  playNote(octave[1] * 2, DURATION * 10, DURATION * 14 );

  playNote(octave[1] * 2, DURATION * 4, DURATION * 4 );
  playNote(octave[6] * 2, DURATION * 4, DURATION * 4 );
  playNote(octave[5] *2 , DURATION * 4, DURATION * 4 );
  
  playNote(octave[4] *2 , DURATION * 8, DURATION * 10);
  playNote(octave[4] *2 , DURATION * 2, DURATION * 2);
  playNote(octave[5] *2 , DURATION * 2, DURATION * 2);
  playNote(932 , DURATION * 6, DURATION * 6);
  playNote(octave[5] *2 , DURATION * 2, DURATION * 2);
  playNote(octave[4] *2 , DURATION * 4, DURATION * 4);
  playNote(octave[2] *2 , DURATION * 6, DURATION * 6);

  playNote(octave[3] *2 , DURATION * 8, DURATION * 10);
  playNote(octave[3] *2 , DURATION * 2, DURATION * 2);
  playNote(octave[4] *2 , DURATION * 2, DURATION * 2);
  playNote(831 , DURATION * 6, DURATION * 6);
  playNote(octave[4] *2 , DURATION * 2, DURATION * 2);
  playNote(831 , DURATION * 4, DURATION * 4);
  playNote(932 , DURATION * 4, DURATION * 4);
  
  playNote(octave[0] * 4, DURATION * 8, DURATION * 16);

 
}

void playNote(int freq, int note_duration, int delay_duration){
 
     tone(pin_buzzer,freq,note_duration);
     delay(delay_duration);
 }
 
 void anim(){
  lcd.clear();
  lcd.setCursor(0,0);
  
  lcd.write(byte(0));
  delay(50);
  lcd.setCursor(0,0);
  
  lcd.write(byte(1));
  delay(50);
  lcd.setCursor(0,0);
  lcd.write(byte(2));
  delay(50);
  lcd.setCursor(0,0);
  lcd.write(byte(3));
  delay(50);
  lcd.setCursor(0,0);
  lcd.write(byte(4));
  delay(50);
  lcd.setCursor(0,0);
  lcd.write(byte(5));
  delay(50);
  lcd.setCursor(0,0);
  
  
  lcd.write(' ');
  lcd.setCursor(0,1);
    lcd.write(byte(0));
delay(50);
  lcd.setCursor(0,1);
  lcd.write(byte(1));
  delay(50);
  lcd.setCursor(0,1);
  lcd.write(byte(2));
  delay(50);
  lcd.setCursor(0,1);
  lcd.write(byte(3));
  delay(50);
  lcd.setCursor(0,1);
  lcd.write(byte(4));
  delay(50);
  lcd.setCursor(0,1);
  lcd.write(byte(5));
  delay(50);
  lcd.setCursor(0,1);
  
  
  lcd.write(byte(6));
 
   
 }
