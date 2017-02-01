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

			if (vol==0) return;
			
			int16_t step = (pitch>>pitch_speed)+1;
			
			if (pitch >= int16_t(lo_pitch)) {
				dest[0] += iter();
				dest[1] += iter();
				dest[2] += iter();
				dest[3] += iter();
				dest[4] += iter();
				dest[5] += iter();
				dest[6] += iter();
				dest[7] += iter();
				pitch -= step;
			} else if (vol > 0) {
				uint8_t scaler = (vol>>4);
				dest[0] += (int16_t(iter())*scaler)>>8;
				dest[1] += (int16_t(iter())*scaler)>>8;
				dest[2] += (int16_t(iter())*scaler)>>8;
				dest[3] += (int16_t(iter())*scaler)>>8;
				dest[4] += (int16_t(iter())*scaler)>>8;
				dest[5] += (int16_t(iter())*scaler)>>8;
				dest[6] += (int16_t(iter())*scaler)>>8;
				dest[7] += (int16_t(iter())*scaler)>>8;
				vol -= vol_speed;
			}
		}
	};

	
}

