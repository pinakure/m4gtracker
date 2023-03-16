#include "really.hpp"
#include "../modules/gpu/gpu.hpp"
#include "../debug.hpp"
#include "../modules/key/key.hpp"
#include "../modules/spu/sequencer.hpp"

u16 ReallyDialog::background = 0x19;

void ReallyDialog::draw(){
	Gpu::blit(MAP_CACHE, 0x13, 0x0, 12, 6, 0x09, 0x5);
	// Remove 2 black Arrows
	/*! HACK */	Gpu::set( 2, 13, 9, 0x100);
	/*! HACK */	Gpu::set( 2, 15, 9, 0x100);
	// Fix dialog borders 
	/*! HACK */	Gpu::set( 1, 12, 6, 0x6);
	/*! HACK */ 	Gpu::set( 1, 12, 7, 0xA);
	/*! HACK */ 	Gpu::set( 1, 12, 8, 0xA);
	/*! HACK */ 	Gpu::set( 1, 12, 9, 0xA);
	/*! HACK */ 	Gpu::set( 1, 12,10, 0x8);
	// Change background color
	for( int y = 0 ; y < 5 ; y++ ){
		for( int x = 0 ; x < 8 ; x++ ){
			Gpu::set( 0, 12 + x , 6 + y , background );
		}
	}
	// Erase old cursor
	Gpu::set( 0, 14, 9, background );
	Gpu::set( 0, 16, 9, background );
	// Draw cursor
	Gpu::set( 0, 14 + (!result?3:0), 9, ((!result? COLOR_ORANGE : COLOR_YELLOW )<<12)|0x18);
}

void ReallyDialog::confirm(){
	if( KEYDOWN_B 	) return;
	if( KEYPRESS_B	) return;
	if( KEYUP_B		) return;
		
	result  = true;
	enabled = false;
}

void ReallyDialog::cancel(){
	result  = false;
	enabled = false;
}

void ReallyDialog::enable(){
	result  	= false;
	enabled 	= true;
	highlight	= false;
	background = 0x19;
	draw();
	
	while(enabled){		
		
		Sequencer::update();
		KEYUPDATE();
		
		if( KEYDOWN_LEFT 	) { result = 1; draw(); };
		if( KEYDOWN_RIGHT	) { result = 0; draw(); };
		if( KEYUP_A			) { if( result ) confirm(); else cancel(); }
		if( KEYUP_B			) cancel();
	}
	KEYUPDATE();
}
