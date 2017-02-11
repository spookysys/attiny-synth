#pragma once
#include "instr/BassDrum.h"
#include "instr/OneSynth.h"
#include "instr/DrumKit.h"
#include "instr/Fabrikklyd.h"
#include "waveforms/this.h"


class Song 
{
	instr::OneSynth<8, 2> squeek;
	instr::BassDrum<> bd;
	instr::Fabrikklyd<> fabrikklyd;
	instr::DrumKit drumkit;
	
	static int8_t squeek_wf(uint32_t t)
	{
		using namespace waveforms;
		return xfade<9>(
			t, 
			tone<SAW, uint8_t>,
			[](uint8_t t){ 
				return scale3(tone<SQUARE, uint8_t>(t), 4); 
			}
		);
	}
	
public:
	void reset() 
	{
		bd.reset();
		squeek.reset();
		fabrikklyd.reset();
		drumkit.reset();		
	}
	
	inline void render(globals::Mixbuff buff_i, uint32_t pos)
	{
		
		if ((pos & 0x7FF) == 0) {
			bd.trigger();
		}
		
		if ((pos & 0x7FF) == 0x400) {
			/*
			squeek.setPos(0);
			squeek.setPitch(1<<((rand8()&3) + 7));
			squeek.trigger();
			*/
			drumkit.trigger(0);
		}
		
		/*
		if ((pos & 0x7FF) == 0x500) {
			squeek.release();
		}
		*/		

		
		//squeek.render<>(buff_i, squeek_wf);
		bd.render(buff_i);
		drumkit.render<6>(buff_i);
		//fabrikklyd.render<3>(buff_i, pos+0x400, waveforms::oneliner<4, uint32_t>);		
	}
};

