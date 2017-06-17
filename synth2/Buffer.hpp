#pragma once
#include "common.hpp"
#include <array>

static const int SAMPLES_PER_BUFFER = 8;
using Buffer = std::array<int16_t, SAMPLES_PER_BUFFER>;
using ClippedBuffer = std::array<int8_t, SAMPLES_PER_BUFFER>;

namespace buffers {
    void clear(Buffer& d);
    void clip(ClippedBuffer& d, Buffer& s);
}

