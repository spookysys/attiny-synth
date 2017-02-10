#pragma once
#include "common.h"

namespace instr
{
	namespace lib
	{
		
		template<uint16_t attack_p=1000, uint16_t release_p=500, uint16_t sustain_p=0x7FFF, uint16_t decay_p=100, uint16_t peak_p=0xFFFF>
		class Envelope
		{
			enum State
			{
				OFF = 0,
				ATTACK, 
				RELEASE, 
				SUSTAIN,
				DECAY
			} state;
		
			uint16_t vol;
		
		public:
	
			void reset()
			{	
				state = OFF;
				vol = 0;
			}
		
			void trigger()
			{
				if (attack_p) {
					state = ATTACK;
					vol = 0;
				} else if (release_p) {
					state = RELEASE;
					vol = peak_p;
				} else {
					state = SUSTAIN;
					vol = sustain_p;
				}
			}
		
			void release()
			{
				if (state) state = DECAY;
			}
		
			inline uint16_t get()
			{
				return vol;
			}
		
			inline bool isPlaying()
			{
				return state!=OFF;
			}
		
			void next()
			{
				switch (state)
				{
					default:
					case OFF: 
					case SUSTAIN:
						break;
					case ATTACK: {
						uint16_t new_vol = vol + attack_p;
						if ((new_vol < vol) || (new_vol > peak_p)) {
							if (release_p) {
								new_vol = peak_p;
								state = RELEASE;
							} else {
								new_vol = sustain_p;
								state = SUSTAIN;
							}
						}
						vol = new_vol;
					} break;
					case RELEASE: {
						uint16_t new_vol = vol - release_p;
						if ((new_vol <= sustain_p) || (new_vol > vol)) {
							new_vol = sustain_p;
							state = SUSTAIN;
						}
						vol = new_vol;
					} break;
					case DECAY: {
						uint16_t new_vol = vol - decay_p;
						if (new_vol > vol) {
							new_vol = 0;
							state = OFF;
						}
						vol = new_vol;
					} break;
				}
			}
		};
		
	}
}