#include<LiquidCrystal.h>

#define PIN_RED 11
#define PIN_YELLOW 10
#define PIN_GREEN 9
#define PIN_SLCT 12
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
 long curr_time = 0;
 long buff_millis = 0;
 long stop_tune = -1;
 long state[6] = {0,0,0,0,0,0};
 long nextState[6] = {-1,-1,-1,-1,-1,-1};
int hasChanged[6] = {0,0,0,0,0,0};
const int glyphPos[6] = {1,4,7,10,13,15}; 
int score = 0;
 long nextOff = 0;

const byte mus_button[100] = {4,3,5,4,2,0,1,2,3,2,4,3,5,4,3,2,1,2,1,0,4,3,5,4,3,2,3,2,4,3,2,1,2,1,3,2,1,0,3,2,1,0,1,2,0,3,2,0,3,2,0,4,3,1,2,3,4,4,5,3,2,3,4,5,4,3,1,3,2,0,3,2,0,3,2,0,4,3,3,5,4,3,3,4,5,4,3,0,1,1,2,3,2,3,4,5,5,-1};
const int mus_note[100] = {524,494,588,524,392,330,349,392,392,349,588,554,660,588,524,494,440,494,440,392,784,740,880,784,698,660,698,660,784,698,660,588,660,622,698,660,588,554,660,588,524,494,524,588,392,588,524,392,588,524,392,660,588,588,660,698,784,784,880,660,588,660,698,784,698,660,524,660,588,392,588,524,392,588,524,392,660,588,588,988,880,784,784,880,932,880,784,660,698,698,784,831,784,831,932,1048,1048,-1};
const int mus_duration[100] = {1100,220,220,220,1100,220,220,220,440,1100,1100,220,220,220,660,440,440,220,440,1100,1100,220,220,220,440,660,1100,220,220,220,440,660,1100,220,220,220,440,220,220,220,440,660,440,660,440,660,660,440,660,660,440,660,1100,440,440,440,880,220,220,1100,220,220,1100,220,220,440,220,220,660,440,660,660,440,660,660,440,660,1100,440,440,440,880,220,220,660,220,440,660,880,220,220,660,220,440,440,880,880,-1};
const int mus_total_duration[100] = {1100,220,220,220,1100,220,220,220,660,2860,1100,220,220,220,660,440,440,220,660,2860,1100,220,220,220,660,1100,1100,220,220,220,660,1100,1100,220,220,220,660,440,440,220,660,1100,660,660,440,660,660,440,660,660,440,660,1540,440,440,440,1100,220,220,1320,220,220,1320,220,220,440,220,220,660,440,660,660,440,660,660,440,660,1540,440,440,440,1100,220,220,660,220,440,660,1100,220,220,660,220,440,440,1760,1760,-1};




 long current_timestamp=0;
 long next_timestamp = 0;
 long note[6] = {0,0,0,0,0,0};


LiquidCrystal lcd(2, 3, 7, 6, 5, 4);

// custom chars

void setup(){
  lcd.begin(16,2);


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
  pinMode(MODE_BUTTON, INPUT);
  pinMode(PIN_SLCT,INPUT);
 
 init_vars();
   
 //  playKeyboardCatSolo();
 //  endGame();
//  playback(); 
  
}
void init_vars(){
  current_timestamp=0;
  next_timestamp = 0;
  int i; for(i=0;i<6;++i){note[i]=0;};
  state[mus_button[0]] = 1;
  nextState[mus_button[0]] = STATE_PERIOD;
  hasChanged[mus_button[0]] = 1;
  note[mus_button[0]] = 0;
  next_timestamp = mus_total_duration[0];
}

void(* resetFunc) (void) = 0;

void select(){
  while(1){
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("restart");
    lcd.setCursor(0,1);
    lcd.print("play music");
    int choice = 0;
    
    lcd.setCursor(15,choice);
    lcd.print("*");
    
    while(digitalRead(MODE_BUTTON) == LOW){
       if(digitalRead(PIN_SLCT) == HIGH ){
         while(digitalRead(PIN_SLCT) == HIGH){
          ;
         }
         lcd.setCursor(15,choice);
         lcd.print(" ");
         choice = (choice == 0 ? 1 : 0);
         lcd.setCursor(15,choice);
         lcd.print("*"); 
         
       }
    }
    while(digitalRead(MODE_BUTTON)){;}
    if(!choice){
      resetFunc();    
    }else{
     playback(); 
    }
  }
}

void playback(){
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("playing...");
  
  current_timestamp = 0;
  do{
    if(mus_note[current_timestamp +1] == -1){break;}
    playNote(mus_note[current_timestamp],mus_duration[current_timestamp], mus_total_duration[current_timestamp]);
  }while(mus_note[++current_timestamp] != -1);
}

void loop(){
  if(next_timestamp == -1){
   select(); 
  }
  
  if(digitalRead(MODE_BUTTON)){
    noTone(pin_buzzer);
    while(digitalRead(MODE_BUTTON)){;}
    select(); 
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
  unsigned long mil = millis();
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
  return 0;
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
 digitalWrite(PIN_RED, HIGH); 

}

void ok(){
 digitalWrite(PIN_RED, HIGH); 
 digitalWrite(PIN_GREEN, HIGH); 
 digitalWrite(PIN_YELLOW, LOW);
}

void good(){
 digitalWrite(PIN_RED, LOW); 
 digitalWrite(PIN_YELLOW, LOW);
 digitalWrite(PIN_GREEN, HIGH); 
}

void excellent(){
 digitalWrite(PIN_RED, LOW); 
 digitalWrite(PIN_YELLOW, LOW);
 digitalWrite(PIN_GREEN, LOW); 
}

void off(){
   digitalWrite(PIN_RED, LOW); 
 digitalWrite(PIN_YELLOW, LOW);
 digitalWrite(PIN_GREEN, LOW); 
}


void playNote(int freq, int note_duration, int delay_duration){
 
     tone(pin_buzzer,freq,note_duration);
     delay(delay_duration);
 }

