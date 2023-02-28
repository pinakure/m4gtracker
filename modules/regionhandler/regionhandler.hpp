#ifndef _MODULES_REGIONHANDLER
#define _MODULES_REGIONHANDLER

#include "../../agb.h"
#include "../../data/enum.h"
#include "../../data/datatypes.h"

typedef struct s_Clipboard {
	u16 	*blockdata[16*5*6];		// 6 channels, 5 fields per channel row, 16 rows max ;; these are actually pointers to each control currently selected 
	u16 	blocklength;			// length of the block used currently (0 by default)
	u8 		blockstart;				// logical tile where the block starts
	u8 		blockwidth;				// width for the block displayed on the screen
	u8 		blockheight;			// height for the block displayed on the screen
	bool	blockclean;				// true if no copy operations were done yet
	bool	blockactive;			// true if block is visible and receiving arrow input, or waiting either B for cancel or A to copy, or B+A to cut 'cancel the cancel'. R + A, clone, R + arrows move the block by the other corner
};

extern s_Clipboard Clipboard;

class Progress {
	public:
		static u32 max;
		static u32 current;
		static u8 step;
		static u8 laststep;
		static E_StatusStrings upperLine;
		static E_StatusStrings lowerLine;
		static bool enabled;
		static u8 redraw;
		static u32 *var;
	
		static void update();
		static void set(u32 value, u32 maxvalue, E_StatusStrings uLine, E_StatusStrings lLine, u32 *var);	
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
	public:
		bool result;
		bool enabled;
		
		ReallyDialog();
		void draw();
		void confirm();
		void cancel();
		void enable();	
};

class REGHND {
	public:
		static Progress progress;
		static const unsigned short *map0;
		static const unsigned short *map1;
		static const unsigned short *map2;
		
		static Cache 	*dirty;
		static Region 	*region;
		static Control 	*control; //focused control
		static bool 	redraw;
		
		static u32 		messages[1024];
		static s32 		messagecount;
		
		static u8 		viewportLastValue;// <--- warning!: this disrupts recursive functionality!
		
		static void init();
		static void dispatchMessages();
		static void sendMessage( u32 message );
		static void load( const Region *r );
		static void drawCache( u8 x, u8 y, const Cache *c, u8 var, bool highlight );
		static void drawVerticalCache( u8 x, u8 y, const Cache *c, u8 var, bool highlight );
		static void drawControl( const Control *c );
		static void drawDisplay( const Display *d );
		static void updateViewport( const Viewport *v, u8 x, u8 y );
		static void loadControls( const Region *r );
		static void loadDisplays( const Region *r );
		static void drawViewport( const Viewport *v, u8 x, u8 y, const Region *r );
		static void draw();
		static void jumpToControl( const Control *c );
		static void controlTrigger( const Control *c, u16 q );
		static void controlModify( const Control *c, bool big, bool add );
		static void updateProgress();	
		static void update( u8 delta );
};

#endif
