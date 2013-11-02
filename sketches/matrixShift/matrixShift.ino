int latchPin = 9;
int clockPin = 8;
int dataPin = 10;


void setup() {
  //set pins to output so you can control the shift register
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, OUTPUT);
}

void loop() {
    int i,j;
    
    for(i = 0; i < 8; ++i){
        for(j = 0; j < 8; ++j){
            digitalWrite(latchPin, LOW);
            shiftOut(dataPin,clockPin,MSBFIRST,(unsigned char)1<<i);
            shiftOut(dataPin,clockPin,MSBFIRST,(unsigned char)0);
            shiftOut(dataPin,clockPin,MSBFIRST,(unsigned char)1<<j);
            digitalWrite(latchPin, HIGH);
            delay(100);
        }
    }
    for(i = 0; i < 8; ++i){
        for(j = 0; j < 8; ++j){
            digitalWrite(latchPin, LOW);
            shiftOut(dataPin,clockPin,MSBFIRST,(unsigned char)1<<i);
            shiftOut(dataPin,clockPin,MSBFIRST,(unsigned char)1<<j);
            shiftOut(dataPin,clockPin,MSBFIRST,(unsigned char)0);
            digitalWrite(latchPin, HIGH);
            delay(100);
        }
    }
    for(i = 0; i < 8; ++i){
        for(j = 0; j < 8; ++j){
            digitalWrite(latchPin, LOW);
            shiftOut(dataPin,clockPin,MSBFIRST,(unsigned char)1<<i);
            shiftOut(dataPin,clockPin,MSBFIRST,(unsigned char)1<<j);
            shiftOut(dataPin,clockPin,MSBFIRST,(unsigned char)1<<j);
            digitalWrite(latchPin, HIGH);
            delay(100);
        }
    }
}

