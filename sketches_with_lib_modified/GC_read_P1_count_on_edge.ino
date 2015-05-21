/*
   Compte le nombre de ticks entre chaque edge. 
 */

#define BUFFSIZE 255

// Variables utilisees sous IT, volatiles.
uint8_t buffer[BUFFSIZE];
//volatile uint8_t buff_index;
#define buff_index GPIOR0
uint8_t overflow;
volatile uint8_t done;
// pris sur http://sphinx.mythic-beasts.com/~markt/ATmega-timers.html
void timer0_setup (byte mode, int prescale, byte outmode_A, byte outmode_B, byte capture_mode)
{
    // enforce field widths for sanity
    mode &= 15 ;
    outmode_A &= 3 ;
    outmode_B &= 3 ;
    capture_mode &= 3 ;

    byte clock_mode = 0 ; // 0 means no clocking - the counter is frozen.
    switch (prescale)
    {
        case 1: clock_mode = 1 ; break ;
        case 8: clock_mode = 2 ; break ;
        case 64: clock_mode = 3 ; break ;
        case 256: clock_mode = 4 ; break ;
        case 1024: clock_mode = 5 ; break ;
        default:
                   if (prescale < 0)
                       clock_mode = 7 ; // external clock
    }
    TCCR0A = (outmode_A << 6) | (outmode_B << 4) | (mode & 3) ;
    TCCR0B = (capture_mode << 6) | ((mode & 0xC) << 1) | clock_mode ;
}

void irs_edge(){
    // On récupère la valeut du timer
    //if(++buff_index < BUFFSIZE - 2 )
    //    buffer[buff_index] = TCNT0;

    // on reset pour le prochain edge
    //TCNT0 = 0;
}


void reset_irs(){

 //   buff_index = 0;
    done = 0;
    overflow = 0;
    attachInterrupt(0,irs_edge,CHANGE);

}

ISR( INT0_vect , ISR_NAKED)
{

    // ASM pour aller plus vite
    //buffer[buffer_index++] = TCNT0;
    //TCNT0 = 0;

    asm volatile(
        " in r26,__SREG__          \n"
        //" clr __zero_reg__        \n"
        " in r30,0x1e             \n" 
        " in r27,0x26             \n" 
        " ldi r31,0               \n"
        " subi r30,lo8(-(buffer)) \n"
        " sbci r31,hi8(-(buffer)) \n"
        " st Z,r27                \n"
        " in r27,0x1e             \n"
        " subi r27,lo8(-(1))      \n"
        " out 0x1e,r27            \n"
        " out 0x26,__zero_reg__   \n"
        " out __SREG__,r26         \n"
        " reti                    \n"
        );
}

ISR( TIMER0_OVF_vect ){
    if(buff_index)
    done = 1;
}

void setup(){
    Serial.begin(115200);
    timer0_setup(0,1,0,0,0);   
    reset_irs();
}

void dump_buffer(){
    uint16_t i;
  //  Serial.println(buff_index, DEC);
    if( (buff_index > 15 && buff_index < 22) || buff_index > 170){
        for(i = 0; i < buff_index ; i++){
            //Serial.print(buffer[i], DEC);
            buffer[i] = 0;
            //Serial.print(' ');
        }
        Serial.println(buff_index,DEC);
        buff_index = 0;
   
    }else{
        buff_index = 0;
    }
}

void loop(){
    while(1){
        done = 0;
        overflow = 0;
        attachInterrupt(0,irs_edge,CHANGE);

        while( !done){
            ;
        }
        detachInterrupt(0);
        dump_buffer();
        //reset_irs();   
    }   
}

