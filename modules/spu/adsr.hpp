#ifndef ADSR_HPP
#define ADSR_HPP

#include "../../agb.h"

//#define FM_ADSR_SCALE 				3
//#define ADSR_LENGTH_SCALE 		0
//#define ADSR_RANGE_SCALE 			0

#define FM_ADSR_SCALE 				1
#define ADSR_LENGTH_SCALE 			1+FM_ADSR_SCALE

//#define ADSR_RANGE_SCALE 			0
#define ADSR_TABLE_SIZE				0x40 // DO NOT CHANGE THIS VALUE, CHANGE SCALE INSTEAD
#define ADSR_RANGE 					0xF0
#define ADSR_TABLE_LENGTH 			( ADSR_TABLE_SIZE << ADSR_LENGTH_SCALE )

#define WAV_ADSR_POSITION 			( Synth::wav_adsr_position >> FM_ADSR_SCALE )
#define FMW_ADSR_POSITION 			( Synth::fmw_adsr_position >> FM_ADSR_SCALE )
#define SMP_ADSR_POSITION 			( Synth::smp_adsr_position >> FM_ADSR_SCALE )

class Adsr {
	
	private:
		static u8 	last_position;
		static u8 	last_arrow_position;
		static u8 	arrow_position;
	
	public:
		static bool	redraw;

		static void drawX4	( u8 adsr_tables[ 4 ][ ADSR_TABLE_LENGTH ] , u8 adsr_position );
		static void draw	( u8 adsr_table[ ADSR_TABLE_LENGTH ] , u8 adsr_position );
	
};

#endif 
