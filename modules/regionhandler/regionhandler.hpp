#ifndef REGIONHANDLER_HPP
#define REGIONHANDLER_HPP

#include "../../agb.h"

#include "../../data/enum.h"
#include "../../data/caches.hpp"
#include "../../data/controls.hpp"
#include "../../data/regions.hpp"

class Progress {
	private:
	public:
		u32 max;
		u32 current;
		u8 step;
		u8 laststep;
		E_StatusStrings upperLine;
		E_StatusStrings lowerLine;
		bool enabled;
		u8 redraw;
		u32 *var;
	
		void update();		
		void set(u32 value, u32 maxvalue, E_StatusStrings uLine, E_StatusStrings lLine, u32 *var);
};

class AlphaDialog {
	private:
		u8 buffer[16];
	public:	
		u8 maxlen;		// Max length of string
		u8 length;
		u8 position;
		u8 cursorX;
		u8 cursorY;
		u8 textX;
		u8 textY;
		u8 oldX;
		u8 oldY;
		u8 Y;
		u16 oldvalue;
		u8 *target;	// The string were to write buffered text at exit
		bool enabled;
		bool redraw;
		
		AlphaDialog();
		void add();
		void rem();
		void draw();
		void confirm();
		void cancel();
		void preview();
		void update();
		void enable(bool bigString, u8* targetVariable, u8 vx, u8 vy);
};

class ReallyDialog {
	private:
	public:
		bool result;
		bool enabled;
		
		ReallyDialog();		
		void draw();
		void confirm();
		void cancel();
		void enable();
};

typedef struct sClipboard {
	u16 	*blockdata[16*5*6];		// 6 channels, 5 fields per channel row, 16 rows max ;; these are actually pointers to each control currently selected 
	u16 	blocklength;			// length of the block used currently (0 by default)
	u8 		blockstart;				// logical tile where the block starts
	u8 		blockwidth;				// width for the block displayed on the screen
	u8 		blockheight;			// height for the block displayed on the screen
	bool	blockclean;				// true if no copy operations were done yet
	bool	blockactive;			// true if block is visible and receiving arrow input, or waiting either B for cancel or A to copy, or B+A to cut 'cancel the cancel'. R + A, clone, R + arrows move the block by the other corner
} Clipboard;

class RegionHandler {
	private:
		
	public:
		Progress 	progress;
		
		const unsigned short *map0;
		const unsigned short *map1;
		const unsigned short *map2;
		
		Cache*		dirty;
		Region*		region;
		Control*	control; 		//focused control
		bool 		redraw;  		// used to notify the loaded region it must be redrawn
		bool 		new_region;  	// turned on @ load, screen responsible of reading this var must turn it off
		
		u32 		messages[1024];
		s32 		messagecount;
		
		u8 			viewportLastValue;// <--- warning!: this disrupts recursive functionality!
		
		RegionHandler();
		void dispatchMessages();
		void dispatchCallback( const Callback *cb, const Control *ctl, u8 add, u8 bigstep, u16 msg, u32 *pointer );
		void sendMessage(u32 message);
		void load(const Region *r);
		void drawCache(u8 x, u8 y, const Cache *c, u8 var, bool highlight);
		void drawVerticalCache(u8 x, u8 y, const Cache *c, u8 var, bool highlight);
		void drawControl(const Control *c);
		void drawDisplay(const Display *d);
		void updateViewport(const Viewport *v, u8 x, u8 y);
		void loadControls(const Region *r);
		void loadDisplays(const Region *r);
		void drawViewport(const Viewport *v, u8 x, u8 y, const Region *r);
		void draw();
		void jumpToControl(const Control *c);
		void controlTrigger(const Control *c, u16 q);
		void controlClear(const Control *c);
		void controlModify(const Control *c, bool big, bool add);
		void updateProgress();
		void update(u8 delta);
};

#define OnNewRegion( f ) if(rh->redraw){ f };

extern RegionHandler regHnd;

#endif
