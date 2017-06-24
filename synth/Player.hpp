#pragma once
#include "Buffer.hpp"
#include "BassDrum.hpp"
#include "Hihats.hpp"
#include "OneLiner.hpp"
#include "Compressor.hpp"
#include "OneSynth.hpp"

class Player
{
    uint32_t pos = 0;
    BassDrum bd;
    Hihats hh;
    OneLiner<uint32_t> one_liner;
    Compressor compressor;
    Buffer pre_compress;
    OneSynth<> synth;
public:
    Player();
    void render(Buffer& db);
};
