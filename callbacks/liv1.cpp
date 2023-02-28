/* ----------------------------------------------------------------------------
AUTHOR		 	Al P.Area ( Smiker )
PURPOSE			Callback routines for the controls at LIVE1 screen and related 
				update routines and helpers.
ORIGINAL DATE 	2016, October
REVISION DATE 	2023-02-28
 --------------------------------------------------------------------------- */
#include "callbacks.hpp"
#include "../data/displays.hpp"
#include "../data/enum.h"
#include "../data/variables.hpp"
#include "../modules/sys/sys.hpp"
#include "../modules/key/key.hpp"
#include "../modules/regionhandler/regionhandler.hpp"

#define CALLBACK(n, c, t, v, nx)			const Callback n = { c , t , v, nx}
CALLBACK( cb_live_retrig	, modify1Bit		, EVENT_KEYDOWN_B 		, &live.perform.retrig		, NULL					);
CALLBACK( cb_live_speed	 	, modify8Bit		, EVENT_MODIFY_B 		, &live.perform.speed		, NULL					);
CALLBACK( cb_live_quantize1	, modify3Bit		, EVENT_MODIFY_B 		, &live.perform.quantize	, NULL					);
#define CB_LIVE_LEFT(n, i, m)		CALLBACK( cb_live_left_##n##_0##i, 	m, EVENT_MODIFY_B, &live.perform.left.n[i], NULL)
#define CB_LIVE_RIGHT(n, i, m)		CALLBACK( cb_live_right_##n##_0##i, m, EVENT_MODIFY_B, &live.perform.right.n[i], NULL)
CB_LIVE_LEFT(val, 0, modifyValue);
CB_LIVE_LEFT(val, 1, modifyValue);
CB_LIVE_LEFT(val, 2, modifyValue);
CB_LIVE_LEFT(val, 3, modifyValue);
CB_LIVE_LEFT(val, 4, modifyValue);
CB_LIVE_LEFT(val, 5, modifyValue);
CB_LIVE_LEFT(val, 6, modifyValue);
CB_LIVE_LEFT(val, 7, modifyValue);

CB_LIVE_LEFT(vol, 0, modifyVolume);
CB_LIVE_LEFT(vol, 1, modifyVolume);
CB_LIVE_LEFT(vol, 2, modifyVolume);
CB_LIVE_LEFT(vol, 3, modifyVolume);
CB_LIVE_LEFT(vol, 4, modifyVolume);
CB_LIVE_LEFT(vol, 5, modifyVolume);
CB_LIVE_LEFT(vol, 6, modifyVolume);
CB_LIVE_LEFT(vol, 7, modifyVolume);

CB_LIVE_LEFT(ins, 0, modifyInst);
CB_LIVE_LEFT(ins, 1, modifyInst);
CB_LIVE_LEFT(ins, 2, modifyInst);
CB_LIVE_LEFT(ins, 3, modifyInst);
CB_LIVE_LEFT(ins, 4, modifyInst);
CB_LIVE_LEFT(ins, 5, modifyInst);
CB_LIVE_LEFT(ins, 6, modifyInst);
CB_LIVE_LEFT(ins, 7, modifyInst);

CB_LIVE_LEFT(chan, 0, modify6Val);
CB_LIVE_LEFT(chan, 1, modify6Val);
CB_LIVE_LEFT(chan, 2, modify6Val);
CB_LIVE_LEFT(chan, 3, modify6Val);
CB_LIVE_LEFT(chan, 4, modify6Val);
CB_LIVE_LEFT(chan, 5, modify6Val);
CB_LIVE_LEFT(chan, 6, modify6Val);
CB_LIVE_LEFT(chan, 7, modify6Val);

CB_LIVE_LEFT(key, 0, modifyNote);
CB_LIVE_LEFT(key, 1, modifyNote);
CB_LIVE_LEFT(key, 2, modifyNote);
CB_LIVE_LEFT(key, 3, modifyNote);
CB_LIVE_LEFT(key, 4, modifyNote);
CB_LIVE_LEFT(key, 5, modifyNote);
CB_LIVE_LEFT(key, 6, modifyNote);
CB_LIVE_LEFT(key, 7, modifyNote);

CB_LIVE_LEFT(cmd, 0, modifyCommand);
CB_LIVE_LEFT(cmd, 1, modifyCommand);
CB_LIVE_LEFT(cmd, 2, modifyCommand);
CB_LIVE_LEFT(cmd, 3, modifyCommand);
CB_LIVE_LEFT(cmd, 4, modifyCommand);
CB_LIVE_LEFT(cmd, 5, modifyCommand);
CB_LIVE_LEFT(cmd, 6, modifyCommand);
CB_LIVE_LEFT(cmd, 7, modifyCommand);

CB_LIVE_RIGHT(val, 0, modifyValue);
CB_LIVE_RIGHT(val, 1, modifyValue);
CB_LIVE_RIGHT(val, 2, modifyValue);
CB_LIVE_RIGHT(val, 3, modifyValue);
CB_LIVE_RIGHT(val, 4, modifyValue);
CB_LIVE_RIGHT(val, 5, modifyValue);
CB_LIVE_RIGHT(val, 6, modifyValue);
CB_LIVE_RIGHT(val, 7, modifyValue);

