#pragma once
#include "instr/include.h"

class Song {
	instr::BassDrum<1000, 7, 8> bd;
public:
	inline void render(uint8_t buff, uint32_t pos)
	{
		if ((pos & 0x7FF) == 0) {
			bd.trigger();
		}
		bd.render(buff);
	}
};

