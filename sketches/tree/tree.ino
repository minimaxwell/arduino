/* led matrix controller on attiny45.
Matrix 5x5.
*/

#define CLK 3
#define LATCH 4
#define DATA 0

void setup(){
  pinMode(CLK,OUTPUT);
  pinMode(LATCH,OUTPUT);
  pinMode(DATA,OUTPUT);
}

unsigned int to_send = 0;
void loop(){

  int i,j;
  for(j=0; j<5; ++j){
    for(i=0; i< 500; ++i){
      light_tree();
    }
    delay(1000);  
  }
   
  for(i=0; i<5; ++i){
    all(); 
  }
}


void light_tree(){
  int i;
   for(i=0; i<5; ++i){
    to_send = 0;
    to_send |= 1 << i;
    to_send |= 31 << 5; 
     
     write_tree(to_send);
    
  }
}

void all(){
  int i,j;
 for(i=0; i<5; ++i){
  for(j=0; j<5; ++j){
    to_send = (1 << (i)) | (1 << (j+5)) ;
    write_tree(to_send);
    delay(100);
  }
 } 
}

void write_tree(unsigned int data){
  digitalWrite(LATCH, LOW);
    
    shiftOut(DATA, CLK, MSBFIRST, (unsigned char) (data >> 8));
    shiftOut(DATA,CLK, MSBFIRST,  (unsigned char)(data));
    
    digitalWrite(LATCH, HIGH);
}
