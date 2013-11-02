// Controlling a servo position using a potentiometer (variable resistor) 
// by Michal Rinott <http://people.interaction-ivrea.it/m.rinott> 

#include <Servo.h> 
 
Servo myservo;  // create servo object to control a servo 

long angle = 90;
long oldangle = 90;
void setup() 
{ 
  myservo.attach(9);  // attaches the servo on pin 9 to the servo object 
  Serial.begin(19200);
  pinMode(13,OUTPUT);
}
char strVal[10];
int index = 0;
char rd;

void loop(){
  
  if(Serial.available()){
    rd = Serial.read();
    
    
    if(index < 6 && isDigit(rd)){
     strVal[index++] = rd; 
    }else{
     if(index != 0){
     strVal[index] = 0;
     angle = map(atoi(strVal), 0, 32767, 0, 155);
     index = 0;
     
     char str2[10];
     sprintf(str2,"%d\n",angle);

     if(angle != oldangle){
       myservo.write(angle);  // sets the servo position according to the scaled value 
        oldangle = angle;
     }
     }
    }
    
  }
 


} 

