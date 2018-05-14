#include "mymath.hpp"
#include <assert.h>

 
namespace mymath
{

	uint16_t mpy8u(uint8_t a, uint8_t b)
	{
#if !defined(AVR)
		return uint16_t(a) * uint16_t(b);
#else
		uint16_t ret = a;
		asm volatile (
			"mpy8u_%=:\n\t"
			"clr %B0\n\t" // clear result High byte
			"lsr %A0\n\t" // shift result Low byte, multiplier
			"brcc noad80_%=\n\t" // if carry set
			"add %B0, %1\n\t" // add multiplicand to result High byte
			"noad80_%=:\n\t"
			"ror %B0\n\t" // shift right result High byte
			"ror %A0\n\t" // rotate right result L byte and multiplier
			"brcc noad81_%=\n\t" // if carry set
			"add %B0, %1\n\t" // add multiplicand to result High byte
			"noad81_%=:\n\t"
			"ror %B0\n\t" // shift right result High byte
			"ror %A0\n\t" // rotate right result L byte and multiplier
			"brcc noad82_%=\n\t" // if carry set
			"add %B0, %1\n\t" // add multiplicand to result High byte
			"noad82_%=:\n\t"
			"ror %B0\n\t" // shift right result High byte
			"ror %A0\n\t" // rotate right result L byte and multiplier
			"brcc noad83_%=\n\t" // if carry set
			"add %B0, %1\n\t" // add multiplicand to result High byte
			"noad83_%=:\n\t"
			"ror %B0\n\t" // shift right result High byte
			"ror %A0\n\t" // rotate right result L byte and multiplier
			"brcc noad84_%=\n\t" // if carry set
			"add %B0, %1\n\t" // add multiplicand to result High byte
			"noad84_%=:\n\t"
			"ror %B0\n\t" // shift right result High byte
			"ror %A0\n\t" // rotate right result L byte and multiplier
			"brcc noad85_%=\n\t" // if carry set
			"add %B0, %1\n\t" // add multiplicand to result High byte
			"noad85_%=:\n\t"
			"ror %B0\n\t" // shift right result High byte
			"ror %A0\n\t" // rotate right result L byte and multiplier
			"brcc noad86_%=\n\t" // if carry set
			"add %B0, %1\n\t" // add multiplicand to result High byte
			"noad86_%=:\n\t"
			"ror %B0\n\t" // shift right result High byte
			"ror %A0\n\t" // rotate right result L byte and multiplier
			"brcc noad87_%=\n\t" // if carry set
			"add %B0, %1\n\t" // add multiplicand to result High byte
			"noad87_%=:\n\t"
			"ror %B0\n\t" // shift right result High byte
			"ror %A0\n\t" // rotate right result L byte and multiplier
		: "+r"(ret)
		: "r"(b)
		);
		return ret;
#endif
	}

