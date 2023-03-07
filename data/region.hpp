#ifndef REGION_HPP
#define REGION_HPP

#include "../agb.h"

#include "cache.hpp"
#include "control.hpp"
#include "display.hpp"
#include "viewport.hpp"

class RegionHandler;

typedef struct sRegion Region;

typedef struct sRegion {
	const u8 			xadd;
	const u8 			yadd;			// where to draw this region, when loaded from a viewport
			
	const u8 			x;				// Horizontal offset in the map
	const u8 			y;				// Vertical offset inside the map
	const u8 			width;			// Width of the region, in 8x8 tiles
	const u8 			height;			// Height of the region, in 8x8 tiles
	const Region*		up;				// Region reached when going up
	const Region*		right;			// Region reached when going right
	const Region*		down;			// Region reached when going down
	const Region*		left;			// Region reached when going left
	const u8 			mapIndex;		// Map where this region belongs to
	const Cache*		dirty;			// Section designed to copy data safely there
	const Control*		controls;		// Array of existing controls
	const Display*		displays;		// Array of existing displays
	Control*			focus;			// Which one of the controls is the active
	u8*					controlVars;	// Copy of the values being monitored by controls, to know when to redraw
	void(*updater)(RegionHandler *rh);	
	const Viewport*		viewport;		// If not NULL, viewport to be processed with this region
}Region;

#endif
