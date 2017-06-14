
//set your clock speed
#define F_CPU 16000000UL

#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <avr/sleep.h>
#include <util/delay.h>
#include "common.h"
#include "Player.h"

static const uint8_t LED_BRIGHTNESS = 0x40;

// initialize
void init()
{
	// Setup ports
	DDRB = 0xFF;

	// PWM
	TCCR0A = (1<<COM0B1) | (1<<COM0A1) | (1<<WGM01) | (1<<WGM00); // Fast PWM, non-inverting mode
	TCCR0B = (1<<CS00);
	OCR0A = 128;
	OCR0B = LED_BRIGHTNESS;
	TCNT0 = 0;

	// Sample streaming interrupt
	TCCR1 = (1<<CTC1) | (1<<CS11) | (1<<CS10); // Set prescaler to F_CPU / 4
	uint8_t timer1_oc = F_CPU / (globals::sample_rate * 4ULL);
	OCR1A = timer1_oc; // isr value
	OCR1C = timer1_oc;
	TCNT1 = 0;
	TIMSK |= (1<<OCIE1A); // timer compare A interrupt
}


// current streaming position in mixbuff
static volatile uint8_t stream_pos = 0;


// streaming interrupt
ISR(TIMER1_COMPA_vect)
{
	int16_t* mixbuff = globals::mixbuff[0]; // flatten the two buffers into one
	int16_t val = uint16_t(mixbuff[stream_pos]) + 0x80;
	if (unlikely(val<0)) val = 0;
	else if (unlikely(val>=0xFF)) val = 0xFF;
	OCR0A = uint8_t(val);
	mixbuff[stream_pos] = 0;
	stream_pos = (stream_pos+1) & ((globals::mixbuff_len<<1)-1);
}



// do it now
int main(void)
{
    static Player player;

	// initialize
	init();
	
	// Start playback
	player.reset();
	sei();

	// Play music
	uint32_t pos=0;
	while(1) {
		// Buffer SWAP0
		while(stream_pos < globals::mixbuff_len) {};
		player.render(globals::SWAP0, pos++);
		if (stream_pos < globals::mixbuff_len) fail();
		
		// Buffer SWAP1
		while(stream_pos >= globals::mixbuff_len) {};
		player.render(globals::SWAP1, pos++);
		if (stream_pos >= globals::mixbuff_len) fail();
	}
}

