#ifndef INSTRUMENT_HPP
#define INSTRUMENT_HPP

#include "../agb.h"
#include "tablecell.hpp"

#define SETTINGS_SIZE			32
#define INSTRUMENT_COUNT 		0x40 
#define INSTRUMENT_SIZE 		0x6D
#define INSTRUMENT_TOTAL_SIZE 	( INSTRUMENT_SIZE * INSTRUMENT_COUNT ) 
#define INSTRUMENT_NAME_SIZE 	6

class Instrument {
	public BACKUP:
/*	6*8: 48 + 				 */	char 		NAME		[ INSTRUMENT_NAME_SIZE ];
/*		  8 + 				 */	u8 			TYPE;
/* 32x8:256 + 				 */	u8			SETTINGS	[ SETTINGS_SIZE ];
/*		544 + 				 */	TableCell	TABLE;

/*	--------- 				 */
/*	    872 bits = 109 (6D) bytes */
	
	public TRANSIENT:
		u8				VISPOSITION	[2];
		u8 				index;
	
		static void 	seek 	( );
		static void 	init	( );
		void 			read	( ); // Read from SRAM
		void 			write	( ); // Write to SRAM
		void 			clear	( ); 
		static void 	writeAll( );
		static void 	readAll	( );
		static void 	clearAll( );
		
} ;

extern Instrument VAR_INSTRUMENTS[ INSTRUMENT_COUNT ];
extern Instrument VAR_INSTRUMENT; // Current instrument MUST be operated from here

#endif
