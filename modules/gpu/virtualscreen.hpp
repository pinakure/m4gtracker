#ifndef VIRTUALSCREEN_HPP
#define VIRTUALSCREEN_HPP

#include "../../agb.h"

#define VIRTUALSCREEN_WIDTH 	32
#define VIRTUALSCREEN_HEIGHT 	32

class VirtualScreen {
	private:
	public:
		static u16 data[ (VIRTUALSCREEN_WIDTH * VIRTUALSCREEN_HEIGHT) / 4];
		static u16 *d; 
		static s16 i;
		
		static void init		( );
		static void load		( const bool *dat 	);
		static void clear		( );
		static void set			( u8 ox	, u8 oy	);
		static void unset		( u8 x		, u8 y		);
		static void draw		( u8 ox	, u8 oy	);	
};	

#endif
