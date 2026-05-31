#ifndef PATTERN_HPP
#define PATTERN_HPP

#include "../agb.h"
#include "patterncell.hpp"

// Represents pattern data for a SINGLE channel

#define ORDER_ITEM_SIZE 	( ORDER_COUNT + ORDER_COUNT )
#define ORDER_DATA_SIZE		((ORDER_ITEM_SIZE * CHANNEL_COUNT))  // 0x200 per channel (128 patterns + 1280 transpose) 

typedef struct sPattern{	
	public BACKUP:
		u8				ORDER		[ ORDER_COUNT ];
		u8				TRANSPOSE	[ ORDER_COUNT ];
	public TRANSIENT:
		//SUG:set pointer to first visible position (in pat screen), and mannipulate order directly by POINTER[x]
		u8 				*POINTER;
		u8				POSITION;	//Internal position
}Pattern;

#endif
