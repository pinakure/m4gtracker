#include "linkmode.hpp" 
#include "../m4g.hpp" 
				
#define THIS(a) 					&LINKMODE_CONTROLS[ CONTROL_LINKMODE_##a ]
#define THISVAR(a) 					((u8*)&( VAR_CFG.LINKMODE.a ))
const Control LINKMODE_CONTROLS[ CONTROL_LINKMODE_MAX ] = { 
//	{	x    , y    , up				, right				, down				, left			, cache						, var						, callback				}
	{	0x04 , 0x04 , NULL				, THIS(MODE		)	, NULL				, NULL			, NULL						, (u8*)&(VAR_CFG.MENUSLOT)	, &cb_cfg_menuindex		},
	{	0x19 , 0x06 , THIS(SEND		)	, THIS(MODE		)	, THIS(MASTER	)	, THIS(MENU2)	, &CACHE_LIVE_MODE			, THISVAR(LINKMODE)			, &cb_cfg_linkmode		},
	{	0x1b , 0x07 , THIS(MODE		)	, THIS(MASTER	)	, THIS(SYNCRATE	)	, THIS(MENU2)	, &CACHE_CHECK 				, THISVAR(MASTERCLOCK)		, &cb_cfg_masterclock	},
	{	0x1b , 0x08 , THIS(MASTER	)	, THIS(SYNCRATE	)	, THIS(SYNCTICKS)	, THIS(MENU2)	, &CACHE_HEXADECIMAL		, THISVAR(SYNCRATE)			, &cb_cfg_syncrate		},
	{	0x1a , 0x09 , THIS(SYNCRATE	)	, THIS(SYNCTICKS)	, THIS(RECEIVE	)	, THIS(MENU2)	, &CACHE_DECIMAL_TWOTILES	, THISVAR(SYNCTICKS)		, &cb_cfg_syncticks		},
	{	0x1b , 0x0a , THIS(SYNCTICKS)	, THIS(RECEIVE	)	, THIS(SEND		)	, THIS(MENU2)	, &CACHE_ARROW_LEFT			, NULL						, &cb_cfg_receivesong	},
	{	0x1b , 0x0b , THIS(RECEIVE	)	, THIS(SEND		)	, THIS(MODE		)	, THIS(MENU2)	, &CACHE_ARROW_LEFT			, NULL						, &cb_cfg_sendsong		},
	CONTROL_TERMINATOR 
};
#undef THISVAR
#undef THIS
