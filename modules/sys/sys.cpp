#include "sys.hpp"

#include "../../macros.h"
#include "../../debug.hpp"

#include "../../data/data.hpp"
#include "../../data/input.hpp"

#include "../../screens/snake.hpp"
#include "../../screens/config.hpp"
#include "../../screens/tracker.hpp"
#include "../../screens/live.hpp"
#include "../../screens/songedit.hpp"
#include "../../screens/patedit.hpp"

#include "../spu/sequencer.hpp"
#include "../spu/synth.hpp"
#include "../spu/mixer.hpp"
#include "../net/net.hpp"
#include "../int/int.hpp"
#include "../sram/sram.hpp"
#include "../tim/tim.hpp"
#include "../clip/clip.hpp"
#include "../key/key.hpp"

u16 	Sys::keyboard;
u8  	Sys::cursor;

void Sys::reset(){
	Sequencer::stop();
	Mixer::stop();
	asm("swi 00");
}

void Sys::setScreen( eScreens screen ){
	switch( screen ){
		case SCREEN_LIV1	: return RegionHandler::load( &REGION_MAP_1_LIVE1 	);
		case SCREEN_LIV2	: return RegionHandler::load( &REGION_MAP_1_LIVE2 	);
		case SCREEN_HLP		: return RegionHandler::load( &REGION_MAP_1_HLP 		);
		case SCREEN_PAT		: return RegionHandler::load( &REGION_MAP_2_PAT		);
		case SCREEN_TRK		: return RegionHandler::load( &REGION_MAP_3_TRK 		);
		case SCREEN_INS		: return RegionHandler::load( &REGION_MAP_2_INS		);
		case SCREEN_SNG		: return RegionHandler::load( &REGION_MAP_2_SNG		);
		case SCREEN_CFG		: return RegionHandler::load( &REGION_MAP_4_CFG		);
		case SCREEN_SNK		: return RegionHandler::load( &REGION_MAP_4_SNK		);
	}		
}

void Sys::init(){
	
	/*!-----------------------------------------------*/
	/* Low level initialization								*/
	/*!-----------------------------------------------*/
	Interrupt::init();	
	Memory::init();
	Sram::init();
	Gpu::init();
	
	// Enable interrupts
	Interrupt::enable( IRQ_VBLANK );
	Interrupt::enable( IRQ_HBLANK );
	//Interrupt::enable(IRQ_KEYPAD); // We poll the keyboard every call to KEY.update, so no input keyboard is used
	
	// Initialize timers
	timer0.init(0);
	timer0.setup(0x0004, 1);
	timer0.enable();

	/*!-----------------------------------------------*/
	/* High level initialization							*/
	/*!-----------------------------------------------*/
	cursor		= 0x00; 
	keyboard 	= 0x0000; // Clear Keypress, down and up buffer (AB, SL, ST, arrows)
	//keyrate  	= 40;
	//retrig    = false;
	
	// Initialize RegionHandler
	RegionHandler::init();
	
	#ifndef NDEBUG
	Debug::init();
	#endif
	
	// First of all load config
	Config::load();
	
	// Initialize Input registers
	KEYINIT();
	
	// Cleanup clipboard dirt
	Clip::init();
	
	// Default restore SPU Subclasses
	Sequencer::init( VAR_SONG.BPM );
	Synth::init();
	Mixer::init();
	
	/*-----------------------------------------------------------*/
	/* WORKAROUND TBF											 */
	/* ----------------------------------------------------------*/
	VAR_CFG.CURRENTINSTRUMENT = 0x1; // 0x0 is not accesible / usable
	/*----------------------------------------------------------*/
	
	Net::init();
	
	SnakeGame::init();
	
	// Load first instrument
	InstEdit::unpack( &VAR_INSTRUMENT );	

	// Finally, if in debug build, run prioritary tests (prototypes), then continue
	#ifndef NDEBUG
	Debug::runTests();
	#endif
	
	// Start SYS
	Gpu::clear();
	LookNFeel::init();
		
	// Start at Tracker screen	
	setScreen( SCREEN_TRK );
	
	if( VAR_CFG.BEHAVIOR.AUTOLOAD ){
		Sram::songLoad(false);
		SongEdit::has_data[ 0 ] = true;	
	}
}

#define BUTTON_A			(kb & 0x0001)
#define BUTTON_B			(kb & 0x0002)
#define BUTTON_SELECT		(kb & 0x0004)
#define BUTTON_START		(kb & 0x0008)
#define BUTTON_R			(kb & 0x0100)
#define BUTTON_L			(kb & 0x0200)
#define BUTTON_RIGHT		(kb & 0x0010)
#define BUTTON_LEFT			(kb & 0x0020)
#define BUTTON_UP			(kb & 0x0040)
#define BUTTON_DOWN			(kb & 0x0080)

#define KEYBOARD_A			0x0001
#define KEYBOARD_B			0x0004
#define KEYBOARD_SELECT		0x0010
#define KEYBOARD_START		0x0040
#define KEYBOARD_R			0x0100
#define KEYBOARD_L			0x0400
#define CURSOR_RIGHT		0x01
#define CURSOR_LEFT			0x04
#define CURSOR_UP			0x10
#define CURSOR_DOWN			0x40

