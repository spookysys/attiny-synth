#pragma once

namespace instr
{
	// Tanken med Fabrikklyd er å fylle inn stille partier i musikken/beaten
	// Dette gjøres ved å analysere volumet til det som allerede ligger i bufferet,
	// og bruke dette som sidechain til en kompressor som så styrer hvor mye av
	// "waveform_func" som skal legges til
	// God ide? Vel, kanskje..
	// TODO: Kompressor.
	class Fabrikklyd
	{
	public:

		template<uint8_t reduce, typename WaveformT>
		void render(uint8_t buff, uint32_t pos, WaveformT waveform_func)
		{
			volatile int16_t* dest = globals::mixbuff[buff];
			pos <<= (3-reduce);
			uint8_t mask = (1<<reduce)-1;
			int8_t v;
			if (!(0 & mask)) v = waveform_func(pos++); dest[0] += v;
			if (!(1 & mask)) v = waveform_func(pos++); dest[1] += v;
			if (!(2 & mask)) v = waveform_func(pos++); dest[2] += v;
			if (!(3 & mask)) v = waveform_func(pos++); dest[3] += v;
			if (!(4 & mask)) v = waveform_func(pos++); dest[4] += v;
			if (!(5 & mask)) v = waveform_func(pos++); dest[5] += v;
			if (!(6 & mask)) v = waveform_func(pos++); dest[6] += v;
			if (!(7 & mask)) v = waveform_func(pos++); dest[7] += v;
		}
		
	};
}
