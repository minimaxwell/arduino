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
#define BAUD 57600       // baudrate

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

void init_pins(void){
    // PIN 2 port D
    PORTD &= ~(_BV(PD2)); // pas de pullup
    DDRD &= ~(_BV(PD2)); // input

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
    for(i = 0; i < 8; ++i ){
        if( buffer[i+start_index] ){
            *data |= 1 << (7-i);
        }
    }
}

void transfer_command(void){
    uint8_t i, data;
    // commande console -> manette
    for(i = 0 ; i < 3 ; ++i){
        read_byte(i*8, &data);
        serie_putchar(data);
    }

    // commande manette -> console
    // offset de 1 dans le premier bit de lecture ( stop bit de la commande precedente )
    for( i = 3 ; i < 11; i++ ){
        read_byte( i*8 + 1 , &data );
        serie_putchar(data);
    }

}

int main(void){

    cli();

    // parametrage du timer
    init_timer();

    // parametrage des IT externes
    init_it();

    // parametrage des pins en input
    init_pins();

    // parametrage de la liaison serie
    init_serie();

    // listening mode
    while( 1 ){
        buff_index = 0;
        done_reading = 0;
        sei();
        while( !done_reading ){
            ;
        }
        cli();
        
        if(buff_index == 90)
            transfer_command();
        
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
*/

ISR( INT0_vect ){
    buffer[buff_index] = PIND & 0x04;
    ++buff_index;
    TCNT0 = 0;
}

