#pragma once
#include <stdint.h>

#ifdef AVR
#include <avr/pgmspace.h>
#else
#define PROGMEM
#include <memory.h>
static uint8_t pgm_read_byte(const void* ptr) { return *(const uint8_t*)ptr; }
#define memcpy_P memcpy
#endif

#define likely(x)      __builtin_expect(!!(x), 1)
#define unlikely(x)    __builtin_expect(!!(x), 0)

namespace globals {
    static constexpr uint16_t SAMPLE_RATE = 22050;
    static constexpr uint8_t  SAMPLES_PER_BUFFER = 16;
    static constexpr uint8_t  SAMPLES_PER_BUFFER_LOG2 = 4;
}
