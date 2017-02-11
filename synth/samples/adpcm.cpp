#include "adpcm.h"
#include <avr/pgmspace.h>

const signed char indexTable[16] = 
{
    -1, -1, -1, -1, 2, 4, 6, 8,
    -1, -1, -1, -1, 2, 4, 6, 8,
};

const unsigned short stepsizeTable[89] PROGMEM = 
{
    7, 8, 9, 10, 11, 12, 13, 14, 16, 17,
    19, 21, 23, 25, 28, 31, 34, 37, 41, 45,
    50, 55, 60, 66, 73, 80, 88, 97, 107, 118,
    130, 143, 157, 173, 190, 209, 230, 253, 279, 307,
    337, 371, 408, 449, 494, 544, 598, 658, 724, 796,
    876, 963, 1060, 1166, 1282, 1411, 1552, 1707, 1878, 2066,
    2272, 2499, 2749, 3024, 3327, 3660, 4026, 4428, 4871, 5358,
    5894, 6484, 7132, 7845, 8630, 9493, 10442, 11487, 12635, 13899,
    15289, 16818, 18500, 20350, 22385, 24623, 27086, 29794, 32767
};


void adpcm_init(adpcm_state_t* state, const unsigned char *adpcm_data)
{
	state->data = adpcm_data;
	state->index = 0;
	state->valprev = 0;
}

void adpcm_render(adpcm_state_t* state, signed char *dst, int len)
{
    int sign;
    int delta;
    int step;
    int vpdiff;
	
	/* fetch states */
	const unsigned char *data = state->data;
	int valprev = state->valprev;
	int index   = state->index;
	
	step = (unsigned short)pgm_read_byte(stepsizeTable + index);

	len >>= 1;

	
    for ( ; len > 0 ; len-- )
	{
		/* fetch data */
		int inputbuffer = *data++;
		
		/* first sample */
		/* extract bottom 4 bits */
		delta = inputbuffer & ((1 << 4) - 1);
		
		/* process */
		index += indexTable[delta];
		if (index < 0)  index = 0;
		if (index > 88) index = 88;
		sign = delta & 8; /* top bit */

		vpdiff = (((delta & 7) * step) >> 2) + (step >> 3);
		if (sign) vpdiff = -vpdiff;
		valprev += vpdiff;
		
		/* clamp and store */
		if      (valprev >  127) valprev =  127;
		else if (valprev < -128) valprev = -128;
		*dst++ = valprev;
		
		step = (unsigned short)pgm_read_byte(stepsizeTable + index);

		/* second sample */
		/* extract top 4 bits */
		delta = (inputbuffer >> 4) & 0xF;
		
		/* process */
		index += indexTable[delta];
		if (index < 0)  index = 0;
		if (index > 88) index = 88;
		sign = delta & 8; /* top bit */

		vpdiff = (((delta & 7) * step) >> 2) + (step >> 3);
		if (sign) vpdiff = -vpdiff;
		valprev += vpdiff;
		
		/* clamp and store */
		if      (valprev >  127) valprev =  127;
		else if (valprev < -128) valprev = -128;
		*dst++ = valprev;

		step = (unsigned short)pgm_read_byte(stepsizeTable + index);
	}
	
	/* store states back */
	state->data = data;
	state->valprev = valprev;
	state->index = index;
}