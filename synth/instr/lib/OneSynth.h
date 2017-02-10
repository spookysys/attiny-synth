#pragma once
#include "common.h"


namespace instr
{
	namespace lib {

		typedef int8_t(*oneliner_t)(uint32_t);

		template<int shr, oneliner_t onelinerFunc, typename Env>
		class OneSynth
		{
			Env env;
			uint32_t pos;
			uint16_t pitch;
		public:
			void reset()
			{
				env.reset();
				pos = 0;
				pitch = 0;
			}
			void render(uint8_t buff)
			{
				volatile int16_t* dest = globals::mixbuff[buff];
				if (!env.isPlaying()) return;
				uint16_t vol = env.get()>>8;
				dest[0] += scale8(onelinerFunc((pos+=pitch)>>shr), vol);
				dest[1] += scale8(onelinerFunc((pos+=pitch)>>shr), vol);
				dest[2] += scale8(onelinerFunc((pos+=pitch)>>shr), vol);
				dest[3] += scale8(onelinerFunc((pos+=pitch)>>shr), vol);
				dest[4] += scale8(onelinerFunc((pos+=pitch)>>shr), vol);
				dest[5] += scale8(onelinerFunc((pos+=pitch)>>shr), vol);
				dest[6] += scale8(onelinerFunc((pos+=pitch)>>shr), vol);
				dest[7] += scale8(onelinerFunc((pos+=pitch)>>shr), vol);
				env.next();
			}
		
			void setPitch(uint16_t pitch)
			{
				this->pitch = pitch;
			}
		
			void setPos(uint32_t pos)
			{
				this->pos = pos;
			}
			
			void trigger()
			{
				env.trigger();
			}
			
			void release()
			{
				env.release();
			}
		};

	}
}