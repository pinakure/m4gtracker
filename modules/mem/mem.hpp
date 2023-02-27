#ifndef __MODULES_MEM
#define __MODULES_MEM

#include "../../agb.h"

class MEM {
	public:
		static void init();
		static void test();
		static void Get(u8 blocks);
		static void Free(u16 addr);
};

#endif
