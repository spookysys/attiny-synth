#include "mymath.hpp"
#include <assert.h>

 
namespace mymath
{

	uint16_t mul_u16_u8u8(uint8_t a, uint8_t b)
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

	uint32_t mul_u24_u16u16(uint16_t a, uint16_t b)
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

}

