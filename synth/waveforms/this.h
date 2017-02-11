#pragma once
#include "common.h"

// Waveforms for OneSynth and Fabrikklyd
// tip: use lambdas to compose
// tip: combine with 'scale3' to adjust volume

namespace waveforms
{
		
	// these were all the oneliners that didn't have mul/div/mod
	template<uint8_t sel, typename t_t>
	inline int8_t oneliner(t_t t)
	{
		switch (sel)
		{
			case 0: return t&t>>8;
			case 1: return t>>6&1?t>>5:-t>>4;
			case 2: return (t>>(t&7))|(t<<(t&42))|(t>>7)|(t<<5);
			case 3: return ((t>>6|t<<1)+(t>>5|t<<3|t>>3))|t>>2|t<<1;
			case 4: return (t>>5)|(t<<4)|((t&1023)^1981)|((t-67)>>4);
			case 5: return (((t+(t>>2))|(t>>5))+(t>>3))|((t>>13)|(t>>7)|(t>>11));
			case 6: return t*(42&t>>10);
			default: return 0;
		}
	}


	enum Tone
	{
		SQUARE,
		SAW,
		SINE,
		SINE_I // sine with 3-bit interpolation
	};
		
	template<Tone sel, typename t_t>
	int8_t tone(t_t t)
	{
		switch(sel)
		{
			case SQUARE: return t&0x80;
			case SAW: return t;
			case SINE: return pgm_read_byte(&tables::bd[t&0xFF]);
			case SINE_I: {
				int8_t v0 = pgm_read_byte(&tables::bd[(t>>3)&0xFF]);
				int8_t v1 = pgm_read_byte(&tables::bd[((t+1)>>3)&0xFF]);
				return lerp3(v0, v1, t&0x7);
			}
			default: return 0;
		}
	}
		
	// Ideally this should have separate time and position inputs, but what the heck
	template<uint8_t fade_time=9, typename func0_t, typename func1_t>
	int8_t xfade(uint32_t t, func0_t func0, func1_t func1)
	{
		if (t <= 8<<fade_time)
			return lerp3(
				func0(t), 
				func1(t), 
				t>>fade_time
			);
		else 
			return func1(t);
	}

}
