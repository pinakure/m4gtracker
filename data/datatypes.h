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

typedef struct _Display {
	const u8 x;							// Horizontal origin in the map
	const u8 y;							// Vertical origin in the map
	const bool invert;					// If true, active will be true when var = 0
	const Cache *cache;				// Cache data used in this display (if type requires cached data)
	const u8 *var;						// Variable bound (8 bit ponter)
	bool active;					// If true, a special background highlights in bgh color
	bool redraw;					// If true, this display will be rendered again next redraw check time
}Display;

//Controls are processed even when they are not reachable in navigation order!
typedef struct _Control Control;
typedef struct _Callback Callback;

typedef struct _Callback {
	void(*callback)	(Control*, bool, bool, u32*); // Function Pointer to be called on trigger
	const u16 msg;								// Message causing this callback to trigger
	void  *var;									// Variable bound
	const Callback *next;						// If not null, another callback will be checked, only if message didnt match
} Callback;


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

class RegionHandler;

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
	void(*updater)(RegionHandler *rh);	
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
