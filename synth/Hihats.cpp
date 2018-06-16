#include "Hihats.hpp"
#include "myrand.hpp"
#include "mymath.hpp"
Hihats::Hihats()
{
}

void Hihats::trigger(uint8_t volume, uint8_t falloff)
{
    this->volume = uint16_t(volume) << 8;
    this->falloff = falloff;
}

void Hihats::render(Buffer &db)
{
    uint8_t vol = this->volume >> 8;
    if (vol) {
        for (int8_t i=0; i<globals::SAMPLES_PER_BUFFER; i+=4)
        {
            int8_t r0 = mymath::mulhi_s8u8(myrand::rand8(), vol);
            int8_t r1 = mymath::mulhi_s8u8(myrand::rand8(), vol);
            db[i+0] += r0; // supercheap highpass filter
            db[i+1] -= r0;
            db[i+2] += r1;
            db[i+3] -= r1;
            if (this->volume >= this->falloff)
                this->volume -= this->falloff;
            else
                this->volume = 0;
        }
    }
}
