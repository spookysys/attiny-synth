
//set your clock speed
#define F_CPU 16000000UL

#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <avr/sleep.h>
#include <util/delay.h>
#include "Song.h"


// Adjust sample rate
static const uint32_t sample_rate = 22000;


// initialize
void init()
{
	// Set B-ports to output
	DDRB = 0xFF;
	
	// Sample streaming interrupt
	TCCR0A = (1<<WGM01);
	TCCR0B = (1<<CS02); // Set prescaler to F_CPU / 256
	OCR0A = F_CPU / (sample_rate * 256ULL);
	TCNT0 = 0;
	
	// PWM
	TCCR2A = (1<<COM2A1) | (1<<WGM21) | (1<<WGM20); // Fast PWM, non-inverting mode
	TCCR2B = (1<<CS20);
	OCR2A = 128;
	TCNT2 = 0;

	// Enable interrupt
	TIMSK0 |= (1<<OCIE0A); // timer compare A interrupt
}


// current streaming position in mixbuff
static volatile uint8_t stream_pos = 0;

// streaming interrupt
ISR(TIMER0_COMPA_vect)
{
	int16_t val = mixbuff[0][stream_pos] + 0x80;
	int8_t hi = val>>8;
	if (unlikely(hi)) OCR2A = ~(hi>>7);
	else OCR2A = uint8_t(val);
	mixbuff[0][stream_pos] = 0;
	stream_pos = (stream_pos+1) & ((mixbuff_len<<1)-1);
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
		while(stream_pos < mixbuff_len) {};
		song.render<0>(pos++);
		while(stream_pos >= mixbuff_len) {};
		song.render<1>(pos++);
	}
}

