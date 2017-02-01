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
Direct Digital Synth of the notes played
*/

#include "config.h"
#include "songdata.h"
#include <stdio.h>
#ifdef AVR
#include <avr/pgmspace.h>
#endif

//channels
typedef struct {
    unsigned short reload_val;
    unsigned short curr_val;
} channel_struct;
channel_struct channel[songTotalTracks];


#ifndef MULTITONE
int pcsp_curr; //variable indicating which channel is in turn in the arpeggio
#endif


//Frequency table. These equations get resolved on compile.
#ifdef AVR
const unsigned short PROGMEM freqtab[]={
#else 
const unsigned short freqtab[]={
#endif
    ((int)(DDS_FREQ/27.5000))<<DDS_POINT,
    ((int)(DDS_FREQ/29.1353))<<DDS_POINT,
    ((int)(DDS_FREQ/30.8677))<<DDS_POINT,
    ((int)(DDS_FREQ/32.7032))<<DDS_POINT,
    ((int)(DDS_FREQ/34.6479))<<DDS_POINT,
    ((int)(DDS_FREQ/36.7081))<<DDS_POINT,
    ((int)(DDS_FREQ/38.8909))<<DDS_POINT,
    ((int)(DDS_FREQ/41.2035))<<DDS_POINT,
    ((int)(DDS_FREQ/43.6536))<<DDS_POINT,
    ((int)(DDS_FREQ/46.2493))<<DDS_POINT,
    ((int)(DDS_FREQ/48.9995))<<DDS_POINT,
    ((int)(DDS_FREQ/51.9130))<<DDS_POINT
};

//Set a certain channel to a certain note. Use note 0 to shut up the
//channel.
void dds_set_channel(unsigned char chan, unsigned char note) {
    unsigned char oct=0;
    channel[chan].curr_val=0; //reset dds synth before fiddling with the channel
#ifndef MULTITONE
    pcsp_curr=0; //reset any arpeggio going on
#endif

    if (note==0) {
	channel[chan].reload_val=0; //disable channel
    } else {
	//get octave. note will become the note in that octave.
	note++; //skip note 0
	while (note>=12) {
	    note-=12;
	    oct++;
	}
	//load the found reloadval into the channel data. We abuse the fact
	//that every octave, the frequency gets doubled by shifting the
	//reload val oct times to the right. This saves space in the 
	//freqtab array.
	channel[chan].reload_val=(unsigned short)pgm_read_word(&freqtab[note])>>oct;
	dbg("Chan %hhi: oct %i note %i reloadval %i\n",chan,oct,note,channel[chan].reload_val);
    }
}

#ifndef MULTITONE
//Should be called every player tick to advance the arpeggio one channel.
void dds_play_next_channel() {
    unsigned char t;
    //Look up next channel to play
    t=0;
    do {    
	pcsp_curr++;
	if (pcsp_curr>=songTotalTracks) pcsp_curr=0;
	t++;
    } while ((t!=songTotalTracks) && channel[pcsp_curr].reload_val==0);
}
#endif

//Generate the next sample. Return value is in the range of 0-songTotalTracks
//for multitone stuff, and 0/1 for arpeggio.
char dds_get_next_sample() {
    int c,r;

    r=0;
#ifdef MULTITONE
    //for multitone, walk all dds channels
    for (c=0; c<songTotalTracks; c++) {
#else
    //for arpeggio, only handle the currently selected channel
    c=pcsp_curr;
#endif
	if (channel[c].reload_val!=0) { //if channel enabled
	    channel[c].curr_val+=(1<<DDS_POINT);
	    if (channel[c].curr_val>channel[c].reload_val) channel[c].curr_val-=channel[c].reload_val;
	    if (channel[c].curr_val>(channel[c].reload_val/2)) r++;
	}
#ifdef MULTITONE
    }
#endif
    return r;
}

