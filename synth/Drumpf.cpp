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
    uint16_t increment;
    uint16_t initial;
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

const Drum drums[] = {
#include "export/structs.inc"
};

const char *names[] = {
#include "export/names.inc"
};

const int num = sizeof(drums) / sizeof(*drums);
}

uint8_t Drumpf::AdpcmDecoder::getIdx()
{
    uint8_t ret = word & 3;
    word >>= 2;
    subidx--;
    if (subidx == 0)
    {
        subidx = 4;
        word = ((ptr + 1) != end) ? *ptr : 0;
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
    treble_amplitude = 0;
    treble_increment = 0;
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
        this->word = *sample.data;
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
    int8_t a2_y1 = mymath::mul_s8_s16s8_shr8(a2, yn_1);
    int8_t a3_y2 = mymath::mul_s8_s16s8_shr8(a3, yn_2);
    int8_t b1_xx = mymath::mul_s8_s8s8_shr8(b1, xx);
    int8_t b2_x1 = mymath::mul_s8_s8s8_shr8(b2, xn_1);
    int8_t b3_x2 = mymath::mul_s8_s8s8_shr8(b3, xn_2);
    int8_t yy = b1_xx + b2_x1 + b3_x2 - a2_y1 - a3_y2;

    this->xn_2 = this->xn_1;
    this->yn_2 = this->yn_1;
    this->xn_1 = xx;
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
static void lerp8(int8_t start, int8_t end, int8_t v[8])
{
    uint8_t dither = myrand::rand8();
    v[0] = start;
    v[4] = average(v[0], end, dither);
    v[2] = average(v[0], v[4], dither);
    v[6] = average(v[4], end, dither);
    v[1] = average(v[0], v[2], dither);
    v[3] = average(v[2], v[4], dither);
    v[5] = average(v[4], v[6], dither);
    v[7] = average(v[6], end, dither);
}

void Drumpf::trigger(DrumEnums op)
{
    const auto drum = drums::drums[op];
    adpcmDecoder.trigger(drum.bass_sample);
    treble_amplitude = uint32_t(drum.treble_env.initial) << 8;
    treble_increment = drum.treble_env.increment;
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
        int8_t v[globals::SAMPLES_PER_BUFFER];
        lerp8(this->bass_1, bass_0, v);
        for (int i = 0; i < globals::SAMPLES_PER_BUFFER; i++)
        {
            dest[i] += v[i];
        }
    }

    // treble
    if (this->treble_amplitude)
    {
        uint8_t amplitude = (this->treble_amplitude >> 16) ? 0xFF : (this->treble_amplitude >> 8);

        for (int i = 0; i < globals::SAMPLES_PER_BUFFER; i++)
        {
            int8_t noiz = myrand::rand8();
            int8_t val = treble_filter.get(noiz);
            val = mymath::mul_s8_s8u8_shr8(val, amplitude);
            dest[i] += val;
        }

        if (this->treble_amplitude >= this->treble_increment)
            this->treble_amplitude -= this->treble_increment;
        else
            this->treble_amplitude = 0;
    }
}
