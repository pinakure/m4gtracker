#include "clip.hpp"
#include "../../callbacks/debug.hpp"

u16 Clipboard::data[16*6];	// 16 rows, 6 channels / 5 fields. If copying patterns, values are lost and viceversa
u16 Clipboard::len;			// length of the block used currently (0 by default)
u8 	Clipboard::x;			
u8 	Clipboard::y;			
u8 	Clipboard::width;	
u8 	Clipboard::height;
u8 	Clipboard::type;

bool 			Clip::visible = false;
bool 			Clip::redraw  = false;
bool 			Clip::blink_monitor  = false;
ClipboardAction Clip::action  = CLIP_NONE;
	
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
	if( !Clipboard::width ) {
		Tracker::icon( 0x7051, 0x00A2);
		return;
	}
	Tracker::icon( 0x7051, 0x00A3);
}

void Clip::cut(){
	if( !Clipboard::width ) {
		Tracker::icon( 0x7051, 0x00A2);
		return;
	}
	Tracker::icon( 0x7050, 0x00A3);
}

void Clip::paste(){
	if( !Clipboard::width ) {
		Tracker::icon( 0x7051, 0x00A2);
		return;
	}
	Tracker::icon( 0x7054, 0x7058);
}

void Clip::clone(){
	Tracker::icon( 0x7052, 0x7053);
}

void Clip::show(){
	if( visible ) return;
	
	Clipboard::height 	= 0;
	Clipboard::width  	= 1;
	Clipboard::x 	 	= AT_TRACKER_SCREEN ? TRACKER_ACTIVE_COLUMN  : PATTERN_ACTIVE_COLUMN;
	Clipboard::y 	 	= AT_TRACKER_SCREEN ? TRACKER_ACTIVE_ROW 	 : PATTERN_ACTIVE_ROW;
	Clipboard::type 	= CLIPDATA_PATTERN; 
	
	visible 			= true;
	redraw  			= true;
	action  			= CLIP_NONE;
}

void Clip::hide(){
	if( !visible ) return;
	redraw = false;
	visible = false;
	regHnd.draw();

	if( AT_TRACKER_SCREEN ){
		for(int i=0; i<6; i++){
			Tracker::drawPosition(i);
		}
		Transient::changed = true;
	}
}

void Clip::processInput(){
	//action = CLIP_NONE;
	if(KEY.down( KEY_SELECT )) return Clip::show();
	if(! Clip::visible ) return;
	
	if( KEY.press ( KEY_L )){
		if( KEY.down( KEY_UP    ) ) maskSizeVert( false ); 
		if( KEY.down( KEY_DOWN  ) ) maskSizeVert( true  ); 
		if( KEY.down( KEY_RIGHT ) ) maskSizeHorz( true  );
		if( KEY.down( KEY_LEFT  ) ) maskSizeHorz( false );
		return;
	}
	
	if(! KEY.press ( KEY_R )){	
		if( KEY.down( KEY_LEFT  ) ) setAction( CLIP_COPY	); 
		if( KEY.down( KEY_RIGHT ) ) setAction( CLIP_CUT		); 
		if( KEY.down( KEY_UP    ) ) setAction( CLIP_PASTE	); 
		if( KEY.down( KEY_DOWN  ) ) setAction( CLIP_CLONE	); 
	} else {
		if( KEY.down( KEY_UP    ) ) maskMoveVert( false ); 
		if( KEY.down( KEY_DOWN  ) ) maskMoveVert( true  ); 
		if( KEY.down( KEY_RIGHT ) ) maskMoveHorz( true  );
		if( KEY.down( KEY_LEFT  ) ) maskMoveHorz( false );
	}
}

void Clip::maskMoveVert( bool increase ){
	if( Clipboard::height == 0xF ) return;
	drawMask(0x00);
	if( (Clipboard::y + (increase ? 1 : -1) + Clipboard::height ) <= 0xF) Clipboard::y += increase ? 1 : -1;
	if( Clipboard::y + Clipboard::height > 0xF ) {
		Clipboard::y 	  = 0;
	}
}

void Clip::maskMoveHorz( bool increase ){
	if( Clipboard::width == 0) return;
	drawMask(0x00);
	u8 max = ( AT_TRACKER_SCREEN ? 5 : 6 );
	Clipboard::x += increase ? 1 : -1;
	if( Clipboard::x + Clipboard::width > max ) Clipboard::x = 0;
}

