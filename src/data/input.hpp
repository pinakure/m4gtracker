#ifndef INPUT_HPP
#define INPUT_HPP

#include "../agb.h"

typedef struct sInput {
	u8			A;	
	u8			B;	
	u8			L;	
	u8			R;	
	u8			SELECT;	
	u8			START;	
	u8			UP;	
	u8			DOWN;	
	u8			LEFT;	
	u8			RIGHT;	
} Input;

extern Input VAR_INPUT; // Input mirror

extern u8 VAR_KEY[4];

#endif
