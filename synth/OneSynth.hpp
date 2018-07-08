#pragma once
#include "common.hpp"
#include "Buffer.hpp"
#include "mymath.hpp"
#include <stdio.h>

// Play a waveform at different pitches with a simple volume envelope
class OneSynth
{
	uint8_t decay_speed;
	uint8_t portamento_speed;
	uint8_t portamento_iterator;

	uint32_t pos;
	uint16_t vol;
	uint16_t pitch;
	uint16_t dest_pitch;


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
				uint16_t prev_vol = vol;
				vol -= decay_speed<<4; // uint16_t(vol >> decay_speed);
				// vol >>= 1; // (decay_speed<<8); // uint16_t(vol >> decay_speed);
				printf( "%hu ", vol);
				if (prev_vol < vol )
				{
					vol = 0;
					this->state = OFF;
					break;
				}
			}
		}
		break;
		}
	}

  public:
	OneSynth(uint8_t decay_speed = 1)
	{
		reset(decay_speed);
	}

	void mul_pitch(int8_t mul)
	{
		if ( mul == 0 ) return;
		if ( mul > 0 ) {
			this->pitch <<= mul;
		} else {
			this->pitch >>= -mul;
		}
		this->portamento_speed = 3;
	}

	void set_portamento_speed(int8_t speed)
	{
		this->portamento_speed = speed;
	}	
	void set_decay_speed(uint8_t speed)
	{
		this->decay_speed = speed;
	}
	void reset(uint8_t decay_speed)
	{
		this->decay_speed = decay_speed;
		this->pos = 0;
		this->vol = 0;
		this->portamento_speed = 60;
		this->state = OFF;
	}

	template <typename WaveformFunc>
	void render(Buffer &db, WaveformFunc waveform_func)
	{
		if (state == OFF)
			return;

		int8_t v[globals::SAMPLES_PER_BUFFER];

		for ( int i = 0; i<globals::SAMPLES_PER_BUFFER; i++ )
		{
			v[i] = waveform_func(pos >> 8);
			pos += pitch;
			if ( portamento_iterator == portamento_speed  )
			{
				if ( pitch < dest_pitch ) pitch+=4;
				if ( pitch > dest_pitch ) pitch-=4;
				portamento_iterator = 0;
			}
			portamento_iterator++;
		}


		render_inner(db, v);
	}

	void trigger(uint16_t pitch, uint16_t pos = -1)
	{
		state = RAMP;
		this->dest_pitch = pitch;
		this->portamento_iterator = 0;
		this->portamento_speed = 140;
//		this->pitch = pitch;
		if (pos != uint16_t(-1))
			this->pos = pos;
	}

	void release()
	{
		vol = 0xFFFF;
		state = DECAY;
	}
};
