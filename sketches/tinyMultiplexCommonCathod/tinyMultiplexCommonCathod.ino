#define PIN_CATHODE1 3
#define PIN_CATHODE2 4
#define PIN_L1 2
#define PIN_L2 1
#define PIN_L3 0

// /!\ Pas plus d'une led allumée à la fois !!!!!!!!!!!

void setup(){
 pinMode(PIN_CATHODE1, OUTPUT); 
 pinMode(PIN_CATHODE2, OUTPUT); 
 pinMode(PIN_L1, OUTPUT); 
 pinMode(PIN_L2, OUTPUT); 
 pinMode(PIN_L3, OUTPUT); 
}

void loop(){
 
  digitalWrite(PIN_CATHODE2, LOW);
  digitalWrite(PIN_CATHODE1, HIGH);
  
  digitalWrite(PIN_L3, LOW);
  digitalWrite(PIN_L1, HIGH);
  delay(500);
  
  digitalWrite(PIN_L1, LOW);
  digitalWrite(PIN_L2, HIGH);
  delay(500);
  
  digitalWrite(PIN_L2, LOW);
  digitalWrite(PIN_L3, HIGH);
  delay(500);
  
  
  digitalWrite(PIN_CATHODE1, LOW);
  digitalWrite(PIN_CATHODE2, HIGH);
  
  digitalWrite(PIN_L3, LOW);
  digitalWrite(PIN_L1, HIGH);
  delay(500);
  
  digitalWrite(PIN_L1, LOW);
  digitalWrite(PIN_L2, HIGH);
  delay(500);
  
  digitalWrite(PIN_L2, LOW);
  digitalWrite(PIN_L3, HIGH);
  delay(500);
 
  
}
