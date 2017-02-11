#pragma once

namespace instr
{
	
	class OneGroove
	{
		template<typename pos_t>
		inline uint8_t eval_inner(uint8_t sel, pos_t t)
		{
			switch (sel)
			{
				case 0: return t&t>>8;
				case 1: return t>>6&1?t>>5:-t>>4;
				case 2: return (t>>(t&7))|(t<<(t&42))|(t>>7)|(t<<5);
				case 3: return ((t>>6|t<<1)+(t>>5|t<<3|t>>3))|t>>2|t<<1;
				case 4: return (t>>5)|(t<<4)|((t&1023)^1981)|((t-67)>>4);
				case 5: return (((t+(t>>2))|(t>>5))+(t>>3))|((t>>13)|(t>>7)|(t>>11));
				case 6: return t*(42&t>>10);
				default: return 0;
			}
		}
		template<typename pos_t>
		inline int8_t eval(uint8_t sel, pos_t t)
		{
			return int32_t(eval_inner(sel, t) - 0x80)>>2;
		}
		
	public:

		template<typename pos_t>
		void render_hires(uint8_t buff, uint8_t sel, pos_t t)
		{
			volatile int16_t* dest = globals::mixbuff[buff];
			t <<= 3;
			dest[0] += eval(sel, t++);
			dest[1] += eval(sel, t++);
			dest[2] += eval(sel, t++);
			dest[3] += eval(sel, t++);
			dest[4] += eval(sel, t++);
			dest[5] += eval(sel, t++);
			dest[6] += eval(sel, t++);
			dest[7] += eval(sel, t++);
		}
		
		template<typename pos_t>
		void render_lores(uint8_t buff, uint8_t sel, pos_t t)
		{
			volatile int16_t* dest = globals::mixbuff[buff];
			int8_t v = eval(sel, t);
			dest[0] += v;
			dest[1] += v;
			dest[2] += v;
			dest[3] += v;
			dest[4] += v;
			dest[5] += v;
			dest[6] += v;
			dest[7] += v;
		}
		
		
		template<typename pos_t>
		void render_midres(uint8_t buff, uint8_t sel, pos_t t)
		{
			volatile int16_t* dest = globals::mixbuff[buff];
			t <<= 2;
			int8_t v0 = eval(sel, t);
			int8_t v1 = eval(sel, t+1);
			int8_t v2 = eval(sel, t+2);
			int8_t v3 = eval(sel, t+3);
			dest[0] += v0;
			dest[1] += v0;
			dest[2] += v1;
			dest[3] += v1;
			dest[4] += v2;
			dest[5] += v2;
			dest[6] += v3;
			dest[7] += v3;
		}
				
	};
}