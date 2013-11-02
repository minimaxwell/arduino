#include <Wire.h>

#define DATA_NONE        0x00
#define DATA_FULL_IMAGE  0x01
#define DATA_RED_IMAGE   0x02
#define DATA_GREEN_IMAGE 0x03
#define DATA_POINT       0x04
#define DATA_LOAD        0x05


void setup(){
  Wire.begin();
  pinMode(2,INPUT);
  pinMode(3,INPUT);
  pinMode(4,INPUT);
  pinMode(5,INPUT);
  pinMode(6,INPUT);

}

byte splashRed[8] = {
  0b00011000,
  0b00011000,
  0b00000000,
  0b00011000,
  0b00100100,
  0b01000010,
  0b10000001,
  0b11100111
};

byte splashGreen[8] = {
  0b11100111,
  0b11100111,
  0b11111111,
  0b11100111,
  0b11011011,
  0b10111101,
  0b01111110,
  0b00011000
};

byte cross[8] = {
  0b11000011,
  0b11100111,
  0b01111110,
  0b00111100,
  0b00111100,
  0b01111110,
  0b11100111,
  0b11000011
};

byte circle[8] = {
  0b00011000,
  0b00111100,
  0b01100110,
  0b01100110,
  0b01100110,
  0b01100110,
  0b00111100,
  0b00011000 
};

byte empty[8] = {
  0x00,
  0x00, 
  0x00, 
  0x00, 
  0x00, 
  0x00, 
  0x00, 
  0x00 
};

byte full[8] = {
  0xFF,
  0xFF,
  0xFF,
  0xFF,
  0xFF,
  0xFF,
  0xFF,
  0xFF
};

int i;

uint8_t red_status = 0;
uint8_t green_status = 0;
uint8_t circle_color = 0;
uint8_t cross_color = 0;

void loop(){

  if(digitalRead(2) == LOW){
    while(digitalRead(2) == LOW){;}
    transmitSplash(); 
  }
 
  if(digitalRead(3) == LOW){
    while(digitalRead(3) == LOW){;}
    if(circle_color){
      transmitColorData(circle,DATA_RED_IMAGE);
      circle_color = 0;
    }else{
      transmitColorData(circle,DATA_GREEN_IMAGE);
      circle_color = 1;
    }
  }
  
  if(digitalRead(4) == LOW){
    while(digitalRead(4) == LOW){;}
    if(cross_color){
      transmitColorData(cross,DATA_RED_IMAGE);
      cross_color = 0;
    }else{
      transmitColorData(cross,DATA_GREEN_IMAGE);
      cross_color = 1;
    }
  }
  
  if(digitalRead(5) == LOW){
    while(digitalRead(5) == LOW){;}
    if(red_status){
      transmitColorData(full,DATA_RED_IMAGE);
      red_status = 0;
    }else{
      transmitColorData(empty,DATA_RED_IMAGE);
      red_status = 1;
    }
  }
  
  if(digitalRead(6) == LOW){
    while(digitalRead(6) == LOW){;}
    if(green_status){
      transmitColorData(full,DATA_GREEN_IMAGE);
      green_status = 0;
    }else{
      transmitColorData(empty,DATA_GREEN_IMAGE);
      green_status = 1;
    }
  }
  
}

void transmitSplash(){
  Wire.beginTransmission(0x04);
  Wire.write(DATA_FULL_IMAGE);
  Wire.endTransmission();
  delay(1);
  
  for(i=0;i<8;++i){
    Wire.beginTransmission(0x04);
    Wire.write(splashRed[i]);
    Wire.write(splashGreen[i]);
    Wire.endTransmission();
    delay(1);
  }
}

void transmitColorData(const byte *data, uint8_t code){
  Wire.beginTransmission(0x04);
  Wire.write(code);
  Wire.endTransmission();
  delay(1);
  
  for(i = 0; i< 8; ++i){
    Wire.beginTransmission(0x04);
    Wire.write(data[i]);
    Wire.endTransmission();
    delay(1);
  }
}

