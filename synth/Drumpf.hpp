#pragma once
#include "Buffer.hpp"

// TODO this class

class Drumpf
{
public:
    Drumpf();
    void trigger(drums::DrumEnum);
    void render(Buffer& d);
};
