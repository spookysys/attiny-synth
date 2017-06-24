#pragma once
#include "Buffer.hpp"
#include "BassDrum.hpp"
#include "Hihats.hpp"

class Player
{
    uint32_t pos = 0;
    BassDrum bd;
    Hihats hh;
public:
    Player();
    void render(Buffer& db);
};
