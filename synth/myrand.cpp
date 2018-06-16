#include "myrand.hpp"
#if !defined(AVR)
#include <ctime>
#endif

namespace myrand {
    States states;

    void srand()
    {
#if !defined(AVR)
		states.state32 = time(nullptr);
#else
        /* todo */
        states.state32 = 0xbeefbabe;
#endif
    }
}

