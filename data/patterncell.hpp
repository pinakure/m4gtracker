#ifndef PATTERNCELL_HPP
#define PATTERNCELL_HPP

#include "../agb.h"

#ifndef PATTERN_LENGTH
#define PATTERN_LENGTH	0x10
#endif
#define ORDER_COUNT				0x100
#define CELL_COUNT				0x80

class PatternCell {
	
	public BACKUP:
			u8 			KEY[ PATTERN_LENGTH ] /*7*/;
			u8			INS[ PATTERN_LENGTH ] /*6*/;
			u8			VOL[ PATTERN_LENGTH ] /*4*/;
			u8			CMD[ PATTERN_LENGTH ] /*5*/;
			u8			VAL[ PATTERN_LENGTH ];
	
	public TRANSIENT:
		u8 				index;	
			
		static void 	seek	( );
		static void 	init	( );
		void 			read	( );
		void 			write	( );
		void 			clear	( );
		static void 	writeAll( );
		static void 	readAll	( );
		static void 	clearAll( );
};

extern PatternCell VAR_DATA	[ CELL_COUNT ];

#endif
