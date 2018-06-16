#pragma once
#include "common.hpp"


namespace mymath
{
    // native
    uint16_t mul_u16_u8u8(uint8_t a, uint8_t b);
    uint32_t mul_u24_u16u16(uint16_t a, uint16_t b);

    // right-shifted
    uint8_t mul_u8_u8u8_shr8(uint8_t a, uint8_t b)
    {
        return mul_u16_u8u8(a, b) >> 8;
    }
    uint16_t mul_u16_u16u16_shr8(uint16_t a, uint16_t b)
    {
        return mul_u24_u16u16(a, b) >> 8;
    }

    // signed
    int8_t mul_s8_s8s8_shr8(int8_t a, int8_t b)
    {
		int8_t s = a^b;
		if (a<0) a = -a;
		if (b<0) b = -b;
		if (s<0)
			return -mul_u8_u8u8_shr8(a, b);
		else
			return mul_u8_u8u8_shr8(a, b);
    }
	int8_t mul_s8_s8u8_shr8(int8_t a, uint8_t b)
    {
		if (a<0)
			return -mul_u8_u8u8_shr8(-a, b);
		else
			return mul_u8_u8u8_shr8(a, b);
    }
    int16_t mul_s16_s16s8_shr8(int16_t a, int8_t b)
    {
		int8_t s = (a>>8)^b;
		if (a<0) a = -a;
		if (b<0) b = -b;
		if (s<0)
			return -mul_u16_u16u16_shr8(a, b);
		else
			return mul_u16_u16u16_shr8(a, b);
    }	

}

