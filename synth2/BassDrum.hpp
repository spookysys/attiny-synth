#pragma once
#include "Buffer.hpp"

class BassDrum
{
public:
    void trigger();
    void render(Buffer& db);
};
