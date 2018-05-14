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
}

#if 0
//#undef AMEN_01
//#define AMEN_01 KICK_VINYL02
//#undef AMEN_13
//#define AMEN_13 KICK_VINYL02
#endif

// Timing here is very inaccurate.. should slice the loop one more time
static void amen(Drumpf &drumpf, BassDrum &db, uint16_t pos)
{
    switch (pos & 0x3FFF)
    {
    case 0x0060:
        drumpf.trigger(AMEN_01);
        db.trigger();
        break;
    case 0x1040:
    case 0x2052:
        drumpf.trigger(AMEN_13);
        db.trigger();
        break;
    case 0x304a:
        drumpf.trigger(AMEN_35);
        //        db.trigger();
        break;

    case 0x315c:
        db.trigger();
        drumpf.trigger(/*AMEN_36*/ /*AMEN_19*/ AMEN_05);
        break;

    case 0x022c:
        db.trigger();
        drumpf.trigger(AMEN_02);
        break;
    case 0x123e:
    case 0x225a:
        db.trigger();
        drumpf.trigger(/*AMEN_14*/ AMEN_13);
        break;
    case 0x3238:
        db.trigger();
        drumpf.trigger(/*AMEN_37*/ AMEN_08);
        break;

    case 0x334c:
        db.trigger();
        drumpf.trigger(/*AMEN_38*/ AMEN_09);
        break;

    case 0x0428:
    case 0x144e:
    case 0x2464:
        drumpf.trigger(AMEN_03);
        break;
    case 0x3426:
        drumpf.trigger(AMEN_39);
        break;

    case 0x064e:
    case 0x166e:
    case 0x2660:
        db.trigger();
        drumpf.trigger(AMEN_04);
        break;
    case 0x363e:
        db.trigger();
        drumpf.trigger(AMEN_40);
        break;

    case 0x0750:
    case 0x1770:
    case 0x276a:
        drumpf.trigger(AMEN_05);
        break;
    case 0x3748:
        db.trigger();
        drumpf.trigger(/*AMEN_41*/ AMEN_23);
        break;

    case 0x084e:
    case 0x186a:
    case 0x285e:
        drumpf.trigger(/*AMEN_06*/ AMEN_04);
        break;
    case 0x3834:
        db.trigger();
        drumpf.trigger(AMEN_42);
        break;

    case 0x0946:
    case 0x1950:
    case 0x295b:
        db.trigger();
        drumpf.trigger(AMEN_05);
        break;
    case 0x3928:
        db.trigger();
        drumpf.trigger(/*AMEN_43*/ AMEN_05);
        break;

    case 0x0a32:
    case 0x1a50:
        drumpf.trigger(AMEN_08);
        break;
    case 0x2a3a:
        drumpf.trigger(AMEN_13);
        break;
    case 0x3a0c:
        drumpf.trigger(AMEN_44);
        break;

    case 0x0b60:
    case 0x1b80:
        drumpf.trigger(AMEN_09);
        break;

    case 0x0c22:
    case 0x1c5a:
        drumpf.trigger(AMEN_10);
        break;
    case 0x2c50:
        drumpf.trigger(/*AMEN_16*/ AMEN_04);
        break;
    case 0x3c00:
        drumpf.trigger(AMEN_45);
        break;

    case 0x0e48:
    case 0x1e5e:
        drumpf.trigger(/*AMEN_11*/ AMEN_04);
        break;
    case 0x2e34:
        drumpf.trigger(/*AMEN_34*/ AMEN_15);
        break;
    case 0x3e0a:
        drumpf.trigger(AMEN_46);
        break;

    case 0x0f50:
    case 0x1f72:
        db.trigger();
        drumpf.trigger(/*AMEN_12*/ AMEN_05);
        break;

    default:;
    }
}

void Player::render(Buffer &db)
{
    myrand::rand32();

    // Trigger amen
    //amen(drumpf, bd, pos+0x60);

    // Trigger bassdrum
    if ((pos & 0x7FF) == 0)
    {
	    bd.trigger();
        drumpf.trigger(KICK_808);
    }
	
    // Trigger snare
    if ((pos & 0x7FF) == 0x400)
    {
	    drumpf.trigger(JK_SNR_03);
    }	

    // Trigger hihat
    if ((pos & 0x3FF) == 0x260)
    {
        hh.trigger(0x40, 0x18);
    }
/*
    // trigger synth
    if (((pos & 0xFFF) == 0x400))
    {
        synth.trigger((uint16_t(myrand::rand8())) + 200);
    }
    else if (((pos & 0xFFF) == 0x680))
    {
        synth.release();
    }
*/

    // change oneliner settings
    if ((pos & 0x3FFF) == 0)
    {
        //one_liner.set_time(0);
		do 
	        one_liner_sel = myrand::rand16() & 0x7;
		while (one_liner_sel > 5);
    }

    // mix
    db.clear();
    pre_compress.clear();

    bd.render(db);
    one_liner.render(pre_compress, one_liner_sel);
    compressor1.render(db, pre_compress);
    hh.render(db);
    drumpf.render(db);

//    pre_compress.clear();
    //one_liner.render(pre_compress, one_liner_sel);
//    compressor2.render(db, pre_compress);
    //synth.render(db, synth_wf);


    pos ++;
}
