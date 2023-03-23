#include "virtualscreen.hpp"
#include "gpu.hpp"

const bool circledata[ VIRTUALSCREEN_WIDTH * VIRTUALSCREEN_HEIGHT ] = { 	
	0,0,0,0,0,0,0,0, 0,0,0,0,0,1,1,1, 1,1,1,0,0,0,0,0, 0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0, 0,0,1,1,1,0,0,0, 0,0,0,1,1,1,0,0, 0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0, 1,1,0,0,0,0,0,0, 0,0,0,0,0,0,1,1, 0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,1,1, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 1,1,0,0,0,0,0,0,
	0,0,0,0,0,1,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,1,0,0,0,0,0,
	0,0,0,0,1,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,1,0,0,0,0,
	0,0,0,1,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,1,0,0,0,
	0,0,0,1,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,1,0,0,0,
	
	0,0,1,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,1,0,0,
	0,0,1,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,1,0,0,
	0,1,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,1,0,
	0,1,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,1,0,
	0,1,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,1,0,
	1,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,1,
	1,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,1,
	1,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,1,
	
	1,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,1,
	1,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,1,
	1,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,1,
	0,1,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,1,0,
	0,1,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,1,0,
	0,1,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,1,0,
	0,0,1,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,1,0,0,
	0,0,1,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,1,0,0,
	
	0,0,0,1,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,1,0,0,0,
	0,0,0,1,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,1,0,0,0,
	0,0,0,0,1,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,1,0,0,0,0,
	0,0,0,0,0,1,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,1,0,0,0,0,0,
	0,0,0,0,0,0,1,1, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 1,1,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0, 1,1,0,0,0,0,0,0, 0,0,0,0,0,0,1,1, 0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0, 0,0,1,1,1,0,0,0, 0,0,0,1,1,1,0,0, 0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0, 0,0,0,0,0,1,1,1, 1,1,1,0,0,0,0,0, 0,0,0,0,0,0,0,0 
};
					
u16 	VirtualScreen::data[ (VIRTUALSCREEN_WIDTH * VIRTUALSCREEN_HEIGHT ) / 4 ];
u16*	VirtualScreen::d; 
s16 	VirtualScreen::i;

void VirtualScreen::init( ){
	clear();
	load(circledata);
}

void VirtualScreen::load(const bool *dat) {
	i=-1;
	for(;++i < VIRTUALSCREEN_SIZE ;){
		if(*dat) set(i & 31, i>>5);
		dat++;
	}
}

void VirtualScreen::clear(){
	i=-1;
	d = data;
	for(; ++i < REALSCREEN_SIZE ; ){
		*d = 0x60;
		d++;
	}
}

void VirtualScreen::set( u8 ox, u8 oy ){
	u8 rx = ox & 1;
	u8 ry = oy & 1;
	u8 x = ox >> 1;
	u8 y = oy >> 1;
	u8 pos = (y<<4) + x;
	u8 current = data[pos] - 0x60;
	data[pos] = TABLE_PIXEL[current][(rx<<1) | ry];
}

void VirtualScreen::unset( u8 x, u8 y ){
	//data[(y*8)+x] = false;
}

void VirtualScreen::draw( u8 ox, u8 oy ){
	d = data;
	oy <<= 6;
	ox <<= 1;
	for(i=0;i<16;i++) {
		DmaCopy(3, d, (SCREEN_BASE1_ADDR) + oy + (i<<6) + ox, 32, 16);
		d+=16;
	}
}
