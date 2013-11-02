
const int octave[7] = {262, 294, 330, 349, 392, 440, 494};
int oct = 1;

void setup(){
 pinMode(2, INPUT); 
}

void loop(){
  //if(digitalRead(2) == LOW){
  // while(digitalRead(2) == LOW){;}
   //start(); 
   
 // }
 
   if(digitalRead(2) == LOW){
    tone(8,octave[0] / 8); 
   }else{
    noTone(8); 
   }
}

void start(){
  int i;
 for(oct = 1; oct < 32; oct = oct << 1){
  for(i = 0; i < 7; ++i){
   tone(8, ( octave[i] / 4 ) * oct, 100); 
   delay(100);
  }
 } 
}
