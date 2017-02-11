#pragma once
#include "common.h"
#include "AdpcmDecoder.h"
#include "../samples/samples.h"

namespace instr
{
	
	class DrumKit
	{
		uint8_t sample_i;
		uint16_t pos;
		
		AdpcmDecoder<2> decoder;
		
	public:
		void reset()
		{
			sample_i = 0xFF;
		}
		
		void trigger(uint8_t sample_i)
		{
			if (sample_i>=samples::num_samples) fail();
			this->pos = 0;
			this->sample_i = sample_i;
			decoder.reset();
		}
		
		template<uint8_t rsh>
		void render(globals::Mixbuff buff_i)
		{
			if (sample_i == 0xFF) return;

			int16_t* dest = globals::mixbuff[buff_i];
			const samples::Sample& sam = samples::samples[this->sample_i];
			uint8_t code;
			
			code = pgm_read_byte(sam.data+pos);
			dest[0] += decoder.decode(code) >> rsh;
			dest[1] += decoder.decode(code) >> rsh;
			dest[2] += decoder.decode(code) >> rsh;
			dest[3] += decoder.decode(code) >> rsh;
			if (pos >= sam.len) {
				sample_i = 0xFF;
				return;
			}

			code = pgm_read_byte(sam.data+pos);
			dest[4] += decoder.decode(code) >> rsh;
			dest[5] += decoder.decode(code) >> rsh;
			dest[6] += decoder.decode(code) >> rsh;
			dest[7] += decoder.decode(code) >> rsh;
			if (pos >= sam.len) {
				sample_i = 0xFF;
				return;
			}
		}
	};
	
	
}