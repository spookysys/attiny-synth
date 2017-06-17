#pragma once
#include "Buffer.hpp"

class HiHat
{
public:
    HiHat();
    void trigger(uint8_t volume, uint16_t falloff);
    void render(Buffer& db);
};
