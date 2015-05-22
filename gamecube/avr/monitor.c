/*
 * Lecture des inputs sur les ports de la gamecube, et envoi des inputs sur liaison serie
 * M.Chevallier
 *
 * Documentation : 
 *
 * Protocole gamecube controller
 * http://www.int03.co.uk/crema/hardware/gamecube/gc-control.html
 *
 * Prog AVR ATMega328P
 * http://www.atmel.com/webdoc/avrassembler/avrassembler.wb_IN.html
 * http://hackaday.com/2010/11/05/avr-programming-03-reading-and-compiling-code/
 * http://www.nongnu.org/avr-libc/user-manual/modules.html
 * http://www.atmel.com/images/Atmel-8271-8-bit-AVR-Microcontroller-ATmega48A-48PA-88A-88PA-168A-168PA-328-328P_datasheet_Complete.pdf
 *
 * Utilisation liaison serie sur ATMega328P
 * http://www.appelsiini.net/2011/simple-usart-with-avr-libc
 *
 * TODO : 
 *  - Lecture sur 2 ports simultanement
 *  - Datation precise des inputs
 *  - Comm via I2C pour partage du bus entre les 2 arduinos et le vocore
 */

#include <avr/io.h>
#include <avr/interrupt.h>

// pour liaison serie
#define F_CPU 16000000UL  // 16MHz
#define BAUD 57600       // baudrate 115200

#include <util/setbaud.h>

#define RISING ( _BV(ISC01) | _BV(ISC00) )
#define FALLING ( _BV(ISC01) )
#define CHANGING ( _BV(ISC00) )

/* ----------------------------------------------------------------- */
/* Variables globales */

volatile uint8_t buff_index;
volatile uint8_t buffer[255];
volatile uint8_t done_reading;

/* ----------------------------------------------------------------- */
void serie_putchar(char c){
    loop_until_bit_is_set(UCSR0A, UDRE0); // On attend d'etre pret à transmettre
    UDR0 = c;
}

void init_timer(void){

    TCCR0B |= _BV(CS00) ; // Prescale : 1
    TIMSK0 |= _BV(TOIE0) ; // Interrupt quand timer overflow ( 255 )

}

void init_it(void){
    
    // parametrage de l'ISR Port1
    //EICRA |= CHANGING; // Declencheur de l'IT
    EICRA |= FALLING; // Declencheur de l'IT
    EIMSK |= _BV(INT0); // Enable IT 0

}

void init_serie(void){
    //
    // definition de la vitesse
    UBRR0H = UBRRH_VALUE;
    UBRR0L = UBRRL_VALUE;

#if USE_2X
    UCSR0A |= _BV(U2X0);
#else
    UCSR0A &= ~(_BV(U2X0));
#endif

    UCSR0C = _BV(UCSZ01) | _BV(UCSZ00); // 8N1
    UCSR0B = _BV(TXEN0);   // enable Tx, pas besoin de Rx

}

void read_byte( uint8_t start_index, uint8_t *data ){
    uint8_t i;
    *data = 0;
    for(i = 0; i <= 14 ; i += 2 ){
        if( buffer[start_index + i + 1] > buffer[start_index + i] ){
            *data |= 1 << (i / 2);
        }
    }
}

int main(void){

    uint8_t i;
    uint8_t data_buffer[11];
    cli();

    // parametrage du timer
    init_timer();

    // parametrage des IT externes
    init_it();

    // parametrage de la liaison serie
    init_serie();
    //serie_putchar('s');

    // listening mode
    while( 1 ){
        buff_index = 0;
        done_reading = 0;
        sei();
        while( !done_reading ){
            ;
        }
        cli();
       /* 
        if( buff_index > 170 ){
            read_byte( 1, &data_buffer[0] );
            read_byte( 17, &data_buffer[1] );
            read_byte( 33, &data_buffer[2] );
        }*/
        //for( i = 0; i < buff_index ; ++i )
        //    serie_putchar( buffer[i] );
        //serie_putchar( data_buffer[0] );
        serie_putchar( buff_index );
     //   serie_putchar( data_buffer[1] );
     //   serie_putchar( data_buffer[2] );
    }

    // analyse de la commande

    // envoi sur le bus/liaison serie
    
}

/**
 * ISR delenchee lors de l'overflow du timer0 ( tous les 255 cycles )
 */
ISR( TIMER0_OVF_vect ){

    if( buff_index)
        done_reading = 1;
}

/**
 * ISR declenchee lors d'un edge sur la ligne d'IT 0 ( port 0 GC, pin 2 arduino )
 * Mode naked : on enleve le prologue et l'epilogue ASM générés par avr-gcc.
 * Le traitement de cette IT doit se faire en envirron 1 microseconde == 16 cycles
 * Sachant qu'il y a un délai incompressible de 4 cycles avant et après l'ISR.
 * En pratique, on peut aller un peu au dessus, 20 cycles semble une limite acceptable.
 */
/*
ISR( INT0_vect, ISR_NAKED){
   asm volatile(
//   " push r1                    \n"
//   " push r0                    \n"
   " in r0,__SREG__             \n"
//   " push r0                    \n"
   " clr __zero_reg__           \n"
//   " push r24                   \n"
   " push r30                   \n"
   " push r31                   \n"
".L__stack_usage = 6             \n"
   " lds r30,buff_index         \n"
   " ldi r31,0                  \n"
   " in r23,0x26                \n"
   " subi r30,lo8(-(buffer))    \n"
   " sbci r31,hi8(-(buffer))    \n"
   " st Z,r23                   \n"
   " lds r23,buff_index         \n"
   " subi r23,lo8(-(1))         \n"
   " sts buff_index,r23         \n"
   " out 0x26,__zero_reg__      \n"
   " pop r31                    \n"
   " pop r30                    \n"
//   " pop r24                    \n"
//   " pop r0                     \n"
   " out __SREG__,r0            \n"
//   " pop r0                     \n"
//   " pop r1                     \n"
   " reti                       \n"
 
   );
   //buffer[buff_index] = TCNT0;
   //++buff_index;
   //TCNT0 = 0;
}*/

ISR( INT0_vect ){
    buffer[buff_index] = TCNT0;
    ++buff_index;
    TCNT0 = 0;

}

