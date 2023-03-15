#include "sng.hpp"
#include "ins.hpp"
#include "pat.hpp"
#include "debug.hpp"
#include "../data/data.hpp"
#include "../data/helpers.hpp"
#include "../modules/sram/sram.hpp"
#include "../modules/spu/sequencer.hpp"
#include "../modules/gpu/gpu.hpp"
#include "../modules/key/key.hpp"

#define CTL(a) &SNG_CONTROLS[CONTROL_SNG_##a]
#define VAR(a) ((u8*)&(VAR_SONG.a))

const Control SNG_CONTROLS[ CONTROL_SNG_MAX ] = { 
//	{ x		, y	, up					  		, right						, down				     , left				     , cache								   , var					  	  			, callback				},
	{ 0x0b 	, 0x0f	, CTL( GROOVE_SWITCH	) 	, CTL( GROOVE_01		) 	, CTL( GROOVE_04		) , CTL( SONGSELECTOR	) , &CACHE_HEXADECIMAL_TWOTILES		, VAR( GROOVE.STEP[ 0x0 ] 	) 	, &cb_sng_groove_00 	},
	{ 0x10 	, 0x0f	, CTL( GROOVE_SWITCH	) 	, CTL( GROOVE_02		) 	, CTL( GROOVE_05		) , CTL( GROOVE_00		) , &CACHE_HEXADECIMAL_TWOTILES		, VAR( GROOVE.STEP[ 0x3 ] 	) 	, &cb_sng_groove_03 	},
	{ 0x15 	, 0x0f	, CTL( GROOVE_SWITCH	) 	, CTL( GROOVE_03		) 	, CTL( GROOVE_06		) , CTL( GROOVE_01		) , &CACHE_HEXADECIMAL_TWOTILES		, VAR( GROOVE.STEP[ 0x7 ] 	) 	, &cb_sng_groove_07 	},
	{ 0x1a 	, 0x0f	, CTL( GROOVE_SWITCH	) 	, CTL( SONGSELECTOR	) 	, CTL( GROOVE_07		) , CTL( GROOVE_02		) , &CACHE_HEXADECIMAL_TWOTILES		, VAR( GROOVE.STEP[ 0xB ] 	) 	, &cb_sng_groove_0B 	},
	{ 0x0b 	, 0x10	, CTL( GROOVE_00		) 	, CTL( GROOVE_05		) 	, CTL( GROOVE_08		) , CTL( SONGSELECTOR	) , &CACHE_HEXADECIMAL_TWOTILES		, VAR( GROOVE.STEP[ 0x1 ] 	) 	, &cb_sng_groove_01 	},
	{ 0x10 	, 0x10	, CTL( GROOVE_01		) 	, CTL( GROOVE_06		) 	, CTL( GROOVE_09		) , CTL( GROOVE_04		) , &CACHE_HEXADECIMAL_TWOTILES		, VAR( GROOVE.STEP[ 0x4 ] 	) 	, &cb_sng_groove_04 	},
	{ 0x15 	, 0x10	, CTL( GROOVE_02		) 	, CTL( GROOVE_07		) 	, CTL( GROOVE_0A		) , CTL( GROOVE_05		) , &CACHE_HEXADECIMAL_TWOTILES		, VAR( GROOVE.STEP[ 0x8 ] 	) 	, &cb_sng_groove_08 	},
	{ 0x1a 	, 0x10	, CTL( GROOVE_03		) 	, CTL( SONGSELECTOR	) 	, CTL( GROOVE_0B		) , CTL( GROOVE_06		) , &CACHE_HEXADECIMAL_TWOTILES		, VAR( GROOVE.STEP[ 0xC ] 	) 	, &cb_sng_groove_0C 	},
	{ 0x0b 	, 0x11	, CTL( GROOVE_04		) 	, CTL( GROOVE_09		) 	, CTL( GROOVE_0C		) , CTL( SONGSELECTOR	) , &CACHE_HEXADECIMAL_TWOTILES		, VAR( GROOVE.STEP[ 0x2 ] 	) 	, &cb_sng_groove_02 	},
	{ 0x10 	, 0x11	, CTL( GROOVE_05		) 	, CTL( GROOVE_0A		) 	, CTL( GROOVE_0D		) , CTL( GROOVE_08		) , &CACHE_HEXADECIMAL_TWOTILES		, VAR( GROOVE.STEP[ 0x5 ] 	) 	, &cb_sng_groove_05 	},
	{ 0x15 	, 0x11	, CTL( GROOVE_06		) 	, CTL( GROOVE_0B		) 	, CTL( GROOVE_0E		) , CTL( GROOVE_09		) , &CACHE_HEXADECIMAL_TWOTILES		, VAR( GROOVE.STEP[ 0x9 ] 	) 	, &cb_sng_groove_09 	},
	{ 0x1a 	, 0x11	, CTL( GROOVE_07		) 	, CTL( SONGSELECTOR	) 	, CTL( GROOVE_0F		) , CTL( GROOVE_0A		) , &CACHE_HEXADECIMAL_TWOTILES		, VAR( GROOVE.STEP[ 0xD ] 	) 	, &cb_sng_groove_0D 	},
	{ 0x0b 	, 0x12	, CTL( GROOVE_08		) 	, CTL( GROOVE_0D		) 	, CTL( LOAD				) , CTL( SONGSELECTOR	) , &CACHE_HEXADECIMAL_TWOTILES		, VAR( GROOVE.STEP[ 0x3 ] 	) 	, &cb_sng_groove_03 	},
	{ 0x10 	, 0x12	, CTL( GROOVE_09		) 	, CTL( GROOVE_0E		) 	, CTL( LOAD				) , CTL( GROOVE_0C		) , &CACHE_HEXADECIMAL_TWOTILES		, VAR( GROOVE.STEP[ 0x6 ] 	) 	, &cb_sng_groove_06 	},
	{ 0x15 	, 0x12	, CTL( GROOVE_0A		) 	, CTL( GROOVE_0F		) 	, CTL( LOAD				) , CTL( GROOVE_0D		) , &CACHE_HEXADECIMAL_TWOTILES		, VAR( GROOVE.STEP[ 0xA ] 	) 	, &cb_sng_groove_0A 	},
	{ 0x1a 	, 0x12	, CTL( GROOVE_0B		) 	, CTL( SONGSELECTOR	) 	, CTL( LOAD				) , CTL( GROOVE_0E		) , &CACHE_HEXADECIMAL_TWOTILES		, VAR( GROOVE.STEP[ 0xE ] 	) 	, &cb_sng_groove_0E 	},
	{ 0x04 	, 0x07	, CTL( SONGSELECTOR	) 	, CTL( ARTIST			) 	, CTL( SONGSELECTOR	) , CTL( ARTIST			) , &CACHE_SONGSLOTS					, (u8*)&( VAR_CFG.SLOT	  	) 	, &cb_sng_slot			},
	{ 0x0d 	, 0x01	, CTL( GROOVE_0F		) 	, CTL( SONGSELECTOR	) 	, CTL( SAVE				) , CTL( SONGSELECTOR	) , &CACHE_ARROW_LEFT					, NULL						  			, &cb_sng_load			},
	{ 0x0d 	, 0x02	, CTL( LOAD				) 	, CTL( SONGSELECTOR	) 	, CTL( PURGE				) , CTL( SONGSELECTOR	) , &CACHE_ARROW_LEFT					, NULL						  			, &cb_sng_save			},
	{ 0x0d 	, 0x03	, CTL( SAVE				) 	, CTL( SONGSELECTOR	) 	, CTL( ERASE				) , CTL( SONGSELECTOR	) , &CACHE_ARROW_LEFT					, NULL						  			, &cb_sng_purge			},
	{ 0x0d 	, 0x04	, CTL( PURGE				) 	, CTL( SONGSELECTOR	) 	, CTL( SLOT 				) , CTL( SONGSELECTOR	) , &CACHE_ARROW_LEFT					, NULL						  			, &cb_sng_erase			},
	{ 0x0d 	, 0x05	, CTL( ERASE				) 	, CTL( SONGSELECTOR	) 	, CTL( ARTIST			) , CTL( SONGSELECTOR	) , &CACHE_HEXADECIMAL				, (u8*)&( VAR_CFG.SLOT		)	, &cb_sng_slot			},
	{ 0x0f 	, 0x07	, CTL( SLOT				) 	, CTL( SONGSELECTOR	) 	, CTL( TITLE				) , CTL( SONGSELECTOR	) , &CACHE_TEXT							, VAR( ARTIST					) 	, &cb_sng_artist		},
	{ 0x0f 	, 0x08	, CTL( ARTIST			) 	, CTL( SONGSELECTOR	) 	, CTL( BPM				) , CTL( SONGSELECTOR	) , &CACHE_TEXT							, VAR( TITLE						) 	, &cb_sng_title			},
	{ 0x1c 	, 0x0a	, CTL( BPM				) 	, CTL( SONGSELECTOR	) 	, CTL( TRANSPOSE		) , CTL( SONGSELECTOR	) , &CACHE_ARROW_LEFT					, NULL						  			, &cb_sng_tempotap		},
	{ 0x1c 	, 0x0c	, CTL( TRANSPOSE		) 	, CTL( SONGSELECTOR	) 	, CTL( GROOVE_SWITCH	) , CTL( SONGSELECTOR	) , &CACHE_HEXADECIMAL				, VAR( PATTERNLENGTH			)	, &cb_sng_patlength	},
	{ 0x1c 	, 0x0b	, CTL( TEMPOTAP			) 	, CTL( SONGSELECTOR	) 	, CTL( PATTERNLENGTH	) , CTL( SONGSELECTOR	) , &CACHE_HEXADECIMAL_DOUBLE		, VAR( TRANSPOSE				)	, &cb_sng_transpose	},
	{ 0x1b 	, 0x09	, CTL( TITLE				) 	, CTL( SONGSELECTOR	) 	, CTL( TEMPOTAP			) , CTL( SONGSELECTOR	) , &CACHE_DECIMAL_DOUBLE_TWOTILES	, VAR( BPM						)	, &cb_sng_bpm			},
	{ 0x1c 	, 0x0e	, CTL( PATTERNLENGTH	) 	, CTL( SONGSELECTOR	) 	, CTL( GROOVE_00		) , CTL( GROOVE_0C		) , &CACHE_CHECK						, VAR( GROOVE.ENABLE			)	, &cb_sng_groove		},
	CONTROL_TERMINATOR
};
#undef VAR
#undef CTL

