#include "sng.hpp"
#include "../data/data.hpp"
#include "../data/helpers.hpp"
#include "../modules/sram/sram.hpp"
#include "../modules/spu/sequencer.hpp"

#define CTL(a) &SNG_CONTROLS[CONTROL_SNG_##a]
#define VAR(a) ((u8*)&(VAR_SONG.a))
const Control SNG_CONTROLS[ CONTROL_SNG_MAX ] = { 
//	{  x	, y		, up					  , right				  , down				  , left				  , cache							, var					  	  , callback			},
	{ 0x0b 	, 0x0f	, CTL( GROOVE_SWITCH	) , CTL( GROOVE_01		) , CTL( GROOVE_04		) , CTL( SONGSELECTOR	) , &CACHE_HEXADECIMAL_TWOTILES		, VAR( GROOVE.STEP	 [0x00] ) , &cb_sng_groove_00 	},
	{ 0x10 	, 0x0f	, CTL( GROOVE_SWITCH	) , CTL( GROOVE_02		) , CTL( GROOVE_05		) , CTL( GROOVE_00		) , &CACHE_HEXADECIMAL_TWOTILES		, VAR( GROOVE.STEP	 [0x01] ) , &cb_sng_groove_01 	},
	{ 0x15 	, 0x0f	, CTL( GROOVE_SWITCH	) , CTL( GROOVE_03		) , CTL( GROOVE_06		) , CTL( GROOVE_01		) , &CACHE_HEXADECIMAL_TWOTILES		, VAR( GROOVE.STEP	 [0x02] ) , &cb_sng_groove_02 	},
	{ 0x1a 	, 0x0f	, CTL( GROOVE_SWITCH	) , CTL( SONGSELECTOR	) , CTL( GROOVE_07		) , CTL( GROOVE_02		) , &CACHE_HEXADECIMAL_TWOTILES		, VAR( GROOVE.STEP	 [0x03] ) , &cb_sng_groove_03 	},
	{ 0x0b 	, 0x10	, CTL( GROOVE_00		) , CTL( GROOVE_05		) , CTL( GROOVE_08		) , CTL( SONGSELECTOR	) , &CACHE_HEXADECIMAL_TWOTILES		, VAR( GROOVE.STEP	 [0x04] ) , &cb_sng_groove_04 	},
	{ 0x10 	, 0x10	, CTL( GROOVE_01		) , CTL( GROOVE_06		) , CTL( GROOVE_09		) , CTL( GROOVE_04		) , &CACHE_HEXADECIMAL_TWOTILES		, VAR( GROOVE.STEP	 [0x05] ) , &cb_sng_groove_05 	},
	{ 0x15 	, 0x10	, CTL( GROOVE_02		) , CTL( GROOVE_07		) , CTL( GROOVE_0A		) , CTL( GROOVE_05		) , &CACHE_HEXADECIMAL_TWOTILES		, VAR( GROOVE.STEP	 [0x06] ) , &cb_sng_groove_06 	},
	{ 0x1a 	, 0x10	, CTL( GROOVE_03		) , CTL( SONGSELECTOR	) , CTL( GROOVE_0B		) , CTL( GROOVE_06		) , &CACHE_HEXADECIMAL_TWOTILES		, VAR( GROOVE.STEP	 [0x07] ) , &cb_sng_groove_07 	},
	{ 0x0b 	, 0x11	, CTL( GROOVE_04		) , CTL( GROOVE_09		) , CTL( GROOVE_0C		) , CTL( SONGSELECTOR	) , &CACHE_HEXADECIMAL_TWOTILES		, VAR( GROOVE.STEP	 [0x08] ) , &cb_sng_groove_08 	},
	{ 0x10 	, 0x11	, CTL( GROOVE_05		) , CTL( GROOVE_0A		) , CTL( GROOVE_0D		) , CTL( GROOVE_08		) , &CACHE_HEXADECIMAL_TWOTILES		, VAR( GROOVE.STEP	 [0x09] ) , &cb_sng_groove_09 	},
	{ 0x15 	, 0x11	, CTL( GROOVE_06		) , CTL( GROOVE_0B		) , CTL( GROOVE_0E		) , CTL( GROOVE_09		) , &CACHE_HEXADECIMAL_TWOTILES		, VAR( GROOVE.STEP	 [0x10] ) , &cb_sng_groove_0A 	},
	{ 0x1a 	, 0x11	, CTL( GROOVE_07		) , CTL( SONGSELECTOR	) , CTL( GROOVE_0F		) , CTL( GROOVE_0A		) , &CACHE_HEXADECIMAL_TWOTILES		, VAR( GROOVE.STEP	 [0x11] ) , &cb_sng_groove_0B 	},
	{ 0x0b 	, 0x12	, CTL( GROOVE_08		) , CTL( GROOVE_0D		) , CTL( LOAD			) , CTL( SONGSELECTOR	) , &CACHE_HEXADECIMAL_TWOTILES		, VAR( GROOVE.STEP	 [0x12] ) , &cb_sng_groove_0C 	},
	{ 0x10 	, 0x12	, CTL( GROOVE_09		) , CTL( GROOVE_0E		) , CTL( LOAD			) , CTL( GROOVE_0C		) , &CACHE_HEXADECIMAL_TWOTILES		, VAR( GROOVE.STEP	 [0x13] ) , &cb_sng_groove_0D 	},
	{ 0x15 	, 0x12	, CTL( GROOVE_0A		) , CTL( GROOVE_0F		) , CTL( LOAD			) , CTL( GROOVE_0D		) , &CACHE_HEXADECIMAL_TWOTILES		, VAR( GROOVE.STEP	 [0x14] ) , &cb_sng_groove_0E 	},
	{ 0x1a 	, 0x12	, CTL( GROOVE_0B		) , CTL( SONGSELECTOR	) , CTL( LOAD			) , CTL( GROOVE_0E		) , &CACHE_HEXADECIMAL_TWOTILES		, VAR( GROOVE.STEP	 [0x15] ) , &cb_sng_groove_0F 	},
	{ 0x04 	, 0x07	, CTL( SONGSELECTOR		) , CTL( ARTIST			) , CTL( SONGSELECTOR	) , CTL( ARTIST			) , &CACHE_SONGSLOTS				, (u8*)&( VAR_CFG.SLOT		) , &cb_sng_slot		},
	{ 0x0d 	, 0x01	, CTL( GROOVE_0F		) , CTL( SONGSELECTOR	) , CTL( SAVE			) , CTL( SONGSELECTOR	) , &CACHE_ARROW_LEFT				, NULL						  , &cb_sng_load		},
	{ 0x0d 	, 0x02	, CTL( LOAD				) , CTL( SONGSELECTOR	) , CTL( PURGE			) , CTL( SONGSELECTOR	) , &CACHE_ARROW_LEFT				, NULL						  , &cb_sng_save		},
	{ 0x0d 	, 0x03	, CTL( SAVE				) , CTL( SONGSELECTOR	) , CTL( COPY			) , CTL( SONGSELECTOR	) , &CACHE_ARROW_LEFT				, NULL						  , &cb_sng_purge		},
	{ 0x0d 	, 0x04	, CTL( PURGE			) , CTL( SONGSELECTOR	) , CTL( ERASE			) , CTL( SONGSELECTOR	) , &CACHE_ARROW_LEFT				, NULL						  , &cb_sng_copy		},
	{ 0x0d 	, 0x05	, CTL( COPY				) , CTL( SONGSELECTOR	) , CTL( ARTIST			) , CTL( SONGSELECTOR	) , &CACHE_ARROW_LEFT				, NULL						  , &cb_sng_erase		},
	{ 0x0f 	, 0x07	, CTL( ERASE			) , CTL( SONGSELECTOR	) , CTL( TITLE			) , CTL( SONGSELECTOR	) , &CACHE_TEXT						, VAR( ARTIST				) , &cb_sng_artist		},
	{ 0x0f 	, 0x08	, CTL( ARTIST			) , CTL( SONGSELECTOR	) , CTL( BPM			) , CTL( SONGSELECTOR	) , &CACHE_TEXT						, VAR( TITLE				) , &cb_sng_title		},
	{ 0x1c 	, 0x0a	, CTL( BPM				) , CTL( SONGSELECTOR	) , CTL( TRANSPOSE		) , CTL( SONGSELECTOR	) , &CACHE_ARROW_LEFT				, NULL						  , &cb_sng_tempotap	},
	{ 0x1c 	, 0x0c	, CTL( TRANSPOSE		) , CTL( SONGSELECTOR	) , CTL( GROOVE_SWITCH	) , CTL( SONGSELECTOR	) , &CACHE_HEXADECIMAL				, VAR( PATTERNLENGTH		) , &cb_sng_patlength	},
	{ 0x1c 	, 0x0b	, CTL( TEMPOTAP			) , CTL( SONGSELECTOR	) , CTL( PATTERNLENGTH	) , CTL( SONGSELECTOR	) , &CACHE_HEXADECIMAL_DOUBLE		, VAR( TRANSPOSE			) , &cb_sng_transpose	},
	{ 0x1b 	, 0x09	, CTL( TITLE			) , CTL( SONGSELECTOR	) , CTL( TEMPOTAP		) , CTL( SONGSELECTOR	) , &CACHE_DECIMAL_DOUBLE_TWOTILES	, VAR( BPM					) , &cb_sng_bpm			},
	{ 0x1c 	, 0x0e	, CTL( PATTERNLENGTH	) , CTL( SONGSELECTOR	) , CTL( GROOVE_00		) , CTL( SONGSELECTOR	) , &CACHE_CHECK					, VAR( GROOVE.ENABLE		) , &cb_sng_groove		},
	CONTROL_TERMINATOR
};
#undef VAR
#undef CTL

