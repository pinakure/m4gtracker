#include "cursor.hpp"

CursorType 	Cursor::type;
u8 	 		Cursor::x;
u8 	 		Cursor::y;
bool 		Cursor::blink;
bool 		Cursor::blink_monitor;




void Cursor::init(){
	x 		= 0;
	y 		= 0;
	type 	= CURSOR_BLOCK_1_X_1,
	blink 	= false;
	
	blink_monitor = false;
}

void Cursor::set( u8 cursor_x, u8 cursor_y, CursorType cursor_type ){
	x 		= cursor_x;
	y 		= cursor_y;
	type 	= cursor_type;
	
}

void Cursor::update(){
	// Update blink status
	
}