CB_LIVE_RIGHT(cmd, 0, modifyCommand);
CB_LIVE_RIGHT(cmd, 1, modifyCommand);
CB_LIVE_RIGHT(cmd, 2, modifyCommand);
CB_LIVE_RIGHT(cmd, 3, modifyCommand);
CB_LIVE_RIGHT(cmd, 4, modifyCommand);
CB_LIVE_RIGHT(cmd, 5, modifyCommand);
CB_LIVE_RIGHT(cmd, 6, modifyCommand);
CB_LIVE_RIGHT(cmd, 7, modifyCommand);

CB_LIVE_RIGHT(chan, 0, modify6Val);
CB_LIVE_RIGHT(chan, 1, modify6Val);
CB_LIVE_RIGHT(chan, 2, modify6Val);
CB_LIVE_RIGHT(chan, 3, modify6Val);
CB_LIVE_RIGHT(chan, 4, modify6Val);
CB_LIVE_RIGHT(chan, 5, modify6Val);
CB_LIVE_RIGHT(chan, 6, modify6Val);
CB_LIVE_RIGHT(chan, 7, modify6Val);

CB_LIVE_RIGHT(ins, 0, modifyInst);
CB_LIVE_RIGHT(ins, 1, modifyInst);
CB_LIVE_RIGHT(ins, 2, modifyInst);
CB_LIVE_RIGHT(ins, 3, modifyInst);
CB_LIVE_RIGHT(ins, 4, modifyInst);
CB_LIVE_RIGHT(ins, 5, modifyInst);
CB_LIVE_RIGHT(ins, 6, modifyInst);
CB_LIVE_RIGHT(ins, 7, modifyInst);

CB_LIVE_RIGHT(vol, 0, modifyVolume);
CB_LIVE_RIGHT(vol, 1, modifyVolume);
CB_LIVE_RIGHT(vol, 2, modifyVolume);
CB_LIVE_RIGHT(vol, 3, modifyVolume);
CB_LIVE_RIGHT(vol, 4, modifyVolume);
CB_LIVE_RIGHT(vol, 5, modifyVolume);
CB_LIVE_RIGHT(vol, 6, modifyVolume);
CB_LIVE_RIGHT(vol, 7, modifyVolume);

CB_LIVE_RIGHT(key, 0, modifyNote);
CB_LIVE_RIGHT(key, 1, modifyNote);
CB_LIVE_RIGHT(key, 2, modifyNote);
CB_LIVE_RIGHT(key, 3, modifyNote);
CB_LIVE_RIGHT(key, 4, modifyNote);
CB_LIVE_RIGHT(key, 5, modifyNote);
CB_LIVE_RIGHT(key, 6, modifyNote);
CB_LIVE_RIGHT(key, 7, modifyNote);
#undef CB_LIVE_LEFT
#undef CB_LIVE_RIGHT

#undef CALLBACK

void updateLive1(){
	if(KEYDOWN_START){
		REGHND::sendMessage(MESSAGE_REDRAW_DISPLAY | (unsigned)(&REGHND::region->displays[LIVE1_DISPLAY_STATUS_FREE])&0x0fffffff);
		live.perform.lock ^= 1;
		REGHND::sendMessage(MESSAGE_REDRAW_DISPLAY | (unsigned)(&REGHND::region->displays[LIVE1_DISPLAY_STATUS_LOCKED])&0x0fffffff);
	}
	
	if(KEYDOWN_START || KEYUP_START){
		VAR_INPUT.START = KEYDOWN_START ? 1 : 0;
		REGHND::sendMessage(MESSAGE_REDRAW_DISPLAY | (unsigned)(&REGHND::region->displays[LIVE1_DISPLAY_START])&0x0fffffff);
	}

	if(KEYDOWN_SELECT || KEYUP_SELECT){
		VAR_INPUT.SELECT = KEYDOWN_SELECT ? 1 : 0;
		REGHND::sendMessage(MESSAGE_REDRAW_DISPLAY | (unsigned)(&REGHND::region->displays[LIVE1_DISPLAY_SELECT])&0x0fffffff);
	}
	
#define MONITOR(a)		if( KEYDOWN_##a || KEYUP_##a ) {	\
						VAR_INPUT.a = KEYDOWN_##a ? 1 : 0;	\
						REGHND::sendMessage(MESSAGE_REDRAW_DISPLAY | (unsigned)(&REGHND::region->displays[LIVE1_DISPLAY_RIGHT_##a])&0x0fffffff); \
						REGHND::sendMessage(MESSAGE_REDRAW_DISPLAY | (unsigned)(&REGHND::region->displays[LIVE1_DISPLAY_LEFT_##a])&0x0fffffff) ; \
					}

	MONITOR(A);
	MONITOR(B);
	MONITOR(L);
	MONITOR(R);

	MONITOR(UP) 
	else MONITOR(RIGHT) 
	else MONITOR(DOWN) 
	else MONITOR(LEFT);
	
#undef MONITOR
	
	// We must send signals from this function, any time a note is pressed / released,
	// and always when live.perform.LOCK = 1
	// If quantize is on, signals must be sent fixing their tick to desired quant time
		
}	

