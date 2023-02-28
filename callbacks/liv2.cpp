/* ----------------------------------------------------------------------------
AUTHOR		 	Al P.Area ( Smiker )
PURPOSE			Callback routines for the controls at LIVE2 screen and related 
				update routines and helpers.
ORIGINAL DATE 	2016, October
REVISION DATE 	2023-02-28
 --------------------------------------------------------------------------- */
#include "callbacks.hpp"
#include "../data/displays.hpp"
#include "../data/variables.hpp"
#include "../modules/regionhandler/regionhandler.hpp"




#define CALLBACK(n, c, t, v, nx)			const Callback n = { c , t , v, nx}
CALLBACK( cb_live_chan1		, modify6Val		, EVENT_MODIFY_B		, &VAR_LIVE.PIANO.CHANNEL[0]	, NULL);
CALLBACK( cb_live_chan2		, modify6Val		, EVENT_MODIFY_B		, &VAR_LIVE.PIANO.CHANNEL[1]	, NULL);
CALLBACK( cb_live_mode		, modify3Val		, EVENT_MODIFY_B		, &VAR_LIVE.PIANO.MODE			, NULL);
CALLBACK( cb_live_octave	, modify3Bit		, EVENT_MODIFY_B		, &VAR_LIVE.PIANO.OCTAVE		, NULL);
CALLBACK( cb_live_transpose , modify8Bit		, EVENT_MODIFY_B		, &VAR_LIVE.PIANO.TRANSPOSE		, NULL);
CALLBACK( cb_live_quantize2 , modify3Bit		, EVENT_MODIFY_B		, &VAR_LIVE.PIANO.QUANTIZE		, NULL);
CALLBACK( cb_live_midichan1	, modify4Bit		, EVENT_MODIFY_B		, &VAR_LIVE.PIANO.MIDICHAN[0]	, NULL);
CALLBACK( cb_live_midichan2	, modify4Bit		, EVENT_MODIFY_B		, &VAR_LIVE.PIANO.MIDICHAN[1]	, NULL);
#undef CALLBACK




#define REDRAW(a)	REGHND::sendMessage(MESSAGE_REDRAW_DISPLAY | (unsigned)(&REGHND::region->displays[a])&0x0fffffff);
void updateLive2(){
	static u8 lastValue = VAR_LIVE.PIANO.MODE;
	if(VAR_LIVE.PIANO.MODE != lastValue){
		lastValue = VAR_LIVE.PIANO.MODE;
		REDRAW(LIVE2_DISPLAY_KEYLAYOUT_A);
		REDRAW(LIVE2_DISPLAY_KEYLAYOUT_B);
		REDRAW(LIVE2_DISPLAY_KEYLAYOUT_C);
	}
}
#undef REDRAW

