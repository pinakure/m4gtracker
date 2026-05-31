#include "../config.hpp"
#include "../../debug.hpp"
#include "../../data/data.hpp"

const Callback cb_cfg_prefetch		= { modify1BIT					, EVENT_KEYDOWN_B	, &VAR_CFG.MEMORY.PREF	 		, NULL };
const Callback cb_cfg_powersave		= { modify1BIT					, EVENT_KEYUP_B		, &VAR_CFG.BEHAVIOR.POWERSAVING	, NULL };
const Callback cb_cfg_revert		= { Config::revert				, EVENT_KEYUP_B		, NULL 							, NULL };
const Callback cb_cfg_memorytest	= { Debug::memoryTest			, EVENT_KEYUP_B		, NULL 							, NULL };
const Callback cb_cfg_format		= { Config::format 				, EVENT_KEYUP_B		, NULL 							, NULL };
const Callback cb_cfg_reset			= { Config::reset				, EVENT_KEYUP_B		, NULL 							, NULL };

#define CTL(a) &MEMORY_CONTROLS[CONTROL_MEMORY_##a]
#define VAR(a) ((u8*)&(VAR_CFG.MEMORY.a))
const Control MEMORY_CONTROLS[CONTROL_MEMORY_MAX] = { 
//  						{ 	x    , y  	, up				 , right			 , down				 , left				 , cache				, var						 				, callback
/* MENU5				*/	{	0x04 , 0x04 , NULL				 , CTL( PREFETCH	), NULL				 , NULL				 , NULL					, (u8*)&( VAR_CFG.MENUSLOT  			)	, &Config::menuindex		}, 
/* prefetch				*/	{	0x1b , 0x06 , CTL( RESET		), CTL( PREFETCH	), CTL( POWERSAVE	), CTL( MENU5		), &CACHE_CHECK			, VAR( PREF								)	, &cb_cfg_prefetch			}, 
/* power saving 		*/	{	0x1b , 0x07 , CTL( PREFETCH		), CTL( POWERSAVE	), CTL( PURGESONGS	), CTL( MENU5		), &CACHE_CHECK			, (u8*)&( VAR_CFG.BEHAVIOR.POWERSAVING	)	, &cb_cfg_powersave			}, 
/* purgesongs -> revert	*/	{	0x1b , 0x08 , CTL( POWERSAVE	), CTL( PURGESONGS	), CTL( MEMORYTEST	), CTL( MENU5		), &CACHE_ARROW_LEFT	, NULL						 				, &cb_cfg_revert			}, 
/* memorytest			*/	{	0x1b , 0x09 , CTL( PURGESONGS	), CTL( MEMORYTEST	), CTL( REINIT		), CTL( MENU5		), &CACHE_ARROW_LEFT	, NULL						 				, &cb_cfg_memorytest		}, 
/* reinit				*/	{	0x1b , 0x0a , CTL( MEMORYTEST	), CTL( REINIT		), CTL( RESET		), CTL( MENU5		), &CACHE_ARROW_LEFT	, NULL						 				, &cb_cfg_format			}, 
/* reset				*/	{	0x1b , 0x0b , CTL( REINIT		), CTL( RESET		), CTL( PREFETCH	), CTL( MENU5		), &CACHE_ARROW_LEFT	, NULL						 				, &cb_cfg_reset			}, 
CONTROL_TERMINATOR
};
#undef VAR
#undef CTL

void Memory::init(){
	
}

void Memory::test(){
	u8* 	test8	=	(  u8* ) malloc ( sizeof (  u8 ) * 256 );
	u16* 	test16	=	( u16* ) malloc ( sizeof ( u16 ) * 256 );
	u32* 	test32	=	( u32* ) malloc ( sizeof ( u32 ) * 256 );
	int x=0;
	
	for(x=0;x<256;x++)
	{
		test8[x]  = 0xCC;
		test16[x] = 0xCAFE;
		test32[x] = 0xDEADBABE;
	}

	for(x=0;x<256;x++){
		//Gpu::PrintF(0,0, " ADDR8=%p  ADDR16=%p      ADDR32=%p  ",test8, test16, test32);
		//Gpu::set(BG_TEXT, TILESET_HEXADECIMAL + test8[x], x%30, 2+(x/30), 0x6); 
	}
	/*
	Memory::free(test8);
	Memory::free(test16);
	Memory::free(test32);
	*/
}

void Memory::get(u8 blocks){
}

void Memory::free(u16 addr){
}

void Memory::update(  ){
	// REGION Callback for Memory Submenu in CFG Screen
}
