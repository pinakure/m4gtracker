CALLBACK( cb_live_retrig	, modify1BIT		, EVENT_KEYDOWN_B 		, &VAR_LIVE.PERFORM.RETRIG		, NULL					);
CALLBACK( cb_live_speed	 	, modify8BIT		, EVENT_MODIFY_B 		, &VAR_LIVE.PERFORM.SPEED		, NULL					);
CALLBACK( cb_live_quantize1	, modify3BIT		, EVENT_MODIFY_B 		, &VAR_LIVE.PERFORM.QUANTIZE	, NULL					);
#define CB_LIVE_LEFT(n, i, m)		CALLBACK( cb_live_left_##n##_0##i, 	m, EVENT_MODIFY_B, &VAR_LIVE.PERFORM.LEFT.n[i], NULL)
#define CB_LIVE_RIGHT(n, i, m)		CALLBACK( cb_live_right_##n##_0##i, m, EVENT_MODIFY_B, &VAR_LIVE.PERFORM.RIGHT.n[i], NULL)
CB_LIVE_LEFT(VAL, 0, modifyValue);
CB_LIVE_LEFT(VAL, 1, modifyValue);
CB_LIVE_LEFT(VAL, 2, modifyValue);
CB_LIVE_LEFT(VAL, 3, modifyValue);
CB_LIVE_LEFT(VAL, 4, modifyValue);
CB_LIVE_LEFT(VAL, 5, modifyValue);
CB_LIVE_LEFT(VAL, 6, modifyValue);
CB_LIVE_LEFT(VAL, 7, modifyValue);

CB_LIVE_LEFT(VOL, 0, modifyVolume);
CB_LIVE_LEFT(VOL, 1, modifyVolume);
CB_LIVE_LEFT(VOL, 2, modifyVolume);
CB_LIVE_LEFT(VOL, 3, modifyVolume);
CB_LIVE_LEFT(VOL, 4, modifyVolume);
CB_LIVE_LEFT(VOL, 5, modifyVolume);
CB_LIVE_LEFT(VOL, 6, modifyVolume);
CB_LIVE_LEFT(VOL, 7, modifyVolume);

CB_LIVE_LEFT(INS, 0, modifyInst);
CB_LIVE_LEFT(INS, 1, modifyInst);
CB_LIVE_LEFT(INS, 2, modifyInst);
CB_LIVE_LEFT(INS, 3, modifyInst);
CB_LIVE_LEFT(INS, 4, modifyInst);
CB_LIVE_LEFT(INS, 5, modifyInst);
CB_LIVE_LEFT(INS, 6, modifyInst);
CB_LIVE_LEFT(INS, 7, modifyInst);

CB_LIVE_LEFT(CHAN, 0, modify6VAL);
CB_LIVE_LEFT(CHAN, 1, modify6VAL);
CB_LIVE_LEFT(CHAN, 2, modify6VAL);
CB_LIVE_LEFT(CHAN, 3, modify6VAL);
CB_LIVE_LEFT(CHAN, 4, modify6VAL);
CB_LIVE_LEFT(CHAN, 5, modify6VAL);
CB_LIVE_LEFT(CHAN, 6, modify6VAL);
CB_LIVE_LEFT(CHAN, 7, modify6VAL);

CB_LIVE_LEFT(KEY, 0, modifyNote);
CB_LIVE_LEFT(KEY, 1, modifyNote);
CB_LIVE_LEFT(KEY, 2, modifyNote);
CB_LIVE_LEFT(KEY, 3, modifyNote);
CB_LIVE_LEFT(KEY, 4, modifyNote);
CB_LIVE_LEFT(KEY, 5, modifyNote);
CB_LIVE_LEFT(KEY, 6, modifyNote);
CB_LIVE_LEFT(KEY, 7, modifyNote);

CB_LIVE_LEFT(CMD, 0, modifyCommand);
CB_LIVE_LEFT(CMD, 1, modifyCommand);
CB_LIVE_LEFT(CMD, 2, modifyCommand);
CB_LIVE_LEFT(CMD, 3, modifyCommand);
CB_LIVE_LEFT(CMD, 4, modifyCommand);
CB_LIVE_LEFT(CMD, 5, modifyCommand);
CB_LIVE_LEFT(CMD, 6, modifyCommand);
CB_LIVE_LEFT(CMD, 7, modifyCommand);

