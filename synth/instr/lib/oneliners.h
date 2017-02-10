#pragma once
#include "common.h"

namespace instr
{
	
	namespace lib
	{
		// Note: Avoid multiplications and especially modulo/divisions
		namespace oneliners
		{
			inline int8_t sine(uint32_t t)
			{
				return tables::bd[t&0xFF];
			}
		
			// kb 2011-10-04 http://pouet.net/topic.php?which=8357&page=8 44kHz
			int8_t talker(uint32_t t)
			{
				return ((t/2*(15&(0x234568a0>>(t>>8&28)))) | (t/2>>(t>>11)^t>>12)) + (t/16&t&24);
			}
		
		}	
	}
}