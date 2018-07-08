#include "Player.hpp"
#include "myrand.hpp"

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include "tables.hpp"

using namespace myrand;

static int8_t synth_wf(uint16_t t)
{
    int16_t tmp = 0;
    tmp += int8_t(t);
    tmp += int8_t(t + (t >> 6));
  //  tmp += int8_t(t >> 1);
    tmp += int8_t((t >> 1) + (t >> 7));
 //   tmp += int8_t(t+t);
 //   tmp += int8_t(t>>2);
//    tmp += int8_t(t + (t >> 6));
 //   tmp += int8_t(t >> 2);
  //   tmp += int8_t((t >> 4) + (t >> 2));
    
    // I'm tweaking the volume using the compressor's "max_volume" setting
    tmp >>= 1; 

    if (tmp < -128)
        return -128;
    else if (tmp > 127)
        return 127;
    return tmp;
}

static int8_t sag_wf(uint16_t t)
{
    int16_t tmp = 0;
    tmp += int8_t(t);
    tmp += int8_t(t+t);
    tmp >>= 1; 

    if (tmp < -128)
        return -128;
    else if (tmp > 127)
        return 127;
    return tmp;
}

#define BASE_PITCH 256 * 214

static constexpr uint16_t pitch_tab[12] = {
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


static const note song[] = { 
	{ MD| 0|O0,0,3 },
	{ MS|_C|O1,1,2 }
};



static const uint16_t chord_pitches[] = {
    uint16_t((pitch_tab[_DH]<<3) + 2),
    uint16_t((pitch_tab[_G]<<2) - 1),
    uint16_t((pitch_tab[A]<<2) + 1),
    uint16_t((pitch_tab[CH]<<3) - 3),
    uint16_t((pitch_tab[_E]<<3) - 2)
};

struct Chord
{
    static const uint8_t n = sizeof(chord_pitches)/sizeof(*chord_pitches);
    uint16_t poses[n] = {};
    uint16_t vibpos1 = 0;
    void render(Buffer& db)
    {
        for (uint8_t i=0; i<globals::SAMPLES_PER_BUFFER; i++)
        {
            int16_t v = 0;
            int8_t vib1 = tables::sin[vibpos1>>8]; 
            vibpos1 += 20; 
            v += synth_wf(poses[0]>>8); poses[0] += chord_pitches[0] - (vib1>>3) + (vib1>>4);
            v += synth_wf(poses[1]>>8); poses[1] += chord_pitches[1] + (vib1>>3);
            v += synth_wf(poses[2]>>8); poses[2] += chord_pitches[2] - (vib1>>3);
            v += synth_wf(poses[3]>>8); poses[3] += chord_pitches[3] + (vib1>>3) - (vib1>>4);
            v += synth_wf(poses[4]>>8); poses[4] += chord_pitches[4] - (vib1>>3) + (vib1>>4);
            db[i] += v>>5;
        }
    }
} chord;

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
#define BREAK_FREQUENCY 10
#define BREAK_OFFSET_MAGNITUDE (9+(myrand::rand16()&0x3))
#define BREAK_SHUFFLE_AMOUNT 2

// Initial sequence used for break generation
uint8_t breaktab[] = {
    0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,
    16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31
};
static const uint8_t breaktab_mask = 0x1F;

static int mul_shl = 12;

bool drumblocker(uint16_t pos)
{
    static uint8_t bit = 0;
    if ((pos & 0xffff) <= 0xbfff)
    {
        bit = -1;
        return false;
    }
    else if ((pos & 0x0fff)==0)
    {
        bit += (myrand::rand8()&3)+1;
    }

    return (pos>>bit)&1;
}

static void amen(Drumpf &drumpf, BassDrum &bd, uint16_t pos)
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
    if ( (((pos<<1) & 0x1C908) >= 0x0800) ) 
    {
        pos += breaktab[breakstep & breaktab_mask] << mul_shl;
    }


    // Trigger amen samples
    switch (pos & 0x1FFF)
    {
    case 0x0000:
        drumpf.trigger(AMEN_HIT);
        bd.trigger(false);
        break;
    case 0x100:
    case 0x800:
    case 0x900:
    case 0x1000:
    case 0x1100:
        drumpf.trigger(AMEN_BDHH_5);
        bd.trigger(false);
        break;
    case 0x500:
    case 0xD00:
    case 0x1900:
        drumpf.trigger(AMEN_BDHHSOFT);
        //bd.trigger(false);
        break;
    case 0x580:
    case 0xD80:
    case 0x1980:
        drumpf.trigger(AMEN_BDSOFT_3);
        bd.trigger(false);
        break;
    case 0x1D00:
        drumpf.trigger(AMEN_CRASHBD);
        bd.trigger(true);
        break;
    case 0x300:
    case 0x400:
    case 0x700:
    case 0xB00:
    case 0xC00:
    case 0xF00:
    case 0x1300:
    case 0x1400:
    case 0x1800:
    case 0x1B00:
    case 0x1C00:
        drumpf.trigger(AMEN_HH_2);
        break;
    case 0x1500:
        drumpf.trigger(AMEN_LOUDBDHH);
        bd.trigger(true);
        break;
    case 0x1600:
        drumpf.trigger(AMEN_RIDE);
        break;
    case 0x200:
    case 0x600:
    case 0xA00:
    case 0xE00:
    case 0x1200:
    case 0x1700:
        drumpf.trigger(AMEN_SNARE);
        //bd.trigger(false);
        break;
    case 0x1A00:
        drumpf.trigger(/*AMEN_SNAREHARD*/AMEN_SNARE);
        bd.trigger(false);
        break;
    case 0x1F00:
        drumpf.trigger(/*AMEN_SNAREHISS*/AMEN_SNARE);
        //bd.trigger(false);
        break;
    case 0x380:
    case 0x480:
    case 0x780:
    case 0xB80:
    case 0xC80:
    case 0xF80:
    case 0x1380:
    case 0x1480:
    case 0x1880:
    case 0x1B80:
    case 0x1C80:
        drumpf.trigger(AMEN_SOFTSNARE_7);
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
        phaser_depth = (myrand::rand16() & 0x0F);
        phaser_shift = 16 - phaser_depth;
        phaser_speed = (myrand::rand16() & 0xff) + 1;
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
            db[i] = a+a+a + (diff >> phaser_strength);
        }
        t += phaser_speed;
    }

    // filter
    if ( 0 )
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
    myrand::rand32();

    // Trigger amen
    amen(drumpf, bd, pos);

    // Trigger hihat
    if (1)
    {
        if ((pos & 0xFF) == 0)
        {
            hh.trigger(0x1A, 0x05);
        }
    }
    else
    {
        if ((pos & 0x7F) == 0)
        {
            hh.trigger(0x60, 0xC0);
        }
    }
    
    // Trigger snare
    if ((pos & 0x7FF) == 0x400)
    {
		//drumpf.trigger(JK_SNR_03);
    }

    // trigger synth
    static int basenote = 0; 
    if (((pos & 0xFFF) == 0x00) || (pos & 0xFFF) == 0x700 )
    {
        static int arp_pos = 0;
        static uint32_t arp[] = { 
                                  pitch_tab[_C], 
                                  pitch_tab[_C],
                                  pitch_tab[_C], 
                                  pitch_tab[FH],
                                  pitch_tab[_C], 
                                  pitch_tab[FH],
                                  uint32_t(pitch_tab[_C]*2),
                                  uint32_t(pitch_tab[FH]*2),
                                  };
       static uint8_t base[] = { 
                                  0, 
                                  0,
                                  0, 
                                  4,
                                  0, 
                                  4,
                                  0,
                                  4
                                  };
         int one = myrand::rand16() & 7;
        int two = myrand::rand16() & 7;
        int tmp = arp[one];
        arp[one] = arp[two];
        arp[two] = tmp;
        tmp = base[one];
        base[one] = base[two];
        base[two] = tmp;
      //  arp_pos += myrand::rand8()&3;
      //  arp_pos &= 3;
        arp_pos++;
        arp_pos  &= 7;
        synth.trigger(arp[arp_pos]);
        basenote = base[arp_pos];
    }

    if ( (pos & 0x7F) == 0x00 )
    {
        static constexpr uint32_t arp[] = { (uint16_t)(pitch_tab[_C]*2),
                                            (uint16_t)(pitch_tab[_C]*3),
                                            (uint16_t)(pitch_tab[_C]*4), 
                                            (uint16_t)(pitch_tab[_C]*6),
                                            (uint16_t)(pitch_tab[FH]*2),
                                            (uint16_t)(pitch_tab[FH]*3),
                                            (uint16_t)(pitch_tab[FH]*4),
                                            (uint16_t)(pitch_tab[FH]*6),
 //                                           (uint16_t)(pitch_tab[_C]*6)
                                        }; 
        arpeggio.trigger(arp[(myrand::rand8()&3)+basenote]);
        arpeggio.set_portamento_speed(1);
        arpeggio.set_decay_speed(15);

    } else if ( (pos & 0x7F) == 0x8 ) 
    {
        arpeggio.release();
    }


    // change oneliner settings
    if ((pos & 0x7FFF) == 0)
    {
        one_liner.set_time(0);
		do 
	        one_liner_sel = myrand::rand16() & 0x7;
		while (one_liner_sel > 5);
    }
    

    // mix
    Buffer sidechain;
    Buffer mixin;

    sidechain.clear();
    mixin.clear();

    if ( !drumblocker(pos) )
    {
        if ( pos > 0x07FFF )
            hh.render(db);
        if ( pos > 0x0DFFF )
            bd.render(sidechain);
        if ( pos > 0x0FFFF )
            drumpf.render(db);
    }

    if ( pos > 0xFFFF && ((pos & 0xFFFF) > 0x07FFF))
        one_liner.render(mixin, one_liner_sel);

    if ( pos <= 0xFFFF || ((pos & 0xFFFF) <= 0x07FFF))
    {
        synth.render(mixin, synth_wf);
        arpeggio.render(mixin, sag_wf);
    }


    if ( pos > 0xFFFF && ((pos & 0xFFFF) > 0x07FFF))
        chord.render(db);

    compressor.render(db, sidechain, mixin);

    db.mixin(sidechain);

    phaser_test(pos, db, pb);


    pos ++;
}