const Callback cb_sng_artist			= { AlphaDialog::getBigString	, EVENT_KEYDOWN_B	, &VAR_SONG.ARTIST			 , NULL };
const Callback cb_sng_bpm				= { SongEdit::setTempo			, EVENT_MODIFY_B	, &VAR_SONG.BPM				 , NULL };
const Callback cb_sng_erase			= { SongEdit::erase				, EVENT_KEYUP_B 	, NULL 						 , NULL };
const Callback cb_sng_groove			= { SongEdit::toggleGroove		, EVENT_KEYDOWN_B	, &VAR_SONG.GROOVE.ENABLE , NULL };
const Callback cb_sng_load			= { SongEdit::load					, EVENT_KEYUP_B 	, NULL 						 , NULL };
const Callback cb_sng_patlength		= { modify4BIT						, EVENT_MODIFY_B	, &VAR_SONG.PATTERNLENGTH , NULL };
const Callback cb_sng_purge			= { SongEdit::purge				, EVENT_KEYUP_B 	, NULL 						 , NULL };
const Callback cb_sng_save			= { SongEdit::save					, EVENT_KEYUP_B 	, NULL 						 , NULL };
const Callback cb_sng_slot			= { SongEdit::select				, EVENT_MODIFY_B	, &VAR_CFG.SLOT				 , NULL };
const Callback cb_sng_tempotap		= { SongEdit::tapTempo			, EVENT_KEYDOWN_B	, NULL							 , NULL };
const Callback cb_sng_title			= { AlphaDialog::getBigString 	, EVENT_KEYDOWN_B	, &VAR_SONG.TITLE			 , NULL };
const Callback cb_sng_transpose		= { modify8BIT						, EVENT_MODIFY_B	, &VAR_SONG.TRANSPOSE		 , NULL };

