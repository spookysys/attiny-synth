#include <stdlib.h>
#include "misc.hpp"
#include "Compressor.hpp"
#include "myrand.hpp"
#include "mymath.hpp"

// settings
static const uint8_t loudness_response_time = 1;
static const uint8_t quietness_response_time = 3;

// low-pass filter for compressor response
template <uint8_t length>
static uint16_t filter(uint16_t state, uint16_t input)
{
    uint16_t bias = (1 << (length - 1)) - 1;
    return ((uint32_t(state) << length) - state + input + bias) >> length;
}

// update compressor and return volume
static uint8_t analyze(uint16_t& sense, const Buffer &sb, uint8_t min_volume, uint8_t max_volume)
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
        sense = filter<loudness_response_time>(sense, vol); // fast response to loudness
    else
        sense = filter<quietness_response_time>(sense, vol); // slow response to quietness

    // calculate desired volume
    uint16_t tmp = max_volume - (sense >> 8);
    if (tmp > max_volume)
        tmp = max_volume;
    if (tmp < 0)
        tmp = 0;
    return tmp;
}

//template <uint8_t reduce, typename WaveformT>
void Compressor::render(Buffer& destination, Buffer& sidechain, const Buffer& mixin, uint8_t min_volume, uint8_t max_volume)
{
    // update compressor response
    uint8_t vol = analyze(sense, sidechain, min_volume, max_volume);

    // render sound
    for (uint8_t i=0; i<globals::SAMPLES_PER_BUFFER; i+=4)
    {
        destination[i+0] += mymath::mulhi_s8u8(clamp8(mixin[i+0]), vol);
        destination[i+1] += mymath::mulhi_s8u8(clamp8(mixin[i+1]), vol);
        destination[i+2] += mymath::mulhi_s8u8(clamp8(mixin[i+2]), vol);
        destination[i+3] += mymath::mulhi_s8u8(clamp8(mixin[i+3]), vol);
    }
}
