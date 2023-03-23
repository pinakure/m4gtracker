#ifndef ADSR_HPP
#define ADSR_HPP

#include "../../agb.h"
#include "../../data/settings.hpp"

//#define FM_ADSR_SCALE 				3
//#define ADSR_LENGTH_SCALE 		0
//#define ADSR_RANGE_SCALE 			0

#define FM_ADSR_SCALE 				1
#define ADSR_LENGTH_SCALE 			1+FM_ADSR_SCALE

//#define ADSR_RANGE_SCALE 			0
#define ADSR_TABLE_SIZE				0x40 // DO NOT CHANGE THIS VALUE, CHANGE SCALE INSTEAD
#define ADSR_RANGE 					0xF0
#define ADSR_TABLE_LENGTH 			( ADSR_TABLE_SIZE << ADSR_LENGTH_SCALE )

#define WAV_ADSR_POSITION 			( Adsr::wav_position >> FM_ADSR_SCALE )
#define FMW_ADSR_POSITION 			( Adsr::fmw_position >> FM_ADSR_SCALE )
#define SMP_ADSR_POSITION 			( Adsr::smp_position >> FM_ADSR_SCALE )

class Adsr {
	
	private:
		static u8 	last_position;
		static u8 	last_arrow_position;
		static u8 	arrow_position;
	
	public:
		static u16 	wav_position;
		static u16 	smp_position;
		static u16 	fmw_position;
		static u8 	wav_table[4][ ADSR_TABLE_LENGTH ];
		static u8 	fmw_table[4][ ADSR_TABLE_LENGTH ];
		static u8 	smp_table    [ ADSR_TABLE_LENGTH ];

		static bool	redraw;

		static void drawX4			( u8 adsr_tables[ 4 ][ ADSR_TABLE_LENGTH ] 	, u8 adsr_position );
		static void draw			( u8 adsr_table[ ADSR_TABLE_LENGTH ] 			, u8 adsr_position );
		static void render			( u8 adsr[ 4 ]										, u8 adsr_table[ ADSR_TABLE_LENGTH ] , u8 gate=0x80);
		static void updateSmp		( SETTINGS_SMP *smp );
		static void updateFmw		( SETTINGS_FMW *fmw );
		static void updateWav		( SETTINGS_WAV *wav );

};

#endif 
