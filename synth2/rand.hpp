#pragma once
#include "common.hpp"

namespace rand {
    union {
        extern uint32_t state32;
        extern uint16_t state16;
        extern uint8_t state8;
    }

    static uint8_t rand8() 
    {
        uint8_t carry = state8>>7;
        state8 <<= 1;
        if ((state8 && carry) || unlikely(!state8 && !carry)) state8 ^= 0x1d;
        return state8;
    }

    static uint16_t rand16()
    {

    }

    static uint32_t rand32()
    {

    }

}
