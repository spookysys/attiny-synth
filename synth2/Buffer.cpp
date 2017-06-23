#include "Buffer.hpp"

namespace buffers {

    void clear(Buffer& db)
    {
        for (auto& iter : db) iter = 0;
    }

}