	uint32_t mpy16u(uint16_t a, uint16_t b)
	{
#if !defined(AVR)
		return (uint32_t(a) * uint32_t(b)) & 0x00FFFFFF;
#else
		uint32_t ret;
		asm volatile (
			"mpy16u_%=:\n\t"
			"clr	%D0\n\t" // clear 2 highest bytes of result
			"clr	%C0\n\t"
			"lsr	%B1\n\t" // rotate multiplier Low
			"ror	%A1\n\t" // rotate multiplier High

			"brcc	noadd0_%=\n\t" // if carry set
			"add	%C0,%A2\n\t" //     add multiplicand Low to byte 2 of res
			//"adc	%D0,%B2\n\t" //     add multiplicand high to byte 3 of res
			"noadd0_%=:\n\t"
			//"ror	%D0\n\t" // shift right result byte 3
			"ror	%C0\n\t" // rotate right result byte 2
			"ror	%B0\n\t" // rotate result byte 1 and multiplier High
			"ror	%A0\n\t" // rotate result byte 0 and multiplier Low

			"brcc	noadd1_%=\n\t" // if carry set
			"add	%C0,%A2\n\t" //     add multiplicand Low to byte 2 of res
			//"adc	%D0,%B2\n\t" //     add multiplicand high to byte 3 of res
			"noadd1_%=:\n\t"
			//"ror	%D0\n\t" // shift right result byte 3
			"ror	%C0\n\t" // rotate right result byte 2
			"ror	%B0\n\t" // rotate result byte 1 and multiplier High
			"ror	%A0\n\t" // rotate result byte 0 and multiplier Low

			"brcc	noadd2_%=\n\t" // if carry set
			"add	%C0,%A2\n\t" //     add multiplicand Low to byte 2 of res
			//"adc	%D0,%B2\n\t" //     add multiplicand high to byte 3 of res
			"noadd2_%=:\n\t"
			//"ror	%D0\n\t" // shift right result byte 3
			"ror	%C0\n\t" // rotate right result byte 2
			"ror	%B0\n\t" // rotate result byte 1 and multiplier High
			"ror	%A0\n\t" // rotate result byte 0 and multiplier Low

			"brcc	noadd3_%=\n\t" // if carry set
			"add	%C0,%A2\n\t" //     add multiplicand Low to byte 2 of res
			//"adc	%D0,%B2\n\t" //     add multiplicand high to byte 3 of res
			"noadd3_%=:\n\t"
			//"ror	%D0\n\t" // shift right result byte 3
			"ror	%C0\n\t" // rotate right result byte 2
			"ror	%B0\n\t" // rotate result byte 1 and multiplier High
			"ror	%A0\n\t" // rotate result byte 0 and multiplier Low

			"brcc	noadd4_%=\n\t" // if carry set
			"add	%C0,%A2\n\t" //     add multiplicand Low to byte 2 of res
			//"adc	%D0,%B2\n\t" //     add multiplicand high to byte 3 of res
			"noadd4_%=:\n\t"
			//"ror	%D0\n\t" // shift right result byte 3
			"ror	%C0\n\t" // rotate right result byte 2
			"ror	%B0\n\t" // rotate result byte 1 and multiplier High
			"ror	%A0\n\t" // rotate result byte 0 and multiplier Low

			"brcc	noadd5_%=\n\t" // if carry set
			"add	%C0,%A2\n\t" //     add multiplicand Low to byte 2 of res
			//"adc	%D0,%B2\n\t" //     add multiplicand high to byte 3 of res
			"noadd5_%=:\n\t"
			//"ror	%D0\n\t" // shift right result byte 3
			"ror	%C0\n\t" // rotate right result byte 2
			"ror	%B0\n\t" // rotate result byte 1 and multiplier High
			"ror	%A0\n\t" // rotate result byte 0 and multiplier Low

			"brcc	noadd6_%=\n\t" // if carry set
			"add	%C0,%A2\n\t" //     add multiplicand Low to byte 2 of res
			//"adc	%D0,%B2\n\t" //     add multiplicand high to byte 3 of res
			"noadd6_%=:\n\t"
			//"ror	%D0\n\t" // shift right result byte 3
			"ror	%C0\n\t" // rotate right result byte 2
			"ror	%B0\n\t" // rotate result byte 1 and multiplier High
			"ror	%A0\n\t" // rotate result byte 0 and multiplier Low

			"brcc	noadd7_%=\n\t" // if carry sett
			"add	%C0,%A2\n\t" //     add multiplicand Low to byte 2 of res
			//"adc	%D0,%B2\n\t" //     add multiplicand high to byte 3 of res
			"noadd7_%=:\n\t"
			//"ror	%D0\n\t" // shift right result byte 3
			"ror	%C0\n\t" // rotate right result byte 2
			"ror	%B0\n\t" // rotate result byte 1 and multiplier High
			"ror	%A0\n\t" // rotate result byte 0 and multiplier Low

			"brcc	noadd8_%=\n\t" // if carry set
			"add	%C0,%A2\n\t" //     add multiplicand Low to byte 2 of res
			//"adc	%D0,%B2\n\t" //     add multiplicand high to byte 3 of res
			"noadd8_%=:\n\t"
			//"ror	%D0\n\t" // shift right result byte 3
			"ror	%C0\n\t" // rotate right result byte 2
			"ror	%B0\n\t" // rotate result byte 1 and multiplier High
			"ror	%A0\n\t" // rotate result byte 0 and multiplier Low

			"brcc	noadd9_%=\n\t" // if carry set
			"add	%C0,%A2\n\t" //     add multiplicand Low to byte 2 of res
			//"adc	%D0,%B2\n\t" //     add multiplicand high to byte 3 of res
			"noadd9_%=:\n\t"
			//"ror	%D0\n\t" // shift right result byte 3
			"ror	%C0\n\t" // rotate right result byte 2
			"ror	%B0\n\t" // rotate result byte 1 and multiplier High
			"ror	%A0\n\t" // rotate result byte 0 and multiplier Low

			"brcc	noad10_%=\n\t" // if carry set
			"add	%C0,%A2\n\t" //     add multiplicand Low to byte 2 of res
			//"adc	%D0,%B2\n\t" //     add multiplicand high to byte 3 of res
			"noad10_%=:\n\t"
			//"ror	%D0\n\t" // shift right result byte 3
			"ror	%C0\n\t" // rotate right result byte 2
			"ror	%B0\n\t" // rotate result byte 1 and multiplier High
			"ror	%A0\n\t" // rotate result byte 0 and multiplier Low

			"brcc	noad11_%=\n\t" // if carry set
			"add	%C0,%A2\n\t" //     add multiplicand Low to byte 2 of res
			//"adc	%D0,%B2\n\t" //     add multiplicand high to byte 3 of res
			"noad11_%=:\n\t"
			//"ror	%D0\n\t" // shift right result byte 3
			"ror	%C0\n\t" // rotate right result byte 2
			"ror	%B0\n\t" // rotate result byte 1 and multiplier High
			"ror	%A0\n\t" // rotate result byte 0 and multiplier Low

			"brcc	noad12_%=\n\t" // if carry set
			"add	%C0,%A2\n\t" //     add multiplicand Low to byte 2 of res
			//"adc	%D0,%B2\n\t" //     add multiplicand high to byte 3 of res
			"noad12_%=:\n\t"
			//"ror	%D0\n\t" // shift right result byte 3
			"ror	%C0\n\t" // rotate right result byte 2
			"ror	%B0\n\t" // rotate result byte 1 and multiplier High
			"ror	%A0\n\t" // rotate result byte 0 and multiplier Low

			"brcc	noad13_%=\n\t" // if carry set
			"add	%C0,%A2\n\t" //     add multiplicand Low to byte 2 of res
			//"adc	%D0,%B2\n\t" //     add multiplicand high to byte 3 of res
			"noad13_%=:\n\t"
			//"ror	%D0\n\t" // shift right result byte 3
			"ror	%C0\n\t" // rotate right result byte 2
			"ror	%B0\n\t" // rotate result byte 1 and multiplier High
			"ror	%A0\n\t" // rotate result byte 0 and multiplier Low

			"brcc	noad14_%=\n\t" // if carry set
			"add	%C0,%A2\n\t" //     add multiplicand Low to byte 2 of res
			//"adc	%D0,%B2\n\t" //     add multiplicand high to byte 3 of res
			"noad14_%=:\n\t"
			//"ror	%D0\n\t" // shift right result byte 3
			"ror	%C0\n\t" // rotate right result byte 2
			"ror	%B0\n\t" // rotate result byte 1 and multiplier High
			"ror	%A0\n\t" // rotate result byte 0 and multiplier Low

			"brcc	noad15_%=\n\t" // if carry set
			"add	%C0,%A2\n\t" //     add multiplicand Low to byte 2 of res
			//"adc	%D0,%B2\n\t" //     add multiplicand high to byte 3 of res
			"noad15_%=:\n\t"
			//"ror	%D0\n\t" // shift right result byte 3
			"ror	%C0\n\t" // rotate right result byte 2
			"ror	%B0\n\t" // rotate result byte 1 and multiplier High
			"ror	%A0\n\t" // rotate result byte 0 and multiplier Low
		: "=r"(ret)
		: "r"(a), "r"(b)
		);
		return ret;
#endif
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

