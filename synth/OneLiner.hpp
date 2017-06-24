#pragma once
#include "Buffer.hpp"

class OneLiner
{
    using T = uint32_t; // shorter types are faster
public:
    void set(T t);
    void render(Buffer& db);
};