void Clip::maskSizeVert( bool increase ){
	drawMask(0x00);
	Clipboard::height += increase ? 1 : -1;
	if( Clipboard::height > 0xF ) Clipboard::height = 0xF;
}

void Clip::maskSizeHorz( bool increase ){
	drawMask(0x00);
	u8 max = ( AT_TRACKER_SCREEN ? 5 : 6 );
	Clipboard::width += increase ? 1 : -1;
	if( Clipboard::width  > max ) Clipboard::width	= max;
}

void Clip::setAction( ClipboardAction selected_action ){
	action = selected_action;
	redraw = true;
}

void Clip::drawMaskPat(int color){
	u8 c1 = 0x10 | (color ? 0x12 : 0x10 );
	
	for( int y = 0; y < Clipboard::height+1; y++){
		u8 cc = 4 + ( Clipboard::x * 4);
		for( int x = 0; x < Clipboard::width; x++){
			gpu.set( 1, cc    , Clipboard::y + 4 + y, c1 );
			gpu.set( 1, cc + 1, Clipboard::y + 4 + y, c1 );
			cc+=4;
		}
	}
}

void Clip::drawMaskTrk(int color){
	u8 cc = 1 + ( 4 * VAR_CFG.CURRENTCHANNEL);
	u8 c1 = 0x10 | (color ? 0x12 : 0x10 );
	u8 c2 = c1;
	
	for( u8 x = 0; x < 0 + Clipboard::width; x++){
		switch(x + Clipboard::x ){
			case 0:	
				for( u8 y = 0; y < Clipboard::height+1; y++){
					gpu.set( 1, cc    , Clipboard::y + 4 + y, c1 );
					gpu.set( 1, cc + 1, Clipboard::y + 4 + y, c1 );
					gpu.set( 1, cc + 2, Clipboard::y + 4 + y, c1 );
				}
				continue;
			case 1:
				for( u8 y = 0; y < Clipboard::height+1; y++){
					gpu.set( 1, cc + 3, Clipboard::y + 4 + y, c2 );
					gpu.set( 1, cc + 4, Clipboard::y + 4 + y, c2 );
				}
				continue;
			case 2:
				for( u8 y = 0; y < Clipboard::height+1; y++){
					gpu.set( 1, cc + 5, Clipboard::y + 4 + y, c1 );
				}
				continue;
			case 3:
				for( u8 y = 0; y < Clipboard::height+1; y++){
					gpu.set( 1, cc + 6, Clipboard::y + 4 + y, c2 );
				}
				continue;
			case 4:
				for( u8 y = 0; y < Clipboard::height+1; y++){
					gpu.set( 1, cc + 7, Clipboard::y + 4 + y, c1 );
					gpu.set( 1, cc + 8, Clipboard::y + 4 + y, c1 );
				}
				continue;				
		}
	}
}

void Clip::drawMask( int color ){
	return AT_TRACKER_SCREEN ? drawMaskTrk( color ) : drawMaskPat( color );
}

void Clip::draw( RegionHandler *rh ){
	redraw = false;
	if( !visible ){
		rh->draw();
		return;
	}
	blink_monitor = gpu.blink;
	int cc = VAR_CFG.CURRENTCHANNEL;
	cc = ( cc < 5 ) ? cc *= 4 : 6; 
 	if(AT_TRACKER_SCREEN){
		rh->drawCache(cc+10, 3, &CACHE_CLIPBOARD2, 0, false);
		gpu.set(0, cc+12 + int(positions[ (int)action ][0]), 5 + int(positions[ (int)action ][1]), gpu.blink ? 0x14 : 0x13);
	} else {
		rh->drawCache(26, 0, &CACHE_CLIPBOARD2, 0, false);
		gpu.set(0, 28 + int(positions[ (int)action ][0]), 2 + int(positions[ (int)action ][1]), gpu.blink ? 0x14 : 0x13);
		gpu.set(1, 29, 4, 0x0f);
		gpu.set(1, 26, 3, 0x08);
		gpu.set(1, 26, 4, 0x00);
	}
	
	// draw clipboard mask
	drawMask( COLOR_DARK_CYAN );
}


void Clip::update(RegionHandler *rh){
	
	gpu.blinkUpdate(8);
	
	// Draw if needed...
	if((visible) && ( redraw || rh->redraw || ( blink_monitor != gpu.blink ) )) draw( rh );
	// Or erase when needed
	if( redraw && !visible ){
		rh->draw();
		redraw = false;
	}
}
