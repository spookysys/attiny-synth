#pragma once
#include <stdint.h>

#define likely(x)      __builtin_expect(!!(x), 1)
#define unlikely(x)    __builtin_expect(!!(x), 0)

static const uint8_t mixbuff_len = 8;
extern volatile int16_t mixbuff[2][mixbuff_len];

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
	return 0;
}

