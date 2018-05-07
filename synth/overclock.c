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


/*
Routines to try and overclock the ATTiny45, and to reset to
the factory 16MHz value if needed.
*/

#include <avr/io.h>
#include "overclock.h"
#define nop() asm("nop")

static unsigned char origOsccal, maxOsccal;
static char currState;

//I seem to remember that OSCCAL[7]=0 gives a range half of that of OSCCAL[7]=1;
//and that they intersect ar OSCCAL[6..0]=0x40, so the speed of 
//osccal=0xC0 and osccal=0x40 are the same. Can't seem to find where I found 
//that anymore... so I hope I'm right :P
static unsigned char toMaxRange(unsigned char osccal) {
	if (osccal&0x80) return osccal;
	return ((osccal-0x40)>>1)+0xC0;
}

//Change the OSCCALL in small increments/decrements so we won't piss
//off the CPU core.
static void slideClockTo(unsigned char clock) {
	static char start, end, cal, mod;
	start=toMaxRange(OSCCAL);
	end=toMaxRange(clock);
	if (end>start) mod=1; else mod=-1;
	for (cal=start; cal!=end; cal+=mod) {
		OSCCAL=cal;
		nop();
		nop();
	}
	OSCCAL=clock;
}

//Get osccal value, calculate the value to write when we want to overclock
void overclockInit(void) {
	origOsccal=OSCCAL; //This is the OSCCAL value required for 16MHz exactly.
	//Overclock to 24MHz, by setting osccal to 150% of its original value.
	maxOsccal=toMaxRange(OSCCAL)&0x7F; //and with 0x7f to cut off range bit
	maxOsccal=maxOsccal+(maxOsccal>>1); //aka: maxOsccal*1.5
	maxOsccal|=0x80; //reset range bit
	currState=OVERCLOCK_STD;
}

//Set the CPU speed
void overclockCpu(char toWhat) {
	if (toWhat==OVERCLOCK_STD) slideClockTo(origOsccal);
	if (toWhat==OVERCLOCK_MAX) slideClockTo(maxOsccal);
	currState=toWhat;
}

//Duh.
char overclockGetState() {
	return currState;
}
