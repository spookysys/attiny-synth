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
Routines for compiling the player on a PC. This hooks libao for the sound.
*/

#include <ao/ao.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "config.h"
#include "player.h"
#include "dds.h"

//buffer for audio output
#define AO_BUFF 4096
//define to divert sound to a file called 'music.wav'.
//#define TO_FILE

int main(int argc, char** argv) {
    int d,x;
    char *samps;
    
    //initialise AO
    ao_device *ao;
    ao_sample_format form;
    
    ao_initialize();
    d=ao_default_driver_id();
    form.bits=8;
    form.rate=DDS_FREQ;
    form.channels=1;
    form.byte_format=AO_FMT_NATIVE;
#ifdef TO_FILE
    ao=ao_open_file(ao_driver_id("wav"),"music.wav",1,&form,NULL);
#else
    ao=ao_open_live(d,&form,NULL);
    if (ao==NULL) {
	printf("Error opening device %d\n");
	exit(0);
    }    
#endif
    //Libao is opened and initialised now.

    //allocate sample buffer
    samps=malloc(AO_BUFF);
    d=0;
    //reset player
    player_reset();
    while(1) {
	//generate AO_BUFF samples, advance the player 60 times per second
	for (x=0; x<AO_BUFF; x++) {
	    d++;
	    if (d>(DDS_FREQ/PLAYER_FREQ)) {
		d=0;
		player_tick();
	    }
	    samps[x]=dds_get_next_sample()<<5;
	}
	//let libao play the samples
	ao_play(ao,samps,AO_BUFF);
    }
    //never happens because the while() above
    ao_close(ao);
    ao_shutdown();
}
