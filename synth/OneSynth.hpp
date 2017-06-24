#pragma once
#include "common.hpp"
#include "Buffer.hpp"
#include "mymath.hpp"

// Play a waveform at different pitches with a simple volume envelope
template <uint8_t decay_speed=6 /*lower is faster*/>
class OneSynth
{
	uint32_t pos;
	uint16_t vol;
	uint16_t pitch;

	enum State
	{
		OFF = 0,
		RAMP,
		SUSTAIN,
		DECAY
	} state;

  public:
	OneSynth() { reset(); }

	void reset()
	{
		pos = 0;
		vol = 0;
		state = OFF;
	}

	template <typename WaveformFunc>
	void render(Buffer &db, WaveformFunc waveform_func)
	{
		if (state == OFF)
			return;
		int8_t v0 = waveform_func(pos >> 8);
		pos += pitch;
		int8_t v1 = waveform_func(pos >> 8);
		pos += pitch;
		int8_t v2 = waveform_func(pos >> 8);
		pos += pitch;
		int8_t v3 = waveform_func(pos >> 8);
		pos += pitch;
		int8_t v4 = waveform_func(pos >> 8);
		pos += pitch;
		int8_t v5 = waveform_func(pos >> 8);
		pos += pitch;
		int8_t v6 = waveform_func(pos >> 8);
		pos += pitch;
		int8_t v7 = waveform_func(pos >> 8);
		pos += pitch;
		switch (state)
		{
		case OFF:
			return;
		case RAMP:
			db[0] += mymath::mul_s8s8u8_shr8(v1, 0x10);
			db[1] += mymath::mul_s8s8u8_shr8(v1, 0x30);
			db[2] += mymath::mul_s8s8u8_shr8(v2, 0x50);
			db[3] += mymath::mul_s8s8u8_shr8(v3, 0x70);
			db[4] += mymath::mul_s8s8u8_shr8(v4, 0x90);
			db[5] += mymath::mul_s8s8u8_shr8(v5, 0xB0);
			db[6] += mymath::mul_s8s8u8_shr8(v6, 0xD0);
			db[7] += mymath::mul_s8s8u8_shr8(v7, 0xF0);
			state = SUSTAIN;
			break;
		case SUSTAIN:
			db[0] += v0;
			db[1] += v1;
			db[2] += v2;
			db[3] += v3;
			db[4] += v4;
			db[5] += v5;
			db[6] += v6;
			db[7] += v7;
			break;
		case DECAY:
		{
			db[0] += mymath::mul_s8s8u8_shr8(v0, vol >> 8);
			db[1] += mymath::mul_s8s8u8_shr8(v1, vol >> 8);
			db[2] += mymath::mul_s8s8u8_shr8(v2, vol >> 8);
			db[3] += mymath::mul_s8s8u8_shr8(v3, vol >> 8);
			db[4] += mymath::mul_s8s8u8_shr8(v4, vol >> 8);
			db[5] += mymath::mul_s8s8u8_shr8(v5, vol >> 8);
			db[6] += mymath::mul_s8s8u8_shr8(v6, vol >> 8);
			db[7] += mymath::mul_s8s8u8_shr8(v7, vol >> 8);
			vol -= uint16_t(vol >> decay_speed);
			if ((vol >> 8) == 0)
				this->state = OFF;
		}
		break;
		}
	}

	void trigger(uint32_t pitch, uint32_t pos = -1)
	{
		state = RAMP;
		this->pitch = pitch;
		if (pos != -1)
			this->pos = pos;
	}

	void release()
	{
		vol = 0xFFFF;
		state = DECAY;
	}
};
