#ifndef PATTERN_HPP
#define PATTERN_HPP

#include "../agb.h"
#include "patterncell.hpp"

// Represents pattern data for a SINGLE channel

typedef struct sPattern{	
	u8				ORDER		[ ORDER_COUNT ];
	u8				TRANSPOSE	[ ORDER_COUNT ];
	
	//SUG:set pointer to first visible position (in pat screen), and mannipulate order directly by POINTER[x]
	u8 				*POINTER;
	u8				POSITION;	//Internal position
}Pattern;

extern Pattern VAR_PATTERN[ CHANNEL_COUNT ]; // Each of the 6 pattern lists on PAT screen 

#endif
