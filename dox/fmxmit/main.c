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

#define F_CPU 24000000 //after overclocking, it is :P
#include <avr/io.h>
#include <avr/wdt.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdio.h>
#include "overclock.h"
#include "music/player.h"
#include "music/dds.h"

//fuses: 0xf1 0xdf

static volatile char c1, c2;
static volatile char doPlayerTick;

ISR(TIM0_COMPA_vect) {
	static int ctr60Hz;

	if (dds_get_next_sample()) {
		OSCCAL=c1; //send 96MHz plus a bit
	} else {
		OSCCAL=c2; //send 96MHz minus a bit
	}

	//Main loop calls player code at 60Hz
	ctr60Hz++;
	if (ctr60Hz>=260) {
		ctr60Hz=0;
		doPlayerTick=1;
	}
}

//Main routine
int main(void) {
	TCCR0A=2; //CTC mode
	TCCR0B=2; //div by 8
	OCR0A=160; //15KHz
	TIMSK=(1<<4); //enable compa int

	TCCR1=1; //Enable tct1 to make some more waves (96MHz clock going through more silicon -> stronger signal)

	player_reset();
	overclockInit();
	overclockCpu(OVERCLOCK_MAX); //GO MORE QUICKER NAU!!!!!11

	c1=OSCCAL-1; c2=OSCCAL;
	sei();
	while(1) {
		//doPlayerTick is set to 1 by the interrupt code 60 times per second.
		if (doPlayerTick) {
			player_tick();
			doPlayerTick=0;
		}
	}
}
