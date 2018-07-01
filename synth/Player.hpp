#pragma once
#include "Buffer.hpp"
#include "BassDrum.hpp"
#include "Hihats.hpp"
#include "OneLiner.hpp"
#include "Compressor.hpp"
#include "Drumpf.hpp"
#include "OneSynth.hpp"

class Player
{
    uint32_t pos = 0;
    uint8_t one_liner_sel = 0;
    BassDrum bd;
    Hihats hh;
    OneLiner<> one_liner;
    Compressor compressor;
    OneSynth synth;
    OneSynth arpeggio;
    Drumpf drumpf;
public:
    Player();
    void render(Buffer& db, Buffer& pb);
};
