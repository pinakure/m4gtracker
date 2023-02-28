CALLBACK( cb_live_chan1		, modify6VAL		, EVENT_MODIFY_B		, &VAR_LIVE.PIANO.CHANNEL[0]	, NULL);
CALLBACK( cb_live_chan2		, modify6VAL		, EVENT_MODIFY_B		, &VAR_LIVE.PIANO.CHANNEL[1]	, NULL);
CALLBACK( cb_live_mode		, modify3VAL		, EVENT_MODIFY_B		, &VAR_LIVE.PIANO.MODE			, NULL);
CALLBACK( cb_live_octave	, modify3BIT		, EVENT_MODIFY_B		, &VAR_LIVE.PIANO.OCTAVE		, NULL);
CALLBACK( cb_live_transpose , modify8BIT		, EVENT_MODIFY_B		, &VAR_LIVE.PIANO.TRANSPOSE		, NULL);
CALLBACK( cb_live_quantize2 , modify3BIT		, EVENT_MODIFY_B		, &VAR_LIVE.PIANO.QUANTIZE		, NULL);
CALLBACK( cb_live_midichan1	, modify4BIT		, EVENT_MODIFY_B		, &VAR_LIVE.PIANO.MIDICHAN[0]	, NULL);
CALLBACK( cb_live_midichan2	, modify4BIT		, EVENT_MODIFY_B		, &VAR_LIVE.PIANO.MIDICHAN[1]	, NULL);

#define REDRAW(a)	REGHND::sendMessage(MESSAGE_REDRAW_DISPLAY | (unsigned)(&REGHND::region->displays[a])&0x0fffffff);
void updateLIVE2(){
	static u8 lastValue = VAR_LIVE.PIANO.MODE;
	if(VAR_LIVE.PIANO.MODE != lastValue){
		lastValue = VAR_LIVE.PIANO.MODE;
		REDRAW(LIVE2_DISPLAY_KEYLAYOUT_A);
		REDRAW(LIVE2_DISPLAY_KEYLAYOUT_B);
		REDRAW(LIVE2_DISPLAY_KEYLAYOUT_C);
	}
}
#undef REDRAW

