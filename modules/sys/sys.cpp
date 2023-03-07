#include "sys.hpp"
#include "../../callbacks/snk.hpp"

Sys sys;

void Sys::reset(){
	Sequencer::stop();
	var_reset = true;
}

void Sys::init(){
	KEY.init();
	
	Clip::init();
	
	Sequencer::init( VAR_SONG.BPM );
	Synth::init();
	Mixer::init();
	
	/*-----------------------------------------------------------*/
	/* WORKAROUND TBF											 */
	/* ----------------------------------------------------------*/
	VAR_CFG.CURRENTINSTRUMENT = 0x1; // 0x0 is not accesible / usable
	/*----------------------------------------------------------*/
	
	Net::init();
	var_reset = false;
	
	//keyrate  = 40;
	//retrig   = false;
	keyboard = 0x0000; // Clear Keypress, down and up buffer (AB, SL, ST, arrows)
	cursor = 0x00; 
	
	SnakeGame::init();
	/*for(int i=0; i<6; i++){
		VAR_PATTERN[i].POINTER = VAR_PATTERN[i].ORDER;
	}*/
}

#define BUTTON_A		(kb & 0x0001)
#define BUTTON_B		(kb & 0x0002)
#define BUTTON_SELECT	(kb & 0x0004)
#define BUTTON_START	(kb & 0x0008)
#define BUTTON_R		(kb & 0x0100)
#define BUTTON_L		(kb & 0x0200)
#define BUTTON_RIGHT	(kb & 0x0010)
#define BUTTON_LEFT		(kb & 0x0020)
#define BUTTON_UP		(kb & 0x0040)
#define BUTTON_DOWN		(kb & 0x0080)

#define KEYBOARD_A		0x0001
#define KEYBOARD_B		0x0004
#define KEYBOARD_SELECT	0x0010
#define KEYBOARD_START	0x0040
#define KEYBOARD_R		0x0100
#define KEYBOARD_L		0x0400
#define CURSOR_RIGHT	0x01
#define CURSOR_LEFT		0x04
#define CURSOR_UP		0x10
#define CURSOR_DOWN		0x40

void Sys::updateInput(){
	u32 msg = 0;

	if(VAR_LIVE.PERFORM.LOCK)return;
	// Handle Navigation
	if(KEY.press(KEY_SELECT)){
		if(KEY.press(KEY_A))return;
		if(KEY.press(KEY_B))return;
		msg  = KEY.down(KEY_LEFT ) ? MESSAGE_NAVIGATE_LEFT  :
			   KEY.down(KEY_RIGHT) ? MESSAGE_NAVIGATE_RIGHT :
			   KEY.down(KEY_UP   ) ? MESSAGE_NAVIGATE_UP    :
			   KEY.down(KEY_DOWN ) ? MESSAGE_NAVIGATE_DOWN  :
			   0x0;
		if(regHnd.region && msg){
			regHnd.sendMessage(msg | (unsigned)regHnd.region);
			return;
		}
	}
	
	// Handle ACTIVATE message
	if(KEY.down(KEY_A) && regHnd.control) {
		regHnd.sendMessage(MESSAGE_ACTIVATE | (unsigned)regHnd.control);
		return;		
	}
	
	if( KEY.down( KEY_START ) ) {
		if( Sequencer::playing ) {
			Sequencer::stop();
			return;
		}
		
		if( KEY.press( KEY_SELECT ) ) {
			Sequencer::play(true);
			return;
		}		
		if( !Sequencer::playing ) Sequencer::play( false );
		return;
	}
	
	// Handle Copy Command (B+A)
	if(KEY.press(KEY_B) && regHnd.control) {
		if(KEY.down( KEY_SELECT )){
			Clip::show();
			return;
		}
		
		if(KEY.press(KEY_A)) {
			regHnd.sendMessage(MESSAGE_PASTE | (unsigned)regHnd.control);
			return;		
		}
		
		msg = KEY.down(KEY_LEFT ) ? MESSAGE_OTHER_PREV : 
			  KEY.down(KEY_RIGHT) ? MESSAGE_OTHER_NEXT : 
			  KEY.down(KEY_DOWN ) ? MESSAGE_OTHER_BIGPREV : 
			  KEY.down(KEY_UP   ) ? MESSAGE_OTHER_BIGNEXT : 
			  0;
		
		if(msg)regHnd.sendMessage(msg);
		return;
	}
	
	// Handle control modifiers
	if(KEY.press(KEY_A)){
		msg  = KEY.down(KEY_LEFT ) ? MESSAGE_MODIFY_SUB 	:
			   KEY.down(KEY_RIGHT) ? MESSAGE_MODIFY_ADD 	:
			   KEY.down(KEY_UP   ) ? MESSAGE_MODIFY_BIGADD  :
			   KEY.down(KEY_DOWN ) ? MESSAGE_MODIFY_BIGSUB  :
			   0x0;
		if(regHnd.control && msg){
			regHnd.sendMessage(msg | (unsigned)regHnd.control);
			return;
		}
		
		// Handle A Cancel (A, then B)
		if(KEY.down(KEY_B) && regHnd.control) {
			regHnd.sendMessage(MESSAGE_CANCEL | (unsigned)regHnd.control);
			return;
		}
	}
		
	// Handle other arbitrary keypresses (MESSAGE_KEYPRESS)
	msg =  KEY.down(KEY_LEFT  ) ? 1 :
		   KEY.down(KEY_RIGHT ) ? 1 :
		   KEY.down(KEY_UP    ) ? 1 :
		   KEY.down(KEY_DOWN  ) ? 1 :
		/*   KEY.down(KEY_A     ) ? 1 :
		   KEY.down(KEY_B     ) ? 1 :*/	
		   KEY.down(KEY_SELECT) ? 1 :
		   KEY.down(KEY_START ) ? 1 :
		   0;
		   
	if(regHnd.control && msg ){
		regHnd.sendMessage(MESSAGE_KEYPRESS | (unsigned)regHnd.control);
	}
	
}

void overloadTest(RegionHandler &regHnd);

void Sys::update(){
	
	Sequencer::update();	
	KEY.update();
	updateInput();
	
	
	//if(KEY.press(KEY_L)) overloadTest(regHnd);
	// if(KEY.Press(KEY_R))gpu.vs->draw(14,2);
}

