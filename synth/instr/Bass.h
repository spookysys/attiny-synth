#pragma once
#include "common.h"
#include "lib/Envelope.h"
#include "lib/oneliners.h"
#include "lib/OneSynth.h"

namespace instr
{

	class Bass
	{
		using MyEnv = lib::Envelope<1000, 256, 0x8000, 50>;
		
		lib::OneSynth<8, lib::oneliners::sine, MyEnv> synth;
		
	public:
		void reset()
		{
			synth.reset();
		}
		void trigger(uint16_t pitch)
		{
			synth.setPitch(pitch);
			synth.trigger();
		}
		void release()
		{
			synth.release();
		}
		void render(uint8_t buff)
		{
			synth.render(buff);
		}
	};
	
	
}