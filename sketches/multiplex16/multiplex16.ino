#define PIN_CATHODE1 2
#define PIN_CATHODE2 3
#define PIN_CATHODE3 4
#define PIN_CATHODE4 5

#define PIN_L1 8
#define PIN_L2 9
#define PIN_L3 10
#define PIN_L4 11

void setup(){
 pinMode(PIN_CATHODE1, OUTPUT); 
 pinMode(PIN_CATHODE2, OUTPUT); 
 pinMode(PIN_CATHODE3, OUTPUT); 
 pinMode(PIN_CATHODE4, OUTPUT); 

 pinMode(PIN_L1, OUTPUT);
 pinMode(PIN_L2, OUTPUT);
 pinMode(PIN_L3, OUTPUT);
 pinMode(PIN_L4, OUTPUT);



}

int del = 1;
  byte screen[4] = {
  0b0110,
  0b1001,
  0b1111,
  0b1001 
  };
void loop(){
  

  int i;
  for(i = 2;i<6;++i){
    digitalWrite(i, HIGH);
    digitalWrite(PIN_L1, ((screen[5-i] & 1) == 1) ? HIGH : LOW);
    digitalWrite(PIN_L2, ((screen[5-i] & 2) == 2) ? HIGH : LOW);
    digitalWrite(PIN_L3, ((screen[5-i] & 4) == 4) ? HIGH : LOW);
    digitalWrite(PIN_L4, ((screen[5-i] & 8) == 8) ? HIGH : LOW);
    delay(del);
    digitalWrite(PIN_L1, LOW);
    digitalWrite(PIN_L2, LOW);
    digitalWrite(PIN_L3, LOW);
    digitalWrite(PIN_L4, LOW);

    digitalWrite(i,LOW);
  }
  
}

