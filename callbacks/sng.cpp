#include "sng.hpp"

CALLBACK( cb_sng_load		, SongEdit::load		, EVENT_KEYUP_B 			, NULL 						, NULL 	);
CALLBACK( cb_sng_save		, SongEdit::save		, EVENT_KEYUP_B 			, NULL 						, NULL 	);
CALLBACK( cb_sng_purge		, SongEdit::purge		, EVENT_KEYUP_B 			, NULL 						, NULL 	);
CALLBACK( cb_sng_copy		, SongEdit::copy		, EVENT_KEYUP_B 			, NULL 						, NULL 	);
CALLBACK( cb_sng_erase		, SongEdit::erase		, EVENT_KEYUP_B 			, NULL 						, NULL 	);
CALLBACK( cb_sng_slot		, SongEdit::select		, EVENT_MODIFY_B			, &VAR_CFG.SLOT				, NULL	);
CALLBACK( cb_sng_artist		, ALPHA14				, EVENT_KEYDOWN_B			, &VAR_SONG.ARTIST			, NULL	);
CALLBACK( cb_sng_title		, ALPHA14				, EVENT_KEYDOWN_B			, &VAR_SONG.TITLE			, NULL	);
CALLBACK( cb_sng_tempotap	, SongEdit::tapTempo	, EVENT_KEYDOWN_B			, NULL						, NULL	);
CALLBACK( cb_sng_patlength	, modify4BIT			, EVENT_MODIFY_B			, &VAR_SONG.PATTERNLENGTH	,NULL	);
CALLBACK( cb_sng_transpose	, modify8BIT			, EVENT_MODIFY_B			, &VAR_SONG.TRANSPOSE		, NULL	);
CALLBACK( cb_sng_bpm		, SongEdit::setTempo	, EVENT_MODIFY_B			, &VAR_SONG.BPM				, NULL	);
CALLBACK( cb_sng_groove		, modify1BIT			, EVENT_KEYDOWN_B			, &VAR_SONG.GROOVE.ENABLE	, NULL	);
#define CB_SNG_GROOVE(a)	CALLBACK(cb_sng_groove_0##a, modify4BIT	, EVENT_MODIFY_B, &VAR_SONG.GROOVE.STEP[0x##a],	NULL)
CB_SNG_GROOVE(0);
CB_SNG_GROOVE(1);
CB_SNG_GROOVE(2);
CB_SNG_GROOVE(3);
CB_SNG_GROOVE(4);
CB_SNG_GROOVE(5);
CB_SNG_GROOVE(6);
CB_SNG_GROOVE(7);
CB_SNG_GROOVE(8);
CB_SNG_GROOVE(9);
CB_SNG_GROOVE(A);
CB_SNG_GROOVE(B);
CB_SNG_GROOVE(C);
CB_SNG_GROOVE(D);
CB_SNG_GROOVE(E);
CB_SNG_GROOVE(F);
#undef CG_SNG_GROOVE

#define REDRAW(a)			rh->sendMessage(MESSAGE_REDRAW_DISPLAY | (unsigned)(&rh->region->displays[a])&0x0fffffff);

static void copystr(u8 *s, u8 *d, u8 len){
	for(int i=0; i<len; i++) s[i] = d[i];	
}

static int compstr(u8 *a, u8*b, u8 len){
	for(int i=0; i<len; i++) if(a[i] != b[i]) return 1;
	return 0;
}

void SongEdit::update( RegionHandler* rh ){
	const Control *ct = &SNG_CONTROLS[CONTROL_SNG_TITLE];
	const Control *ca = &SNG_CONTROLS[CONTROL_SNG_ARTIST];
	
	if(compstr( (ca->var), (u8*)VAR_SONG.ARTIST, 14)){
		copystr( (ca->var), (u8*)VAR_SONG.ARTIST, 14);
	}
	STRING(true, ca->x, ca->y, ca->var);
	if(compstr( (ct->var), (u8*)VAR_SONG.TITLE, 14)){
		copystr( (ct->var), (u8*)VAR_SONG.TITLE, 14);
	}
	STRING(true, ct->x, ct->y, ct->var);
}
#undef REDRAW

void SongEdit::load( Control *c, bool bigstep, bool add, u32 *pointer ){
	// Set really confirm callback to FlashManager->save
	// Set regionHander in modal really dialog mode
	SRAM.songLoad( true );
	regHnd.redraw = true;
}

void SongEdit::save( Control *c, bool bigstep, bool add, u32 *pointer ){
	// Set really confirm callback to FlashManager->save
	// Set regionHander in modal really dialog mode
	SRAM.songSave( true );	
}

void SongEdit::purge( Control *c, bool bigstep, bool add, u32 *pointer ){
	
}

void SongEdit::select( Control *c, bool bigstep, bool add, u32 *pointer ){
	modify6VAL( c, bigstep, add, pointer );
	// get current selected index 
}

void SongEdit::copy( Control *c, bool bigstep, bool add, u32 *pointer ){
	
}

void SongEdit::erase( Control *c, bool bigstep, bool add, u32 *pointer ){
	ReallyDialog r;
	
	r.enable();
	if( r.result ){
		VAR_CFG.loadCount = 0;		
		// Clear SongData
		SRAM.songDefaults( true );
		// Load shared data
		SRAM.sharedDataLoad( true );
		// Save empty song
		SRAM.songSave( true );
	}
	regHnd.redraw=true;
}

/* Called each time BPM is changed by the control on SNG screen */
void SongEdit::setTempo( Control *c, bool bigstep, bool add, u32 *pointer ){	
	(*(u8*) c->var) = ((*(u8*) c->var) + ((bigstep?0x10:0x1)* (add?1:-1)) ) & 0xFF;
	( (RegionHandler*)pointer )->sendMessage(MESSAGE_REDRAW_CONTROL | (unsigned)&SNG_CONTROLS[CONTROL_SNG_BPM]);
	Sequencer::setTempo(*(c->var));
}

/* Calculates new a new BPM Values each 4th time user calls this function based on previous 3 calls */
void SongEdit::tapTempo( Control *c, bool bigstep, bool add, u32 *pointer ){
	
	Sequencer::jumpToPatternAsync(VAR_PATTERN[VAR_CFG.CURRENTCHANNEL].ORDER[VAR_CHANNEL[VAR_CFG.CURRENTCHANNEL].POSITION]);
	return;
	
	//TODO: WRITE PROPER TEMPO TAP FUNCTION WHICH DOES NOT TURN BPM CRAZY!
	//(u8*)&(VAR_SONG.TAPTICKS)
	/*
	static bool calc = false;
	if(!calc) {
		//Reset timer3
		*((volatile u16*)0x0400010E) = 0x0;    //Disable (bit 7)
		*((volatile u16*)0x0400010C) = 0x0000; //Set counter to 0
		*((volatile u16*)0x0400010E) = 0x82;   //Enable (bit 7)		
		calc = true;
	} else {
		int t = *((volatile u16*)0x0400010C);
		Sequencer::setTempo(int(100 * ((float(t) * 100) / BPM_MAGIC) ));
		// Send message to redraw BPM control too ;)
		( (RegionHandler*)pointer )->sendMessage(MESSAGE_REDRAW_CONTROL | (unsigned)&SNG_CONTROLS[CONTROL_SNG_BPM]);
		calc = false;
	}*/
}