void SongEdit::toggleGroove(Control *a, bool b, bool c, u32 *d){	
	modify1BIT( a, b, c, d);
	if( VAR_SONG.GROOVE.ENABLE ) return;
	// Erase target ticks so the sequencer can continue playing back
	Channel *ch = &VAR_CHANNEL[ 0 ];
	for(int i=0; i<6; i++){
		ch->target_tick = 0;
		ch++;
	}
}


void SongEdit::redrawCtl(eSngControl control){
	regHnd.drawControl( &SNG_CONTROLS[ control ] );
}

void SongEdit::focusCtl(eSngControl control){
	Control *prior 	= regHnd.control;
	regHnd.control 	= (Control*)&SNG_CONTROLS[ control ];
	if( prior ) regHnd.drawControl( prior );
	regHnd.drawControl( regHnd.control );
}

#define CALLBACK(n, c, t, v, nx)			const Callback n = { c , t , v, nx}
#define CB_SNG_GROOVE(a)						CALLBACK(cb_sng_groove_0##a, modify8BIT	, EVENT_MODIFY_B, &VAR_SONG.GROOVE.STEP[0x##a],	NULL)
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

MEM_IN_EWRAM bool SongEdit::has_data[ SONG_SLOT_COUNT ] = { false, false, false, false, false, false };

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
	if( Sequencer::playing ){
		SongEdit::runGroove();
	}
}
#undef REDRAW

