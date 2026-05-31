#ifndef CURSOR_HPP
#define CURSOR_HPP

#include "../../agb.h"

typedef enum eCursorType {
	CURSOR_BLOCK_1_X_1, // single values
	CURSOR_BLOCK_1_X_2, // hex double values 
	CURSOR_BLOCK_1_X_3, // note values
	CURSOR_ROW,			// playback row
}CursorType;

class Cursor {
	private:
		static u8 			x;
		static u8 			y;
		static bool 		blink;
		static bool 		blink_monitor;
		static CursorType	type;
		
	public:
		static void init();
		static void update();
		static void set( u8 x, u8 y, CursorType type);
};

#endif
