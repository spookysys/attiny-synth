#pragma once

struct adpcm_state_t {
	const unsigned char* data;
	int valprev;
	int index;
};

void adpcm_init(adpcm_state_t* state, const unsigned char *adpcm_data);
void adpcm_render(adpcm_state_t* state, signed char *dst, int len);
