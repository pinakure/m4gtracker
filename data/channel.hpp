#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include "../agb.h"
#include "patterncell.hpp"

typedef struct sChannel Channel;

typedef struct sChannel {
	bool 				mute;
	bool				solo;

	u8 					key;
	u8 					cmd;
	u8 					inst;
	u8 					value;
	u8 					volume;

	PatternCell		*CELL;
	PatternCell		*NEXTCELL;
	u8					VOLUME /*5*/;
	u8					lastpeak /*4*/;
	u8					peak /*4*/;
	bool				PLAYING;

	u8					LENGTH;
	u8					LASTSTEP /*4*/;
	u8					STEP /*4*/; 		// Current pattern step
	u8					JUMPSTEP /*4*/; 	// Next pattern step
	
	u8					POSITION /*8*/;  	// Index on chanel Pattern Order array
	u8					NEXTPOSITION /*8*/; // Next index on channel pattern order array
	u8					LASTPOSITION /*8*/; // Next index on channel pattern order array
	
	bool 				reset;
	bool 				retrig;
	u8 					transpose;
	u8 					fine_tune;
	int 				target_tick;
	u8 					tsp_position;
	u8 					vol_position;
		
	void 				(*trigger)(Channel*);
		
}Channel;

typedef enum eChannelTypes {
	CHANNEL_PWM1,
	CHANNEL_PWM2,
	CHANNEL_NZE,
	CHANNEL_WAV,
	CHANNEL_FMW,
	CHANNEL_SMP,
};

extern Channel VAR_CHANNEL[6];

#endif
