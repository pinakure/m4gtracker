#ifndef _DATATYPES_H
#define _DATATYPES_H

//Cache declaration 
#include "cache.hpp"
#include "display.hpp"
#include "callback.hpp"
#include "control.hpp"

class REGHND;

typedef struct _Viewport Viewport;
typedef struct _Region Region;
typedef struct _Region {
	const u8 xadd;
	const u8 yadd;				// where to draw this region, when loaded from a viewport
	
	const u8 x;					// Horizontal offset in the map
	const u8 y;					// Vertical offset inside the map
	const u8 width;				// Width of the region, in 8x8 tiles
	const u8 height;			// Height of the region, in 8x8 tiles
	const Region *up;			// Region reached when going up
	const Region *right;		// Region reached when going right
	const Region *down;			// Region reached when going down
	const Region *left;			// Region reached when going left
	const u8 mapIndex;			// Map where this region belongs to
	const Cache *dirty;			// Section designed to copy data safely there
	const Control *controls;	// Array of existing controls
	const Display *displays;	// Array of existing displays
	Control *focus;				// Which one of the controls is the active
	u8 *controlVars;			// Copy of the values being monitored by controls, to know when to redraw
	void(*updater)();	
	const Viewport *viewport;			// If not NULL, viewport to be processed with this region
}Region;

//Viewport declaration 
typedef struct _Viewport {
	const u8 x;					// Horizontal position in the map
	const u8 y;					// Vertical position in the map
	const u8 width;				// Width of the viewport
	const u8 height;			// Height of the viewport
	u8 *var;					// Variable controlling the active region
	const Region *const *regions;// Region array pointer
	u8 count;					// Region array item count
} Viewport;


//Help structs declaration
typedef struct s_Help_Topic helptopic;
typedef struct s_Help_Button {
	int					destination_index;
	const char			text[32];
	u16					x;
	u16					y;
} helpbutton;

typedef struct s_Help_Topic {
	const u32			button_count;
	const u16			*map_data;
	const int 		 	*button_list;
} helptopic;

#endif
