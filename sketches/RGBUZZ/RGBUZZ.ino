#define BUZZ_PIN A0

#define RED_PIN 11
#define BLUE_PIN 10
#define GREEN_PIN 9

void setup(){
  pinMode(RED_PIN, OUTPUT);
  pinMode(BLUE_PIN, OUTPUT);
  pinMode(GREEN_PIN, OUTPUT);
}

int redval,greenval,blueval;

void loop(){
  int sensor = analogRead(BUZZ_PIN);
  
  blueval = greenval = redval = 0;
  if(sensor < 512)
    blueval = 511 - sensor;
  
  if (sensor > 255 && sensor <= 767)
    greenval = 511 - ( 767 - sensor );
  
  if (sensor > 512 )
    redval = 511 - (1023 - sensor);
 
  analogWrite(BLUE_PIN, blueval/2);
  analogWrite(GREEN_PIN, greenval/2);
  analogWrite(RED_PIN, redval/2);
}
