#ifndef _DATATYPE_DISPLAY
#define _DATATYPE_DISPLAY

#include "../agb.h"

typedef struct _Display {
	const u8 x;							// Horizontal origin in the map
	const u8 y;							// Vertical origin in the map
	const bool invert;					// If true, active will be true when var = 0
	const Cache *cache;				// Cache data used in this display (if type requires cached data)
	const u8 *var;						// Variable bound (8 bit ponter)
	bool active;					// If true, a special background highlights in bgh color
	bool redraw;					// If true, this display will be rendered again next redraw check time
}Display;

#endif
