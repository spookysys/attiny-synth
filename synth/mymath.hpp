#pragma once
#include "common.hpp"


namespace mymath
{
	static int8_t mul_s8_s8u8_shr8(int8_t a, uint8_t b)
    {
        return (a*int16_t(b))>>8;
    }

    static int8_t mul_s8_s8s8_shr8(int8_t a, int8_t b)
    {
        return (int16_t(a)*b)>>8;
    }

    static int8_t mul_s8_s16s8_shr8(int16_t a, int8_t b)
    {
        return (a*b)>>8;
    }

}

