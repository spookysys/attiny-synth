#pragma once

#define STEP_SIZE 8
using mix_t = std::array<int16_t, STEP_SIZE>;
using stream_t = std::array<uint8_t, STEP_SIZE>;

// range of scale: 0-8 (maps to 0.-1.)
static inline int8_t scale(int8_t val, int8_t scale)
{
	switch (scale) {
		case 0: return 0;
		case 1: return val>>3;
		case 2: return val>>2;
		case 3: return (val>>1) - (val>>3);
		case 4: return val>>1;
		case 5: return (val>>1) + (val>>3);
		case 6: return val - (val>>2);
		case 7: return val - (val>>3);
		case 8: return val;
	}
}
