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
	void init()
	{
		this->portamento_iterator = 0;
		this->pitch = 0;
		this->dest_pitch = 0;
		this->decay_speed = 1;
		this->pos = 0;
		this->vol = 0;
		this->portamento_speed = 60;
		this->state = OFF;
	}

	void render(Buffer &db, int16_t(*synth_wf)(uint16_t))
	{
		if (state == OFF)
			return;

		int8_t v[globals::SAMPLES_PER_BUFFER];

		for ( int i = 0; i<globals::SAMPLES_PER_BUFFER; i++ )
		{
			int16_t tmp = synth_wf(pos >> 8);
			if (tmp < -128)
				tmp = -128;
			else if (tmp > 127)
				tmp = 127;
			v[i] = tmp;
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

	void trigger(uint16_t pitch)
	{
		state = RAMP;
		this->dest_pitch = pitch;
		this->portamento_iterator = 0;
		this->portamento_speed = 140;
	}

	void release()
	{
		vol = 0xFFFF;
		state = DECAY;
	}
};
