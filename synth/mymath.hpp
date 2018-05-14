#pragma once
#include "common.hpp"


namespace mymath
{
	int8_t mul_s8_s8u8_shr8(int8_t a, uint8_t b);
    int8_t mul_s8_s8s8_shr8(int8_t a, int8_t b);
    int16_t mul_s16_s16s8_shr8(int16_t a, int8_t b);
	void testme();
}

