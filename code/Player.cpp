#include "misc.hpp"
#include "Player.hpp"
#include "myrand.hpp"
#include "tables.hpp"

uint8_t one_liner_sel;
void new_one_liner()
{
    uint8_t old = one_liner_sel;
    do
    {
        one_liner_sel = myrand::rand16() & 0x7;
    } while (one_liner_sel >= OneLiner::n || one_liner_sel == old);
}

using namespace myrand;

bool filter_on;

static int16_t synth_wf(uint16_t t)
{
	int16_t tmp = 0;
	tmp += int8_t(t);
	tmp += int8_t(t + (t >> 6));
	tmp += int8_t((t >> 1) + (t >> 7));
    tmp += int8_t(pgm_read_byte(&tables::sin[(t)&0xff]));
    tmp += int8_t(pgm_read_byte(&tables::sin[(t>>1)&0xff]));
    tmp += int8_t(pgm_read_byte(&tables::sin[(t>>1)&0xff]));
 //   tmp += int8_t(pgm_read_byte(&tables::sin[(t>>1)&0xff]));
	return tmp>>2;
}

static int16_t sag_wf(uint16_t t)
{
    int16_t tmp = 0;
    tmp += int8_t(t);
    tmp += int8_t(t+t);
//    tmp += int8_t(pgm_read_byte(&tables::sin[(t<<1)&0xff]));
    return tmp>>1;
}


static int16_t sin_wf(uint16_t t)
{
    int16_t tmp = 0;
//    tmp += int8_t(t);
//    tmp += int8_t(t+t);
    tmp += int8_t(pgm_read_byte(&tables::sin[(t<<1)&0xff]));
      return tmp;
}

static int16_t sqr_wf(uint16_t t)
{
    int16_t tmp = 0;
//    tmp += int8_t(t);
//    tmp += int8_t(t+t);
    tmp += (( t & 0xff ) < ((int8_t(pgm_read_byte(&tables::sin[t>>10]))>>1) + 0x7f)) ? -64 : 63;
  //  tmp += (( t & 0xff ) < ((int8_t(pgm_read_byte(&tables::sin[t>>8]))>>1) + 0x7f)) ? -64 : 63;
      return tmp;
}

#define BASE_PITCH double(uint16_t(1)<<15)
#define PITCH_C uint16_t(BASE_PITCH*1)
#define PITCH_CH uint16_t(BASE_PITCH*1.0594630943593)
#define PITCH_D uint16_t(BASE_PITCH*1.12246204830937)
#define PITCH_DH  uint16_t(BASE_PITCH*1.18920711500272)
#define PITCH_E uint16_t(BASE_PITCH*1.25992104989487)
#define PITCH_F uint16_t(BASE_PITC*1.33483985417003)
#define PITCH_FH uint16_t(BASE_PITCH*1.4142135623731)
#define PITCH_G uint16_t(BASE_PITCH*1.49830707687668)
#define PITCH_GH uint16_t(BASE_PITCH*1.5874010519682)
#define PITCH_A uint16_t(BASE_PITCH*1.68179283050743)
#define PITCH_AH uint16_t(BASE_PITCH*1.78179743628068)
#define PITCH_B uint16_t(BASE_PITCH*1.88774862536339)

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

typedef struct note {
 uint8_t machine_oct_note;
 uint8_t pos;
 uint8_t len;
} note;


static const note song[] PROGMEM = { 
	{ MD| 0|O0,0,3 },
	{ MS|_C|O1,1,2 }
};


class Shuffler
{
    uint8_t breaktab[32];
public:
    static const uint8_t n = 32;
    uint8_t operator[](uint8_t i)
    {
        return breaktab[i & (n-1)];
    }
    void init()
    {
        for (uint8_t i=0; i<n; i++)
            breaktab[i] = i;
    }
    void shuffle()
    {
        uint8_t one = myrand::rand16() & (n-1);
        uint8_t two = myrand::rand16() & (n-1);
        uint8_t tmp = breaktab[one];
        breaktab[one] = breaktab[two];
        breaktab[two] = tmp;
    }
} shuffler;


struct Chord
{
    uint8_t shr;
    uint16_t poses[5];
    uint16_t vibpos;
    void render(Buffer& db)
    {
        for (uint8_t i=0; i<globals::SAMPLES_PER_BUFFER; i++)
        {
            int16_t v = 0;
            int8_t vib = pgm_read_byte(&tables::sin[(vibpos+0)>>8]); 
            v += sag_wf(poses[0]>>8); poses[0] += ((PITCH_DH>>1) + 7 - vib)>>3;
            v += sag_wf(poses[1]>>8); poses[1] += ((PITCH_G>>3) - 3 + vib)>>3;
            v += sag_wf(poses[2]>>8); poses[2] += ((PITCH_A>>3) + 3 - vib)>>3;
            v += sag_wf(poses[3]>>8); poses[3] += ((PITCH_CH>>1) - 1 + vib)>>3;
            v += sag_wf(poses[4]>>8); poses[4] += ((PITCH_E>>1) - 3 - vib)>>3;
            vibpos += 20; 
            db[i] += v>>shr;
        }
    }
} chord;

