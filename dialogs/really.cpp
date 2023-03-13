#include "really.hpp"
#include "../modules/gpu/gpu.hpp"
#include "../modules/key/key.hpp"
#include "../modules/spu/sequencer.hpp"

void ReallyDialog::draw(){
	gpu.blit(MAP_CACHE, 0x13, 0x0, 12, 6, 0x09, 0x5);
	gpu.set(1, 12, 6, 0x6);
	gpu.set(1, 12, 7, 0xA);
	gpu.set(1, 12, 8, 0xA);
	gpu.set(1, 12, 9, 0xA);
	gpu.set(1, 12,10, 0x8);
	
	gpu.set(0, 14, 9, 0x00);
	gpu.set(0, 16, 9, 0x00);
	gpu.set(0, 14 + (!result?3:0), 9, 0x15 );
}

void ReallyDialog::confirm(){
	if(KEY.down	(KEY_B)) return;
	if(KEY.press(KEY_B)) return;
	if(KEY.up	(KEY_B)) return;
		
	result  = true;
	enabled = false;
}

void ReallyDialog::cancel(){
	result  = false;
	enabled = false;
}

void ReallyDialog::enable(){
	result    = false;
	enabled   = true;
	highlight = false;
	
	draw();
	
	while(enabled){		
		
		Sequencer::update();	
		KEY.update();
		
		if(KEY.down( KEY_LEFT  ) ) { result = 1; draw(); };
		if(KEY.down( KEY_RIGHT ) ) { result = 0; draw(); };
		if(KEY.up(KEY_A) ) { if( result ) confirm(); else cancel();}
		if(KEY.up(KEY_B) ) cancel();		
	}
	KEY.update();
}

