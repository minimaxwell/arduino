#include <Wire.h>

void setup(){
  Wire.begin();
  pinMode(2,INPUT);
}

byte dataRed1[8] = {
  0b10001000,
  0b00000001,
  0b00000000,
  0b00011000,
  0b10011000,
  0b00000001,
  0b00000000,
  0b01000100
};

byte dataRed2[8] = {
  0b01000100,
  0b00000000,
  0b00111101,
  0b10100100,
  0b00100100,
  0b00111100,
  0b00000001,
  0b10001000
};

byte dataRed3[8] = {
  0b00100010,
  0b01111110,
  0b11000010,
  0b01000011,
  0b01000010,
  0b01000010,
  0b11111110,
  0b00010001
};

byte dataRed4[8] = {
  0b00010001,
  0b10000000,
  0b00111100,
  0b00100100,
  0b00100101,
  0b10111100,
  0b00000000,
  0b00100010
};

byte dataGreen1[8] = {
  0b01110111,
  0b11111110,
  0b11111111,
  0b11100111,
  0b01100111,
  0b11111110,
  0b11111111,
  0b10111011
};


byte dataGreen2[8] = {
  0b10111011,
  0b11111111,
  0b11000010,
  0b01011011,
  0b11011011,
  0b11000011,
  0b11111110,
  0b01110111
};

byte dataGreen3[8] = {
  0b11011101,
  0b10000001,
  0b00111101,
  0b10111100,
  0b10111101,
  0b10111101,
  0b00000001,
  0b11101110
};

byte dataGreen4[8] = {
  0b11101110,
  0b01111111,
  0b11000011,
  0b11011011,
  0b11011010,
  0b01000011,
  0b11111111,
  0b11011101
};



byte* animRed[4] = {dataRed1, dataRed2, dataRed3, dataRed4};
byte* animGreen[4] = {dataGreen1, dataGreen2, dataGreen3, dataGreen4};

int current_delay = 1000;
  int i;
  int curr_frame = 0;

void loop(){

  if(digitalRead(2) == LOW){
    while(digitalRead(2) == LOW){;} // wait for button release
    current_delay -= 15;
    if(current_delay < 0){
      current_delay = 0;
    }
  }
  

  for(i = 0; i < 8; ++i){
    Wire.beginTransmission(0x04);
    Wire.write(animRed[curr_frame][i]);
    Wire.write(animGreen[curr_frame][i]);
    Wire.endTransmission();
    delay(1);
  }
  ++curr_frame;
  if(curr_frame == 4){
   curr_frame = 0; 
  }
  delay(current_delay);

}

