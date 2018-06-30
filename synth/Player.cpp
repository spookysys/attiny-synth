#include "Player.hpp"
#include "myrand.hpp"

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include "tables.hpp"

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

#define BASE_PITCH 65536

uint16_t pitch_tab[12] = {
  BASE_PITCH/214, // c
  BASE_PITCH/202, // c#
  BASE_PITCH/190, // d 
  BASE_PITCH/180, // d#
  BASE_PITCH/170, // e
  BASE_PITCH/160, // f
  BASE_PITCH/151, // f#
  BASE_PITCH/143, // g
  BASE_PITCH/135, // g#
  BASE_PITCH/127, // a
  BASE_PITCH/120, // a#
  BASE_PITCH/113  // b
};

#define MD (1<<6) // drumpf
#define MS (1<<6) // synth
#define O0 (0<<4) // octave 0
#define O1 (1<<4) // octave 1
#define O2 (2<<4) // octave 1
#define O3 (3<<4) // octave 1
#define _C 0
#define CH 1
#define _D 2
#define _DH 3
#define _E 4
#define _F 5
#define FH 6
#define _G 7
#define GH 8
#define A 9
#define AH 10
#define _B 11

static int curr_note = 0;

typedef struct note {
 uint8_t machine_oct_note;
 uint8_t pos;
 uint8_t len;
} note;


static note song[] = { 
	{ MD| 0|O0,0,3 },
	{ MS|_C|O1,1,2 }
};


Player::Player()
{
}

#if 0
//#undef AMEN_01
//#define AMEN_01 KICK_VINYL02
#undef AMEN_13
#define AMEN_13 KICK_VINYL02
#endif

// 
#define BREAK_FREQUENCY 9
#define BREAK_OFFSET_MAGNITUDE (9+(myrand::rand16()&0x3))
#define BREAK_SHUFFLE_AMOUNT 2

// Initial sequence used for break generation
uint8_t breaktab[] = {
    0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,
    16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31
};
static const uint8_t breaktab_mask = 0x1F;

static int mul_shl = 12;

// Timing here is very inaccurate.. should slice the loop one more time
static void amen(Drumpf &drumpf, BassDrum &db, uint16_t pos)
{
    static int prev_breakstep = 0;
    int breakstep = (pos>>BREAK_FREQUENCY); 
    if ( prev_breakstep != breakstep )
    {
        // Shuffle sequence
        for ( int breaks = 0; breaks<BREAK_SHUFFLE_AMOUNT; breaks++)
        {
            int one = myrand::rand16() & breaktab_mask;
            int two = myrand::rand16() & breaktab_mask;
            int tmp = breaktab[one];
            breaktab[one] = breaktab[two];
            breaktab[two] = tmp;
        }
        // Break offset multiplier
        mul_shl = BREAK_OFFSET_MAGNITUDE;
    }
    prev_breakstep = breakstep;

    // Only apply break during a specific timewindow part of the sequence
    if ( ((pos & 0xfff) >= 0x0800) ) 
    {
        pos += breaktab[breakstep & breaktab_mask] << mul_shl;
    }

    switch (pos & 0x3FFF)
    {
    case 0x0060:
        drumpf.trigger(AMEN_01);
       // db.trigger();
        break;
    case 0x1040:
    case 0x2052:
        drumpf.trigger(AMEN_13);
      //  db.trigger();
        break;
    case 0x304a:
        drumpf.trigger(AMEN_35);
        //        db.trigger();
        break;

    case 0x315c:
        //db.trigger();
        drumpf.trigger(/*AMEN_36*/ /*AMEN_19*/ AMEN_05);
        break;

    case 0x022c:
        //db.trigger();
        drumpf.trigger(AMEN_02);
        break;
    case 0x123e:
    case 0x225a:
        //db.trigger();
        drumpf.trigger(/*AMEN_14*/ AMEN_13);
        break;
    case 0x3238:
        //db.trigger();
        drumpf.trigger(/*AMEN_37*/ AMEN_08);
        break;

    case 0x334c:
        //db.trigger();
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
        //db.trigger();
        drumpf.trigger(AMEN_04);
        break;
    case 0x363e:
        //db.trigger();
        drumpf.trigger(AMEN_40);
        break;

    case 0x0750:
    case 0x1770:
    case 0x276a:
        drumpf.trigger(AMEN_05);
        break;
    case 0x3748:
        //db.trigger();
        drumpf.trigger(/*AMEN_41*/ AMEN_23);
        break;

    case 0x084e:
    case 0x186a:
    case 0x285e:
        drumpf.trigger(/*AMEN_06*/ AMEN_04);
        break;
    case 0x3834:
        //db.trigger();
        drumpf.trigger(AMEN_42);
        break;

    case 0x0946:
    case 0x1950:
    case 0x295b:
        //db.trigger();
        drumpf.trigger(AMEN_05);
        break;
    case 0x3928:
        //db.trigger();
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
        //db.trigger();
        drumpf.trigger(/*AMEN_12*/ AMEN_05);
        break;

    default:;
    }
}


