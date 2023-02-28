#ifndef _DATATYPE_CONTROL
#define _DATATYPE_CONTROL

#include "../agb.h"
#include "cache.hpp"
#include "callback.hpp"

//Controls are processed even when they are not reachable in navigation order!
typedef struct _Control {
	const u8 x;							// Horizontal origin in the map
	const u8 y;							// Vertical origin in the map
	const Control *up;					// Control reached when jumping up
	const Control *right;				// Control reached when jumping right
	const Control *down;				// Control reached when jumping down
	const Control *left;				// Control reached when jumping left
	const Cache *cache;					// Cache used by this control to represent var
	u8 *var;							// Variable bound (8 bit ponter)
	const Callback *callback;
} Control;

#endif
