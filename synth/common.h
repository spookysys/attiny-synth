#pragma once
#include <stdint.h>
#include "tables.h"

#define likely(x)      __builtin_expect(!!(x), 1)
#define unlikely(x)    __builtin_expect(!!(x), 0)

namespace globals {	
	static const uint8_t mixbuff_len = 8;
	extern volatile int16_t mixbuff[2][mixbuff_len];
	static const uint32_t sample_rate = 22000;
}


// hint: Try to call the scale functions repeatedly with the same variable for scale.
// Compiler can then move the if/switch statements to the outermost scope and give a big speedup


// range of 3-bit scale: [0:7] maps to [1/8 : 1]
template<typename T>
static inline T scale3(T val, int8_t scale)
{
	switch (scale) {
		case 0: return val>>3;
		case 1: return val>>2;
		case 2: return (val>>1) - (val>>3);
		case 3: return val>>1;
		case 4: return (val>>1) + (val>>3);
		case 5: return val - (val>>2);
		case 6: return val - (val>>3);
		case 7: return val;
		default:return 0;
	}
	
}


// range of scale: [0:255] maps to [1/256 : 1]
template<typename T>
static inline T scale8(T val, uint8_t scaler)
{
#if 0
	uint16_t scaler2 = uint16_t(scaler)+1;
	return (int32_t(val)*scaler2)>>8;
#else
	if (scaler==0xFF) return val;
	scaler++; // 0 is not interesting
	int32_t res = 0;
	if (val & 0xF0) {
		if (scaler & 0x01) res += val;
		if (scaler & 0x02) res += val << 1;
		if (scaler & 0x04) res += val << 2;
		if (scaler & 0x08) res += val << 3;
	}
	if (scaler & 0xF0) {
		if (scaler & 0x10) res += val << 4;
		if (scaler & 0x20) res += val << 5;
		if (scaler & 0x40) res += val << 6;
		if (scaler & 0x80) res += val << 7;
	}
	return res>>8; // TODO: Rewrite this routine to kill this shift
#endif
}


// t range 0-15
static inline int8_t lerp3(int8_t v0, int8_t v1, uint8_t t)
{
	int16_t u0 = v0;
	int16_t u1 = v1;
	switch (t) {
		case 0: return u0;
		case 1: return (u0*7 + u1) >> 3;
		case 2: return (u0*3 + u1) >> 2;
		case 3: return (u0*5 + u1*3) >> 3;
		case 4: return (v0+v1)>>1;
		case 5: return (u0*3 + u1*5) >> 3;
		case 6: return (u0   + u1*3) >> 2;
		case 7: return (u0   + u1*7) >> 3;
		default: return 0;
	}
}



// from http://codebase64.org/doku.php?id=base:small_fast_8-bit_prng
static inline uint8_t rand8()
{
	static uint8_t seed = 0;
	uint8_t carry = seed>>7;
	seed <<= 1;
	if ((seed && carry) || unlikely(!seed && !carry)) seed ^= 0x1d;
	return seed;
}
