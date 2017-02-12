#pragma once
#include "common.h"

namespace samples
{
	struct Sample
	{
		const uint16_t len;
		const uint8_t* data;
	};
	
	static const uint8_t num_samples = 1;
	extern const Sample samples[num_samples];
	
}
