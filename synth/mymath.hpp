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

    static int16_t mul_s16_s16s8_shr8(int16_t a, int8_t b)
    {
        return (int32_t(a)*b)>>8;
    }	

	
	static void testme()
	{
		int32_t test;

		test = mul_s8_s8u8_shr8(0x10, 0x10);
		assert(test == 0x01);
		test = mul_s8_s8u8_shr8(-0x10, 0x10);
		assert(test == -0x01);
		test = mul_s8_s8u8_shr8(0x10, 0x80);
		assert(test == 0x08);
		test = mul_s8_s8u8_shr8(-0x10, 0x80);
		assert(test == -0x08);
		test = mul_s8_s8u8_shr8(0x10, 0xFF);
		assert(test == 0x0F);
		test = mul_s8_s8u8_shr8(-0x10, 0xFF);
		assert(test == -0x10);
		

		test = mul_s8_s8s8_shr8(0x10, 0x10);
		assert(test == 0x01);
		test = mul_s8_s8s8_shr8(-0x10, -0x10);
		assert(test == 0x01);
		test = mul_s8_s8s8_shr8(-0x10, 0x10);
		assert(test == -0x01);
		test = mul_s8_s8s8_shr8(0x10, -0x10);
		assert(test == -0x01);
		test = mul_s8_s8s8_shr8(0x10, -128);
		assert(test == -0x08);
		test = mul_s8_s8s8_shr8(-0x10, -128);
		assert(test == 0x08);
		test = mul_s8_s8s8_shr8(0x10, 0xFF);
		assert(test == -1);
		test = mul_s8_s8s8_shr8(-0x10, 0xFF);
		assert(test == 0);
	}
	
}

