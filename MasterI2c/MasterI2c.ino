#include <Wire.h>

void setup(){
  Wire.begin();
 pinMode(2,INPUT); 
 pinMode(7,OUTPUT);
}

byte dataRed1[8] = {
  0b10101010,
  0b10101010,
  0b10101010,
  0b10101010,
  0b10101010,
  0b10101010,
  0b10101010,
  0b10101010
};

byte dataGreen1[8] = {
  0b01010101,
  0b01010101,
  0b01010101,
  0b01010101,
  0b01010101,
  0b01010101,
  0b01010101,
  0b01010101
};

byte dataRed2[8] = {
  0b00011000, 
  0b01100110, 
  0b01000010, 
  0b10011001, 
  0b10011001, 
  0b01000010, 
  0b01100110, 
  0b00011000
};

byte dataGreen2[8] = {
  0b11100111, 
  0b10011001, 
  0b10111101, 
  0b01111110, 
  0b01111110, 
  0b10111101, 
  0b10011001, 
  0b11100111
};

void loop(){
   digitalWrite(7,LOW);
  while(digitalRead(2) == HIGH){;}
  while(digitalRead(2) == LOW){;}
 // delay(10000);
 
  digitalWrite(7,HIGH);
  int i;
  for(i = 0; i < 8; ++i){
    Wire.beginTransmission(0x04);
    Wire.write(dataRed1[i]);
    Wire.write(dataGreen1[i]);
    Wire.endTransmission();
    delay(1);
  }
  
     digitalWrite(7,LOW);
  while(digitalRead(2) == HIGH){;}
  while(digitalRead(2) == LOW){;}
 // delay(10000);
 
  digitalWrite(7,HIGH);
  for(i = 0; i < 8; ++i){
    Wire.beginTransmission(0x04);
    Wire.write(dataRed2[i]);
    Wire.write(dataGreen2[i]);
    Wire.endTransmission();
    delay(1);
  }
}
