#pragma once
#include "common.hpp"

#define BROKEN_MUL16

namespace mymath
{
    // regular widening multiplies
    uint16_t mul_u16_u8u8(uint8_t a, uint8_t b);
#ifndef BROKEN_MUL16
    uint32_t mul_u24_u16u8(uint16_t a, uint8_t b);
#endif

    // mulhi
    // - result has same type as a
    // - result has been right-shifted by the width of b
    uint8_t mulhi_u8u8(uint8_t a, uint8_t b);
    int8_t mulhi_s8s8(int8_t a, int8_t b);
    int8_t mulhi_s8u8(int8_t a, uint8_t b);
#ifndef BROKEN_MUL16
    uint16_t mulhi_u16u8(uint16_t a, uint8_t b);
    int16_t mulhi_s16s8(int16_t a, int8_t b);
    int16_t mulhi_s16u8(int16_t a, uint8_t b);
#endif
}
