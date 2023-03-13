#include "../agb.h"
#include "../modules/gpu/gpu.hpp"
#include "../data/helpers.hpp"
#include "debug.hpp"

#ifndef NDEBUG

u16 		Console::cursor_color;
u16 	 	Console::title_color;
u8 	 		Console::cursor_x;
u8 	 		Console::cursor_y;
const char* Console::title;

Console::Console(const char *new_title){
	cursor_x = 0;
	cursor_y = 0;
	if( new_title ) setTitle( new_title );
	render();
}

void Console::setTitle(const char *new_title, u16 color){
	title = new_title;
	title_color = COLOR_CYAN;
}

void Console::percent( float q ){
	if( q == 1.0f ) DECIMAL_DOUBLE_TWOTILES(27, cursor_y, COLOR_WHITE, 100);
	else DECIMAL_DOUBLE_TWOTILES( 27, cursor_y, COLOR_CYAN, (u8)(q*100.0f) );
	gpu.set(2, 29, cursor_y, (COLOR_RED<<12) | 0x6F);
}

void Console::print( const char *text, u16 color){
	gpu.set(2, cursor_x, cursor_y, 0x0100);
	cursor_y++;
	Gpu::ascii( 0 , cursor_y , text , color );
	cursor_x=strlen( text )/2;
	if(cursor_x > 30){
		cursor_y += (cursor_x / 30);
		cursor_x = 0;
	}
	cursor_color = color;
}

void Console::setCursor( u8 x, u8 y){
	cursor_x = x;
	cursor_y = y;
}

void Console::render(){
	//gpu.clear( 0x10 | COLOR_DARK_BLUE );
	gpu.clear( 0x00 | COLOR_BLUE );
	if(title) 
		Gpu::bigString(15 - (strlen(title)>>1), 0, title, title_color);
}

void Console::update(){		
	static bool bm;
	if(bm != gpu.blink){
		bm = gpu.blink;
		gpu.set(2, cursor_x, cursor_y, (cursor_color << 12) | (gpu.blink ? 0x0116 : 0x0100) );
	}
	gpu.blinkUpdate(8);
}

void Console::wait( u8 time ){
	for(int i=0; i<0x7FF<<time; i++){
		while( gpu.isVblank()){}
		while(!gpu.isVblank()){}
		while( gpu.isVblank()){}
		while(!gpu.isVblank()){}
		
		update();
	}
}

#endif
