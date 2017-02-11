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

		inline int8_t slideIter() {
			pos += pitch;
			return tables::bd[pos_hi];
		}
			
		inline int8_t fadeIter() {
			pos_hi++;
			vol = vol - scale3(vol>>8, decay_speed) - 1;
			return scale8(tables::bd[pos_hi], vol>>8);
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
			int16_t* dest = globals::mixbuff[buff];
			
			switch(state) {
				case OFF: return;
				case SLIDE: {
					dest[0] += slideIter();
					dest[1] += slideIter();
					dest[2] += slideIter();
					dest[3] += slideIter();
					dest[4] += slideIter();
					dest[5] += slideIter();
					dest[6] += slideIter();
					int8_t v = slideIter();
					dest[7] += v;
					pitch -= pitch >> slide_speed;
					pitch--;
					static const uint16_t end_pitch = 0x80;
					if (pitch <= end_pitch) {
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
					if (!(vol>>8)) this->state = OFF;
					dest[0] += (this->v+v0)>>1;
					dest[1] += v0;
					dest[2] += (v0+v1)>>1;
					dest[3] += v1;
					dest[4] += (v1+v2)>>1;
					dest[5] += v2;
					dest[6] += (v2+v3)>>1;
					dest[7] += v3;
					this->v = v3;
				} break;
			}
		}
	};

	
}

