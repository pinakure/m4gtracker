/* ----------------------------------------------------------------------------
AUTHOR		 	Al P.Area ( Smiker )
PURPOSE			Callback routines for the controls at SONG screen and related
				update routines and helpers.
ORIGINAL DATE 	2016, October
REVISION DATE 	2023-02-28
 --------------------------------------------------------------------------- */
#include "callbacks.hpp"
#include "../data/enum.h"
#include "../data/variables.hpp"
#include "../data/controls.hpp"
#include "../modules/spu/spu.hpp"
#include "../modules/regionhandler/regionhandler.hpp"
#include "../modules/sram/sram.hpp"

#define CALLBACK(n, c, t, v, nx)			const Callback n = { c , t , v, nx}
CALLBACK( cb_sng_load		, loadSlot		, EVENT_KEYUP_B 			, NULL 					, NULL 	);
CALLBACK( cb_sng_save		, saveSlot		, EVENT_KEYUP_B 			, NULL 					, NULL 	);
CALLBACK( cb_sng_purge		, purgeSlot		, EVENT_KEYUP_B 			, NULL 					, NULL 	);
CALLBACK( cb_sng_copy		, copySlot		, EVENT_KEYUP_B 			, NULL 					, NULL 	);
CALLBACK( cb_sng_erase		, eraseSlot		, EVENT_KEYUP_B 			, NULL 					, NULL 	);
CALLBACK( cb_sng_slot		, modify6Val	, EVENT_MODIFY_B			, &CFG::slot			, NULL	);
CALLBACK( cb_sng_artist		, ALPHA14		, EVENT_KEYDOWN_B			, &song.artist			, NULL	);
CALLBACK( cb_sng_title		, ALPHA14		, EVENT_KEYDOWN_B			, &song.title			, NULL	);
CALLBACK( cb_sng_tempotap	, tempoTap		, EVENT_KEYDOWN_B			, NULL					, NULL	);
CALLBACK( cb_sng_patlength	, modify4Bit	, EVENT_MODIFY_B			, &song.pattern_length	, NULL	);
CALLBACK( cb_sng_transpose	, modify8Bit	, EVENT_MODIFY_B			, &song.transpose		, NULL	);
CALLBACK( cb_sng_bpm		, modifyBPM		, EVENT_MODIFY_B			, &song.bpm				, NULL	);
CALLBACK( cb_sng_groove		, modify1Bit	, EVENT_KEYDOWN_B			, &song.groove.enable	, NULL	);
#define CB_SNG_GROOVE(a)	CALLBACK(cb_sng_groove_0##a, modify4Bit	, EVENT_MODIFY_B			, &song.groove.step[0x##a],	NULL)
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
#undef CALLBACK 

static void copystr(u8 *s, u8 *d, u8 len){
	for(int i=0; i<len; i++) s[i] = d[i];	
}

static int compstr(u8 *a, u8*b, u8 len){
	for(int i=0; i<len; i++) if(a[i] != b[i]) return 1;
	return 0;
}

void updateSng(){
	const Control *ct = &SNG_CONTROLS[CONTROL_SNG_TITLE];
	const Control *ca = &SNG_CONTROLS[CONTROL_SNG_ARTIST];
	
	if(compstr( (ca->var), (u8*)song.artist, 14)){
		copystr( (ca->var), (u8*)song.artist, 14);
	}
	STRING(true, ca->x, ca->y, ca->var);
	if(compstr( (ct->var), (u8*)song.title, 14)){
		copystr( (ct->var), (u8*)song.title, 14);
	}
	STRING(true, ct->x, ct->y, ct->var);
}

void loadSlot(Control *c, bool bigstep, bool add, u32 *pointer){
	// Set really confirm callback to FlashManager->save
	// Set regionHander in modal really dialog mode
	SRAM::songLoad();
	REGHND::redraw = true;
}
void saveSlot(Control *c, bool bigstep, bool add, u32 *pointer){
	// Set really confirm callback to FlashManager->save
	// Set regionHander in modal really dialog mode
	SRAM::songSave();	
}
void purgeSlot(Control *c, bool bigstep, bool add, u32 *pointer){
	
}
void copySlot(Control *c, bool bigstep, bool add, u32 *pointer){
	
}
void eraseSlot(Control *c, bool bigstep, bool add, u32 *pointer){
	ReallyDialog r;
	
	r.enable();
	if(r.result){
		CFG::load_count = 0;		
		// Clear SongData
		SRAM::songDefaults();		
		// Load shared data
		SRAM::sharedDataLoad();		
		// Save empty song
		SRAM::songSave();
	}
	REGHND::redraw=true;
}

/* Called each time BPM is changed by the control on SNG screen */
void modifyBPM(Control *c, bool bigstep, bool add, u32 *pointer){	
	(*(u8*) c->var) = ((*(u8*) c->var) + ((bigstep?0x10:0x1)* (add?1:-1)) ) & 0xFF;
	( (REGHND*)pointer )->sendMessage(MESSAGE_REDRAW_CONTROL | (unsigned)&SNG_CONTROLS[CONTROL_SNG_BPM]);
	SPU::setTempo(*(c->var));
}

/* Calculates new a new BPM Values each 4th time user calls this function based on previous 3 calls */
void tempoTap(Control *c, bool bigstep, bool add, u32 *pointer){
	
	SPU::jumpToPatternAsync( VAR_PATTERN[ CFG::current_channel ].order[ channel[ CFG::current_channel ].position ] );
	return;
	
	//TODO: WRITE PROPER TEMPO TAP FUNCTION WHICH DOES NOT TURN BPM CRAZY!
	//(u8*)&( song.tap_ticks )
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
		SPU::setTempo(int(100 * ((float(t) * 100) / BPM_MAGIC) ));
		// Send message to redraw BPM control too ;)
		( (REGHND*)pointer )->sendMessage(MESSAGE_REDRAW_CONTROL | (unsigned)&SNG_CONTROLS[CONTROL_SNG_BPM]);
		calc = false;
	}*/
}


