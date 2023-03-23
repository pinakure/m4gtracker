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
		static void forceNoInput(); // Same as Key::forceNoInput, but this version allows audio processing to be executed while waiting for the keys to be released
};

extern Sys sys;

#endif 
