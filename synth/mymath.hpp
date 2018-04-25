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
    
    static int16_t mul_s16_s16s8_shr8(int16_t a, int8_t b)
    {
        return (int32_t(a)*b)>>8;
    }	

    static uint16_t mul_u16_u16u16_shr16(uint16_t a, uint16_t b)
    {
        return (uint32_t(a)*b)>>16;
    }
	
}

