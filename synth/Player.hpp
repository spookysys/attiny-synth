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
    uint32_t pos;
    uint8_t one_liner_sel;
    BassDrum bd;
    Hihats hh;
    OneLiner one_liner;
    Compressor compressor;
    OneSynth synth;
    OneSynth arpeggio;
    Drumpf drumpf;
    uint8_t basenote;
public:
    void init();
    bool render(Buffer& db, Buffer& pb);
};
