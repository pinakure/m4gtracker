#ifndef GROOVETABLE_HPP
#define GROOVETABLE_HPP

#include "../agb.h"

#define GROOVE_TABLE_LENGTH	0x10
#define GROOVE_TABLE_SIZE	(1 + GROOVE_TABLE_LENGTH )

class GrooveTable {
	public BACKUP:
		u8				STEP[ GROOVE_TABLE_LENGTH ];/* 8 x GROOVE_TABLE_LENGTH bits */
		u8				ENABLE;						/* 4 bits 						*/
		u8				length;						/* 4 bits 						*/
	public TRANSIENT:
		u8				position; // TRANSIENT
		
		static void 	seek	( );
		void 			read	( );
		void 			write	( );
		void 			clear	( );
};
#endif 
