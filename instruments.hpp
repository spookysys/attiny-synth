#pragma once
#include "../common.hpp"
#include "instr_tables.hpp"

namespace instruments
{
	struct BassDrum
	{
		uint8_t pitch;
		uint16_t pos;
		
		inline int8_t iter() {
			static_assert(sizeof(tables::bd)==256);
			auto tmp = tables::bd[pos>>8];
			pos += pitch;
		}

		inline int16_t iterRamp(uint8_t ramp) {
			int8_t ret = iter();

		}

	public:

		void trigger()
		{
			pitch = 255;
		}

		void step(mixbuff_t*& mix)
		{
			if (pitch==0) return;
			if (pitch==1) {
				mix[0] += scale(iter(), 7);
				mix[1] += scale(iter(), 6);
				mix[2] += scale(iter(), 5);
				mix[3] += scale(iter(), 4);
				mix[4] += scale(iter(), 4);
				mix[5] += scale(iter(), 3);
				mix[6] += scale(iter(), 2);
				mix[7] += scale(iter(), 1);
				mix[8] += scale(iter(), 1);
			} else {
				mix[0] += iter();
				mix[1] += iter();
				mix[2] += iter();
				mix[3] += iter();
				mix[4] += iter();
				mix[5] += iter();
				mix[6] += iter();
				mix[7] += iter();
				mix[8] += iter();
			}
			pitch--;
		}
	};

	class Bass
	{
	public:
		void trigger(int8_t note)
		{
		}

		void step(mixbuff_t*& mix)
		{
			if (pitch==0) return;
			mix[0] = iter();
			mix[1] = iter();
			mix[2] = iter();
			mix[3] = iter();
			mix[4] = iter();
			mix[5] = iter();
			mix[6] = iter();
			mix[7] = iter();
			mix[8] = iter();
			pitch--;
		}
	};

	class SamplePlayer
	{
	public:
		void trigger(uint8_t sample)
		{

		}

	};
	
}

