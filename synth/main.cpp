
//set your clock speed
#define F_CPU 8000000UL

#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <avr/sleep.h>
#include <util/delay.h>
#include "Song.h"



// initialize
void init()
{
	// Set B-ports to output
	DDRB = 0xFF;

	// PWM
	TCCR0A = (1<<COM0A1) | (1<<WGM01) | (1<<WGM00); // Fast PWM, non-inverting mode
	TCCR0B = (1<<CS00);
	OCR0A = 128;
	TCNT0 = 0;

	// Sample streaming interrupt
	TCCR1 = (1<<CTC1) | (1<<CS11) | (1<<CS10); // Set prescaler to F_CPU / 4
	uint8_t timer1_oc = F_CPU / (globals::sample_rate * 4ULL);
	OCR1A = timer1_oc; // isr value
	OCR1C = timer1_oc; // reset value
	TCNT1 = 0;
	TIMSK |= (1<<OCIE1A); // timer compare A interrupt
	
}


// current streaming position in mixbuff
static volatile uint8_t stream_pos = 0;

// streaming interrupt
ISR(TIMER1_COMPA_vect)
{
	int16_t val = globals::mixbuff[0][stream_pos] + 0x80;
	int8_t hi = val>>8;
	if (unlikely(hi)) OCR0A = ~(hi>>7);
	else OCR0A = uint8_t(val);
	globals::mixbuff[0][stream_pos] = 0;
	stream_pos = (stream_pos+1) & ((globals::mixbuff_len<<1)-1);
}


// the song
static Song song;


// do it now
int main(void)
{
	// initialize
	init();
	
	// Start playback
	sei();

	// Play the song
	uint32_t pos=0;
	while(1) {
		while(stream_pos < globals::mixbuff_len) {};
		song.render(0, pos++);
		while(stream_pos >= globals::mixbuff_len) {};
		song.render(1, pos++);
	}
}

