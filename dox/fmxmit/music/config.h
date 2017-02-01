//Config of the AVR-monotone port


//Frequency of the direct digital synthesis output
#ifdef AVR
#define DDS_FREQ 15625
#else
#define DDS_FREQ 44100
#endif
//Where the fixed point of the DDS is. 5 is a good value. (Gives 5 bit behind the fixed point)
#define DDS_POINT 5
//Times / second the player_tick()-routine should get called
#define PLAYER_FREQ 60

//Define this to do a sine-wave-mixing-like multitone approach.
//#define MULTITONE


#ifdef AVR
#define dbg(a...) 
#else
#define dbg(a...) printf(a)
#include "avrpc_compat.h"
#endif
