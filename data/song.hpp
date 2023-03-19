#ifndef SONG_HPP
#define SONG_HPP

#include "../agb.h"
#include "groovetable.hpp"
#include "channel.hpp"

#define SONG_STRING_SIZE		14
#define SONG_DETAILS_SIZE		(14 + 14 + 1 +1)

class Song {
	public:
		MEM_IN_EWRAM static u8	slot;
		GrooveTable				GROOVE;
		Pattern					PATTERNS[ CHANNEL_COUNT ]; // STORED PATTERNS FOR THIS SONG
		u8						TRANSPOSE;
		u8						BPM;
		u8						TITLE[14];
		char					ARTIST[14];
		u8 						TAPTICKS;
		bool					NOTEMPTY;		// If true, song has data
		
		static void 			seek();
		void 					write();
		void 					read();
		void 					clear();
};

extern MEM_IN_EWRAM Song VAR_SONG; //Current loaded song ( unpacked and copied from SRAM active slot )

#endif 
