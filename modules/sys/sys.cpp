/* ----------------------------------------------------------------------------
AUTHOR		 	Al P.Area ( Smiker )
PURPOSE			Singleton. 
				Provides an abstraction layer for the inner workings of m4g.
				This class is initialized from m4g.cpp, and its update function
				called once per main loop iteration.
ORIGINAL DATE 	2016, October
REVISION DATE 	2023-02-28
 --------------------------------------------------------------------------- */
#include "sys.hpp"
#include "../../data/enum.h"
#include "../../data/variables.hpp"
#include "../regionhandler/regionhandler.hpp"
#include "../key/key.hpp"
#include "../spu/spu.hpp"
#include "../int/int.hpp"
#include "../tim/tim.hpp"
#include "../mem/mem.hpp"
#include "../gpu/gpu.hpp"
#include "../sram/sram.hpp"

vu32 	SYS::timer			= 0;
u32 	SYS::profiled_time	= 0;
vu32 	SYS::profile_timer	= 0;
vu32 	SYS::fps			= 0;
vu32 	SYS::frames			= 0;
u16 	SYS::keyboard 		= 0x00;
u8  	SYS::cursor			= 0x0000;
bool	SYS::var_reset		= false;


void SYS::reset(){
	//SPU::stop();
	var_reset = true;
}

void SYS::init(){
	cursor	 	= 0x00; 
	keyboard 	= 0x0000;
	keyboard 	= 0x0000; // Clear Keypress, down and up buffer (AB, SL, ST, arrows)
	cursor 		= 0x00; 
	var_reset 	= false;
	//keyrate  	= 40;
	//retrig   	= false;
	INT::init();	
	MEM::init();	
	KEY::init();
	SPU::init( VAR_SONG.BPM );		
	SPU::enable(2);

	// This block may be moved before int init...
	SRAM::init();
	GPU::start();
   	INT::enable(IRQ_VBLANK);
	INT::enable(IRQ_HBLANK);
	TIM0.init(0);
	//INT.Enable(IRQ_KEYPAD);
	TIM0.setup(0x0004, 1);
	TIM0.enable();

	REGHND::init();
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

void SYS::updateInput(){
	
	KEY::update();
	
	u32 msg = 0;

	if(VAR_LIVE.PERFORM.LOCK)return;
	// Handle Navigation
	if(KEY::press(KEY_SELECT)){
		msg  = KEY::down(KEY_LEFT ) ? MESSAGE_NAVIGATE_LEFT  :
			   KEY::down(KEY_RIGHT) ? MESSAGE_NAVIGATE_RIGHT :
			   KEY::down(KEY_UP   ) ? MESSAGE_NAVIGATE_UP    :
			   KEY::down(KEY_DOWN ) ? MESSAGE_NAVIGATE_DOWN  :
			   0x0;
		if(REGHND::region && msg){
			REGHND::sendMessage(msg | (unsigned)REGHND::region);
			return;
		}
	}
	
	// Handle ACTIVATE message
	if(KEY::down(KEY_A) && REGHND::control) {
		REGHND::sendMessage(MESSAGE_ACTIVATE | (unsigned)REGHND::control);
		return;		
	}
	
	if(KEY::down(KEY_START)) {
		if(SPU::playing) {
			SPU::stop();
			return;
		}
		
		if(KEY::press(KEY_SELECT)) {
			SPU::play(true);
			return;
		}		
		if(!SPU::playing) SPU::play(false);
		return;
	}
	
	
	// Handle Copy Command (B+A)
	if(KEY::press(KEY_B) && REGHND::control) {
		if(KEY::down(KEY_A)) {
			//REGHND::sendMessage(MESSAGE_COPY | (unsigned)REGHND::control);
			return;		
		}
		
		msg = KEY::down(KEY_LEFT ) ? MESSAGE_OTHER_PREV : 
			  KEY::down(KEY_RIGHT) ? MESSAGE_OTHER_NEXT : 
			  KEY::down(KEY_DOWN ) ? MESSAGE_OTHER_BIGPREV : 
			  KEY::down(KEY_UP   ) ? MESSAGE_OTHER_BIGNEXT : 
			  0;
		
		if(msg)REGHND::sendMessage(msg);
		return;
	}
	
	// Handle control modifiers
	if(KEY::press(KEY_A)){
		msg  = KEY::down(KEY_LEFT ) ? MESSAGE_MODIFY_SUB 	:
			   KEY::down(KEY_RIGHT) ? MESSAGE_MODIFY_ADD 	:
			   KEY::down(KEY_UP   ) ? MESSAGE_MODIFY_BIGADD  :
			   KEY::down(KEY_DOWN ) ? MESSAGE_MODIFY_BIGSUB  :
			   0x0;
		if(REGHND::control && msg){
			REGHND::sendMessage(msg | (unsigned)REGHND::control);
			return;
		}
		
		// Handle A Cancel (A, then B)
		if(KEY::down(KEY_B) && REGHND::control) {
			REGHND::sendMessage(MESSAGE_CANCEL | (unsigned)REGHND::control);
			return;
		}
	}
		
	// Handle other arbitrary keypresses (MESSAGE_KEYPRESS)
	msg =  KEY::down(KEY_LEFT  ) ? 1 :
		   KEY::down(KEY_RIGHT ) ? 1 :
		   KEY::down(KEY_UP    ) ? 1 :
		   KEY::down(KEY_DOWN  ) ? 1 :
		/*   KEY::down(KEY_A     ) ? 1 :
		   KEY::down(KEY_B     ) ? 1 :*/	
		   KEY::down(KEY_SELECT) ? 1 :
		   KEY::down(KEY_START ) ? 1 :
		   0;
		   
	if(REGHND::control && msg ){
		REGHND::sendMessage(MESSAGE_KEYPRESS | (unsigned)REGHND::control);
	}
	
}


void SYS::update(){
	
	SPU::update();	
	updateInput();
	
	// if(KEY::Press(KEY_L))overloadTest(REGHND);
	// if(KEY::Press(KEY_R))GPU::vs->draw(14,2);
}

