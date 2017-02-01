#pragma once
#include "instr/include.h"

class Song {
	uint32_t pos = 0;
	instr::BassDrum<5000, 200, 7, 3> bd;
public:
	template<uint8_t buff>
	void render()
	{
		if ((pos & 0x7FF) == 0) {
			bd.trigger(1000);
		}
		bd.render<buff>();

		/*
		volatile int16_t* dest = mixbuff[buff];
		for(int i=0; i<mixbuff_len; i++, pos++) {
			dest[i] += tables::bd[uint8_t(pos)];
		}
		*/
		pos++;
	}
};

