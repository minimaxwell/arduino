#define LASER_1 2
#define LASER_2 3
#define LASER_3 4
#define LASER_4 5
#define LASER_5 6

void setup(){
  pinMode(LASER_1,OUTPUT);
  pinMode(LASER_2,OUTPUT);
  pinMode(LASER_3,OUTPUT);
  pinMode(LASER_4,OUTPUT);
  pinMode(LASER_5,OUTPUT);
}

void loop(){
  int i;
  for(i = 0; i<32; ++i){
    digitalWrite(LASER_1, i & 0x01 ? LOW : HIGH);
    digitalWrite(LASER_2, i & 0x02 ? LOW : HIGH);
    digitalWrite(LASER_3, i & 0x04 ? LOW : HIGH);
    digitalWrite(LASER_4, i & 0x08 ? LOW : HIGH);
    digitalWrite(LASER_5, i & 0x10 ? LOW : HIGH);    
  }
}
