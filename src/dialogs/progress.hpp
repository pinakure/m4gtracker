#ifndef PROGRESS_HPP
#define PROGRESS_HPP

#include "../agb.h"
#include "../data/enum.h"

class Progress {
	private:
		static u32 				max;
		static u32 				current;
		static u8 					step;
		static u8 					laststep;
		static E_StatusStrings 	upperLine;
		static E_StatusStrings 	lowerLine;
		static bool 				enabled;
		static u8 					redraw;
		static u32*				var;
	
	public:
		static bool update	( );		
		static void disable   ( );
		static void enable	( u32 value, u32 maxvalue, E_StatusStrings uLine, E_StatusStrings lLine, u32 *var );
};

#endif
