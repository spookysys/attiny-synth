#include "common.h"

namespace globals
{
	int16_t mixbuff[2][mixbuff_len];
}

void fail()
{
	// just die
	while(1){}
}