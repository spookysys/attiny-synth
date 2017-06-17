#include "rand.hpp"


namespace rand {
    union {
        uint32_t state32 = 0xbeefbabe;
        uint16_t state16;
        uint8_t state8;
    }
}

