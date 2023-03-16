#ifndef INSTRUMENT_HPP
#define INSTRUMENT_HPP

#include "../agb.h"
#include "tablecell.hpp"

#define SETTINGS_SIZE			32
#define INSTRUMENT_COUNT 		0x40 //64

typedef struct sInstrument {
	TableCell	TABLE;
	u8			VISPOSITION[ 2 ];
	char 		NAME[ 6 ];
	u8			TYPE /*2*/;
	u8			SETTINGS[ SETTINGS_SIZE ];
} Instrument;

extern Instrument VAR_INSTRUMENTS[ INSTRUMENT_COUNT ];
extern Instrument VAR_INSTRUMENT; // Current instrument MUST be operated from here

#endif
