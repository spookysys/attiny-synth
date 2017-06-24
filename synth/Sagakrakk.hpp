#pragma once
#include "Buffer.hpp"

// TODO this class

class Sagakrakk
{
public:
    Sagakrakk();
    void trigger(uint16_t pitch);
    void stop();
    void render(Buffer& db);
};
