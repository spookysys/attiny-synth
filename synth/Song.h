#pragma once
#include "instr/BassDrum.h"
#include "instr/OneSynth.h"
#include "instr/Fabrikklyd.h"
#include "instr/waveforms.h"

class Song 
{
	instr::OneSynth squeek;
	instr::BassDrum<1000, 6, 1> bd;
	instr::Fabrikklyd fabrikklyd;
	
	static int8_t squeek_wf(uint32_t t)
	{
		using namespace instr::wfs;
		return xfade<9>(
			t, 
			tone<SAW>,
			[](uint32_t t){ 
				return scale3(tone<SQUARE>(t), 4); 
			}
		);
	}
	
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
		
		if ((pos & 0x7FF) == 0x400) {
			squeek.setPos(0);
			squeek.setPitch(200+rand8());
			squeek.trigger();
		}
		
		if ((pos & 0x7FF) == 0x700) {
			squeek.release();
		}		

		
		squeek.render<8, 31>(buff, squeek_wf);
		bd.render(buff);
		//og.render_midres(buff, 5, pos+0x200);
	}
};

