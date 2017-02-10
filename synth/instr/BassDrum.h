#pragma once
#include "common.h"

namespace instr
{
	template<uint16_t start_pitch, uint16_t pitch_speed, uint16_t fade_speed>
	class BassDrum
	{
		enum State {
			OFF = 0,
			SLIDE,
			FADE
		} state;
		
		union {		
			uint16_t pos;
			struct {
				int8_t last_v;
				uint8_t pos_hi;
			};
		};
		
		union {
			uint16_t pitch;
			uint16_t vol;
		};

		inline int8_t slideIter(uint16_t pitch) {
			pos += pitch;
			return tables::bd[pos_hi];
		}
			
		inline int8_t fadeIter(uint8_t scaler) {
			pos_hi++;
			return scale8(tables::bd[pos_hi], scaler);
		}

	public:
		void reset()
		{
			this->state = OFF;
		}

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
					int8_t last_v = slideIter(pitch);
					dest[7] += last_v;
					pitch -= pitch >> pitch_speed;
					pitch--;
					static const uint16_t end_pitch = 0x80;
					if (pitch <= end_pitch) {
						this->vol = 0xFFFF;
						this->last_v = last_v;
						this->state = FADE;
					}
				} break;
				case FADE: {
					uint8_t scaler = (vol>>8);
					int16_t v0 = fadeIter(scaler);
					int16_t v1 = fadeIter(scaler);
					int16_t v2 = fadeIter(scaler);
					int16_t v3 = fadeIter(scaler);
					dest[0] += (this->last_v+v0)>>1;
					dest[1] += v0;
					dest[2] += (v0+v1)>>1;
					dest[3] += v1;
					dest[4] += (v1+v2)>>1;
					dest[5] += v2;
					dest[6] += (v2+v3)>>1;
					dest[7] += v3;
					this->last_v = v3;
					uint16_t old_vol = vol;
					vol -= vol>>fade_speed;
					vol--;
					if (vol > old_vol) this->state = OFF;
				} break;
			}
		}
	};

	
}