void SongEdit::load( Control *c, bool bigstep, bool add, u32 *pointer ){
	// Set really confirm callback to FlashManager->save
	// Set regionHander in modal really dialog mode
	SRAM.songLoad( true );
	SongEdit::has_data[ 0 ] = false;
	SongEdit::has_data[ 1 ] = false;
	SongEdit::has_data[ 2 ] = false;
	SongEdit::has_data[ 3 ] = false;
	SongEdit::has_data[ 4 ] = false;
	SongEdit::has_data[ 5 ] = false;
	SongEdit::has_data[ VAR_CFG.SLOT ] = true;
	
	regHnd.redraw = true;
}

void SongEdit::save( Control *c, bool bigstep, bool add, u32 *pointer ){
	// Set really confirm callback to FlashManager->save
	// Set regionHander in modal really dialog mode
	SRAM.songSave( true );	
}

void SongEdit::purge( Control *c, bool bigstep, bool add, u32 *pointer ){
	#define INSTRUMENT_COUNT 		0x40
	#define PATTERN_COUNT				0x80
	#define PATTERN_TOTAL 			9216 // 6 SONGS * 6 CHANNELS * 256 places to check 256*6*6
	#define INSTRUMENT_TOTAL 		128016.0f	
	
	float inc								= 0;
	bool 	used_instruments			[ INSTRUMENT_COUNT ];
	bool 	used_patterns				[ PATTERN_COUNT		];
	u8 		unused_instrument_count 	= INSTRUMENT_COUNT;
	u8 		unused_pattern_count 		= PATTERN_COUNT;
	u8 		previous_slot 				= VAR_CFG.SLOT;
	
	ReallyDialog::enable();
	if(!ReallyDialog::result) {
		regHnd.redraw=true;
		return;
	}
	regHnd.redraw=true;
	regHnd.update(0);
	gpu.clear( 0x9010);
	Console console("PLEASE  WAIT", 0x9010);
	
	Gpu::drawDialog( 7 , 6 , 16, 8, " Data Purge ");
	
	KEYFORCENOINPUT();
	
	SRAM.songSave( false );
	
	// Set all items as unused 
	for(int i=1/*ignore first index*/; i<INSTRUMENT_COUNT; i++){
		used_instruments[i]=false;
	}
	
	for(int i=1/*ignore first index*/; i<PATTERN_COUNT; i++){
		used_patterns[i] =false;
	}

	// Check for unused instruments
	Gpu::ascii( 8 << 1 ,	 7	, " Lookup Instruments       %" , COLOR_DARK_CYAN );
	float q = 100.0f / float(PATTERN_COUNT);
	for( int pattern = 0; pattern < PATTERN_COUNT ; pattern++ ){
		DECIMAL_DOUBLE( 20	, 7	, COLOR_CYAN, (u8)( float(pattern) * q  ) );
		for( int instrument = 0 ; instrument < INSTRUMENT_COUNT ; instrument++ ){
			u8 *cell = VAR_DATA[pattern].INS;
			for( u8 *cend = cell + 16; cell < cend; cell++ ){ 
				// dont care about instruments already declared as used
				if( used_instruments[ instrument ] ) continue;
				used_instruments[ *cell] = true;
			}
		}
	}
	/*HACK*/gpu.set(2, 19	, 7	, (COLOR_CYAN<<12) | 0x91);// put 1 on 100...
	/*HACK*/gpu.set(2, 20	, 7	, (COLOR_CYAN<<12) | 0x300);// put 1 on 100...
	
	// Check for unused patterns	
	Gpu::ascii( 8 << 1 ,	 8	, " Lookup Patterns          %" , COLOR_DARK_CYAN );
	inc 	= 0;
	q 		= 100.0f / float(PATTERN_TOTAL);
	
	// FOR EACH SONG
	for( VAR_CFG.SLOT=0; VAR_CFG.SLOT < SONG_SLOT_COUNT; VAR_CFG.SLOT++){
		SRAM.songLoad( false );
		
		// FOR EACH CHANNEL IN SONG
		Pattern*	p_channel 		= VAR_SONG.PATTERNS;
		for( Pattern* p_channel_end = p_channel + 6 ; p_channel < p_channel_end ; p_channel++ ){
			
			// FOR EACH ORDER (PATTERNPOS)
			u8* p_order 		= p_channel->ORDER;			
			for( u8* p_order_end = p_order + 256; p_order < p_order_end ; p_order++){
				DECIMAL_DOUBLE( 20	, 8	, COLOR_CYAN, (u8)( q * inc));
				inc++;
				if( used_patterns[ *p_order ] ) continue;
				used_patterns[ *p_order ] = true;
			}
		}		
	}
	/*HACK*/gpu.set(2, 19	, 8	, (COLOR_CYAN<<12) | 0x91);// put 1 on 100...
	/*HACK*/gpu.set(2, 20	, 8	, (COLOR_CYAN<<12) | 0x300);// put 1 on 100...
	
	// Erase unused instruments
	Gpu::ascii( 8 << 1 ,	 9	, " Erasing Instruments      %" , COLOR_DARK_CYAN );
	for(int i=1; i<INSTRUMENT_COUNT; i++){
		DECIMAL_DOUBLE( 20	, 9	, COLOR_CYAN, (u8)((float(i) / float(INSTRUMENT_COUNT-1)) * 100.0f));
		if( !used_instruments[ i ] ) continue;
		InstEdit::clear( i ); 
		unused_instrument_count--;
	}
	/*HACK*/gpu.set(2, 19	, 9	, (COLOR_CYAN<<12) | 0x91);// put 1 on 100...
	/*HACK*/gpu.set(2, 20	, 9	, (COLOR_CYAN<<12) | 0x300);// put 1 on 100...
	
	// Erase unused patterns
	Gpu::ascii( 8 << 1 ,	10	, " Erasing Patterns         %" , COLOR_DARK_CYAN );
	for(int p=1; p<PATTERN_COUNT; p++){
		DECIMAL_DOUBLE( 20	, 10,COLOR_CYAN, (u8)((float(p) / float(PATTERN_COUNT-1)) * 100.0f));
		if( !used_patterns[ p ] ) continue;
		PatEdit::clear( p ); 
		unused_pattern_count--;
	}
	/*HACK*/gpu.set(2, 19	,10, (COLOR_CYAN<<12) | 0x91);// put 1 on 100...
	/*HACK*/gpu.set(2, 20	,10, (COLOR_CYAN<<12) | 0x300);// put 1 on 100...
	

	Gpu::ascii( 8 << 1 	, 11	, " Avail. Instruments" 	, COLOR_WHITE 	);
	Gpu::ascii( 8 << 1 	, 12	, " Avail. Patterns" 		, COLOR_WHITE 	);
	DECIMAL_DOUBLE( 20	, 11	, COLOR_CYAN, unused_instrument_count	  	);
	gpu.set(2, 19 , 12	, (COLOR_CYAN<<12) | 0x90 + (unused_pattern_count / 100)	);
	DECIMAL_DOUBLE( 20	, 12	, COLOR_CYAN, unused_pattern_count		  	);
	// Save shared data (Instruments and Patterns)
	
	
	SRAM.sharedDataSave( false );
	VAR_CFG.SLOT = previous_slot;
	SRAM.songLoad( false );

	Gpu::bigString( 7, 15, "PRESS ANY BUTTON");
	
	while( !KEYACTIVITY() ){ KEYUPDATE(); };
	regHnd.redraw = 1;
	regHnd.update(1);
	#undef PATTERN_TOTAL
	#undef PATTERN_COUNT
	#undef INSTRUMENT_TOTAL
	#undef INSTRUMENT_COUNT
}

