#include "mymath.hpp"
#include <assert.h>

 
namespace mymath
{
	uint16_t mul_u16_u8u8(uint8_t a, uint8_t b)
	{
#if !defined(AVR)
		return uint16_t(a) * uint16_t(b);
#else
		uint16_t ret = b;
		asm volatile (
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
		: "r"(a)
		);
		return ret;
#endif
	}

	uint32_t mul_u24_u16u8(uint16_t a, uint8_t b)
	{
#if !defined(AVR)
		return (uint32_t(a) * uint32_t(b)) & 0x00FFFFFF;
#else
		uint32_t ret = b;
		asm volatile (
			"lsr	%A0\n\t" // rotate multiplier
			"brcc	noadd0_%=\n\t" // if carry set
			"add	%B0,%A1\n\t" //     add multiplicand Low to byte 1 of res
			"adc	%C0,%B1\n\t" //     add multiplicand high to byte 2 of res
			"noadd0_%=:\n\t"
			
			"lsr	%C0\n\t" // shift right result byte 2
			"ror	%B0\n\t" // rotate result byte 1
			"ror	%A0\n\t" // rotate result byte 0 and multiplier
			"brcc	noadd1_%=\n\t" // if carry set
			"add	%B0,%A1\n\t" //     add multiplicand Low to byte 1 of res
			"adc	%C0,%B1\n\t" //     add multiplicand high to byte 2 of res
			"noadd1_%=:\n\t"

			"lsr	%C0\n\t" // shift right result byte 2
			"ror	%B0\n\t" // rotate result byte 1
			"ror	%A0\n\t" // rotate result byte 0 and multiplier
			"brcc	noadd2_%=\n\t" // if carry set
			"add	%B0,%A1\n\t" //     add multiplicand Low to byte 1 of res
			"adc	%C0,%B1\n\t" //     add multiplicand high to byte 2 of res
			"noadd2_%=:\n\t"

			"lsr	%C0\n\t" // shift right result byte 2
			"ror	%B0\n\t" // rotate result byte 1
			"ror	%A0\n\t" // rotate result byte 0 and multiplier
			"brcc	noadd3_%=\n\t" // if carry set
			"add	%B0,%A1\n\t" //     add multiplicand Low to byte 1 of res
			"adc	%C0,%B1\n\t" //     add multiplicand high to byte 2 of res
			"noadd3_%=:\n\t"

			"lsr	%C0\n\t" // shift right result byte 2
			"ror	%B0\n\t" // rotate result byte 1
			"ror	%A0\n\t" // rotate result byte 0 and multiplier
			"brcc	noadd4_%=\n\t" // if carry set
			"add	%B0,%A1\n\t" //     add multiplicand Low to byte 1 of res
			"adc	%C0,%B1\n\t" //     add multiplicand high to byte 2 of res
			"noadd4_%=:\n\t"

			"lsr	%C0\n\t" // shift right result byte 2
			"ror	%B0\n\t" // rotate result byte 1
			"ror	%A0\n\t" // rotate result byte 0 and multiplier
			"brcc	noadd5_%=\n\t" // if carry set
			"add	%B0,%A1\n\t" //     add multiplicand Low to byte 2 of res
			"adc	%C0,%B1\n\t" //     add multiplicand high to byte 3 of res
			"noadd5_%=:\n\t"

			"lsr	%C0\n\t" // shift right result byte 2
			"ror	%B0\n\t" // rotate result byte 1
			"ror	%A0\n\t" // rotate result byte 0 and multiplier
			"brcc	noadd6_%=\n\t" // if carry set
			"add	%B0,%A1\n\t" //     add multiplicand Low to byte 2 of res
			"adc	%C0,%B1\n\t" //     add multiplicand high to byte 3 of res
			"noadd6_%=:\n\t"

			"lsr	%C0\n\t" // shift right result byte 2
			"ror	%B0\n\t" // rotate result byte 1
			"ror	%A0\n\t" // rotate result byte 0 and multiplier
			"brcc	noadd7_%=\n\t" // if carry set
			"add	%B0,%A1\n\t" //     add multiplicand Low to byte 2 of res
			"adc	%C0,%B1\n\t" //     add multiplicand high to byte 3 of res
			"noadd7_%=:\n\t"

			"lsr	%C0\n\t" // shift right result byte 2
			"ror	%B0\n\t" // rotate result byte 1
			"ror	%A0\n\t" // rotate result byte 0
		: "=r"(ret)
		: "r"(a)
		);
		return ret;
#endif
	}


	// mulhi wrappers

    uint8_t mulhi_u8u8(uint8_t a, uint8_t b)
    {
        return mul_u16_u8u8(a, b) >> (sizeof(b) * 8);
    }

    uint16_t mulhi_u16u8(uint16_t a, uint8_t b)
    {
        return mul_u24_u16u8(a, b) >> (sizeof(b) * 8);
    }



	// signed wrappers

    int8_t mulhi_s8s8(int8_t a, int8_t b)
    {
        int8_t s = a ^ b;
        if (a < 0)
            a = -a;
        if (b < 0)
            b = -b;
        int8_t r = mulhi_u8u8(a, b);
        if (s < 0)
            return -r;
        else
            return r;
    }

    int8_t mulhi_s8u8(int8_t a, uint8_t b)
    {
        if (a < 0)
            return -mulhi_u8u8(-a, b);
        else
            return mulhi_u8u8(a, b);
    }

    int16_t mulhi_s16s8(int16_t a, int8_t b)
    {
        int8_t s = (a >> 8) ^ b;
        if (a < 0)
            a = -a;
        if (b < 0)
            b = -b;
        int8_t r = mulhi_u16u8(a, b);
        if (s < 0)
            return -r;
        else
            return r;
    }

}

