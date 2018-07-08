#include "myrand.hpp"
#if false && !defined(AVR)
#include <ctime>
#endif

namespace myrand {
    States states;

    void srand()
    {
#if false && !defined(AVR)
		states.state32 = time(nullptr);
#else
        /* todo */
        states.state32 = 0xbeefbabe;
#endif
    }
}

