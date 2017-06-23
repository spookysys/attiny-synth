#pragma once
#include "Buffer.hpp"
#include "Hihats.hpp"

class Player
{
    uint32_t pos = 0;
    Hihats hh;
public:
    Player();
    void render(Buffer& db);
};
