#include "buffers.hpp"

namespace buffers {

    std::array<Buffer, NUM_BUFFERS> buffers;

    void clear(BufferID id)
    {
        for (auto& iter : buffers[id]) iter = 0;
    }

    void clip(int8_t* dest, BufferID id)
    {
        for (uint8_t i=0; i<SAMPLES_PER_BUFFER; i++) {
            int8_t& dval = dest[i];
            int16_t sval = buffers[id][i];
            if (sval<=-128) dval = -128;
            else if (sval>=127) dval = 127;
            else dval = sval;
        }
    }
}

