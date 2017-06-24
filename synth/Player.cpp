#include "Player.hpp"
#include "myrand.hpp"
#include <stdlib.h>
using namespace myrand;

static int8_t synth_wf(uint32_t t)
{
    int16_t tmp = 0;
    tmp += int8_t(t);
    tmp += int8_t(t + (t>>6));
    tmp += int8_t(t>>1);
    tmp += int8_t((t>>1) + (t>>7));
    if (tmp < -128) return -128;
    else if (tmp > 127) return 127;
    return tmp;
}

Player::Player()
{
    one_liner.set_time(0x2000);
}

void Player::render(Buffer &db)
{
    myrand::rand32();

    // Trigger bassdrum
    if ((pos & 0x7FF) == 0)
    {
        bd.trigger();
    }

    // Trigger hihat
    if ((pos & 0xFF) == 0)
    {
        uint16_t vol = (((pos >> 8) & 0x7) == 4) ? 0x40 : 0x20;
        hh.trigger(vol, 0x80);
    }

    // trigger synth
    if (((pos & 0xFFF) == 0x400))
    {
        synth.trigger((uint16_t(myrand::rand8())) + 200);
    }
    else if (((pos & 0xFFF) == 0x680))
    {
        synth.release();
    }

    pre_compress.clear();
    one_liner.render(pre_compress, 3);
    
    db.clear();
    bd.render(db);
    hh.render(db);
    synth.render(db, synth_wf);
    compressor.render(db, pre_compress);

    pos++;
}
