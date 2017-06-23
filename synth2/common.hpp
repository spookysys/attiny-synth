#pragma once
#include <cstdint>

#ifdef AVR
#include <avr/pgmspace.h>
#else
#define PROGMEM
static uint8_t pgm_read_byte(const void* ptr) { return *(const uint8_t*)ptr; }
#endif

#define likely(x)      __builtin_expect(!!(x), 1)
#define unlikely(x)    __builtin_expect(!!(x), 0)

namespace globals {
    static const uint16_t SAMPLE_RATE = 22050;
    static const uint8_t  SAMPLES_PER_BUFFER = 8;
}
