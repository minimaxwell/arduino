int latchPin = 9;
int clockPin = 8;
int dataPin = 10;

  byte dataRed[8] = {
    0b11111111,
    0b10001000,
    0b10101011,
    0b10001011,
    0b10101011,
    0b10101000,
    0b11111111,
    0b11111111
  };
  
  byte dataGreen[8] = {
    0b00000000,
    0b01110111,
    0b01010100,
    0b01110100,
    0b01010100,
    0b01010111,
    0b00000000,
    0b00000000 
  };

void setup() {
  //set pins to output so you can control the shift register
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, OUTPUT);

}

void loop() {
  int i;
      for(i = 0; i < 8; ++i){
            digitalWrite(latchPin, LOW);
            shiftOut(dataPin,clockPin,MSBFIRST,(unsigned char)1<<i);
            shiftOut(dataPin,clockPin,MSBFIRST,dataRed[i]);
            shiftOut(dataPin,clockPin,MSBFIRST,dataGreen[i]);
            digitalWrite(latchPin, HIGH);
            delay(1);
      }
}