const Callback cb_sng_load		= { SongEdit::load		, EVENT_KEYUP_B 	, NULL 						, NULL };
const Callback cb_sng_save		= { SongEdit::save		, EVENT_KEYUP_B 	, NULL 						, NULL };
const Callback cb_sng_purge		= { SongEdit::purge		, EVENT_KEYUP_B 	, NULL 						, NULL };
const Callback cb_sng_copy		= { SongEdit::copy		, EVENT_KEYUP_B 	, NULL 						, NULL };
const Callback cb_sng_erase		= { SongEdit::erase		, EVENT_KEYUP_B 	, NULL 						, NULL };
const Callback cb_sng_slot		= { SongEdit::select	, EVENT_MODIFY_B	, &VAR_CFG.SLOT				, NULL };
const Callback cb_sng_artist	= { ALPHA14				, EVENT_KEYDOWN_B	, &VAR_SONG.ARTIST			, NULL };
const Callback cb_sng_title		= { ALPHA14				, EVENT_KEYDOWN_B	, &VAR_SONG.TITLE			, NULL };
const Callback cb_sng_tempotap	= { SongEdit::tapTempo	, EVENT_KEYDOWN_B	, NULL						, NULL };
const Callback cb_sng_patlength	= { modify4BIT			, EVENT_MODIFY_B	, &VAR_SONG.PATTERNLENGTH	, NULL };
const Callback cb_sng_transpose	= { modify8BIT			, EVENT_MODIFY_B	, &VAR_SONG.TRANSPOSE		, NULL };
const Callback cb_sng_bpm		= { SongEdit::setTempo	, EVENT_MODIFY_B	, &VAR_SONG.BPM				, NULL };
const Callback cb_sng_groove	= { modify1BIT			, EVENT_KEYDOWN_B	, &VAR_SONG.GROOVE.ENABLE	, NULL };

#define CALLBACK(n, c, t, v, nx)			const Callback n = { c , t , v, nx}
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



