#pragma once
#include "Buffer.hpp"

class Hihats
{
    uint16_t volume;
    uint8_t falloff;
public:
    Hihats();
    void trigger(uint8_t volume, uint8_t falloff);
    void render(Buffer& db);
};
