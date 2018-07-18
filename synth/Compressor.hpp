#pragma once
#include "Buffer.hpp"

class Compressor
{
    uint16_t sense;
public:
    void init() { sense = 0; }
    void render(Buffer& destination, Buffer& sidechain, const Buffer& mixin, uint8_t min_volume=10, uint8_t max_volume=100);
};

