#ifndef CACHE_HPP
#define CACHE_HPP

#include "../agb.h"

//Cache declaration 
typedef struct _Cache {
	void (*element)(u8, u8, u16, u16);					// If not null, data is read from a table instead of a map
	const u8 mapIndex;
	const u8 x; 
	const u8 y; 
	const u8 width;					// Width of the cache
	const u8 height;				// Height of the cache
	const u8 itemwidth;				// Width of each ELEMENT
	const u8 itemheight;			// Height of each ELEMENT
	const u8 palActive;				// Focused palette (0xF0 == White Over Black, 0x0F == Black Over White)
	const u8 palInactive;			// Unfocused palette (0xF0 == White Over Black, 0x0F == Black Over White)
	const u8 vertical;				// If vertical, reverse the read order
} Cache;

#endif
