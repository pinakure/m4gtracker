#ifndef SYS_HPP
#define SYS_HPP

#include "../../agb.h"
#include "../../data/enum.h"

class Sys {
	private:
		static u16 	keyboard;
		static u8  	cursor;
	
	public:		
		static void init();
		static void reset();
		static void update();
		static void updateInput();
		static void setScreen( eScreens screen );
};

extern Sys sys;

#endif 
