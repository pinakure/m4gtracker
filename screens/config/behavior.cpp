#include "../config.hpp"
#include "../../data/data.hpp"

const Callback cb_cfg_autoload		= { modify1BIT					, EVENT_KEYDOWN_B	, &VAR_CFG.BEHAVIOR.AUTOLOAD	, NULL };
const Callback cb_cfg_autosave		= { modify1BIT					, EVENT_KEYDOWN_B	, &VAR_CFG.BEHAVIOR.AUTOSAVE	, NULL };
const Callback cb_cfg_keyrate		= { modify4BIT					, EVENT_MODIFY_B	, &VAR_CFG.BEHAVIOR.KEYRATE		, NULL };
const Callback cb_cfg_saveconfig	= { Config::save				, EVENT_KEYUP_B		, NULL 							, NULL };
const Callback cb_cfg_loadconfig	= { Config::load				, EVENT_KEYUP_B		, NULL 							, NULL };
const Callback cb_cfg_initconfig	= { Config::defaults			, EVENT_KEYUP_B		, NULL 							, NULL };

#define CTL(a) &BEHAVIOR_CONTROLS[CONTROL_BEHAVIOR_##a]
#define VAR(a) ((u8*)&(VAR_CFG.BEHAVIOR.a))
const Control BEHAVIOR_CONTROLS[CONTROL_BEHAVIOR_MAX] = { 
//				{	x	 , y     , up					 , right				 , down					 , left					 , cache				, var						 , callback
/* MENU3	*/ 	{	0x04 , 0x04	, NULL					 , CTL( AUTOLOAD		), NULL					 , NULL					 , NULL					, (u8*)&(VAR_CFG.MENUSLOT	), &cb_cfg_menuindex	}, 
/* AUTOLOAD	*/	{	0x1b , 0x06	, CTL( DEFAULTS			), CTL( AUTOLOAD		), CTL( AUTOSAVE		), CTL( MENU3			), &CACHE_CHECK			, VAR( AUTOLOAD				), &cb_cfg_autoload	 	}, 
/* autosave	*/	{	0x1b , 0x07	, CTL( AUTOLOAD			), CTL( AUTOSAVE		), CTL( KEYRATE			), CTL( MENU3			), &CACHE_CHECK			, VAR( AUTOSAVE				), &cb_cfg_autosave		}, 
/* keyrate	*/	{	0x1b , 0x08	, CTL( AUTOSAVE			), CTL( KEYRATE			), CTL( SAVE			), CTL( MENU3			), &CACHE_HEXADECIMAL	, VAR( KEYRATE				), &cb_cfg_keyrate		}, 
/* save		*/	{	0x1b , 0x09	, CTL( KEYRATE			), CTL( SAVE			), CTL( LOAD			), CTL( MENU3			), &CACHE_ARROW_LEFT	, NULL						 , &cb_cfg_saveconfig	}, 
/* load		*/	{	0x1b , 0x0a	, CTL( SAVE 			), CTL( LOAD			), CTL( DEFAULTS		), CTL( MENU3			), &CACHE_ARROW_LEFT	, NULL						 , &cb_cfg_loadconfig	}, 
/* defaults	*/	{	0x1b , 0x0b	, CTL( LOAD 			), CTL( DEFAULTS		), CTL( AUTOLOAD		), CTL( MENU3			), &CACHE_ARROW_LEFT	, NULL						 , &cb_cfg_initconfig	}, 
				CONTROL_TERMINATOR
};
#undef VAR
#undef CTL


void Behavior::update(  ){
	
}
