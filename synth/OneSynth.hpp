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
		{
			constexpr uint8_t amp_inc = 255 / (globals::SAMPLES_PER_BUFFER + 1);
			uint8_t amp = 0;
			for (uint8_t i = 0; i < globals::SAMPLES_PER_BUFFER; i += 4)
			{
				amp += amp_inc;
				db[i + 0] += mymath::mulhi_s8u8(v[i + 0], amp);
				amp += amp_inc;
				db[i + 1] += mymath::mulhi_s8u8(v[i + 1], amp);
				amp += amp_inc;
				db[i + 2] += mymath::mulhi_s8u8(v[i + 2], amp);
				amp += amp_inc;
				db[i + 3] += mymath::mulhi_s8u8(v[i + 3], amp);
			}
			state = SUSTAIN;
		}
		break;
		case SUSTAIN:
		{
			for (uint8_t i = 0; i < globals::SAMPLES_PER_BUFFER; i += 4)
			{
				db[i + 0] += v[i + 0];
				db[i + 1] += v[i + 1];
				db[i + 2] += v[i + 2];
				db[i + 3] += v[i + 3];
			}
		}
		break;
		case DECAY:
		{
			for (uint8_t i = 0; i < globals::SAMPLES_PER_BUFFER; i += 4)
			{
				db[i + 0] += mymath::mulhi_s8u8(v[i + 0], vol >> 8);
				db[i + 1] += mymath::mulhi_s8u8(v[i + 1], vol >> 8);
				db[i + 2] += mymath::mulhi_s8u8(v[i + 2], vol >> 8);
				db[i + 3] += mymath::mulhi_s8u8(v[i + 3], vol >> 8);
				vol -= uint16_t(vol >> decay_speed);
				if ((vol >> 8) == 0)
				{
					this->state = OFF;
					break;
				}
			}
		}
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