void Sys::updateInput(){
	u32 msg = 0;

	/* IGNORE this update function if LIVE1 lock var is true */
	if(Live::PERFORM.LOCK)return;
	
	
	/* ------------------------------------------------------------------------
		( Tracker Screen Only )
		- Clipboard Management 
		- Grow Clipboard 
		- Cancel selection
		- Confirm selection
	---------------------------------------------------------------------------
		L + SELECT
	-------------------------------------------------------------------------*/ 
	if( AT_TRACKER_SCREEN ){
		if( KEYPRESS_L ) {
			return Clip::processInput();
		}
		if( KEYPRESS_R ) {
			if( Clip::visible ) 
				return Clip::processInput();
			else 
				return Tracker::processInput();
		}
	}
	
	if( AT_PATTERN_SCREEN ){
		if( KEYPRESS_L ) {
			return Clip::processInput();
		}
		if( KEYPRESS_R ) {
			if( Clip::visible ) 
				return Clip::processInput();
			else 
				return PatEdit::processInput();
		}
	}
	
	/* ------------------------------------------------------------------------
		Handle Navigation
	---------------------------------------------------------------------------
		SELECT + LEFT
		SELECT + RIGHT
		SELECT + UP
		SELECT + DOWN
	-------------------------------------------------------------------------*/ 
	if(
	KEYPRESS_SELECT &&
	!KEYPRESS_A		&&
	!KEYPRESS_B		){
		msg  	= KEYDOWN_LEFT  ? MESSAGE_NAVIGATE_LEFT  
				: KEYDOWN_RIGHT ? MESSAGE_NAVIGATE_RIGHT 
				: KEYDOWN_UP   	? MESSAGE_NAVIGATE_UP    
				: KEYDOWN_DOWN	? MESSAGE_NAVIGATE_DOWN 
				: 0x0;
		if(RegionHandler::region && msg){
			RegionHandler::sendMessage(msg | (unsigned)RegionHandler::region);
			return;
		}
	}
	
	/* ------------------------------------------------------------------------
		Control ACTIVATION 
	---------------------------------------------------------------------------
		A
		B
	-------------------------------------------------------------------------*/ 
	if(RegionHandler::control){
		if( ( KEYUP_A || KEYDOWN_A ) && ( !KEYUP_B && !KEYPRESS_B && !KEYDOWN_B ) ) {
			RegionHandler::sendMessage(MESSAGE_ACTIVATE | (unsigned)RegionHandler::control);
		}
	}
	/* ------------------------------------------------------------------------
		Play / Stop / Play from Start
	---------------------------------------------------------------------------
		START
		START
		SELECT + START
	-------------------------------------------------------------------------*/ 
	if( KEYDOWN_START ) {
		if( Sequencer::playing ) {
			Sequencer::stop();
			return;
		}
		
		if( KEYPRESS_SELECT ) {
			Sequencer::play(true);
			return;
		}	
		if( !Sequencer::playing ) 
			Sequencer::play( false );
		return;
	}
		// Handle Copy Command (B+A)
	if( KEYPRESS_B && RegionHandler::control) {
		
		
		if( KEYPRESS_A ) {
			RegionHandler::sendMessage(MESSAGE_PASTE | (unsigned)RegionHandler::control);
			return;		
		}
		
		msg	= KEYDOWN_LEFT 	? MESSAGE_OTHER_PREV 
				: KEYDOWN_RIGHT	? MESSAGE_OTHER_NEXT 
				: KEYDOWN_DOWN 	? MESSAGE_OTHER_BIGPREV
				: KEYDOWN_UP   	? MESSAGE_OTHER_BIGNEXT 
				: 0x00;
		
		if(msg)RegionHandler::sendMessage(msg);
		return;
	}
	
	// Handle control modifiers
	if( KEYPRESS_A ){
		msg  	= KEYDOWN_LEFT 	? MESSAGE_MODIFY_SUB 	
				: KEYDOWN_RIGHT	? MESSAGE_MODIFY_ADD 	
				: KEYDOWN_UP   	? MESSAGE_MODIFY_BIGADD  
				: KEYDOWN_DOWN	? MESSAGE_MODIFY_BIGSUB  
				: 0x0;
				
		if(RegionHandler::control && msg){
			RegionHandler::sendMessage(msg | (unsigned)RegionHandler::control);
			return;
		}
		
		// Handle A Cancel (A, then B)
		if( KEYDOWN_B && RegionHandler::control) {
			RegionHandler::sendMessage(MESSAGE_CANCEL | (unsigned)RegionHandler::control);
			return;
		}
	
	}
		
	// Handle other arbitrary keypresses (MESSAGE_KEYPRESS)
	msg 	= KEYDOWN_LEFT 		? 1 
			: KEYDOWN_RIGHT 	? 1 
			: KEYDOWN_UP    		? 1 
			: KEYDOWN_DOWN  	? 1 
			/* 
			: KEYDOWN_A 			? 1 
			: KEYDOWN_B 			? 1 
			*/	
			: KEYDOWN_SELECT 	? 1 
			: KEYDOWN_START  	? 1 
			: 0x00;
		   
	if(RegionHandler::control && msg ){
		RegionHandler::sendMessage(MESSAGE_KEYPRESS | (unsigned)RegionHandler::control);
	}
	
}

void overloadTest();

void Sys::update(){
	
	//DEBUG_UPDATE();
	
	Sequencer::update();	
	KEYUPDATE();
	updateInput();	
	
	//if( KEYPRESS_L ) overloadTest();
	//if( KEYPRESS_R ) VirtualScreen::draw(14,2);
}

