
//set your clock speed
#define F_CPU 16000000UL


#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <avr/sleep.h>
#include <util/delay.h>

void timer0_init()
{
	// set up timer with no prescaling
	TCCR0B = (3<<CS00); // divide by 64
	TCCR0A = 0x00;
	
	// initialize counter
	TCNT0 = 0;
	
	
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

int main(void)
{
	timer0_init();
	
	// Set B-ports to output
	DDRB = 0xFF;
		
	// initialize timer
	timer0_init();
	
	// loop forever
	while(1)
	{
		if (TCNT0 > 128)
			PORTB |= (1 << 0);
		else 
			PORTB &= ~(1 << 0);
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