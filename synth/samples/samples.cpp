#include "samples.h"
#include <avr/pgmspace.h>

namespace samples
{

	/*
	const uint8_t hihat_808[] PROGMEM = {
		#include "inc/hihat-808.inc"
	};
	*/
	
	/*
	const uint8_t hihat_analog[] PROGMEM = {
		#include "inc/hihat-analog.inc"
	};
	
	const uint8_t hihat_electro[] PROGMEM = {
		#include "inc/hihat-electro.inc"
	};
	*/
	
	
	const uint8_t perc_808[] PROGMEM = {
		#include "inc/perc-808.inc"
	};
	
	
		
	/*
	const uint8_t perc_metal[] PROGMEM = {
		#include "inc/perc-metal.inc"
	};
	*/

			/*	
	const uint8_t snare_electro[] PROGMEM = {
		#include "inc/snare-electro.inc"
	};*/
	
		
	/*
	const uint8_t snare_808[] PROGMEM = {
		#include "inc/snare-808.inc"
	};
	*/
	
	/*
	const uint8_t snare_tape[] PROGMEM = {
		#include "inc/snare-tape.inc"
	};*/
	
		
	
	const Sample samples[num_samples] = {
		//{ sizeof(hihat_808), hihat_808 },
		//{ sizeof(hihat_analog), hihat_analog },
		//{ sizeof(hihat_electro), hihat_electro },
		{ sizeof(perc_808), perc_808 },
		//{ sizeof(perc_metal), perc_metal },
		//{ sizeof(snare_electro), snare_electro },
		//{ sizeof(snare_808), snare_808 },
		//{ sizeof(snare_tape), snare_tape }
	};
	
		
}