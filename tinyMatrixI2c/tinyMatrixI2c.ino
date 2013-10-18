#define DELAY 3
#define SLAVE_ADDRESS 0x04
#include "TinyWireS.h"

#define DATA_NONE        0x00
#define DATA_FULL_IMAGE  0x01
#define DATA_RED_IMAGE   0x02
#define DATA_GREEN_IMAGE 0x03
#define DATA_POINT       0x04
#define DATA_LOAD        0x05

int latchPin = 3;
int clockPin = 4;
int dataPin = 1;
int nxt= 0;

byte remaining_to_read = 0;
byte current_data_type = 0;

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

int i = 0;
unsigned long current_millisecond = 0;
byte dataReceived;
int receiving = 0;

void setup() {
  //set pins to output so you can control the shift register
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, OUTPUT);
  TinyWireS.begin(SLAVE_ADDRESS);  

}

#define RED 0
#define GREEN 1


uint8_t data_full_current_color = RED;
uint8_t data_current_index = 0;

void loop() {
  
  //reading incoming data
  if(TinyWireS.available()){
    
    if(!remaining_to_read){
      current_data_type = TinyWireS.receive();
      switch(current_data_type){
        case DATA_FULL_IMAGE  : remaining_to_read = 16; break;
        case DATA_RED_IMAGE   : remaining_to_read = 8;  break;
        case DATA_GREEN_IMAGE : remaining_to_read = 8;  break;
        case DATA_POINT       : remaining_to_read = 2;  break;
        case DATA_LOAD        : remaining_to_read = 1;  break;  
      }
      
      data_current_index = 0;
      data_full_current_color = RED;
      
    }else{
      
      byte data_received = TinyWireS.receive();
      
      switch(current_data_type){
        case DATA_FULL_IMAGE  : read_full_data(data_received); break;
        case DATA_RED_IMAGE   : dataRed[data_current_index++] = data_received; break;
        case DATA_GREEN_IMAGE : dataGreen[data_current_index++] = data_received; break;
        case DATA_POINT       : break;
        case DATA_LOAD        : break;
      }
      
      --remaining_to_read;
    }
    
  }
  
  if(current_millisecond < millis()){
    current_millisecond = millis();
    write_matrix_line();
  }
}

void write_matrix_line(){
  if(i == 8){
    i = 0; 
  }
    
  digitalWrite(latchPin, LOW);
  
  shiftOut(dataPin,clockPin,MSBFIRST,(unsigned char)1<<i);
  shiftOut(dataPin,clockPin,MSBFIRST,dataRed[i]);
  shiftOut(dataPin,clockPin,MSBFIRST,dataGreen[i]);
  
  digitalWrite(latchPin, HIGH); 
  ++i;
}

void read_full_data(byte data){
  
  if(data_full_current_color == RED){
    
    dataRed[data_current_index] = data;
    
    data_full_current_color = GREEN; 
    
  }else{
    
    dataGreen[data_current_index] = data;
    
    data_full_current_color = RED;
    
    ++data_current_index;
    
  }
  
}
