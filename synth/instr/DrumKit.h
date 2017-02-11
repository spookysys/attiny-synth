#pragma once
#include "common.h"
#include "../samples/samples.h"
#include "../samples/adpcm.h"

namespace instr
{
	
	class DrumKit
	{
		uint16_t left;
		adpcm_state_t state;
		
	public:
	
		void reset()
		{
			left = 0;
		}
		
		void trigger(uint8_t sample_i)
		{
			const samples::Sample& sam = samples::samples[sample_i];
			this->left = sam.len;
			adpcm_init(
				&state,
				sam.data
			);
		}
		
		template<uint8_t rsh>
		void render(globals::Mixbuff buff_i)
		{
			if (!left) return;
			int16_t* dest = globals::mixbuff[buff_i];
			uint8_t num = (left < 4) ? left : 4;
			int8_t tmp[8];
			for (int i=0; i<8; i++) tmp[i] = 0;
			adpcm_render(&state, tmp, num);
			for (int i=0; i<8; i++) dest[i] += tmp[i];
			left -= num;
		}
	};
	
	
}