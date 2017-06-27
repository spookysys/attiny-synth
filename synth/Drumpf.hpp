#pragma once
#include "common.hpp"
#include "Buffer.hpp"

#ifdef AVR
#define USE_AMEN_01
#define USE_AMEN_02
#define USE_AMEN_03
#define USE_AMEN_04
#define USE_AMEN_05
#define USE_AMEN_06
//#define USE_AMEN_07
#define USE_AMEN_08
#define USE_AMEN_09
#define USE_AMEN_10
#define USE_AMEN_11
#define USE_AMEN_12
#define USE_AMEN_13
//#define USE_AMEN_14
#define USE_AMEN_15
#define USE_AMEN_16
//#define USE_AMEN_17
//#define USE_AMEN_18
#define USE_AMEN_19
#define USE_AMEN_20
//#define USE_AMEN_21
//#define USE_AMEN_22
#define USE_AMEN_23
//#define USE_AMEN_24
//#define USE_AMEN_25
//#define USE_AMEN_26
//#define USE_AMEN_27
//#define USE_AMEN_28
//#define USE_AMEN_29
//#define USE_AMEN_30
//#define USE_AMEN_31
//#define USE_AMEN_32
//#define USE_AMEN_33
//#define USE_AMEN_34
#define USE_AMEN_35
//#define USE_AMEN_36
//#define USE_AMEN_37
//#define USE_AMEN_38
#define USE_AMEN_39
#define USE_AMEN_40
//#define USE_AMEN_41
#define USE_AMEN_42
#define USE_AMEN_43
#define USE_AMEN_44
#define USE_AMEN_45
#define USE_AMEN_46
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
        int8_t a2, a3, b1, b2, b3;
        int8_t xn_1, xn_2;
        int8_t yn_1, yn_2;

      public:
        void init(const drums::DrumFilter &op);
        int8_t get(int8_t xx);
    };

    AdpcmDecoder adpcmDecoder;
    int8_t bass_0, bass_1;
    uint32_t treble_amplitude;
    uint16_t treble_increment;
    Filter treble_filter;

  public:
    Drumpf() { reset(); }
    void reset();
    void trigger(DrumEnums op);
    void render(Buffer &db);
};
