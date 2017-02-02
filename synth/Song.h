#pragma once
#include "instr/include.h"

class Song {
	instr::BassDrum<31000, 150, 7, 7> bd;
public:
	template<uint8_t buff>
	void render(uint32_t pos)
	{
		if ((pos & 0x7FF) == 0) {
			bd.trigger(1000);
		}
		bd.render<buff>();
	}
};

