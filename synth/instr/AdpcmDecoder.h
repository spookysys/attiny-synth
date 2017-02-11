#pragma once
#include "common.h"
#include <stdlib.h>

// from asm-file
extern "C" long update_yl(long yl, int yu); 
	
namespace instr
{


	struct adpcm_state_t
	{
		long yl;	// Locked (slow) state scale factor step size mult.
		short yu;	// Unlocked (fast) state scale factor step size mult.
		short dms;	// Short term energy estimate.
		short dml;	// Long term energy estimate.
		short ap;	// Linear weighting coefficient of 'yl' and 'yu'.

		short a[2];	// Coefficients of pole portion of prediction filter.
		short b[6];	// Coefficients of zero portion of prediction filter.
		short pk[2];	// Signs of previous partially reconstructed signals.

		short dq[6];	// Previous samples of the quantized difference signal.
		short sr[2];	// Previous samples of the reconstructed signal .
		char td;	// Tone detection indicator.
	};

	void initialize_adpcm(adpcm_state_t* state);
	
	// Decodes ADPCM signal i. Returns 14-bit linear PCM signal.
	template<uint8_t BITS>
	int decode_adpcm(char i, adpcm_state_t* state);
	
	
	template<uint8_t BITS=2> // bit level (2, 3, 4 or 5)
	class AdpcmDecoder
	{
		adpcm_state_t state;
	public:

		void reset()
		{
			initialize_adpcm(&state);		
		}
	
		// decode one sample of data
		int16_t decode(uint8_t& src)
		{
			static const uint8_t mask = (1<<BITS)-1;
			uint8_t code = src & mask; src >>= BITS;
			return decode_adpcm<BITS>(code, &state); 
		}
	
	};

}
		