CB_LIVE_RIGHT(VAL, 0, modifyValue);
CB_LIVE_RIGHT(VAL, 1, modifyValue);
CB_LIVE_RIGHT(VAL, 2, modifyValue);
CB_LIVE_RIGHT(VAL, 3, modifyValue);
CB_LIVE_RIGHT(VAL, 4, modifyValue);
CB_LIVE_RIGHT(VAL, 5, modifyValue);
CB_LIVE_RIGHT(VAL, 6, modifyValue);
CB_LIVE_RIGHT(VAL, 7, modifyValue);

CB_LIVE_RIGHT(CMD, 0, modifyCommand);
CB_LIVE_RIGHT(CMD, 1, modifyCommand);
CB_LIVE_RIGHT(CMD, 2, modifyCommand);
CB_LIVE_RIGHT(CMD, 3, modifyCommand);
CB_LIVE_RIGHT(CMD, 4, modifyCommand);
CB_LIVE_RIGHT(CMD, 5, modifyCommand);
CB_LIVE_RIGHT(CMD, 6, modifyCommand);
CB_LIVE_RIGHT(CMD, 7, modifyCommand);

CB_LIVE_RIGHT(CHAN, 0, modify6VAL);
CB_LIVE_RIGHT(CHAN, 1, modify6VAL);
CB_LIVE_RIGHT(CHAN, 2, modify6VAL);
CB_LIVE_RIGHT(CHAN, 3, modify6VAL);
CB_LIVE_RIGHT(CHAN, 4, modify6VAL);
CB_LIVE_RIGHT(CHAN, 5, modify6VAL);
CB_LIVE_RIGHT(CHAN, 6, modify6VAL);
CB_LIVE_RIGHT(CHAN, 7, modify6VAL);

CB_LIVE_RIGHT(INS, 0, modifyInst);
CB_LIVE_RIGHT(INS, 1, modifyInst);
CB_LIVE_RIGHT(INS, 2, modifyInst);
CB_LIVE_RIGHT(INS, 3, modifyInst);
CB_LIVE_RIGHT(INS, 4, modifyInst);
CB_LIVE_RIGHT(INS, 5, modifyInst);
CB_LIVE_RIGHT(INS, 6, modifyInst);
CB_LIVE_RIGHT(INS, 7, modifyInst);

CB_LIVE_RIGHT(VOL, 0, modifyVolume);
CB_LIVE_RIGHT(VOL, 1, modifyVolume);
CB_LIVE_RIGHT(VOL, 2, modifyVolume);
CB_LIVE_RIGHT(VOL, 3, modifyVolume);
CB_LIVE_RIGHT(VOL, 4, modifyVolume);
CB_LIVE_RIGHT(VOL, 5, modifyVolume);
CB_LIVE_RIGHT(VOL, 6, modifyVolume);
CB_LIVE_RIGHT(VOL, 7, modifyVolume);

CB_LIVE_RIGHT(KEY, 0, modifyNote);
CB_LIVE_RIGHT(KEY, 1, modifyNote);
CB_LIVE_RIGHT(KEY, 2, modifyNote);
CB_LIVE_RIGHT(KEY, 3, modifyNote);
CB_LIVE_RIGHT(KEY, 4, modifyNote);
CB_LIVE_RIGHT(KEY, 5, modifyNote);
CB_LIVE_RIGHT(KEY, 6, modifyNote);
CB_LIVE_RIGHT(KEY, 7, modifyNote);
#undef CB_LIVE_LEFT
#undef CB_LIVE_RIGHT

void updateLIVE1(){
	if(KEYDOWN_START){
		REGHND::sendMessage(MESSAGE_REDRAW_DISPLAY | (unsigned)(&REGHND::region->displays[LIVE1_DISPLAY_STATUS_FREE])&0x0fffffff);
		VAR_LIVE.PERFORM.LOCK ^= 1;
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
	// and always when VAR_LIVE.PERFORM.LOCK = 1
	// If quantize is on, signals must be sent fixing their tick to desired quant time
		
}	

