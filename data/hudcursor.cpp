#include "hudcursor.hpp"
#include "../debug.hpp"
#include "../kernel/gpu/gpu.hpp"
#include "../kernel/sys/sys.hpp"
#include "../kernel/key/key.hpp"
#include "../kernel/clip/clip.hpp"
#include "../kernel/regionhandler/regionhandler.hpp"
#include "../data/data.hpp"

Sprite 	HudCursor::sprite[8];
Point 	HudCursor::target;
Point 	HudCursor::size;
Sprite 	HudCursor::playback[24];		
Sprite 	HudCursor::clip_icons[4];
Sprite 	HudCursor::clip_selection;

OBJ_ATTR obj_buffer[128];
OBJ_AFFINE *const obj_aff_buffer= (OBJ_AFFINE*)obj_buffer;

Sprite::Sprite( u8 index, u8 coord_x, u8 coord_y){
	position.x			= coord_x;		
	position.y			= coord_y;
	init(index);
}

void Sprite::init( u8 index ){
	this->index			= index;		
	flip_h				= false;
	flip_v				= false;
	size				= SPRITESIZE_8X8__16x8__8X16;
	mode				= SPRITEMODE_SEMITRANSPARENT;
	tile_number			= 0x18;
	palette				= 0x3;
	priority			= 3;
	rotoscale 			= false;
	double_size 		= false;
	disable				= false;
	mosaic 				= false;
	trans_index 		= 0x1;
	colors 				= 0;// 16/16
	shape 				= 0;// 0 square // 1 horz // 2 vert // 3 forbidden
}

void Sprite::render(){
	if(!(((R_VCOUNT&0x00FF) >=160)&&((R_VCOUNT&0x00FF) <200))) return;
	(*(u16*)0x4000000) |= 0x1080; 
	OBJ_ATTR* p = (OBJ_ATTR*)&(*(u16*)(OAM+(index*8)));
	p->attr0 	= ( (position.y>>3) & 0xFF)
				| ( (rotoscale) 	<< 8  )
				| (( double_size || disable ) << 9 )
				| ( mode 		<< 10 )
				| ( mosaic 		<< 12 )
				| ( colors 		<< 13 )
				| ( shape		<< 14 );
	p->attr1 	= ( (position.x>>3) & 0x1FF )
				| ( trans_index <<  9 )
				| ( flip_h 		<< 12 )
				| ( flip_v 		<< 13 )
				| ( size        << 14 );
	p->attr2 	= ( tile_number & 0x3FF)
				| ( priority 	<< 10 )
				| ( palette  	<< 12 );
	(*(u16*)0x4000000) &= 0xff7f;
}

void HudCursor::init(){
	/*
	
	OBJ_AFFINE* p 	= &(*(OBJ_AFFINE*)(OAM+(1*32)));
	
	s32 dx = 1200;
	s32 dy = 0;
	s32 sx = 3800;
	s32 sy = 0;
	
	p->pa 		  	= dx;
	p->pb 			= dy;
	p->pc 		  	= sx;
	p->pd 			= sy;
	*/
	
	size.x 				= 4;
	size.y 				= 1;
	
	// Control cursor ( max 8 wide )
	for(int i=0; i<8; i++){
		sprite[i].init(i);
	}
	
	// Tracker playback cursors ( 1 per channel, 6 total )
	for(int i=0,x=1; i<24; i++,x++){
		playback[i].init(8+i);
		playback[i].palette = 0x5;
		playback[i].position.y = 32<<3;
		playback[i].position.x = (x*8)<<3;
		playback[i].channel_index = i/6;
	}
	
	clip_icons[0].init(32);
	clip_icons[0].tile_number 		= 0x2D0;
	clip_icons[0].priority 		= 0;
	clip_icons[0].palette 			= 0x6;
	/*
	clip_icons[CLIP_COPY-1].init(33);
	clip_icons[CLIP_COPY-1].tile_number 	= 0x2D1;
	clip_icons[CLIP_COPY-1].priority 		= 0;
	clip_icons[CLIP_COPY-1].palette 		= 0x6;
	
	clip_icons[CLIP_CLONE - 1].init(34);
	clip_icons[CLIP_CLONE - 1].tile_number 	= 0x2D2;
	clip_icons[CLIP_CLONE - 1].priority 	= 0;
	clip_icons[CLIP_CLONE - 1].palette 		= 0x6;
	
	clip_icons[CLIP_PASTE-1].init(35);
	clip_icons[CLIP_PASTE-1].tile_number 	= 0x2D4;
	clip_icons[CLIP_PASTE-1].priority 		= 0;
	clip_icons[CLIP_PASTE-1].palette 		= 0x6;
	*/
	clip_selection.init(36);
	clip_selection.priority 	= 1;
	clip_selection.palette  	= 3;
}

