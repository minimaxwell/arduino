#define PIN3 3

void setup(){
  pinMode(PIN3, OUTPUT); 
}

void loop(){
 digitalWrite(PIN3, HIGH);

  delay(900);
 
   digitalWrite(PIN3, LOW);
  
  delay(900);
}
