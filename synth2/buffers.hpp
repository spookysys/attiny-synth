#pragma once
#include <array>

namespace buffers
{
    static const int SAMPLES_PER_BUFFER = 8;

    enum BufferID
    {
        MIX0 = 0,
        MIX1,
        EXTRA,
        NUM_BUFFERS
    }

    using Buffer = std::array<int16_t, SAMPLES_PER_BUFFER>
    std::array<Buffer, NUM_BUFFERS> buffers;

    void clear(BufferID id);
    void clip(int8_t* dest, BufferID id);
}
