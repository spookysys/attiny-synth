#include "Buffer.hpp"

namespace buffers {

    void clear(Buffer& db)
    {
        for (auto& iter : db) iter = 0;
    }

    void clip(ClippedBuffer& db, Buffer& sb)
    {
        for (uint8_t i=0; i<SAMPLES_PER_BUFFER; i++) {
            int8_t& dval = db[i];
            int16_t sval = sb[i];
            if (sval<=-128) dval = -128;
            else if (sval>=127) dval = 127;
            else dval = sval;
        }
    }
}

