int latchPin = 8;
int clockPin = 12;
int dataPin = 11;

const unsigned char k2000[14] = {1, 2, 4, 8, 16, 32, 64, 128, 64, 32, 16, 8, 4, 2};

void setup() {
  //set pins to output so you can control the shift register
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, OUTPUT);
}

void loop() {
    int i;
    for(i=0;i<14;++i){
    // take the latchPin low so 
    // the LEDs don't change while you're sending in bits:
    digitalWrite(latchPin, LOW);
    // shift out the bits:
    shiftOut(dataPin, clockPin, MSBFIRST, k2000[i]);  

    //take the latch pin high so the LEDs will light up:
    digitalWrite(latchPin, HIGH);
    // pause before next value:
    delay(100);
    }
}

