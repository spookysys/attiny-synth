#pragma once
#include "Buffer.hpp"
#include "myrand.hpp"

// high quality lerp from one value to another over a buffer
static void lerp(int8_t start, int8_t end, Buffer &v)
{
    int16_t iter = start << globals::SAMPLES_PER_BUFFER_LOG2;
    int16_t step = int16_t(end) - int16_t(start);
    uint16_t dither = myrand::rand16();

    /* approx 10 cycles plus loop */
    for (uint8_t i = 0; i < globals::SAMPLES_PER_BUFFER; i++)
    {
		int16_t tmp = iter >> (globals::SAMPLES_PER_BUFFER_LOG2 - 1);
		tmp += dither & 1;
		tmp >>= 1;
        v[i] += tmp;
        dither >>= 1;
        iter += step;
    }
}


static int8_t clamp8(int16_t op)
{
    if (op<-128)
        return -128;
    if (op> 127)
        return 127;
    return op;
}

