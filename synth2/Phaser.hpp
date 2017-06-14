#pragma once
#include <array>
#include "buffers.hpp"

template<NUM_SAMPLES=64, MIN_DELAY=16, SPEED=int8_t(.1*256)>
class Phaser
{
    std::array<int8_t, NUM_SAMPLES> buffer = {0};
    int8_t direction = 0;
private:
    void readIn(buffers::BufferID sourceBuffer);
    void render(buffers::BufferID destBuffer);
};

