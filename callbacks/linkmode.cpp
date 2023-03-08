#include "linkmode.hpp" 
#include "../data/data.hpp" 
#include "../modules/gpu/gpu.hpp" 
				
#define CTL(a) 	&LINKMODE_CONTROLS[ CONTROL_LINKMODE_##a ]
#define VAR(a) 	((u8*)&( VAR_CFG.LINKMODE.a ))
const Control LINKMODE_CONTROLS[ CONTROL_LINKMODE_MAX ] = { 
//	{	x    , y    , up				, right				, down				, left			, cache						, var						, callback				}
	{	0x04 , 0x04 , NULL				, CTL(MODE		)	, NULL				, NULL			, NULL						, (u8*)&(VAR_CFG.MENUSLOT ) , &cb_cfg_menuindex		},
	{	0x19 , 0x06 , CTL(SEND		)	, CTL(MODE		)	, CTL(MASTER	)	, CTL(MENU2)	, &CACHE_LIVE_MODE			, VAR(LINKMODE			  ) , &cb_cfg_linkmode		},
	{	0x1b , 0x07 , CTL(MODE		)	, CTL(MASTER	)	, CTL(SYNCRATE	)	, CTL(MENU2)	, &CACHE_CHECK 				, VAR(MASTERCLOCK		  ) , &cb_cfg_masterclock	},
	{	0x1b , 0x08 , CTL(MASTER	)	, CTL(SYNCRATE	)	, CTL(SYNCTICKS	)	, CTL(MENU2)	, &CACHE_HEXADECIMAL		, VAR(SYNCRATE			  ) , &cb_cfg_syncrate		},
	{	0x1a , 0x09 , CTL(SYNCRATE	)	, CTL(SYNCTICKS	)	, CTL(RECEIVE	)	, CTL(MENU2)	, &CACHE_DECIMAL_TWOTILES	, VAR(SYNCTICKS			  ) , &cb_cfg_syncticks		},
	{	0x1b , 0x0a , CTL(SYNCTICKS	)	, CTL(RECEIVE	)	, CTL(SEND		)	, CTL(MENU2)	, &CACHE_ARROW_LEFT			, NULL						, &cb_cfg_receivesong	},
	{	0x1b , 0x0b , CTL(RECEIVE	)	, CTL(SEND		)	, CTL(MODE		)	, CTL(MENU2)	, &CACHE_ARROW_LEFT			, NULL						, &cb_cfg_sendsong		},
	CONTROL_TERMINATOR 
};
#undef VAR
#undef CTL

void LinkMode::update( RegionHandler* rh ){
	const Region *c = &REGION_MAP_4_LINKSTATUS;	
	gpu.otherBlit(MAPDATA + ((MAP_CFG * 3) << 12), c->x, c->y, 0xb, 0xf, c->width, c->height);
}

void LinkMode::songRecv( Control *c, bool bigstep, bool add, u32 *pointer ){
	
}

void LinkMode::songSend( Control *c, bool bigstep, bool add, u32 *pointer ){
	
}

void LinkMode::monitor( RegionHandler* rh ){
}