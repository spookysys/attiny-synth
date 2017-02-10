#pragma once
#include "instr/BassDrum.h"
#include "instr/Bass.h"

class Song {
	instr::BassDrum<1000, 6, 7> bd;
	instr::Bass bass;
public:
	void reset() 
	{
		bd.reset();
		bass.reset();
	}
	inline void render(uint8_t buff, uint32_t pos)
	{
		if ((pos & 0x7FF) == 0) {
			bd.trigger();
		}
		if ((pos & 0x7FF) == 0x400) {
			bass.trigger(rand8() + 200);
		}
		if ((pos & 0x7FF) == 0x700) {
			bass.release();
		}

		bass.render(buff);
		bd.render(buff);
	}
};

