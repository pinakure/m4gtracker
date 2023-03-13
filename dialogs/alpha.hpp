#ifndef ALPHADIALOG_HPP
#define ALPHADIALOG_HPP

#include "../agb.h"
#include "../data/control.hpp"

class AlphaDialog {
	private:
		static u8 		buffer[16];
		static u8 		maxlen;		// Max length of string
		static u8 		length;
		static u8 		position;
		static u8 		cursorX;
		static u8 		cursorY;
		static u8 		textX;
		static u8 		textY;
		static u8 		oldX;
		static u8 		oldY;
		static u8 		Y;
		static u16 	oldvalue;
		static u8*		target;	// The string were to write buffered text at exit
		static bool 	enabled;
		
		/*  make static  */
		static void add			( );
		static void rem			( );
		static void draw			( );
		static void confirm		( );
		static void cancel		( );
		static void preview		( );
		static void update		( );
		
		static bool redraw;
		static void enable		(bool bigString, u8* targetVariable, u8 vx, u8 vy);
	public:	
		static void getBigString	(Control *c, bool bigstep, bool add, u32 *pointer);
		static void getString		(Control *c, bool bigstep, bool add, u32 *pointer);
};

#endif