struct Amen
{
    uint8_t mul_shl;
    uint8_t prev_breakstep;

    #define BREAK_FREQUENCY 10
    #define BREAK_OFFSET_MAGNITUDE (9+(myrand::rand16()&0x3))
    #define BREAK_SHUFFLE_AMOUNT 2


    void init()
    {
        prev_breakstep = 0;
        mul_shl = 12; 
    }

    void trigger(Drumpf &drumpf, BassDrum &bd, uint16_t pos)
    {
        
        uint8_t breakstep = (pos>>BREAK_FREQUENCY); 
        if ( prev_breakstep != breakstep )
        {
            // Shuffle sequence
            for ( uint8_t breaks = 0; breaks<BREAK_SHUFFLE_AMOUNT; breaks++)
            {
                shuffler.shuffle();
            }
            // Break offset multiplier
            mul_shl = BREAK_OFFSET_MAGNITUDE;
        }
        prev_breakstep = breakstep;

        // Only apply break during a specific timewindow part of the sequence
        if ( (((pos<<1) & 0x1C908) >= 0x0800) ) 
        {
            pos += shuffler[breakstep] << mul_shl;
        }


        // Trigger amen samples
        switch (pos & 0x1FFF)
        {
        case 0x0000:
            drumpf.trigger(AMEN_LOUDBDHH);
            bd.trigger(true);
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

} amen;


void Player::init()
{
    filter_on = false;
    basenote = 0;
    new_one_liner();
    shuffler.init();
    amen.init();
    bd.init();
    drumpf.init();
    synth.init();
    arpeggio.init();
    one_liner.init();
    hh.init();
    compressor.init();
    pos = 0;
//    synth.trigger(PITCH_C);
}

bool drumblocker(uint16_t pos)
{
    static uint8_t bit;
    if ((pos & 0xffff) < 0xC000)
    {
        bit = -1;
        return false;
    }
    else if ((pos & 0xffff) == 0xf000)
    {
        uint8_t tmp = bit;
        bit = 7 + (myrand::rand8()&1);
        if (bit == tmp) bit++;
    }
    else if ((pos & 0x0fff)==0)
    {
        bit += (myrand::rand8()&1)+1;
    }

    return (pos>>bit)&1;
}

void phaser_test(int pos, Buffer &db, Buffer &pb)
{
    static bool prev_entered_phaser;

    static int8_t phaser_depth;
    static int8_t phaser_shift;
    static int8_t phaser_speed;
 //   static int8_t filter_speed;
    static bool phaser_neg;
    static int8_t phaser_strength;

    if ((pos & 0x7ff) == 0x80)
    {
        phaser_depth = (myrand::rand16() & 0x0F);
        phaser_shift = 16 - phaser_depth;
        phaser_speed = (myrand::rand16() & 0xff) + 1;
 //       filter_speed = (myrand::rand16() & 0x3f) + 1;
        phaser_neg = int16_t(myrand::rand16()) >= 0;
        phaser_strength = (myrand::rand16() & 1) + 1;
    }

    // phaser
    {
        static uint16_t t;

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
    #if 0
    static uint16_t filter_t;
    static int16_t rprev;
    if (!filter_on)
    {
        filter_t = 0;
        rprev = 0;
    }
    else
    {
        uint8_t phaser_filter = pgm_read_byte(&tables::sin[filter_t >> 8]) + 128;

        for (uint8_t i = 0; i < globals::SAMPLES_PER_BUFFER; i++)
        {
            int16_t r = db[i];
            if (r >= 0x0800)
                r = 0x07FF;
            if (r < -0x0800)
                r = -0x0800;
            r <<= 4;
            r = mymath::mulhi_s8u8(clamp8(r - rprev), phaser_filter) + rprev;
            db[i] = r >> 4;
            rprev = r;
        }
        filter_t += filter_speed;
    }
    #endif
}



bool Player::render(Buffer &db, Buffer &pb)
{
    myrand::rand32();

    uint8_t row = pos >> 7;
    uint8_t pat = pos >> 13;
    bool nrow = (pos & 0x7F) == 0x00;
    bool npat = (pos & 0x1FFF) == 0x00;

    Buffer mixin;
    db.clear();
    mixin.clear();


    
    if (pat >= 0x1e) // exit
        return false;
    else if (pat < 4 || pat >= 0x1C) // intro and ending
    {
        chord.shr = 4;

        if (npat)
            synth.trigger(PITCH_C>>7);
  
        // change oneliner settings
        if (npat)
            new_one_liner();
        
        synth.render(mixin, synth_wf);

        if (pat>=2  && pat < 0x1c)
            one_liner.render(mixin, one_liner_sel);
            
        compressor.render(db, db, mixin);

        if (pat>=3 && pat < 0x1d)
            chord.render(db);
    }
    else
    {
        chord.shr = 4;

        // Trigger amen
        if ( pos >= 0x10000 )
        {
            amen.trigger(drumpf, bd, pos);
        }   

        // trigger some extra drums
        if (npat)
        {
            if (pat < 8)
            {
                if ((pat & 1) == 0)
                {
                    drumpf.trigger(AMEN_CRASHBD);
                    bd.trigger(true);
                }
            }
        }
        uint8_t t = row&63;
        if (nrow)
        {
            if ( pat == 7 && (t==0 || t== 6 || t==32 || t == 38 || t == 48 || t == 54 )
            || (pat > 4 && t == 62))
            {
                drumpf.trigger(AMEN_LOUDBDHH);
                bd.trigger(false);
            }
        }
        // change oneliner settings
        if (npat)
            new_one_liner();

        // Trigger hihat
        const uint16_t hh_pre = 0x100;
        const uint16_t hh_pre2 = 0x600;
        if (pos < 0x08000 || ((pos & 0xFFFF) >= 0x08000 - hh_pre2) && ((pos & 0xFFFF) < 0x10000 - hh_pre))
        {
            if ((pos & 0xFF) == 0)
                hh.trigger(0x1A, 0x05);
        }
        else
        {
            if ((pos & 0x7F) == 0)
                hh.trigger(0x60, 0xC0);
        }

        // trigger synth
        if (((pos & 0xFFF) == 0x00) || (pos & 0xFFF) == 0x700 )
        {
            static const uint16_t arp[] PROGMEM = {
                PITCH_C>>7,
                PITCH_C>>7,
                PITCH_C>>7,
                PITCH_FH>>7,
                PITCH_C>>7,
                PITCH_FH>>7,
                PITCH_C>>6,
                PITCH_FH>>6,
            };
            static const uint8_t base[] PROGMEM = {
                0,0,0,4,0,4,0,4
            };
            uint8_t t = shuffler[(pos+0x100)>>11] & 7;
            synth.trigger(pgm_read_word(&arp[t]));
            basenote = pgm_read_word(&base[t]);
        }

        if ( (pos & 0x7F) == 0x00 )
        {
            static const uint16_t arp_arp[] PROGMEM = { 
                (uint32_t(PITCH_C)*2)>>7,
                (uint32_t(PITCH_C)*3)>>7,
                (uint32_t(PITCH_C)*4)>>7, 
                (uint32_t(PITCH_C)*6)>>7,
                (uint32_t(PITCH_FH)*2)>>7,
                (uint32_t(PITCH_FH)*3)>>7,
                (uint32_t(PITCH_FH)*4)>>7,
                (uint32_t(PITCH_FH)*6)>>7,
            }; 
            uint8_t t = shuffler[pos>>7] & 3;
            arpeggio.trigger(pgm_read_word(&arp_arp[t+basenote]));
            arpeggio.set_portamento_speed(1);
            if ( pos >= 0x20000 )
            {
                arpeggio.set_decay_speed((myrand::rand8()&0xf) +1);
            } else {
            arpeggio.set_decay_speed(15);
        }
        } else if ( (pos & 0x7F) == 0x8 ) 
        {
            arpeggio.release();
        }



        if ( pos < 0x8000 || (pat < 8) || !drumblocker(pos) )
        {
            if ( pos >= 0x10000 )
            {
                bd.render(db);
            }
        }

        if ( pos < 0x10000 || ((pos & 0xFFFF) < 0x08000))
            synth.render(mixin, synth_wf);

        if ( pos >= 0x20000 && ((pos & 0xFFFF) < 0x08000))
        {
            switch ( (pos>>16) & 1 )
            {
                case 0: 
                    arpeggio.render(mixin, sin_wf);
                    break;
                case 1: 
                    arpeggio.render(mixin, sqr_wf);
                    break;
            }
        }

        /* anything rendered to db below here does not affect compressor */
        compressor.render(db, db, mixin);

        if ( pos >= 0x10000 && ((pos & 0xFFFF) >= 0x08000))
        {
            chord.render(db);
        }

        if (pos >= 0x8000)
            hh.render(db);

        if ( pos < 0x8000 || (pat < 8) || !drumblocker(pos) )
        {
            drumpf.render(db);
        }

    }

    phaser_test(pos, db, pb);
    
    pos ++;

    return true;
}