void SongEdit::select( Control *c, bool bigstep, bool add, u32 *pointer ){
	modify6VAL( c, bigstep, add, pointer );
	SongEdit::redrawCtl	( CONTROL_SNG_SONGSELECTOR   );
	SongEdit::focusCtl		( CONTROL_SNG_SONGSELECTOR 	);
	SongEdit::redrawCtl	( CONTROL_SNG_SLOT				);
}

void SongEdit::erase( Control *c, bool bigstep, bool add, u32 *pointer ){
	ReallyDialog::enable();
	if( ReallyDialog::result ){
		VAR_CFG.loadCount = 0;		
		// Clear SongData
		SRAM.songDefaults( true );
		// Load shared data
		SRAM.sharedDataLoad( true );
		// Save empty song
		SRAM.songSave( true );
		/*HACK*/while( KEYDOWN_A || KEYUP_A || KEYPRESS_A ){ KEYUPDATE(); }
	}
	/*HACK*/while( KEYDOWN_B || KEYUP_B || KEYPRESS_B ){ KEYUPDATE(); }
	regHnd.redraw=true;
}

/* Called each time BPM is changed by the control on SNG screen */
void SongEdit::setTempo( Control *c, bool bigstep, bool add, u32 *pointer ){	
	(*(u8*) c->var) = ((*(u8*) c->var) + ((bigstep?0x10:0x1)* (add?1:-1)) ) & 0xFF;
	u8 val = *(c->var);
	if(regHnd.control)
		regHnd.sendMessage(MESSAGE_REDRAW_CONTROL | (unsigned)&SNG_CONTROLS[CONTROL_SNG_BPM]);
	Sequencer::setTempo(val);
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

/* Updates groove table position arrows */
void SongEdit::runGroove(){

	static int last_y = 0;
	static int last_x = 0;

	GrooveTable *groove = &VAR_SONG.GROOVE;

	int y = groove->position % 4;
	int x = (groove->position / 4) * 5;
	if((last_x != x)||(last_y!=y)){
		gpu.set(2, 9 + ( last_x 	 )	, 15 + last_y	, 0x0100 );
		gpu.set(2, 9 + ( x			 )	, 15 + y			, 0x708D );
		last_y = y;
		last_x = x;
	}
}
