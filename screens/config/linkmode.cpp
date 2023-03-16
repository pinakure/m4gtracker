#include "linkmode.hpp" 
#include "../config.hpp" 
#include "../../data/enum.h" 
#include "../../data/data.hpp" 
#include "../../modules/gpu/gpu.hpp" 
#include "../../modules/net/net.hpp" 

const Callback cb_cfg_linkmode		= { Net::alterMode				, EVENT_MODIFY_B	, &VAR_CFG.LINKMODE.LINKMODE	, NULL };
const Callback cb_cfg_masterclock	= { LinkMode::toggleMaster		, EVENT_KEYDOWN_B	, &VAR_CFG.LINKMODE.MASTERCLOCK	, NULL };
const Callback cb_cfg_syncrate		= { modify4BIT					, EVENT_MODIFY_B	, &VAR_CFG.LINKMODE.SYNCRATE	, NULL };
const Callback cb_cfg_syncticks  	= { modify8BIT					, EVENT_MODIFY_B	, &VAR_CFG.LINKMODE.SYNCTICKS	, NULL };
const Callback cb_cfg_receivesong	= { LinkMode::songRecv			, EVENT_KEYUP_B		, NULL 							, NULL };
const Callback cb_cfg_sendsong		= { LinkMode::songSend			, EVENT_KEYUP_B		, NULL 							, NULL };
				
#define CTL(a) 	&LINKMODE_CONTROLS[ CONTROL_LINKMODE_##a ]
#define VAR(a) 	((u8*)&( VAR_CFG.LINKMODE.a ))
const Control LINKMODE_CONTROLS[ CONTROL_LINKMODE_MAX ] = { 
//	{	x    , y    , up				, right				, down				, left			, cache						, var						, callback				}
	{	0x04 , 0x04 , NULL				, CTL(MODE		)	, NULL				, NULL			, NULL						, (u8*)&(VAR_CFG.MENUSLOT ) , &Config::menuindex		},
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

void LinkMode::update(  ){
	const Region *c = &REGION_MAP_4_LINKSTATUS;	
	Gpu::blitAlt(MAPDATA + ((MAP_CFG * 3) << 12), c->x, c->y, 0xb, 0xf, c->width, c->height);
	switch( VAR_CFG.LINKMODE.LINKMODE ){
		case LINKMODE_GBA: /*! let the user decide !*/break;
		case LINKMODE_IBM			: VAR_CFG.LINKMODE.MASTERCLOCK = true; break;
		case LINKMODE_SYNC_SLAVE	: VAR_CFG.LINKMODE.MASTERCLOCK = false; break;
		case LINKMODE_SYNC_MASTER: VAR_CFG.LINKMODE.MASTERCLOCK = true; break;
	}
}

void LinkMode::toggleMaster(Control *c, bool bigstep, bool add, u32 *pointer){
	modify1BIT( c, bigstep, add, pointer);
	LinkMode::update();
	RegionHandler::drawControl(&LINKMODE_CONTROLS[ CONTROL_LINKMODE_MASTER]);
}

void LinkMode::songRecv( Control *c, bool bigstep, bool add, u32 *pointer ){
	
}

void LinkMode::songSend( Control *c, bool bigstep, bool add, u32 *pointer ){
	
}

void LinkMode::monitor(  ){
}
