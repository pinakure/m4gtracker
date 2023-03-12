#ifndef SONG_HPP
#define SONG_HPP

#include "../agb.h"
#include "groovetable.hpp"
#include "pattern.hpp"

typedef struct sSong {
	GrooveTable		GROOVE;
	PatternCell 	DATA[128];//This should BE ON PATTERN STRUCTURE!!!*/
	Pattern			PATTERNS[6];//orders and pattern for each
	u8				TRANSPOSE;
	u8				BPM;
	u8				PATTERNLENGTH /*4*/;
	u8				TITLE[14];
	char			ARTIST[14];
	u8 				TAPTICKS;
	bool			NOTEMPTY;		// If true, song has data
}Song;

extern Song VAR_SONG; //Current loaded song ( unpacked and copied from SRAM active slot )

#endif 
