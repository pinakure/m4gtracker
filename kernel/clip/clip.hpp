#ifndef CLIP_HPP
#define CLIP_HPP

#include "../../agb.h"
#include "../regionhandler/regionhandler.hpp"

typedef enum eClipboardAction {
	CLIP_NONE,
	CLIP_CUT,
	CLIP_PASTE,
	CLIP_COPY,
	CLIP_CLONE
}ClipboardAction;

typedef enum eClipData {
	CLIPDATA_PATTERN	= 0x00,
	CLIPDATA_NOTE		= 0x01,
	CLIPDATA_INSTRUMENT	= 0x02,
	CLIPDATA_VOLUME 	= 0x04,
	CLIPDATA_COMMAND	= 0x08,
	CLIPDATA_VALUE		= 0x10,
} ClipData;

typedef struct sClipboard {
	static u16 	data[16*6];	// 16 rows, 6 channels / 5 fields. If copying patterns, values are lost and viceversa
	static u16 	len;		// length of the block used currently (0 by default)
	static u8 	x;			
	static u8 	y;			
	static u8 	width;	
	static u8 	height;
	static u8	type;
	static u8	content;	// last type
	static u8 	columns; 	// How many data columns are currently on clipboard 
	static u8 	rows;		// How many data rows are currently on clipboard 
} Clipboard;

class Notifier {
	private:
		static u16 					time;
		static void					clear();
	
	public: 
		static void					init();
		static void					update();
		static void 				icon( u16 upper, u16 lower, u16 extra=0x0000 );
};

class Clip {
	private:
		static bool 				redraw;
		
		static void drawMaskTrk		(int color);
		static void drawMaskPat		(int color);
		
	public:
		static bool 				visible;
		static bool 				blink_monitor;
		static ClipboardAction 		action;
		
		static void init			();
		static void draw			(  );
		static void update			(  );
		static void show			();
		static void hide			();
		static void activate		();
		static void processInput	();
	
		static void copy			();
		static void paste			();
		static void clone			();
		static void cut				();
		static void setAction		( ClipboardAction selected_action );
		static void maskMoveVert	( bool increase );
		static void maskMoveHorz	( bool increase );
		static void maskSizeVert	( bool increase );
		static void maskSizeHorz	( bool increase );
		static void resizeMask		( int direction );
		static void drawMask		( int color );
};

#endif
