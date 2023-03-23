#ifndef MEM_H
#define MEM_H

#include "../../agb.h"
#include "../regionhandler/regionhandler.hpp"

class Mem {
	private:
		
	public:
		static void init	( );
		static void test	( );
		static void get		(u8 blocks);
		static void free	(u16 addr);
		static void update	( RegionHandler* rh );
};

#endif