void phaser_test(int pos, Buffer &db, Buffer &pb)
{
    static uint8_t phaser_entry_num = 0;
    static bool prev_entered_phaser = false;
    if ((pos & 0x3ff) == 0x80)
        prev_entered_phaser = false;

    static int8_t phaser_depth = 0;
    static int8_t phaser_shift = 0;
    static int8_t phaser_speed = 0;
    static int8_t filter_speed = 0;
    static bool phaser_neg = false;
    static int8_t phaser_strength = 0;
    if (!prev_entered_phaser)
    {
        phaser_entry_num++;
        phaser_depth = (myrand::rand16() & 0x0F) + 1;
        phaser_shift = 16 - phaser_depth;
        phaser_speed = (myrand::rand16() & 0x1f) + 1;
        filter_speed = (myrand::rand16() & 0x3f) + 1;
        phaser_neg = int16_t(myrand::rand16()) >= 0;
        phaser_strength = (myrand::rand16() & 1) + 1;
    }
    prev_entered_phaser = true;

    // phaser
    {
        static uint16_t t = 0;

        uint8_t so;
        so = pgm_read_byte(&tables::sin[t >> 8]) + 128;
        so = mymath::mulhi_u8u8(so, phaser_depth);
        so += phaser_shift;

        for (uint8_t i = 0; i < globals::SAMPLES_PER_BUFFER; i++)
        {
            int16_t b;
            if (so <= i)
            {
                uint8_t offs = i - so;
                assert(offs >= 0 && offs < globals::SAMPLES_PER_BUFFER);
                b = db[offs];
            }
            else
            {
                uint8_t offs = i - so + globals::SAMPLES_PER_BUFFER;
                assert(offs >= 0 && offs < globals::SAMPLES_PER_BUFFER);
                b = pb[offs];
            }
            int16_t a = db[i];
            int16_t diff = a - b;
            if (phaser_neg)
                diff = -diff;
            db[i] = a + (diff >> phaser_strength);
        }
        t += phaser_speed;
    }

    // filter
    {
        static uint16_t filter_t = 0;
        uint8_t phaser_filter = pgm_read_byte(&tables::sin[filter_t >> 8]) + 128;

        static int16_t rprev = 0;
        for (uint8_t i = 0; i < globals::SAMPLES_PER_BUFFER; i++)
        {
            int16_t r = db[i];
            if (r >= 0x0800)
                r = 0x07FF;
            if (r < -0x0800)
                r = -0x0800;
            r <<= 4;
            r = mymath::mulhi_s16u8(r - rprev, phaser_filter) + rprev;
            db[i] = r >> 4;
            rprev = r;
        }
        filter_t += filter_speed;
    }
}


void Player::render(Buffer &db, Buffer &pb)
{
	bool do_amen = true;
	
    myrand::rand32();

	if (do_amen) {
		// Trigger amen
//	    amen(drumpf, bd, (pos<<1)+0x40);

		// Trigger hihat
		if ((pos & 0x7F) == 0)
		{
			hh.trigger(0x60, 0xC0);
		}
		
//	} else {
		// Trigger bassdrum
		if ((pos & 0x7FF) == 0)
		{
			bd.trigger();
//			drumpf.trigger(KICK_808);
		}
	
		// Trigger snare
		if ((pos & 0x7FF) == 0x400)
		{
			drumpf.trigger(JK_SNR_03);
		}	

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
        one_liner.set_time(0);
		do 
	        one_liner_sel = myrand::rand16() & 0x7;
		while (one_liner_sel > 5);
    }

    // mix
    db.clear();
//    pre_compress.clear();

    bd.render(db);
//    one_liner.render(pre_compress, one_liner_sel);
//    hh.render(db);
    drumpf.render(db);
 
//    compressor1.render(db, pre_compress);
//    phaser_test(pos, db, pb );

//    pre_compress.clear();
    //one_liner.render(pre_compress, one_liner_sel);
//    compressor2.render(db, pre_compress);
    //synth.render(db, synth_wf);


    pos ++;
}
