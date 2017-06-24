#pragma once
#include "common.hpp"
#include <assert.h>
//#include <array>

//using Buffer = int16_t[globals::SAMPLES_PER_BUFFER];

//namespace buffers {
//    void clear(Buffer& d);
//}

struct Buffer 
{
	int16_t data_[globals::SAMPLES_PER_BUFFER];
public:
	const int16_t operator[](uint8_t i) const
	{
		assert(i<globals::SAMPLES_PER_BUFFER);
		return data_[i];
	}
	int16_t& operator[](uint8_t i)
	{
		assert(i<globals::SAMPLES_PER_BUFFER);
		return data_[i];
	}
	int16_t* data() {
		return data_;
	}
	void clear() {
	    for (auto& iter : data_) iter = 0;
	}
};


