#include "Compressor.hpp"
#include "mymath.hpp"
#include <stdlib.h>

// settings
static const uint8_t loudness_response = 3;
static const uint8_t quietness_response = 5;
static const uint8_t max_volume = 100;
static const uint8_t min_volume = 16;

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
    vol += abs(sb[globals::SAMPLES_PER_BUFFER*0/4]);
    vol += abs(sb[globals::SAMPLES_PER_BUFFER*1/4]);
    vol += abs(sb[globals::SAMPLES_PER_BUFFER*2/4]);
    vol += abs(sb[globals::SAMPLES_PER_BUFFER*3/4]);
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
    for (uint8_t i=0; i<globals::SAMPLES_PER_BUFFER; i+=4)
    {
        db[i+0] += mymath::mul_s8_s8u8_shr8(sb[i+0], vol);
        db[i+1] += mymath::mul_s8_s8u8_shr8(sb[i+1], vol);
        db[i+2] += mymath::mul_s8_s8u8_shr8(sb[i+2], vol);
        db[i+3] += mymath::mul_s8_s8u8_shr8(sb[i+3], vol);
    }
}
