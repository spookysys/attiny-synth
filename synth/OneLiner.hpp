#pragma once
#include "Buffer.hpp"
#include "myrand.hpp"

class OneLiner
{
    uint16_t t;

    static int8_t eval(uint16_t t, uint8_t sel)
    {
        switch (sel)
        {
        case 0: // 8
            return (t >> 5) | (t << 4) | ((t & 1023) ^ 1981) | ((t - 67) >> 4);
        case 1: // 8
            return (t >> (t & 7)) | (t << (t & 42)) | (t >> 7) | (t << 5);
        case 2: // 6
            return t & t >> 8;
        case 3: // 5
            return ((t >> 6 | t << 1) + (t >> 5 | t << 3 | t >> 3)) | t >> 2 | t << 1;
#if 0
        case 4: // 4
            return t >> 6 & 1 ? t >> 5 : -t >> 4;
        case 5: // 4
            return (((t + (t >> 2)) | (t >> 5)) + (t >> 3)) | ((t >> 13) | (t >> 7) | (t >> 11));
#endif
        default:
            return 0;
        }
    }

  public:
    static const uint8_t n = 4;

    void init() { t = 0; }
    void render(Buffer &db, uint8_t sel)
    {
        assert(sel<n);
        // reduce resolution if you need cycles
        for (uint8_t i = 0; i < globals::SAMPLES_PER_BUFFER; i += 4)
        {
            db[i+0] += eval(t++, sel);
            db[i+1] += eval(t++, sel);
            db[i+2] += eval(t++, sel);
            db[i+3] += eval(t++, sel);
        }
    };
};
