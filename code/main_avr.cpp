//set your clock speed
#define F_CPU 32000000UL
#include <avr/sleep.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include <util/delay.h>
extern "C"
{
	#include "overclock.h"
};
#include "common.hpp"
#include "Player.hpp"
#include "Buffer.hpp"
#include "myrand.hpp"


// how much to shift down before clipping
static const int downshift = 2;

// how much to led
static const uint8_t LED_BRIGHTNESS = 0x40;

// current streaming position in mixbuffers
static volatile uint8_t stream_pos = 0;

// mixbuffers
static Buffer mixbuffers[2];

// the player
static Player player;


// initialize
void init()
{
	// Overclock
	overclockInit();
	overclockCpu(OVERCLOCK_MAX);
	
	// Setup ports
	DDRB = 0xFF;

	// PWM
	TCCR0A = (1<<COM0B1) | (1<<COM0A1) | (1<<WGM01) | (1<<WGM00); // Fast PWM, non-inverting mode
	TCCR0B = (1<<CS00);
	OCR0A = 128;
	OCR0B = LED_BRIGHTNESS;
	TCNT0 = 0;

	// Sample streaming interrupt
	TCCR1 = (1<<CTC1) | (1<<CS12); // Set prescaler to F_CPU / 8
	static const uint8_t timer1_oc = (F_CPU) / (globals::SAMPLE_RATE * 8ULL);
	OCR1A = timer1_oc; // isr value
	OCR1C = timer1_oc;
	TCNT1 = 0;
	TIMSK |= (1<<OCIE1A); // timer compare A interrupt
}



// streaming interrupt
ISR(TIMER1_COMPA_vect)
{
	int16_t* mixbuff = mixbuffers[0].data(); // flatten the two buffers into one
	int16_t val = uint16_t(mixbuff[stream_pos]);
	val >>= downshift;
	val += 0x80;
	if (unlikely(val<0)) val = 0;
	else if (unlikely(val>=0xFF)) val = 0xFF;
	OCR0A = uint8_t(val);
	stream_pos = (stream_pos+1) & ((globals::SAMPLES_PER_BUFFER<<1)-1);
}

extern "C"
void exit(int op)
{
//	TCCR0A = 0;
//	TCCR0B = 0;
//	TCCR1 = 0;
//	TIMSK = 0;
	// blink led to make user turn off the device
	DDRB = 0x02; // cut sound, keep led
	overclockCpu(OVERCLOCK_STD);
	while(1) {
		OCR0B = LED_BRIGHTNESS + 0x20;
		_delay_ms(250);
		OCR0B = 0;		
		_delay_ms(250);
	};
}

// do it now
int main(void)
{
	// srand
	myrand::srand();

	// initialize
	init();
	player.init();

	// Start playback
	sei();

	// Play music
	mixbuffers[1].clear();
	while(1) {
		// Buffer SWAP0
		while(stream_pos < globals::SAMPLES_PER_BUFFER) {};
		if (!player.render(mixbuffers[0], mixbuffers[1]))
			exit(0);
		if (stream_pos < globals::SAMPLES_PER_BUFFER) assert(0);
		
		// Buffer SWAP1
		while(stream_pos >= globals::SAMPLES_PER_BUFFER) {};
		if (!player.render(mixbuffers[1], mixbuffers[0]))
			exit(0);
		if (stream_pos >= globals::SAMPLES_PER_BUFFER) assert(0);
	}
}

