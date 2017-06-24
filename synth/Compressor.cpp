#include "Compressor.hpp"
#include "mymath.hpp"
#include <stdlib.h>

// settings
static const uint8_t loudness_response = 3;
static const uint8_t quietness_response = 5;
static const uint8_t max_volume = 220;
static const uint8_t min_volume = 32;

// low-pass filter for compressor response
template <uint8_t length>
static uint16_t filter(uint16_t state, uint16_t input)
{
    static const uint16_t bias = (1 << (length - 1)) - 1;
    return ((uint32_t(state) << length) - state + input + bias) >> length;
}

// update compressor and return volume
uint8_t Compressor::analyze(const Buffer &sb)
{

    // grab volume from buffer
    uint16_t vol = 0;
    vol += abs(sb[0]);
    vol += abs(sb[2]);
    vol += abs(sb[4]);
    vol += abs(sb[6]);
    if (vol > max_volume-min_volume)
        vol = max_volume-min_volume;
    vol = (vol << 8) | vol;

    // response
    if (vol > sense)
        sense = filter<loudness_response>(sense, vol); // fast response to loudness
    else
        sense = filter<quietness_response>(sense, vol); // slow response to quietness

    // calculate desired volume
    uint16_t tmp = sense >> 8;
    if (tmp > max_volume)
        tmp = max_volume;
    return max_volume - tmp;
}

//template <uint8_t reduce, typename WaveformT>
void Compressor::render(Buffer& db, const Buffer& sb)
{
    // update compressor response
    uint8_t vol = analyze(db);

    // render sound
    db[0] += mymath::mul_s8s8u8_shr8(sb[0], vol);
    db[1] += mymath::mul_s8s8u8_shr8(sb[1], vol);
    db[2] += mymath::mul_s8s8u8_shr8(sb[2], vol);
    db[3] += mymath::mul_s8s8u8_shr8(sb[3], vol);
    db[4] += mymath::mul_s8s8u8_shr8(sb[4], vol);
    db[5] += mymath::mul_s8s8u8_shr8(sb[5], vol);
    db[6] += mymath::mul_s8s8u8_shr8(sb[6], vol);
    db[7] += mymath::mul_s8s8u8_shr8(sb[7], vol);
}
