#pragma once
#include "Buffer.hpp"

class Sagakrakk
{
public:
    Sagakrakk();
    void trigger(uint16_t pitch);
    void stop();
    void render(Buffer& db);
};
