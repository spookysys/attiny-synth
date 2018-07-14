#pragma once
#include "misc.hpp"
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


  public:

	void set_portamento_speed(int8_t speed)
	{
		this->portamento_speed = speed;
	}	
	void set_decay_speed(uint8_t speed)
	{
		this->decay_speed = speed << 2;
	}
	void init()
	{
		this->portamento_iterator = 0;
		this->pitch = 0;
		this->dest_pitch = 0;
		this->decay_speed = 1 << 2;
		this->pos = 0;
		this->vol = 0;
		this->portamento_speed = 60;
		this->state = OFF;
	}

	void render(Buffer &db, int16_t(*synth_wf)(uint16_t))
	{
		if (state == OFF)
			return;

		constexpr uint16_t amp_inc_ramp = 0x10000 / (globals::SAMPLES_PER_BUFFER + 1);
		int8_t amp_inc = (state==RAMP) ? amp_inc_ramp : 
						 (state==DECAY) ? -decay_speed
						 : 0;

		for (uint8_t i = 0; i < globals::SAMPLES_PER_BUFFER; i ++)
		{
			vol += amp_inc;
			int16_t v = synth_wf(pos >> 8);
			if (state!=SUSTAIN)
				v = mymath::mulhi_s8u8(clamp8(v), vol >> 8);
			db[i] += v;

			pos += pitch;
			if ( portamento_iterator == portamento_speed  )
			{
				if ( pitch < dest_pitch ) pitch+=4;
				if ( pitch > dest_pitch ) pitch-=4;
				portamento_iterator = 0;
			}
			portamento_iterator++;
		}

		if (state==RAMP)
			state = SUSTAIN;
		else if (state==DECAY && vol < decay_speed * 8)
			state = OFF;
	}

	void trigger(uint16_t pitch)
	{
		state = RAMP;
		this->vol = 0;
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
