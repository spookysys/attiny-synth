
//set your clock speed
#define F_CPU 16000000UL


#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <avr/sleep.h>
#include <util/delay.h>
#include "tables.hpp"

static const uint32_t sample_rate = 2 * 440;

void init_timers()
{
	// Samplestream
	TCCR0A = (1<<WGM01);
	TCCR0B = (4<<CS00); // rate is F_CPU / 256
	OCR0A = F_CPU / (sample_rate * 256ULL);
	TCNT0 = 0;

	// PWM
	TCCR2B = (1<<CS00);
	TCCR2A = 0x00;
	TCNT2 = 0;

	// Enable interrupts
	TIMSK0 |= (1<<OCIE0A); // timer compare A interrupt
	
	
	/*
	// Init playback
	TCCR1B = (1<<CS00);                     // No prescaling	TCCR1A = 0x21;                          // 8 bit PWM, using COM1B
	TCNT1 = 0x00;                           // set counter1 to zero
	TIFR = 0x04;                            // clear counter1 overflow flag
	TIMSK = 0x04;                           // enable counter1 overflow interrupt

	OCR1B = 0x00;                           // set output compare register B to zero
	
	// interrupt disabled, SPI port enabled, master mode, MSB first,  SPI mode 3, Fcl/4
	SPCR = 0x5C;
	*/
		
}

static bool val = false;
ISR(TIMER0_COMPA_vect)
{
	val = !val;
}



int main(void)
{
	init_timers();
	
	// Set B-ports to output
	DDRB = 0xFF;
		
	// initialize timer
	init_timers();
	
    // enable global interrupts
    sei();
	
	// loop forever
	while(1)
	{
		if (val) {
			PORTB |= (1 << 0);
		} else {
			PORTB &= ~(1 << 0);
		}
	}
}



/*
ISR(TIMER0_COMPA_vect) {
	OCR1B = flash.readNextByte();      // Read flash, write PWM reg.
	if(++index >= samples) {           // End of audio data?
		index = 0;                       // We must repeat!
		flash.endRead();
		flash.beginRead(6);              // Skip 6 byte header
	}
}

*/