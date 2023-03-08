#include "clip.hpp"
#include "../../callbacks/debug.hpp"


bool 			Clip::visible = false;
bool 			Clip::redraw  = false;
ClipboardAction Clip::action  = CLIP_NONE;
bool 			Clip::blink_monitor;
	
const u8 positions[ 5 ][2] = {
	{ 0x00, 0x00 }, // CLIP_NONE
	{ 0x01, 0x00 }, // CLIP_CUT
	{ 0x00, 0xFF }, // CLIP_PASTE
	{ 0xFF, 0x00 }, // CLIP_COPY
	{ 0x00, 0x01 }, // CLIP_CLONE
};

void Clip::init(){
	redraw  = false;
	visible = false;
	action  = CLIP_NONE;
}

void Clip::activate(){
	if( !visible || action==CLIP_NONE ) return;
	redraw = true;
	hide();
	switch(action){
		default:
			break;
		case CLIP_COPY	: return copy(); 
		case CLIP_PASTE	: return paste();
		case CLIP_CUT	: return cut();
		case CLIP_CLONE	: return clone();
	} 
}

void Clip::copy(){

}

void Clip::cut(){

}

void Clip::paste(){

}

void Clip::clone(){

}

void Clip::show(){
	if( visible ) return;
	visible = true;
	redraw  = true;
	action  = CLIP_NONE;
}

void Clip::hide(){
	if( !visible ) return;
	redraw = false;
	visible = false;
	regHnd.draw();

	for(int i=0; i<6; i++){
		Tracker::drawPosition(i);
	}
	Transient::changed = true;
}

void Clip::processInput(){
	action = CLIP_NONE; 
	if( KEY.down( KEY_LEFT  ) ) action = CLIP_COPY; 
	if( KEY.down( KEY_RIGHT ) ) action = CLIP_CUT; 
	if( KEY.down( KEY_UP    ) ) action = CLIP_PASTE; 
	if( KEY.down( KEY_DOWN  ) ) action = CLIP_CLONE; 
	redraw = true;
}

void Clip::draw( RegionHandler *rh ){
	redraw = false;
	if( !visible ){
		rh->draw();
		return;
	}
	int cc = VAR_CFG.CURRENTCHANNEL;
	cc = ( cc < 5 ) ? cc *= 4 : 6; 
 	rh->drawCache(cc+10, 3, &CACHE_CLIPBOARD2, 0, false);
	gpu.set(0, cc+12 + int(positions[ (int)action ][0]), 5 + int(positions[ (int)action ][1]), gpu.blink ? 0x14 : 0x13);
	blink_monitor = gpu.blink;
}


void Clip::update(RegionHandler *rh){
	
	gpu.blinkUpdate( 2 );
	
	// Draw if needed...
	if((visible) && ( redraw || rh->redraw || ( blink_monitor != gpu.blink ) )) draw( rh );
	// Or erase when needed
	if( redraw && !visible ){
		rh->draw();
		redraw = false;
	}
}
