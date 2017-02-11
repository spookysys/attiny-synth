#pragma once
#include <stdlib.h>

namespace instr
{
	// Fabrikklyd med sidechain-kompressor
	// note: må legges til helt til slutt for at kompressoren skal gi mening
	// analyserer lyden som allerede ligger i mixbufferet og bruker dette
	// til å styre en innebygget kompressor som justerer fabrikkstøyvolumet

	template<uint8_t loudness_response=3, uint8_t quietness_response=6>
	class Fabrikklyd
	{
		uint16_t sense;

		// low-pass filter for compressor response		
		template<uint8_t length>
		static uint16_t filter(uint16_t state, uint16_t input)
		{		
			static const uint16_t bias = (1<<(length-1))-1;
			return ((uint32_t(state) << length) - state + input + bias) >> length;
		}
		
		// update compressor and return volume
		uint8_t analyze(uint8_t buff)
		{
			int16_t* dest = globals::mixbuff[buff];
			
			// grab volume from buffer
			uint16_t vol = 0;
			vol += abs(dest[0]);
			vol += abs(dest[2]);
			vol += abs(dest[4]);
			vol += abs(dest[6]);
			if (vol>255) vol = 255;
			vol = (vol<<8) | vol;
			
			// response
			if (vol > sense)
				sense = filter<loudness_response>(sense, vol); // fast response to loudness
			else
				sense = filter<quietness_response>(sense, vol); // slow response to quietness
			
			// calculate desired volume
			uint16_t tmp = sense>>8;
			if (tmp>255) tmp = 255;
			return 255 - tmp;
		}
		
	public:
		void reset() {}
			
		template<uint8_t reduce, typename WaveformT>
		void render(uint8_t buff, uint32_t pos, WaveformT waveform_func)
		{
			int16_t* dest = globals::mixbuff[buff];
			
			// update compressor response
			uint8_t vol = analyze(buff);
			
			// render sound
			pos <<= (3-reduce);
			uint8_t mask = (1<<reduce)-1;
			int8_t v;
			if (!(0 & mask)) v = scale8(waveform_func(pos++), vol); dest[0] += v;
			if (!(1 & mask)) v = scale8(waveform_func(pos++), vol); dest[1] += v;
			if (!(2 & mask)) v = scale8(waveform_func(pos++), vol); dest[2] += v;
			if (!(3 & mask)) v = scale8(waveform_func(pos++), vol); dest[3] += v;
			if (!(4 & mask)) v = scale8(waveform_func(pos++), vol); dest[4] += v;
			if (!(5 & mask)) v = scale8(waveform_func(pos++), vol); dest[5] += v;
			if (!(6 & mask)) v = scale8(waveform_func(pos++), vol); dest[6] += v;
			if (!(7 & mask)) v = scale8(waveform_func(pos++), vol); dest[7] += v;
		}
		
	};
}
