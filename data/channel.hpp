#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include "../agb.h"

#include "enum.h"
#include "control.hpp"
#include "pattern.hpp"
#include "instrument.hpp"

#define CHANNEL_COUNT 			6

typedef enum eChannelTypes {
	CHANNEL_PWM1,
	CHANNEL_PWM2,
	CHANNEL_NZE,
	CHANNEL_WAV,
	CHANNEL_FMW,
	CHANNEL_SMP,
}ChannelType;



class Channel {
public:
	ChannelType 		type;
	u8 					index;

	bool 				mute;
	bool				solo;

	u8 					key;
	u8 					cmd;
	u8 					inst;
	u8 					value;
	u8 					volume;

	PatternCell			*CELL;
	PatternCell			*NEXTCELL;
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
	s8 					tsp_delta;
	s8 					vol_delta;
	u8 					tsp_target;
	u8 					vol_target;
	LoopMode			tsp_loop;
	
	u8 					vol_position;
	LoopMode			vol_loop;
		
	Pattern*			pattern;		// Replaces VAR_PATTERN	[ 6 ] GLOBAL -> Rename to Pattern
	Pattern*			song_patterns;	// Replaces VAR_SONG.PATTERNS[ 6 ] GLOBAL -> Rename to Order
	PatternCell* 		cells;			// Replaces VAR_CELL	[ 6 ] GLOBAL -> Rename to cell
	Instrument*			instrument;
	
	void 				(*trigger	)(Channel*);
	void 				(*noteOn	)(Channel*);
	void 				(*noteOff	)(Channel*);
	void 				(*enable	)();
	void 				(*disable	)();
	
	void init( u8 channel_index );
	
	// SRAM Functions
	static void 		seek();
	static void 		clearAll();
	static void 		readAll();
	static void 		writeAll();
	void 				read();
	void 				write();
	void 				clear();
	
	const Control*		tracker_controls[ CHANNEL_COUNT ];		// TRK screen, 6 Channels
	/*
	Control*			pattern_controls[ CHANNEL_COUNT ];		// PAT screen, 6 ORDER channels
	Control*			transpose_controls[ CHANNEL_COUNT ];	// PAT screen, 6 TRANSPOSE channels 
	*/
};

extern Channel 		VAR_CHANNEL	[ CHANNEL_COUNT ];
extern PatternCell 	VAR_CELLS	[ CHANNEL_COUNT ]; // Current pattern cells, 6 pointers dereferenced
extern Pattern 		VAR_PATTERN	[ CHANNEL_COUNT ]; // EDITABLE BY USER

#endif
