/* ----------------------------------------------------------------------------
AUTHOR		 	Al P.Area ( Smiker )
PURPOSE			Snake minigame class and update routine
ORIGINAL DATE 	2016, October
REVISION DATE 	2023-02-28
 --------------------------------------------------------------------------- */
#include "callbacks.hpp"
#include "../modules/gpu/gpu.hpp"
#include "../modules/key/key.hpp"
#include "../modules/sram/sram.hpp"
#include "../modules/sys/sys.hpp"

#include "../modules/snk/snk.hpp"

SnakeCell 		SNK::cells[32];
u8 				SNK::data[32*32];
u8 				SNK::length			= 3;
u8 				SNK::tick_level		= 255;
u8 				SNK::tick			= 0;
u8 				SNK::speed			= 0;
SnakeDir 		SNK::direction		= SNAKE_UP;
SnakeDir 		SNK::next_direction	= SNAKE_RIGHT;
VirtualScreen*	SNK::vs				= NULL;
bool 			SNK::turbo			= false;
bool 			SNK::game_start		= false;

void SNK::init(){
	vs 			= &GPU::vs;
	game_start  = false;
}

void SNK::resetPosition(){
	cells[0].x = 15;
	cells[0].y = 15;
	cells[1].x = 16;
	cells[1].y = 15;
	cells[2].x = 17;
	cells[2].y = 15;
}

void SNK::start(){
	direction 		= SNAKE_RIGHT;
	next_direction 	= SNAKE_RIGHT;
	speed 			= 1;
	tick_level 		= 32;
	length 			= 6;	
	tick 			= tick_level;
	game_start 		= true;
	resetPosition();
}

void SNK::draw(){
	GPU::vs.clear();
	
	int i;
	for( i=0; i<length; i++ ){
		vs->set( cells[i].x, cells[i].y );
	}
	
	GPU::vs.draw( 11, 6 );
}

void SNK::move(){
	for( int i=1; i<length; i++ ){
		cells[i].x = cells[i-1].x;
		cells[i].y = cells[i-1].y;
	}
	
	if( direction == SNAKE_UP	 ) cells[0].y--; else 
	if( direction == SNAKE_DOWN	 ) cells[0].y++; else
	if( direction == SNAKE_LEFT	 ) cells[0].x--; else
	if( direction == SNAKE_RIGHT ) cells[0].x++;
	
	cells[0].x &= 31;
	cells[0].y &= 31;
}

void SNK::update(){
	if( !game_start ){
		if( KEYUP_START ){
			game_start = true;
			start();
			//TODO: avoid START signal propagation from this point
		}
		return;
	}

	if( KEYPRESS_LEFT  && ( direction !=SNAKE_RIGHT ) ) next_direction = SNAKE_LEFT	; else
	if( KEYPRESS_UP    && ( direction !=SNAKE_DOWN  ) ) next_direction = SNAKE_UP	; else
	if( KEYPRESS_RIGHT && ( direction !=SNAKE_LEFT  ) ) next_direction = SNAKE_RIGHT ; else
	if( KEYPRESS_DOWN  && ( direction !=SNAKE_UP    ) ) next_direction = SNAKE_DOWN	;
	
	turbo = KEYPRESS_B ? true : false;
	
	
	// Legacy UPDATE code
	if( tick > 0 ){
		tick--;
	} else {
		move();				
		direction 	= next_direction;
		tick 		= tick_level >> turbo;
		SNK::draw();
	}
}
