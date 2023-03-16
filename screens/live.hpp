#ifndef LIVE_HPP
#define LIVE_HPP

#include "../agb.h"

#include "live/piano.hpp"
#include "live/performance.hpp"

class Live {
	public:
		MEM_IN_EWRAM static Performance		PERFORM;
		MEM_IN_EWRAM static Piano			PIANO;
		MEM_IN_EWRAM static u8	 			KEYPRESS[10];

};

#endif 
