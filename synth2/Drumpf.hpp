#pragma once
#include "Buffer.hpp"

class Drumpf
{
public:
    Drumpf();
    void trigger(drums::DrumEnum);
    void render(Buffer& d);
};
