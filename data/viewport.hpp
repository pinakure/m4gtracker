#ifndef VIEWPORT_HPP
#define VIEWPORT_HPP

#include "../agb.h"

typedef struct sRegion Region;

typedef struct _Viewport {
	const u8 x;					// Horizontal position in the map
	const u8 y;					// Vertical position in the map
	const u8 width;				// Width of the viewport
	const u8 height;			// Height of the viewport
	u8 *var;					// Variable controlling the active region
	const Region *const *regions;// Region array pointer
	u8 count;					// Region array item count
} Viewport;

#endif
