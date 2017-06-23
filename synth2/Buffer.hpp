#pragma once
#include "common.hpp"
#include <array>

using Buffer = std::array<int16_t, globals::SAMPLES_PER_BUFFER>;

namespace buffers {
    void clear(Buffer& d);
}

