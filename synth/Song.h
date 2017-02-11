#pragma once
#include "instr/BassDrum.h"
#include "instr/OneSynth.h"
#include "instr/Fabrikklyd.h"
#include "instr/waveforms.h"

class Song 
{
	instr::OneSynth<8, 31> squeek;
	instr::BassDrum<> bd;
	instr::Fabrikklyd<> fabrikklyd;
	
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
		fabrikklyd.reset();
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
		
		if ((pos & 0x7FF) == 0x500) {
			squeek.release();
		}		

		
		squeek.render<>(buff, squeek_wf);
		bd.render(buff);
		
		// fabrikklyd med sidechain-kompressor
		fabrikklyd.render<3>(buff, pos+0x400, instr::wfs::oneliner<4>);
	}
};

