#pragma once
#include <array>
#include "Buffer.hpp"

// TODO this class

template<NUM_BUFFERS=4, SPEED=64>
class Phaser
{
    std::array<Buffer, NUM_BUFFERS> buffers;
    int8_t direction = 0;
    int8_t position = 0;
    int8_t position_countdown = 0;
private:
    Buffer& getActiveBuffer();
    void render();
    void next();
};
