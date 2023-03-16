#ifndef PATTERNCELL_HPP
#define PATTERNCELL_HPP

#include "../agb.h"

#define ORDER_COUNT				0x100
#define PATTERN_COUNT			0x80

#include "channel.hpp"
	
typedef struct sPatternCell {
	u8 				KEY[ 0x10 ] /*7*/;
	u8				INS[ 0x10 ] /*6*/;
	u8				VOL[ 0x10 ] /*4*/;
	u8				CMD[ 0x10 ] /*5*/;
	u8				VAL[ 0x10 ];
}PatternCell;

extern PatternCell VAR_CELLS[ CHANNEL_COUNT ]; // Current pattern cells, 6 pointers dereferenced
extern PatternCell VAR_DATA	[ PATTERN_COUNT ];

#endif
