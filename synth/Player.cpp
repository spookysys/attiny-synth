#include "Player.hpp"
#include "myrand.hpp"
#include <stdlib.h>
using namespace myrand;

static int8_t synth_wf(uint32_t t)
{
    int16_t tmp = 0;
    tmp += int8_t(t);
    tmp += int8_t(t + (t >> 6));
    tmp += int8_t(t >> 1);
    tmp += int8_t((t >> 1) + (t >> 7));
    if (tmp < -128)
        return -128;
    else if (tmp > 127)
        return 127;
    return tmp;
}

Player::Player()
{
    one_liner.set_time(0x2000);
}

void Player::render(Buffer &db)
{
    myrand::rand32();

    if (pos < 0x8000)
    {
        switch (pos & 0x3FFF)
        {
        case 0x0000:
            drumpf.trigger(AMEN_01);
            break;
        case 0x1041:
        case 0x2052:
            drumpf.trigger(AMEN_13);
            break;
        case 0x304b:
            drumpf.trigger(AMEN_35);
            break;

        case 0x315d:
            drumpf.trigger(/*AMEN_36*//*AMEN_19*/AMEN_05);
            break;

        case 0x022d:
            drumpf.trigger(AMEN_02);
            break;
        case 0x123f:
        case 0x225a:
            drumpf.trigger(/*AMEN_14*/AMEN_13);
            break;
        case 0x3238:
            drumpf.trigger(/*AMEN_37*/AMEN_08);
            break;

        case 0x334c:
            drumpf.trigger(/*AMEN_38*/AMEN_09);
            break;

        case 0x0429:
        case 0x144e:
        case 0x2464:
            drumpf.trigger(AMEN_03);
            break;
        case 0x3426:
            drumpf.trigger(AMEN_39);
            break;

        case 0x064f:
        case 0x166e:
        case 0x2660:
            drumpf.trigger(AMEN_04);
            break;
        case 0x363e:
            drumpf.trigger(AMEN_40);
            break;

        case 0x0751:
        case 0x1770:
        case 0x276a:
            drumpf.trigger(AMEN_05);
            break;
        case 0x3749:
            drumpf.trigger(/*AMEN_41*/AMEN_23);
            break;

        case 0x084e:
        case 0x186b:
        case 0x285e:
            drumpf.trigger(/*AMEN_06*/AMEN_04);
            break;
        case 0x3835:
            drumpf.trigger(AMEN_42);
            break;

        case 0x0947:
        case 0x1951:
        case 0x295b:
            drumpf.trigger(AMEN_05);
            break;
        case 0x3929:
            drumpf.trigger(/*AMEN_43*/AMEN_05);
            break;

        case 0x0a33:
        case 0x1a50:
            drumpf.trigger(AMEN_08);
            break;
        case 0x2a3a:
            drumpf.trigger(AMEN_13);
            break;
        case 0x3a0d:
            drumpf.trigger(AMEN_44);
            break;

        case 0x0b61:
        case 0x1b81:
            drumpf.trigger(AMEN_09);
            break;

        case 0x0c23:
        case 0x1c5b:
            drumpf.trigger(AMEN_10);
            break;
        case 0x2c51:
            drumpf.trigger(/*AMEN_16*/AMEN_04);
            break;
        case 0x3c3f:
            drumpf.trigger(AMEN_45);
            break;

        case 0x0e48:
        case 0x1e5f:
            drumpf.trigger(/*AMEN_11*/AMEN_04);
            break;
        case 0x2e34:
            drumpf.trigger(/*AMEN_34*/AMEN_15);
            break;
        case 0x3e0b:
            drumpf.trigger(AMEN_46);
            break;
            
        case 0x0f51:
        case 0x1f72:
            drumpf.trigger(/*AMEN_12*/AMEN_05);
            break;

        default:;
        }

        db.clear();
        drumpf.render(db);
    }
    else
    {

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
    }

    pos++;
}
