#pragma once
#include "Buffer.hpp"

class Compressor
{
    uint16_t sense;
    uint8_t analyze(const Buffer& sb);
public:
    void render(Buffer& destination, Buffer& sidechain, const Buffer& mixin);
};

