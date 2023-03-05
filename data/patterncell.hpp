#ifndef PATTERNCELL_HPP
#define PATTERNCELL_HPP

#include "../agb.h"

typedef struct sPatternCell {
	u16				KEY[16] /*7*/;
	u8				INS[16] /*6*/;
	u8				VOL[16] /*4*/;
	u8				CMD[16] /*5*/;
	u8				VAL[16];
}PatternCell;

extern PatternCell VAR_CELLS[6]; // Current pattern cells, 6 pointers dereferenced
extern PatternCell VAR_DATA[128];

#endif
