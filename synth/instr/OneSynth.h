#pragma once
#include "common.h"


namespace instr
{
	
	// Play a waveform at different pitches with a simple volume envelope
	template<uint8_t shr, uint8_t decay_speed, typename PosType=uint16_t>
	class OneSynth
	{
		PosType pos;
		uint16_t vol;
		uint16_t pitch;

		enum State
		{
			OFF=0,
			RAMP,
			SUSTAIN,
			DECAY
		} state;
		
		
	public:
	
		void reset()
		{
			pos = 0;
			vol = 0;
			state = OFF;
		}
			

		template<typename WaveformFunc>
		void render(globals::Mixbuff buff_i, WaveformFunc waveform_func)
		{
			int16_t* dest = globals::mixbuff[buff_i];
			if (state==OFF) return;
			int8_t v0 = waveform_func(pos >> shr); pos += pitch;
			int8_t v1 = waveform_func(pos >> shr); pos += pitch;
			int8_t v2 = waveform_func(pos >> shr); pos += pitch;
			int8_t v3 = waveform_func(pos >> shr); pos += pitch;
			int8_t v4 = waveform_func(pos >> shr); pos += pitch;
			int8_t v5 = waveform_func(pos >> shr); pos += pitch;
			int8_t v6 = waveform_func(pos >> shr); pos += pitch;
			int8_t v7 = waveform_func(pos >> shr); pos += pitch;
			switch (state) {
				case OFF: return;
				case RAMP:
					// dest[0] += 0;
					dest[1] += scale3(v1, 0);
					dest[2] += scale3(v2, 1);
					dest[3] += scale3(v3, 2);
					dest[4] += scale3(v4, 3);
					dest[5] += scale3(v5, 4);
					dest[6] += scale3(v6, 5);
					dest[7] += scale3(v7, 6);
					state = SUSTAIN;
					break;
				case SUSTAIN:
					dest[0] += v0;
					dest[1] += v1;
					dest[2] += v2;
					dest[3] += v3;
					dest[4] += v4;
					dest[5] += v5;
					dest[6] += v6;
					dest[7] += v7;
					break;
				case DECAY: {
					dest[0] += scale8_s8(v0, vol>>8);
					dest[1] += scale8_s8(v1, vol>>8);
					dest[2] += scale8_s8(v2, vol>>8);
					dest[3] += scale8_s8(v3, vol>>8);
					dest[4] += scale8_s8(v4, vol>>8);
					dest[5] += scale8_s8(v5, vol>>8);
					dest[6] += scale8_s8(v6, vol>>8);
					dest[7] += scale8_s8(v7, vol>>8);
					vol -= uint16_t(vol>>8)*decay_speed;
					if ((vol>>8) == 0) this->state = OFF;
				} break;
			}
		}
		
		
		// tip: call setPos(0) along with trigger()
		void setPos(uint32_t pos)
		{
			this->pos = pos;
		}
		
		// tip: call with 'note'
		// tip: call along with trigger()
		void setPitch(uint32_t pitch)
		{
			this->pitch = pitch;
		}
		
		// tip: call setPitch and setPos together with trigger()
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