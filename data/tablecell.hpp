#ifndef TABLECELL_HPP
#define TABLECELL_HPP

#include "../agb.h"

typedef struct sTableCell {
	u8		TRANSPOSE[16] /*8*/;
	u8		VOLUME[16] /*4*/;
	u8		COMMAND[2][16]/*5*/;
	u8		VALUE[2][16]/*8*/;
	u8		JUMP[2]/*4*/;
	u8		POSITION[2]/*4*/;
	bool	PLAYING;	
}TableCell;

#endif
