#pragma once
#include <cstdint>

#define likely(x)      __builtin_expect(!!(x), 1)
#define unlikely(x)    __builtin_expect(!!(x), 0)

namespace globals {
    static const uint16_t SAMPLE_RATE = 22050;
    static const uint8_t  SAMPLES_PER_BUFFER = 8;
}
