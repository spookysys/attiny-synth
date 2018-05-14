#include "mymath.hpp"
#include <assert.h>

 
namespace mymath
{

	uint16_t mpy8u(uint8_t a, uint8_t b)
	{
		return uint16_t(a) * uint16_t(b);
	}

	uint32_t mpy16u(uint16_t a, uint16_t b)
	{
		return uint32_t(a) * uint32_t(b);
	}

	// via mpy8u
	uint8_t mul_u8_u8u8_shr8(uint8_t a, uint8_t b)
	{
		return mpy8u(a, b) >> 8;
	}

	// via mpy16u
	uint8_t mul_u16_u16u16_shr16(uint16_t a, uint16_t b)
	{
		return mpy16u(a, b) >> 16;
	}

	// via mpy16u
	uint8_t mul_u16_u16u16_shr8(uint16_t a, uint16_t b)
	{
		return mpy16u(a, b) >> 8;
	}

	uint8_t mul_u16_u16u8_shr8(uint16_t a, uint8_t b)
	{
		return mul_u16_u16u16_shr8(a, b);
	}

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

	
	void testme()
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

