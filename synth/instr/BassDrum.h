#pragma once
#include "common.h"
#include "tables.h"

namespace instr
{
	template<uint16_t hi_pitch, uint16_t lo_pitch, uint16_t pitch_speed, uint16_t vol_speed>
	struct BassDrum
	{
		int16_t pitch;
		int16_t vol;
		uint16_t pos;
		
		inline int8_t iter() {
			static_assert(sizeof(tables::bd)==256, "Unexpected table size");
			pos += pitch;
			return tables::bd[pos>>8];
		}

	public:

		void trigger(uint16_t pitch)
		{
			this->pitch = int16_t(pitch);
			this->vol = 0xFFF;
		}

		template<uint8_t buff>
		void render()
		{
			static_assert(mixbuff_len == 8, "Unexpected mix-buffer length");
			volatile int16_t* dest = mixbuff[buff];
			
			if (pitch >= int16_t(lo_pitch)) {
				dest[0] += iter();
				dest[1] += iter();
				dest[2] += iter();
				dest[3] += iter();
				dest[4] += iter();
				dest[5] += iter();
				dest[6] += iter();
				dest[7] += iter();
				pitch -= (pitch>>pitch_speed)+1;
			} else if (vol > 0) {
				uint8_t scaler = (vol>>4);
				dest[0] += bulk_scale(iter(), scaler);
				dest[1] += bulk_scale(iter(), scaler);
				dest[2] += bulk_scale(iter(), scaler);
				dest[3] += bulk_scale(iter(), scaler);
				dest[4] += bulk_scale(iter(), scaler);
				dest[5] += bulk_scale(iter(), scaler);
				dest[6] += bulk_scale(iter(), scaler);
				dest[7] += bulk_scale(iter(), scaler);
				vol -= (vol>>vol_speed)+1;
			}
		}
	};

	
}

