#include "adsr.hpp"
#include "../../data/tileset.hpp"
#include "../gpu/gpu.hpp"
#include "../gpu/virtualscreen.hpp"
#include "../regionhandler/regionhandler.hpp"
#include "../../callbacks/debug.hpp"
#include "../../data/instrument.hpp"

u8 		Adsr::last_position						= 0;
u8 		Adsr::last_arrow_position				= 0;
u8 		Adsr::arrow_position						= 0;
bool	Adsr::redraw 								= false;
u16 	Adsr::wav_position							= 0;
u16	Adsr::smp_position							= 0;
u16	Adsr::fmw_position							= 0;
u8 		Adsr::wav_table[4][ ADSR_TABLE_LENGTH ];
u8 		Adsr::fmw_table[4][ ADSR_TABLE_LENGTH ];
u8 		Adsr::smp_table    [ ADSR_TABLE_LENGTH ];

void Adsr::drawX4( u8 adsr_tables[ 4 ][ ADSR_TABLE_LENGTH ] , u8 adsr_position ){
	
	if( regHnd.region != &REGION_MAP_2_INS ) return;
	
	if( ( last_position == adsr_position ) || redraw ) return;
	last_position = adsr_position;
	
	arrow_position = adsr_position>>(1+ADSR_LENGTH_SCALE);

	if( adsr_position < (ADSR_TABLE_LENGTH-2) ){
		
		VirtualScreen::clear();
		
		for( int x = 0 , half = 0, quad = 0	
			; x < ( VIRTUALSCREEN_WIDTH << 1 ) 
			; x+=2 , quad = x << 2, half = x >> 1
		){
			VirtualScreen::set( half ,  7 - ( adsr_tables[ 0 ][ quad ] >> 5  ) );
			VirtualScreen::set( half , 15 - ( adsr_tables[ 1 ][ quad ] >> 5  ) );
			VirtualScreen::set( half , 23 - ( adsr_tables[ 2 ][ quad ] >> 5  ) );
			VirtualScreen::set( half , 31 - ( adsr_tables[ 3 ][ quad ] >> 5  ) );
		}
		//gpu.number(26, 1, ADSR_TABLE_LENGTH, COLOR_RED);

		gpu.set( last_arrow_position 	& 1 ? 2 : 1 , 14 + ( last_arrow_position 	>> 1 ) , 18 , 0x0100 ); 
		gpu.set( arrow_position 			& 1 ? 2 : 1 , 14 + ( arrow_position			>> 1 ) , 18 , 0x711E ); 
		last_arrow_position = arrow_position;
		
		//DECIMAL_DOUBLE(28,1,adsr_position < (ADSR_TABLE_LENGTH-1)?COLOR_YELLOW:COLOR_ORANGE ,adsr_position/100);
		//DECIMAL_DOUBLE(29,1,adsr_position < (ADSR_TABLE_LENGTH-1)?COLOR_YELLOW:COLOR_ORANGE ,adsr_position);
		
		VirtualScreen::draw(14,6);
		
	} 
	
	redraw = false;
}

void Adsr::draw( u8 adsr_table[ ADSR_TABLE_LENGTH ] , u8 adsr_position ){
	if( regHnd.region != &REGION_MAP_2_INS ) return;
	
	if( ( last_position == adsr_position ) || redraw ) return;
	last_position = adsr_position;
	
	arrow_position = adsr_position>>(1+ADSR_LENGTH_SCALE);

	if( adsr_position < (ADSR_TABLE_LENGTH-2) ){

		VirtualScreen::clear();
		
		for( int x = 0 , half = 0, quad = 0	
			; x < ( VIRTUALSCREEN_WIDTH << 1 ) 
			; x+=2 , quad = x << 2, half = x >> 1
		){
			VirtualScreen::set( half ,  31 - ( adsr_table[ quad ] >> 3  ) );
		}
		//gpu.number(26, 1, ADSR_TABLE_LENGTH, COLOR_RED);

		gpu.set( last_arrow_position 	& 1 ? 2 : 1 , 14 + ( last_arrow_position 	>> 1 ) , 18 , 0x0100 ); 
		gpu.set( arrow_position 			& 1 ? 2 : 1 , 14 + ( arrow_position			>> 1 ) , 18 , 0x711E ); 
		last_arrow_position = arrow_position;
		
		//DECIMAL_DOUBLE(28,1,adsr_position < (ADSR_TABLE_LENGTH-1)?COLOR_YELLOW:COLOR_ORANGE ,adsr_position/100);
		//DECIMAL_DOUBLE(29,1,adsr_position < (ADSR_TABLE_LENGTH-1)?COLOR_YELLOW:COLOR_ORANGE ,adsr_position);
		
		VirtualScreen::draw(14,6);
		
	} 
	
	redraw = false;
}

