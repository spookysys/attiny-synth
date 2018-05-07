#include "Drumpf.hpp"
#include "mymath.hpp"
#include "myrand.hpp"

using namespace std;

namespace drums
{
struct AdpcmSample
{
    uint16_t len;
    const uint8_t *data;
};

struct DrumEnv
{
    uint16_t volume;
    uint16_t slope;
};

struct DrumFilter
{
    int16_t a2, a3;
    int8_t b1, b2, b3;
};

struct Drum
{
    DrumEnv treble_env;
    DrumFilter treble_filter;
    AdpcmSample bass_sample;
};

#include "export/datas.inc"

const Drum drums[] PROGMEM = {
#include "export/structs.inc"
};

}

uint8_t Drumpf::AdpcmDecoder::getIdx()
{
    uint8_t ret = word & 3;
    word >>= 2;
    subidx--;
    if (subidx == 0)
    {
        subidx = 4;
        word = ((ptr + 1) != end) ? pgm_read_byte(ptr) : 0;
        ptr++;
    }
    return ret;
}

void Drumpf::AdpcmDecoder::reset()
{
    ptr = end;
}

void Drumpf::reset()
{
    adpcmDecoder.reset();
    bass_0 = 0;
    bass_1 = 0;
    treble_volume = 0;
    treble_half = 0;
    treble_slope = 0;
}

void Drumpf::AdpcmDecoder::trigger(const drums::AdpcmSample &sample)
{
    if (sample.data)
    {
        this->ptr = sample.data + 1;
        this->end = sample.data + sample.len + 1;
    }
    else
    {
        this->ptr = nullptr;
        this->end = nullptr;
    }
    if (isActive())
    {
        this->word = pgm_read_byte(sample.data);
        this->subidx = 4;
    }
    this->recon_1 = 0;
    this->recon_2 = 0;
    this->modifier = 0;
}

bool Drumpf::AdpcmDecoder::isActive()
{
    return ptr != end;
}

int8_t Drumpf::AdpcmDecoder::get()
{
    uint8_t idx = getIdx();
    switch (idx)
    {
    case 0:
        break;
    case 1:
        modifier = -modifier;
        break;
    case 2:
        modifier = modifier ? (modifier << 1) : 1;
        break;
    case 3:
        modifier = modifier ? (modifier >> 1) : -1;
        break;
    }
    int8_t prediction = recon_1 + recon_1 - recon_2;
    int8_t recon = prediction + modifier;
    this->recon_2 = this->recon_1;
    this->recon_1 = recon;
    return recon;
}

void Drumpf::Filter::init(const drums::DrumFilter &op)
{
    a2 = op.a2;
    a3 = op.a3;
    b1 = op.b1;
    b2 = op.b2;
    b3 = op.b3;
    xn_1 = 0;
    yn_1 = 0;
    xn_2 = 0;
    yn_2 = 0;
}
int8_t Drumpf::Filter::get(int8_t xx)
{
    int8_t b1_xx = mymath::mul_s8_s8s8_shr8(b1, xx);
    int8_t b2_x1 = mymath::mul_s8_s8s8_shr8(b2, xn_1);
    int8_t b3_x2 = mymath::mul_s8_s8s8_shr8(b3, xn_2);
    this->xn_2 = this->xn_1;
    this->xn_1 = xx;

    int16_t a2_y1 = mymath::mul_s16_s16s8_shr8(a2, yn_1);
    int16_t a3_y2 = mymath::mul_s16_s16s8_shr8(a3, yn_2);
    int16_t yy = int8_t(b1_xx + b2_x1 + b3_x2) - a2_y1 - a3_y2;
    if (yy > 127)
        yy = 127;
    if (yy < -128)
        yy = -128;
    this->yn_2 = this->yn_1;
    this->yn_1 = yy;
    return yy;
}

static inline int8_t average(int8_t a, int8_t b, uint8_t &dither)
{
    int8_t val = (int16_t(a) + int16_t(b) + (dither & 1)) >> 1;
    dither >>= 1;
    return val;
}

// lerp
static void lerp(int8_t start, int8_t end, Buffer &v)
{
    int16_t iter = start << globals::SAMPLES_PER_BUFFER_LOG2;
    int16_t step = int16_t(end) - int16_t(start);
    uint16_t dither = myrand::rand16();

    for (uint8_t i = 0; i < globals::SAMPLES_PER_BUFFER; i++)
    {
		int16_t tmp = iter >> (globals::SAMPLES_PER_BUFFER_LOG2 - 1);
		tmp += dither & 1;
		tmp >>= 1;
        v[i] += tmp;
        dither >>= 1;
        iter += step;
    }
}

void Drumpf::trigger(DrumEnums op)
{
    drums::Drum drum;
	memcpy_P(&drum, &drums::drums[op], sizeof(drums::Drum));
    adpcmDecoder.trigger(drum.bass_sample);
    treble_volume = uint32_t(drum.treble_env.volume) << 16;
    treble_half = treble_volume >> 1;
    treble_slope = uint32_t(drum.treble_env.slope) << 1;
    treble_filter.init(drum.treble_filter);
    bass_0 = 0;
    bass_1 = 0;
}

void Drumpf::render(Buffer &dest)
{
    // bass
    if (adpcmDecoder.isActive())
    {
        this->bass_1 = bass_0;
        this->bass_0 = adpcmDecoder.get();
        lerp(this->bass_1, bass_0, dest);
    }

    // treble
    if (this->treble_volume)
    {
        uint16_t vol_ = this->treble_volume >> 16;
        uint8_t vol = (vol_ > 255) ? 255 : vol_;

        for (int i = 0; i < globals::SAMPLES_PER_BUFFER; i++)
        {
            int8_t noiz = myrand::rand8();
			//int8_t val = noiz;
            //int8_t val = treble_filter.get(noiz);
            //val = mymath::mul_s8_s8u8_shr8(val, vol);
            //dest[i] += noiz;
        }

        if (treble_half < treble_slope)
        {
            treble_half = treble_volume >> 1;
            treble_slope = treble_slope >> 1;
        }
        treble_volume -= treble_slope;
        treble_half -= treble_slope;
    }
}
