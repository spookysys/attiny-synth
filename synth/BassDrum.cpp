#include "BassDrum.hpp"
#include "tables.hpp"
#include "mymath.hpp"
#include "misc.hpp"

static const uint16_t start_pitch=1000;
static const uint16_t end_pitch=128;
static const uint16_t slide_speed_1=6;
static const uint16_t slide_speed_2=7;
static const uint16_t decay_speed=3;

void BassDrum::trigger()
{
    this->vol = 0xFF;
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
                db[i+0] += (int8_t)pgm_read_byte(&tables::sin[pos>>8]); pos += pitch;
                db[i+1] += (int8_t)pgm_read_byte(&tables::sin[pos>>8]); pos += pitch;
                db[i+2] += (int8_t)pgm_read_byte(&tables::sin[pos>>8]); pos += pitch;
                lastval = (int8_t)pgm_read_byte(&tables::sin[pos>>8]); pos += pitch;
                db[i+3] += lastval;
            }
            int16_t pitch_diff = (pitch >> slide_speed_1) + (pitch >> slide_speed_2);
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

