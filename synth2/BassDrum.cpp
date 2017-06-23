#include "BassDrum.hpp"
#include "tables.hpp"
#include "mymath.hpp"

static const uint16_t start_pitch=1000;
static const uint16_t slide_speed=6;
static const uint16_t decay_speed=1;

/*
int8_t BassDrum::fadeIter() {
    vol -= mymath::mul_u8u8u8_shr8(vol, decay_speed) + 1;
    int8_t v = pgm_read_byte(&tables::sin[++pos_hi]);
    return scale8_s8(v, vol>>8);
}
*/


void BassDrum::trigger()
{
    this->pitch_vol = start_pitch;
    this->state = SLIDE;
}

void BassDrum::render(Buffer& db)
{
    static_assert(tables::sin.size()==256, "Unexpected table size");
    
    switch(state) {
        case OFF: 
            break;
        case SLIDE: {
            int8_t last_v;
            db[0] += (int8_t)pgm_read_byte(&tables::sin[pos>>8]); pos += pitch_vol;
            db[1] += (int8_t)pgm_read_byte(&tables::sin[pos>>8]); pos += pitch_vol;
            db[2] += (int8_t)pgm_read_byte(&tables::sin[pos>>8]); pos += pitch_vol;
            db[3] += (int8_t)pgm_read_byte(&tables::sin[pos>>8]); pos += pitch_vol;
            db[4] += (int8_t)pgm_read_byte(&tables::sin[pos>>8]); pos += pitch_vol;
            db[5] += (int8_t)pgm_read_byte(&tables::sin[pos>>8]); pos += pitch_vol;
            db[6] += (int8_t)pgm_read_byte(&tables::sin[pos>>8]); pos += pitch_vol;
            last_v = (int8_t)pgm_read_byte(&tables::sin[pos>>8]); pos += pitch_vol;
            db[7] += last_v;
            pitch_vol -= pitch_vol>>6;
            if (pitch_vol < 0x80) {
                this->last_v = last_v;
                this->state = DECAY;
                this->pitch_vol = 0xFFFF; // pitch_vol is now volume
            }
        } break;
        case DECAY:
        {
            int8_t v0, v1, v2, v3;
            v0 = (int8_t)pgm_read_byte(&tables::sin[pos>>8]); pos += 0x100;
            v1 = (int8_t)pgm_read_byte(&tables::sin[pos>>8]); pos += 0x100;
            v2 = (int8_t)pgm_read_byte(&tables::sin[pos>>8]); pos += 0x100;
            v3 = (int8_t)pgm_read_byte(&tables::sin[pos>>8]); pos += 0x100;
            v0 = mymath::mul_s8s8u8_shr8(v0, this->pitch_vol>>8); pitch_vol -= pitch_vol>>6;
            v1 = mymath::mul_s8s8u8_shr8(v1, this->pitch_vol>>8); pitch_vol -= pitch_vol>>6;
            v2 = mymath::mul_s8s8u8_shr8(v2, this->pitch_vol>>8); pitch_vol -= pitch_vol>>6;
            v3 = mymath::mul_s8s8u8_shr8(v3, this->pitch_vol>>8); pitch_vol -= pitch_vol>>6;
            db[0] += (this->last_v>>1)+(v0>>1);
            db[1] += v0;
            db[2] += (v0>>1)+(v1>>1);
            db[3] += v1;
            db[4] += (v1>>1)+(v2>>1);
            db[5] += v2;
            db[6] += (v2>>1)+(v3>>1);
            db[7] += v3;
            this->last_v = v3;
        } break;
    }
}

