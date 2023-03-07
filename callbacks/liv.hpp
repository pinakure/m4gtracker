#ifndef LIV_HPP
#define LIV_HPP

#include "../agb.h"

class LiveSet {
	public:
	
		static u8 	last_piano_value;
	
		static void updateCmder(RegionHandler* rh);
		static void updatePiano(RegionHandler* rh);
};

#endif
