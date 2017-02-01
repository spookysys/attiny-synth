/*
Player for compressed Monotone songs.
(C) 2012 Jeroen Domburg (jeroen AT spritesmods.com)
Monotone, by Trixter: http://www.oldskool.org/pc/MONOTONE

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
Parse player structures and generate the notes for the direct digital synthesis.
The notes etc are stored a bit differently than in the .mon-file itself, and
mksong.php converts from that to the song format used here. The song format 
basically is a compressed version of the .mon format. A row of notes
is compressed as follows. Assume a 4-track .mon-file: First of all, the row 
is broken up into 8 bytes instead of 4 shorts. Of these bytes, most of the
time some will be 00. These bytes are left out and the row is prepended
with a bitmap of the bytes that are available. E.g.

FE00 0000 C400 01C0
 |
 V
FE 00 00 00 C4 00 01 C0
 |
 V
8B  FE C4 01 C0

(8B is the bitmap, FE-C0 are the non-zero bytes in the original)

Pattern order table:
Because we can't just calculate the next pattern by multiplying the
pattern number by 64*2*NoOfTracks, we'll have to use something else
to indicate the next pattern. For that use, the pattern order table is 
changed: instead of giving the number of the pattern to play, it now is a
pointer directly to the start of the pattern in the songData table.


All in all, I can get a 50% compression out of most of the songs. Not bad, eh?
*/

#include "config.h"
#include "dds.h"
#include "songdata.h"
#include <stdio.h>
#ifdef AVR
#include <avr/pgmspace.h>
#endif
unsigned char patpos;
unsigned char pos;
unsigned char speed;
unsigned char pcsp_curr;

#define NOTESPERPATTERN 64

//Resets the player
void player_reset() {
	dbg("player_reset\n");
	patpos=0xff;
	speed=1;
	pos=NOTESPERPATTERN;
}

//Get the next row of notes.
static void getnextrow(unsigned short row[]) {
	static unsigned char *myrow;
	unsigned char bm,d;
	unsigned short p;
	pos++;
	if (pos>=NOTESPERPATTERN) { //Pattern done?
		patpos++;  //ext pattern
		//get it
		p=pgm_read_word(&songPatterns[patpos]);
		if (p==0xffff) { //song done? Reset it.
			p=0;
			patpos=0;
		}
		pos=0; //reset in-pattern position
		myrow=&songData[p]; //reset pointer to data
		dbg("New pattern: pattern %i = %x\n",patpos,p);
	}
	//Read the bitmap of nonzero values.
	bm=pgm_read_byte(myrow++);
	for (p=0; p<8; p++) {
		if ((bm&1)==0) { //This bit zero and left out?
			d=0; //yes
		} else {
			d=pgm_read_byte(myrow++); //no -> actually read it
		}
		bm>>=1;
		//bytes-> 16-bit words
		if (p&1) {
			row[p/2]+=d;
		} else {
			row[p/2]=d<<8;
		}
	}
}


//This routine should be called 60 times a second.
void player_tick() {
	static char subtick=0;

	unsigned char note,effect,data;
	unsigned char t;
	unsigned short row[songTotalTracks];

#ifndef MULTITONE
	dds_play_next_channel();
#endif

	//only advance to the next tone every [speed] ticks.
	subtick++; 
	if (subtick<speed) return;
	subtick=0;
	
	//get next row of notes
	getnextrow(&row);
	dbg("%04x: |",pos);
    
	//parse note info
	for (t=0; t<songTotalTracks; t++) {
		note=(row[t]>>9)&0x7f;
		effect=(row[t]>>6)&7;
		data=(row[t]&0x1f);
		
		dbg("%02x %01x %02x|",note,effect,data);

		if (note!=0) {
			if (note!=0x7f) {
				dds_set_channel(t, note); //generate sound for the new value
			} else {
				dds_set_channel(t, 0); //mute note
			}
		}

		//parse effects
		if (effect==0) {
		    ; //no effect
		} else if (effect==7) { //speed
			speed=data; //set new speed
			dbg("New speed: %i",speed);
		} else {
			dbg("Unsupported effect: %i\n",effect);
		}
	}
	dbg("\n");
}
