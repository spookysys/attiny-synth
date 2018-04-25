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
    static_assert(sizeof(tables::sin)==256, "Unexpected table size");
    
    switch(state) {
        case OFF: 
            break;
        case SLIDE: {
            int8_t tmp;
            for (uint8_t i=0; i<globals::SAMPLES_PER_BUFFER; i+=4)
            {
                db[i+0] += (int8_t)pgm_read_byte(&tables::sin[pos>>8]); pos += pitch_vol;
                db[i+1] += (int8_t)pgm_read_byte(&tables::sin[pos>>8]); pos += pitch_vol;
                db[i+2] += (int8_t)pgm_read_byte(&tables::sin[pos>>8]); pos += pitch_vol;
                tmp = (int8_t)pgm_read_byte(&tables::sin[pos>>8]); pos += pitch_vol;
                db[i+3] += tmp;
            }
            pitch_vol -= pitch_vol>>5;
            if (pitch_vol < 0x80) {
                this->state = DECAY;
                this->pitch_vol = 0xFFFF; // pitch_vol is now volume
                this->last_v = tmp;
            }
        } break;
        case DECAY:
        {
            int8_t tmp = this->last_v;
            for (uint8_t i=0; i<globals::SAMPLES_PER_BUFFER; i+=4)
            {
                int8_t v0, v1;
                v0 = (int8_t)pgm_read_byte(&tables::sin[pos>>8]); pos += 0x100;
                v1 = (int8_t)pgm_read_byte(&tables::sin[pos>>8]); pos += 0x100;
                v0 = mymath::mul_s8_s8u8_shr8(v0, this->pitch_vol>>8); pitch_vol -= pitch_vol>>7;
                v1 = mymath::mul_s8_s8u8_shr8(v1, this->pitch_vol>>8); pitch_vol -= pitch_vol>>7;
                db[i+0] += (tmp>>1)+(v0>>1);
                db[i+1] += v0;
                db[i+2] += (v0>>1)+(v1>>1);
                db[i+3] += v1;
                tmp = v1;
            }
            this->last_v = tmp;
            if (pitch_vol < 0x100)
                state = OFF;
        } break;
    }
}