void Adsr::render( u8 adsr[ 4 ], u8 adsr_table[ ADSR_TABLE_LENGTH ] , u8 gate){
	// Scale ADSR values from         
	// 								0x0 ~ 0xF       to       0x00 ~ 0xFF
	#define ATTACK 			( adsr[0] << 2 )  
	#define DECAY  			( adsr[1] << 2 )
	#define SUSTAIN 		( adsr[2] << 2 )
	#define RELEASE 		( adsr[3] << 2 )
	#define TABLE			adsr_table[ position ]
	for( int position=0; position < ADSR_TABLE_LENGTH; position++){
		TABLE = SUSTAIN<<4;
	}
	
	u8 quantum;
	u16 level = ATTACK > 0 ? 0 : ADSR_RANGE-1;
	
	int release_time = ADSR_TABLE_LENGTH - gate;// how many samples are available to render release
	int adsr_quantum 	= (release_time<<2) / (adsr[3]);
	
	for( int position = 0, index = 0; position < ADSR_TABLE_LENGTH; position++ ){

		if( position < ATTACK ){
			// Attack Phase
			quantum   = (ADSR_RANGE<<2) / ATTACK;// Attack will never be 0 here, so no zerodiv danger.
			index		= position<<4;
			level 		= (( quantum * index )>>6) ;	
			
		} else if( position <= ( ATTACK + DECAY ) ){
			
			// Decay Phase
			if(level<0xFF){
				if(level > (gate?(SUSTAIN<<2):0x00)){
					quantum 	= (ADSR_RANGE<<2) / DECAY;
					level -=(quantum>>2);
					// Avoid overflow
					if(level>0xFF)
						level = gate?(SUSTAIN<<2):0x00;
				} else {
					level   = gate?(SUSTAIN<<2):0x00;
				}
			}
			
		}  
		
		if(gate){
			gate--;
		} else {
			// Release Phase
			if( level < 0xFF ){
				if( level > 0 ){
					// level -= (0xF-adsr[3])<<1;//= ( quantum >> 2 );
					level -= (adsr_quantum/(0xF-adsr[3])) >> 2;//(0xF-adsr[3])<<1;//= ( quantum >> 2 );
					// Avoid overflow
					if( level > 0xFF )
						level = 0x00;
				}
			}
		} 
		TABLE = level;
	}
	
	
	#undef ATTACK
	#undef DECAY
	#undef SUSTAIN
	#undef RELEASE
	#undef TABLE
}

void Adsr::updateWav( SETTINGS_WAV *wav ){
	wav_position = 0;
	
	render( wav->OP1_ADSR, wav_table[0] );
	render( wav->OP2_ADSR, wav_table[1] );
	render( wav->OP3_ADSR, wav_table[2] );
	render( wav->OP4_ADSR, wav_table[3] );
	// If we're on instrument editor and displayed instrument is WAV type, display ADSR
	// TODO: Lock to display only instrument being edit
	if		( VAR_INSTRUMENT.TYPE != INSTRUMENT_TYPE_WAV ) return;
	drawX4( Adsr::wav_table, WAV_ADSR_POSITION );
}

void Adsr::updateFmw( SETTINGS_FMW *fmw ){
	fmw_position = 0;
	
	render( fmw->OP1_ADSR, fmw_table[0] );
	render( fmw->OP2_ADSR, fmw_table[1] );
	render( fmw->OP3_ADSR, fmw_table[2] );
	render( fmw->OP4_ADSR, fmw_table[3] );
	// If we're on instrument editor and displayed instrument is FMW type, display ADSR
	// TODO: Lock to display only instrument being edit
	if		( VAR_INSTRUMENT.TYPE != INSTRUMENT_TYPE_FMW ) return;
	drawX4( fmw_table, FMW_ADSR_POSITION );
}

void Adsr::updateSmp( SETTINGS_SMP *smp ){
	smp_position = 0;
		
	render( smp->ADSR, smp_table );
	// If we're on instrument editor and displayed instrument is SMP type, display ADSR
	// TODO: Lock to display only instrument being edit
	if		( VAR_INSTRUMENT.TYPE != INSTRUMENT_TYPE_SMP ) return;
	draw	( smp_table, smp_position );
}
