#pragma once
#include "Buffer.hpp"
#include "myrand.hpp"

template <typename T = uint32_t> // shorter types are faster but they loop
class OneLiner
{
    T t = 0;

    static int8_t eval(T t, uint8_t sel)
    {
        switch (sel)
        {
        case 0:
            return t & t >> 8;
        case 1:
            return t >> 6 & 1 ? t >> 5 : -t >> 4;
        case 2:
            return (t >> (t & 7)) | (t << (t & 42)) | (t >> 7) | (t << 5);
        case 3:
            return ((t >> 6 | t << 1) + (t >> 5 | t << 3 | t >> 3)) | t >> 2 | t << 1;
        case 4:
            return (t >> 5) | (t << 4) | ((t & 1023) ^ 1981) | ((t - 67) >> 4);
        case 5:
            return (((t + (t >> 2)) | (t >> 5)) + (t >> 3)) | ((t >> 13) | (t >> 7) | (t >> 11));
        default:
            return 0;
        }
    }

  public:
    void set_time(T t) { this->t = t; }
    void render(Buffer &db, uint8_t sel)
    {
        int8_t val;
        // 4x downsampled... change this if you want
        for (uint8_t i = 0; i < globals::SAMPLES_PER_BUFFER; i += 4)
        {
            val = eval(t++, sel);
            db[i+0] += val;
            db[i+1] += val;
            db[i+2] += val;
            db[i+3] += val;
        }
    };
};
