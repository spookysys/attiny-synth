/*
Firmware to abuse an ATTiny45 as an FM-transmitter
(C) 2012 Jeroen Domburg (jeroen AT spritesmods.com)

This program is free software: you can redistribute it and/or modify
t under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
#include <avr/interrupt.h>
#include "io.h"
#include "music/config.h"
#include "music/dds.h"
#include "music/player.h"

void music_init() {
	player_reset();
}

ISR(TIM1_OVF_vect) {
	static short d=0;
	d++;
	if (d==(DDS_FREQ/PLAYER_FREQ)) {
		d=0;
		player_tick();
//		dds_play_next_channel();
	}
	setspeaker(dds_get_next_sample());
}