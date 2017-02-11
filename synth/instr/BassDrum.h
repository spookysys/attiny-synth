#pragma once
#include "common.h"

namespace instr
{
	template<uint16_t start_pitch=1000, uint16_t slide_speed=6, uint16_t decay_speed=1>
	class BassDrum
	{
		enum State {
			OFF = 0,
			SLIDE,
			DECAY
		} state;
		
		union {		
			uint16_t pos;
			struct {
				int8_t v;
				uint8_t pos_hi;
			};
		};
		
		union {
			uint16_t pitch;
			uint16_t vol;
		};


		int8_t fadeIter() {
			vol -= scale3(vol>>8, decay_speed);
			int8_t v = pgm_read_byte(&tables::bd[++pos_hi]);
			return scale8_s8(v, vol>>8);
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

		void render(globals::Mixbuff buff_i)
		{
			static_assert(sizeof(tables::bd)==256, "Unexpected table size");
			static_assert(globals::mixbuff_len == 8, "Unexpected mix-buffer length");
			int16_t* dest = globals::mixbuff[buff_i];
			
			switch(state) {
				case OFF: return;
				case SLIDE: {
					dest[0] += (int8_t)pgm_read_byte(&tables::bd[pos>>8]); pos += pitch;
					dest[1] += (int8_t)pgm_read_byte(&tables::bd[pos>>8]); pos += pitch;
					dest[2] += (int8_t)pgm_read_byte(&tables::bd[pos>>8]); pos += pitch;
					dest[3] += (int8_t)pgm_read_byte(&tables::bd[pos>>8]); pos += pitch;
					dest[4] += (int8_t)pgm_read_byte(&tables::bd[pos>>8]); pos += pitch;
					dest[5] += (int8_t)pgm_read_byte(&tables::bd[pos>>8]); pos += pitch;
					dest[6] += (int8_t)pgm_read_byte(&tables::bd[pos>>8]); pos += pitch;
					int8_t v = (int8_t)pgm_read_byte(&tables::bd[pos>>8]); pos += pitch;
					dest[7] += v;
					pitch -= scale3(pitch>>6, slide_speed);
					if (pitch < 0x80) {
						this->vol = 0xFFFF;
						this->v = v;
						this->state = DECAY;
					}
				} break;
				case DECAY: {
					int16_t v0 = fadeIter();
					int16_t v1 = fadeIter();
					int16_t v2 = fadeIter();
					int16_t v3 = fadeIter();
					dest[0] += (this->v+v0)>>1;
					dest[1] += v0;
					dest[2] += (v0+v1)>>1;
					dest[3] += v1;
					dest[4] += (v1+v2)>>1;
					dest[5] += v2;
					dest[6] += (v2+v3)>>1;
					dest[7] += v3;
					this->v = v3;
					if (!(vol>>8)) this->state = OFF;
				} break;
			}
		}
	};

	
}

