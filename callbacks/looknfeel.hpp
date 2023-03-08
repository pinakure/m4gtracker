#ifndef LOOKNFEEL_HPP
#define LOOKNFEEL_HPP

#include "../agb.h"

#include "../modules/regionhandler/regionhandler.hpp"

class LookNFeel {
	public:
		static void init	( );
		static void logoWait( );
		static void logoFade( );
		static void update	( RegionHandler* rh );
		static void credits	( RegionHandler* rh );
		
};

#endif
