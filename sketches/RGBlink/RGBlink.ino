// Blinking RGB led

// PINS : red -> 3
//        blue -> 4
//        green -> 5

//CONSTANTS

int DELAY = 1000; // delay in ms between 2 iterations
int states = 0;
int rstate,gstate,bstate;

void setup(){
pinMode(3, OUTPUT);
pinMode(4, OUTPUT);
pinMode(5, OUTPUT);
}


void loop(){
  if(states == 8){
   states = 0;
  }
  
  rstate = (states & 1) == 1 ? HIGH : LOW;
  gstate = (states & 2) == 2 ? HIGH : LOW;
  bstate = (states & 4) == 4 ? HIGH : LOW;
  
  digitalWrite(3, rstate);
  digitalWrite(4, bstate);
  digitalWrite(5, gstate);
  
  delay(DELAY);
  
  states++;
}
