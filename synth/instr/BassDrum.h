#pragma once
#include "common.h"
#include "tables.h"

namespace instr
{
	template<uint16_t start_pitch, uint16_t pitch_speed, uint16_t end_pitch, uint16_t fade_speed>
	struct BassDrum
	{
		enum State {
			OFF = 0,
			SLIDE,
			FADE
		};
		
		State state = OFF;
		uint16_t pos = 0;
		
		union {
			uint16_t pitch;
			uint16_t vol;
		};


		inline int8_t slideIter(uint16_t pitch) {
			pos += pitch;
			return tables::bd[pos>>8];
		}
			
		inline int8_t fadeIter(uint16_t pitch, uint8_t scaler) {
			pos += pitch;
			return scale8(tables::bd[pos>>8], scaler);
		}

	public:

		void trigger()
		{
			pitch = start_pitch;
			state = SLIDE;
		}

		void render(uint8_t buff)
		{
			static_assert(sizeof(tables::bd)==256, "Unexpected table size");
			static_assert(globals::mixbuff_len == 8, "Unexpected mix-buffer length");
			volatile int16_t* dest = globals::mixbuff[buff];
			
			switch(state) {
				case OFF: return;
				case SLIDE: {
					dest[0] += slideIter(pitch);
					dest[1] += slideIter(pitch);
					dest[2] += slideIter(pitch);
					dest[3] += slideIter(pitch);
					dest[4] += slideIter(pitch);
					dest[5] += slideIter(pitch);
					dest[6] += slideIter(pitch);
					dest[7] += slideIter(pitch);
					uint16_t old_pitch = pitch;
					pitch -= pitch >> pitch_speed;
					pitch--;
					if (pitch > old_pitch || pitch <= end_pitch) {
						vol = 0xFFF;
						state = FADE;
					}
				}
				break;
				case FADE:
					uint8_t scaler = (vol>>4);
					dest[0] += fadeIter(end_pitch, scaler);
					dest[1] += fadeIter(end_pitch, scaler);
					dest[2] += fadeIter(end_pitch, scaler);
					dest[3] += fadeIter(end_pitch, scaler);
					dest[4] += fadeIter(end_pitch, scaler);
					dest[5] += fadeIter(end_pitch, scaler);
					dest[6] += fadeIter(end_pitch, scaler);
					dest[7] += fadeIter(end_pitch, scaler);
					uint16_t old_vol = vol;
					vol -= vol>>fade_speed;
					vol--;
					if (vol > old_vol) {
						state = OFF;
					}
				break;
			}
		}
	};

	
}

