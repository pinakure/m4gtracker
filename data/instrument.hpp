#ifndef INSTRUMENT_HPP
#define INSTRUMENT_HPP

#include "../agb.h"
#include "tablecell.hpp"

typedef struct sInstrument {
	TableCell	TABLE;
	u8			VISPOSITION[2];
	char 		NAME[6];
	u8			TYPE /*2*/;
	u8			SETTINGS[32];
} Instrument;

extern Instrument VAR_INSTRUMENTS[64];
extern Instrument VAR_INSTRUMENT; // Current instrument MUST be operated from here

#endif
