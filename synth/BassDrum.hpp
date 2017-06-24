#pragma once
#include "Buffer.hpp"

class BassDrum
{
    enum State {
        OFF = 0,
        SLIDE,
        DECAY
    } state = OFF;
    
    uint16_t pos;
    uint16_t pitch_vol;
    int8_t last_v;

public:
    void trigger();
    void render(Buffer& db);
};
