#include "BassDrum.hpp"
#include "tables.hpp"
#include "mymath.hpp"
#include "misc.hpp"

// settings
static const uint16_t start_pitch=1000;
static const uint16_t end_pitch=100;
static const uint16_t slide_speed_1=5;
static const uint16_t slide_speed_2=6;
static const uint16_t decay_speed=3;
static const bool make_it_loud = false;

void BassDrum::trigger(bool accent)
{
    this->vol = accent ? 0xFF : 0x7F;
    this->state = SLIDE;
    this->pitch = start_pitch;
}

void BassDrum::render(Buffer& db)
{
    static_assert(sizeof(tables::sin)==256, "Unexpected table size");
    
    switch(state) {
        case OFF: 
            break;
        case SLIDE: {
            int8_t lastval;
            for (uint8_t i=0; i<globals::SAMPLES_PER_BUFFER; i+=4)
            {
                int8_t v0 = pgm_read_byte(&tables::sin[pos>>8]); pos += pitch;
                int8_t v1 = pgm_read_byte(&tables::sin[pos>>8]); pos += pitch;
                int8_t v2 = pgm_read_byte(&tables::sin[pos>>8]); pos += pitch;
                int8_t v3 = pgm_read_byte(&tables::sin[pos>>8]); pos += pitch;
                if (!make_it_loud)
                {
                    v0 >>= 1;
                    v1 >>= 1;
                    v2 >>= 1;
                    v3 >>= 1;
                }
                if (!(this->vol & 0x80))
                {
                    v0 >>= 1;
                    v1 >>= 1;
                    v2 >>= 1;
                    v3 >>= 1;
                }
                db[i+0] += v0; 
                db[i+1] += v1;
                db[i+2] += v2;
                db[i+3] += v3;
                lastval = v3;
            }
            int16_t pitch_diff = (pitch >> slide_speed_1) + (pitch >> slide_speed_2) + 1;
            if (pitch >= pitch_diff + end_pitch)
                pitch -= pitch_diff;
            else {
                this->lastval = lastval;
                this->state = DECAY;
                this->pitch = end_pitch << globals::SAMPLES_PER_BUFFER_LOG2;
            }
        } break;
        case DECAY:
        {
            int8_t nextval;
            pos += pitch;
            nextval = (int8_t)pgm_read_byte(&tables::sin[pos>>8]);
            nextval = mymath::mulhi_s8u8(nextval, this->vol);
            lerp(lastval, nextval, db);
            this->lastval = nextval;
            if (vol > decay_speed)
                vol -= decay_speed;
            else
                state = OFF;
        } break;
    }
}

