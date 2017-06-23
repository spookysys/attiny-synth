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
        int8_t r0 = mymath::mul_s8s8u8_shr8(myrand::rand8(), vol);
        int8_t r1 = mymath::mul_s8s8u8_shr8(myrand::rand8(), vol);
        int8_t r2 = mymath::mul_s8s8u8_shr8(myrand::rand8(), vol);
        int8_t r3 = mymath::mul_s8s8u8_shr8(myrand::rand8(), vol);
        db[0] += r0; // supercheap highpass filter
        db[1] -= r0;
        db[2] += r1;
        db[3] -= r1;
        db[4] += r2;
        db[5] -= r2;
        db[6] += r3;
        db[7] -= r3;
        if (this->volume >= this->falloff)
            this->volume -= this->falloff;
        else
            this->volume = 0;
    }
}
