#pragma once
#include "common.hpp"

namespace myrand {
    union States {
        uint32_t state32;
        uint16_t state16;
        uint8_t state8;
    };

    extern States states;

    void srand();

    static uint8_t rand8() 
    {
        uint8_t lsb = states.state8 & 1;
        states.state8 >>= 1;
        if ((states.state8 && lsb) || unlikely(!states.state8 && !lsb)) states.state8 ^= 0xb8;
        return states.state8;
    }

    static uint16_t rand16()
    {
        uint8_t lsb = states.state16 & 1;
        states.state16 >>= 1;
        if (lsb || unlikely(!states.state16 && !lsb)) states.state16 ^= 0xb400;
		return states.state16;
    }

    static uint32_t rand32()
    {
        uint8_t lsb = states.state32 & 1;
        states.state32 >>= 1;
        if (lsb || unlikely(!states.state32 && !lsb)) states.state32 ^= 0x80200003;
		return states.state32;
    }

}