void HudCursor::render(){
	size.x = RegionHandler::control->cache ? RegionHandler::control->cache->itemwidth : 0;
	size.x = size.x > 7 ? 7 : size.x;
	// if(!size.x)HALT;
	/* Overlap all sprites */ 
	for(int i=1; i<8; i++){
		sprite[i].position.x = sprite[0].position.x ;
		sprite[i].position.y = sprite[0].position.y ;	
		sprite[i].disable = true;
	}
	
	/* Deploy only needed sprites together */ 
	for(int i=1; i < size.x; i++){
		sprite[i].position.x = sprite[0].position.x + ((i*8)<<3);
		sprite[i].disable = false;
	}

	sprite[0].disable = (size.x == 0) && (!sprite[0].delta.x);
	
	/* Copy cursor data to OAM */
	for(int i=0; i<8; i++){
		sprite[i].render();
	}
	
	/* Update tracker cursors position */
	u8 positions[6] = {
		VAR_CHANNEL[0].LASTSTEP,
		VAR_CHANNEL[1].LASTSTEP,
		VAR_CHANNEL[2].LASTSTEP,
		VAR_CHANNEL[3].LASTSTEP,
		VAR_CHANNEL[4].LASTSTEP,
		VAR_CHANNEL[5].LASTSTEP,
	};	
	for(int i=0, p=0, x=0; i<6; i++){
		u8 remain = 9;
		while(remain>0){
			if(( i != VAR_CFG.CURRENTCHANNEL )&&(remain==6))break;
			playback[p].position.y 	= ((4*8)<<3)+(( positions[ i ] * 8 )<<3);
			playback[p].position.x 	= ((x+1)*8)<<3;
			playback[p].disable 	= !AT_TRACKER_SCREEN;
			playback[p].render();
			remain--;
			p++;
			x++;
		}
		x++;
	}
	
	/* draw cliboard icons */
	// if(!Clip::visible) return;
	// return;
	int x = Clip::x;
	int y = Clip::y;
	
	clip_icons[ CLIP_CUT	- 1 ].disable    = !Clip::visible;
	clip_icons[ CLIP_CUT	- 1 ].position.x = ((x+2)*8)<<3;
	clip_icons[ CLIP_CUT	- 1 ].position.y = ((y+1)*8)<<3;
	clip_icons[ CLIP_CUT	- 1 ].render();
	clip_icons[ CLIP_PASTE 	- 1 ].disable    = !Clip::visible;
	clip_icons[ CLIP_PASTE 	- 1 ].position.x = ((x+1)*8)<<3;
	clip_icons[ CLIP_PASTE 	- 1 ].position.y = ((y  )*8)<<3;
	clip_icons[ CLIP_PASTE 	- 1 ].render();
	clip_icons[ CLIP_COPY 	- 1 ].disable    = !Clip::visible;
	clip_icons[ CLIP_COPY 	- 1 ].position.x = ((x)*8)<<3;
	clip_icons[ CLIP_COPY 	- 1 ].position.y = ((y+1)*8)<<3;
	clip_icons[ CLIP_COPY 	- 1 ].render();
	clip_icons[ CLIP_CLONE 	- 1 ].disable    = !Clip::visible;
	clip_icons[ CLIP_CLONE 	- 1 ].position.x = ((x+1)*8)<<3;
	clip_icons[ CLIP_CLONE 	- 1 ].position.y = ((y+2)*8)<<3;
	clip_icons[ CLIP_CLONE 	- 1 ].render();
	
	clip_selection.disable    = !Clip::visible;
	if(!Clip::action){
		clip_selection.position.x = clip_icons[ CLIP_COPY - 1 ].position.x + (8<<3);
		clip_selection.position.y = clip_icons[ CLIP_COPY - 1 ].position.y;
		clip_selection.palette = 0x3;
	} else {
		clip_selection.position.x = clip_icons[ Clip::action-1 ].position.x;// skip none
		clip_selection.position.y = clip_icons[ Clip::action-1 ].position.y;// skip none
		clip_selection.palette = 0x7;
	}
	clip_selection.render();
}

void HudCursor::update(){
	if(!RegionHandler::control) return;
	
	Sprite *spr = &sprite[0];
		
	
	if(((R_VCOUNT&0x00FF) >=160)&&((R_VCOUNT&0x00FF) <200)){
		
		target.x = RegionHandler::control->x<<6;
		target.y = RegionHandler::control->y<<6;
		
		// (Re) set target point 
		/*
		if(KEYPRESS_LEFT	) target.x = ((sprite.x >> 3)- 8)<<3;
		if(KEYPRESS_RIGHT	) target.x = ((sprite.x >> 3)+ 8)<<3;
		if(KEYPRESS_DOWN	) target.y = ((sprite.y >> 3)+ 8)<<3;
		if(KEYPRESS_UP   	) target.y = ((sprite.y >> 3)- 8)<<3;
		*/
		
		
		// Check if we reached target point
			 if	( spr->position.x < target.x ) spr->delta.x += ( 1<<3 );
		else if	( spr->position.x > target.x ) spr->delta.x -= ( 1<<3 );
		else spr->delta.x = 0;
		
			 if ( spr->position.y < target.y ) spr->delta.y += ( 1<<3 );
		else if ( spr->position.y > target.y ) spr->delta.y -= ( 1<<3 );
		else spr->delta.y = 0;	
		
		// Move cursor 
		spr->position.x += ( spr->delta.x>>3 );
		spr->position.y += ( spr->delta.y>>3 );
		
		// If we get past in X axis, stick to the target X coordinate
		if( (( spr->position.x > target.x )&&( spr->delta.x > 0 )) || (( spr->position.x < target.x )&&( spr->delta.x < 0 ) )) {
			spr->position.x /= 8;
			spr->position.x *= 8;
			spr->delta.x 	 = 0;
		}
		
		// If we get past in Y axis, stick to the target Y coordinate
		if( (( spr->position.y > target.y )&&( spr->delta.y > 0 )) || (( spr->position.y < target.y )&&( spr->delta.y < 0 ) )) {
			spr->position.y /= 8;
			spr->position.y *= 8;
			spr->delta.y 	 = 0;
		}
		
		// Limit boundaries
		if( spr->position.x < 0 ) spr->position.x = 0;
		if( spr->position.y < 0 ) spr->position.y = 0;
		if( ( spr->position.x>>3 ) > (240-8)) spr->position.x = (240-8)<<3;
		if( ( spr->position.y>>3 ) > (160-8)) spr->position.y = (160-8)<<3;
		Key::update();
	}
	render();
}

