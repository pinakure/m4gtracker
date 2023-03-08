#ifndef SYS_HPP
#define SYS_HPP

#include "../../agb.h"

class Sys {
	private:
	
	public:
		bool var_reset;
		
		u16 keyboard;
		u8  cursor;
		
		Sys(){ cursor= 0x00; keyboard = 0x0000;};
		
		void init();
		void reset();
		void update();
		void updateInput();
};

extern Sys sys;

#endif 
