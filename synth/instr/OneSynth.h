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
		
		template<typename WaveformFunc>
		int8_t iter(WaveformFunc waveform_func)
		{
			return waveform_func((pos += pitch) >> shr);
		}
	public:
		void reset()
		{
			pos = 0;
			vol = 0;
			state = OFF;
		}
			

		template<typename WaveformFunc>
		void render(uint8_t buff, WaveformFunc waveform_func)
		{
			int16_t* dest = globals::mixbuff[buff];
			switch (state) {
				case OFF: return;
				case RAMP:
					// dest[0] += 0;
					dest[1] += scale3(iter(waveform_func), 0);
					dest[2] += scale3(iter(waveform_func), 1);
					dest[3] += scale3(iter(waveform_func), 2);
					dest[4] += scale3(iter(waveform_func), 3);
					dest[5] += scale3(iter(waveform_func), 4);
					dest[6] += scale3(iter(waveform_func), 5);
					dest[7] += scale3(iter(waveform_func), 6);
					state = SUSTAIN;
					break;
				case SUSTAIN:
					dest[0] += iter(waveform_func);
					dest[1] += iter(waveform_func);
					dest[2] += iter(waveform_func);
					dest[3] += iter(waveform_func);
					dest[4] += iter(waveform_func);
					dest[5] += iter(waveform_func);
					dest[6] += iter(waveform_func);
					dest[7] += iter(waveform_func);
					break;
				case DECAY: {
					dest[0] += scale8(iter(waveform_func), vol>>8);
					dest[1] += scale8(iter(waveform_func), vol>>8);
					dest[2] += scale8(iter(waveform_func), vol>>8);
					dest[3] += scale8(iter(waveform_func), vol>>8);
					dest[4] += scale8(iter(waveform_func), vol>>8);
					dest[5] += scale8(iter(waveform_func), vol>>8);
					dest[6] += scale8(iter(waveform_func), vol>>8);
					dest[7] += scale8(iter(waveform_func), vol>>8);
					vol -= scale8(vol>>8, decay_speed)<<4;
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