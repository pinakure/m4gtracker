#ifndef MEM_H
#define MEM_H

#include "../../agb.h"

class Mem {
	private:
		
	public:
		static void init	( );
		static void test	( );
		static void get		(u8 blocks);
		static void free	(u16 addr);
};

#endif
