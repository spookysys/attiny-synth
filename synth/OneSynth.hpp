#pragma once
#include "common.hpp"
#include "Buffer.hpp"
#include "mymath.hpp"

// Play a waveform at different pitches with a simple volume envelope
class OneSynth
{
	uint8_t decay_speed;
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

	void render_inner(Buffer &db, int8_t *v)
	{
		switch (state)
		{
		default:
		case OFF:
			return;
		case RAMP:
			db[0] += mymath::mul_s8s8u8_shr8(v[0], 0x10);
			db[1] += mymath::mul_s8s8u8_shr8(v[1], 0x30);
			db[2] += mymath::mul_s8s8u8_shr8(v[2], 0x50);
			db[3] += mymath::mul_s8s8u8_shr8(v[3], 0x70);
			db[4] += mymath::mul_s8s8u8_shr8(v[4], 0x90);
			db[5] += mymath::mul_s8s8u8_shr8(v[5], 0xB0);
			db[6] += mymath::mul_s8s8u8_shr8(v[6], 0xD0);
			db[7] += mymath::mul_s8s8u8_shr8(v[7], 0xF0);
			state = SUSTAIN;
			break;
		case SUSTAIN:
			db[0] += v[0];
			db[1] += v[1];
			db[2] += v[2];
			db[3] += v[3];
			db[4] += v[4];
			db[5] += v[5];
			db[6] += v[6];
			db[7] += v[7];
			break;
		case DECAY:
			db[0] += mymath::mul_s8s8u8_shr8(v[0], vol >> 8);
			db[1] += mymath::mul_s8s8u8_shr8(v[1], vol >> 8);
			db[2] += mymath::mul_s8s8u8_shr8(v[2], vol >> 8);
			db[3] += mymath::mul_s8s8u8_shr8(v[3], vol >> 8);
			db[4] += mymath::mul_s8s8u8_shr8(v[4], vol >> 8);
			db[5] += mymath::mul_s8s8u8_shr8(v[5], vol >> 8);
			db[6] += mymath::mul_s8s8u8_shr8(v[6], vol >> 8);
			db[7] += mymath::mul_s8s8u8_shr8(v[7], vol >> 8);
			vol -= uint16_t(vol >> decay_speed);
			if ((vol >> 8) == 0)
				this->state = OFF;
			break;
		}
	}

  public:
	OneSynth(uint8_t decay_speed = 6)
	{
		reset(decay_speed);
	}

	void reset(uint8_t decay_speed)
	{
		this->decay_speed = decay_speed;
		this->pos = 0;
		this->vol = 0;
		this->state = OFF;
	}

	template <typename WaveformFunc>
	void render(Buffer &db, WaveformFunc waveform_func)
	{
		if (state == OFF)
			return;

		int8_t v[globals::SAMPLES_PER_BUFFER];

		v[0] = waveform_func(pos >> 8);
		pos += pitch;
		v[1] = waveform_func(pos >> 8);
		pos += pitch;
		v[2] = waveform_func(pos >> 8);
		pos += pitch;
		v[3] = waveform_func(pos >> 8);
		pos += pitch;
		v[4] = waveform_func(pos >> 8);
		pos += pitch;
		v[5] = waveform_func(pos >> 8);
		pos += pitch;
		v[6] = waveform_func(pos >> 8);
		pos += pitch;
		v[7] = waveform_func(pos >> 8);
		pos += pitch;
		render_inner(db, v);
	}

	void trigger(uint32_t pitch, uint32_t pos = -1)
	{
		state = RAMP;
		this->pitch = pitch;
		if (pos != uint32_t(-1))
			this->pos = pos;
	}

	void release()
	{
		vol = 0xFFFF;
		state = DECAY;
	}
};
