#pragma once
#include "common.hpp"
#include "Buffer.hpp"

#ifdef AVR

//#define USE_JK_SNR_03
//#define USE_KICK_VINYL02
//#define USE_KICK_808

#define USE_AMEN_BDHH_5
#define USE_AMEN_BDHHSOFT
#define USE_AMEN_BDSOFT_3
#define USE_AMEN_CRASHBD
#define USE_AMEN_HH_2
//#define USE_AMEN_HIT
#define USE_AMEN_LOUDBDHH
#define USE_AMEN_RIDE
#define USE_AMEN_SNARE
//#define USE_AMEN_SNAREHARD
//#define USE_AMEN_SNAREHISS
#define USE_AMEN_SOFTSNARE_7

#else
#include "export/use_all.inc"
#endif

enum DrumEnums
{
#include "export/enums.inc"
};




namespace drums
{
    struct AdpcmSample;
    class DrumFilter;
}

class Drumpf
{
    
    class AdpcmDecoder
    {
        const uint8_t *ptr;
        const uint8_t *end;
        uint8_t subidx;
        uint8_t word;

        uint8_t getIdx();

        int8_t recon_1;
        int8_t recon_2;
        int8_t modifier;

      public:
        void reset();
        void trigger(const drums::AdpcmSample &sample);
        bool isActive();
        int8_t get();
    };

    class Filter
    {
        int16_t a2, a3;
        int8_t b1, b2, b3;
        int8_t xn_1, xn_2;
        int8_t yn_1, yn_2;

      public:
        void init(const drums::DrumFilter &op);
        int8_t get(int8_t xx);
    };

    AdpcmDecoder adpcmDecoder;
    int8_t bass_0, bass_1;
	uint32_t treble_volume;
	uint32_t treble_half;
	uint32_t treble_slope;
    Filter treble_filter;

  public:
    void init() { reset(); }
    void reset();
    void trigger(DrumEnums op);
    void render(Buffer &db);
};
