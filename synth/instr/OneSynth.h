#pragma once
#include "common.h"


namespace instr
{
	
	template<int8_t(*synth_func)(uint32_t), uint8_t shr, uint8_t decay_speed>
	class OneSynth
	{
		uint32_t pos;
		uint16_t vol;
		uint16_t pitch;
		enum State
		{
			OFF=0,
			RAMP,
			SUSTAIN,
			DECAY
		} state;
		
		int8_t iter()
		{
			pos += pitch;
			return synth_func(pos>>shr);
		}
	public:
		void reset()
		{
			pos = 0;
			vol = 0;
			state = OFF;
		}
			
		void render(uint8_t buff)
		{
			volatile int16_t* dest = globals::mixbuff[buff];
			switch (state) {
				case OFF: return;
				case RAMP:
					// dest[0] += 0;
					dest[1] += scale4(iter(), 0);
					dest[2] += scale4(iter(), 1);
					dest[3] += scale4(iter(), 2);
					dest[4] += scale4(iter(), 3);
					dest[5] += scale4(iter(), 4);
					dest[6] += scale4(iter(), 5);
					dest[7] += scale4(iter(), 6);
					state = SUSTAIN;
					break;
				case SUSTAIN:
					dest[0] += iter();
					dest[1] += iter();
					dest[2] += iter();
					dest[3] += iter();
					dest[4] += iter();
					dest[5] += iter();
					dest[6] += iter();
					dest[7] += iter();
					break;
				case DECAY: {
					dest[0] += scale8(iter(), vol>>8);
					dest[1] += scale8(iter(), vol>>8);
					dest[2] += scale8(iter(), vol>>8);
					dest[3] += scale8(iter(), vol>>8);
					dest[4] += scale8(iter(), vol>>8);
					dest[5] += scale8(iter(), vol>>8);
					dest[6] += scale8(iter(), vol>>8);
					dest[7] += scale8(iter(), vol>>8);
					vol -= scale8(vol>>8, decay_speed)<<4;
					if ((vol>>8) == 0) this->state = OFF;
				} break;
			}
		}
		
		
		void setPos(uint32_t pos)
		{
			this->pos = pos;
		}
		
		void setPitch(uint32_t pitch)
		{
			this->pitch = pitch;
		}
		
		void trigger()
		{
			state = RAMP;
		}

		void release()
		{
			vol = 0xFFFF;
			state = DECAY;
		}
	};

}