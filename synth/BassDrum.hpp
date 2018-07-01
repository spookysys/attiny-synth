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
    uint16_t pitch;
    uint8_t vol;
    int8_t lastval;
public:
    void trigger(bool accent=true);
    void render(Buffer& db);
};
