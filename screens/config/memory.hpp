#ifndef MEM_H
#define MEM_H

#include "../../agb.h"
#include "../../modules/regionhandler/regionhandler.hpp"

typedef struct{

}t_chunk;

enum E_MEMORY_CONTROLS { 
	CONTROL_MEMORY_MENU5,			CONTROL_MEMORY_PREFETCH,		CONTROL_MEMORY_SLOTUSAGE,		CONTROL_MEMORY_PURGESONGS,
	CONTROL_MEMORY_MEMORYTEST,		CONTROL_MEMORY_REINIT,			CONTROL_MEMORY_RESET,			CONTROL_MEMORY_COUNT,
	CONTROL_MEMORY_MAX	
};

class Memory {
	private:
		
	public:
		static void init	( );
		static void test	( );
		static void get		(u8 blocks);
		static void free	(u16 addr);
		static void update	(  );
};

extern const Callback cb_cfg_prefetch;
extern const Callback cb_cfg_backup;
extern const Callback cb_cfg_revert;
extern const Callback cb_cfg_memorytest;
extern const Callback cb_cfg_format;
extern const Callback cb_cfg_reset;

extern const Control MEMORY_CONTROLS[CONTROL_MEMORY_MAX];

#endif
