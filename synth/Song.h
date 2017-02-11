#pragma once
#include "instr/BassDrum.h"
#include "instr/OneSynth.h"
#include "instr/OneGroove.h"

class Song {
	
	static int8_t bass_waveform(uint32_t t)
	{ 
		//return tables::bd[t&0xFF] >> 1;
		//return int8_t(t&t>>8)>>1; 
		//return t>>6&1?t>>5:-t>>4;
		return (t>>(t&7))|(t<<(t&42))|(t>>7)|(t<<5);
		//return ((t>>6|t<<1)+(t>>5|t<<3|t>>3))|t>>2|t<<1;
		//return (t>>5)|(t<<4)|((t&1023)^1981)|((t-67)>>4);
		//return (((t+(t>>2))|(t>>5))+(t>>3))|((t>>13)|(t>>7)|(t>>11));
		//return t*(42&t>>10);
	}
	
	instr::OneSynth<bass_waveform, 4, 31> squeek;
	instr::BassDrum<1000, 6, 1> bd;
	instr::OneGroove og;
	
public:
	void reset() 
	{
		bd.reset();
		squeek.reset();
	}
	inline void render(uint8_t buff, uint32_t pos)
	{
		if ((pos & 0x7FF) == 0) {
			bd.trigger();
		}
		
		if ((pos & 0x7FF) == 0x200) {
			squeek.setPitch(200+rand8());
			squeek.trigger();
		}
		
		if ((pos & 0x7FF) == 0) {
			squeek.release();
		}		

		squeek.render(buff);
		bd.render(buff);
		//og.render_midres(buff, 5, pos+0x200);
	}
};

