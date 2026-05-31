#ifndef TABLECELL_HPP
#define TABLECELL_HPP

#include "../agb.h"

#ifndef PATTERN_LENGTH
#define PATTERN_LENGTH	0x10
#endif
#define TABLECELL_SIZE 0x44



class TableCell {
	public BACKUP:
/* 16*5: 80 + 				 */	u8			TRANSPOSE	[ PATTERN_LENGTH 	];
/* 32*4:128 + 				 */	u8			COMMAND		[2][ PATTERN_LENGTH ];
/* 16*4: 64 + 				 */	u8			VOLUME		[ PATTERN_LENGTH 	];
/* 32*8:256 + 				 */	u8			VALUE 		[2][ PATTERN_LENGTH ];
/*  2*4:  8 + 				 */	u8			JUMP		[2];
/*		  8 +				 */ u8			MODE;// 0 : disabled, 1: forward, 2: loop, 3: pingpong
/*	--------- 				 */
/*	    544 bits = 68 (44h) bytes per table */
	
	public TRANSIENT:
		bool	PLAYING;	
		u8		POSITION[2];
		
		void 	read();
		void 	write();
};

#endif
