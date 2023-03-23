#include "linkmode.hpp" 
#include "../config.hpp" 
#include "../../debug.hpp" 
#include "../../data/enum.h" 
#include "../../data/data.hpp" 
#include "../../kernel/gpu/gpu.hpp" 
#include "../../kernel/net/net.hpp" 
#include "../../kernel/spu/sequencer.hpp" 

const Callback cb_cfg_linkmode		= { LinkMode::alterMode			, EVENT_MODIFY_B	, &LinkMode::mode				, NULL };
const Callback cb_cfg_masterclock	= { LinkMode::toggleMaster		, EVENT_KEYDOWN_B	, &LinkMode::master_clock_enable, NULL };
const Callback cb_cfg_syncrate		= { modify4BIT					, EVENT_MODIFY_B	, &LinkMode::sync_rate			, NULL };
const Callback cb_cfg_syncticks  	= { modify8BIT					, EVENT_MODIFY_B	, &LinkMode::sync_ticks			, NULL };
const Callback cb_cfg_receivesong	= { LinkMode::songRecv			, EVENT_KEYUP_B		, NULL 							, NULL };
const Callback cb_cfg_sendsong		= { LinkMode::songSend			, EVENT_KEYUP_B		, NULL 							, NULL };
				
#define CTL(a) 	&LINKMODE_CONTROLS[ CONTROL_LINKMODE_##a ]
const Control LINKMODE_CONTROLS[ CONTROL_LINKMODE_MAX ] = { 
//	{	x    , y    , up				, right				, down				, left			, cache						, var							, callback				}
	{	0x04 , 0x04 , NULL				, CTL(MODE		)	, NULL				, NULL			, NULL						, (u8*)&(VAR_CFG.MENUSLOT ) 	, &Config::menuindex	},
	{	0x19 , 0x06 , CTL(SEND		)	, CTL(MODE		)	, CTL(MASTER	)	, CTL(MENU2)	, &CACHE_LIVE_MODE			, &LinkMode::mode				, &cb_cfg_linkmode		},
	{	0x1b , 0x07 , CTL(MODE		)	, CTL(MASTER	)	, CTL(SYNCRATE	)	, CTL(MENU2)	, &CACHE_CHECK 				, &LinkMode::master_clock_enable, &cb_cfg_masterclock	},
	{	0x1b , 0x08 , CTL(MASTER	)	, CTL(SYNCRATE	)	, CTL(SYNCTICKS	)	, CTL(MENU2)	, &CACHE_HEXADECIMAL		, &LinkMode::sync_rate 			, &cb_cfg_syncrate		},
	{	0x1a , 0x09 , CTL(SYNCRATE	)	, CTL(SYNCTICKS	)	, CTL(RECEIVE	)	, CTL(MENU2)	, &CACHE_DECIMAL_TWOTILES	, &LinkMode::sync_ticks			, &cb_cfg_syncticks		},
	{	0x1b , 0x0a , CTL(SYNCTICKS	)	, CTL(RECEIVE	)	, CTL(SEND		)	, CTL(MENU2)	, &CACHE_ARROW_LEFT			, NULL							, &cb_cfg_receivesong	},
	{	0x1b , 0x0b , CTL(RECEIVE	)	, CTL(SEND		)	, CTL(MODE		)	, CTL(MENU2)	, &CACHE_ARROW_LEFT			, NULL							, &cb_cfg_sendsong		},
	CONTROL_TERMINATOR 
};
#undef CTL

u8 		LinkMode::mode;
u8 		LinkMode::master_clock_enable;
u8 		LinkMode::sync_rate;
u8 		LinkMode::sync_ticks;
bool  	LinkMode::needs_redraw;
bool  	LinkMode::notify_write;
bool  	LinkMode::notify_read;
u16  	LinkMode::notify_timeout;


void LinkMode::alterMode(Control *c, bool bigstep, bool add, u32 *pointer){	
	LinkMode::mode = ( bigstep ? (add?0x3:0) : (LinkMode::mode + (add?1:-1)) ) & 0x3; 	
	switch( mode ){
		case LINKMODE_GBA: /*! let the user decide !*/break;
		case LINKMODE_SYNC_MASTER	: master_clock_enable = true; break;
		case LINKMODE_IBM			: master_clock_enable = true; break;
		case LINKMODE_SYNC_SLAVE	: master_clock_enable = false; break;
	}	
	Net::setMode( (eNetMode) mode );//alterMode(c, bigstep, add, pointer);
	const Control *control = &LINKMODE_CONTROLS[ CONTROL_LINKMODE_MASTER ];
	RegionHandler::redraw = 1;
	RegionHandler::drawControl( control );
	RegionHandler::update(0);
	RegionHandler::redraw = 0;
	RegionHandler::control = c;
	notifyStatus();
}

void LinkMode::notifyDisconnect(){
	Gpu::set(2, 11, 17, 0x0072);
	Gpu::set(2, 11, 18, 0x0100);
	Gpu::set(2, 28, 17, 0x0072);
	Gpu::set(2, 28, 18, 0x0100);
}

void LinkMode::notifyStatus(){
	
	Gpu::blitAlt(
		MAPDATA + ((MAP_CFG * 3) << 12), 
		REGION_MAP_4_LINKSTATUS.x, REGION_MAP_4_LINKSTATUS.y, 
		0xb, 0xf, 
		REGION_MAP_4_LINKSTATUS.width, REGION_MAP_4_LINKSTATUS.height
	);
	
	// Draw master/slave icons
	if(master_clock_enable){
		Gpu::set(2, 28, 17, 0x30E2);
		Gpu::set(2, 28, 18, 0x30F2);
		Gpu::set(2, 11, 17, 0xF0CC);
		Gpu::set(2, 11, 18, 0xF0DC);
		return;
	}
	Gpu::set(2, 28, 17, 0xF0CC);
	Gpu::set(2, 28, 18, 0xF0DC);
	Gpu::set(2, 11, 17, 0x30E2);
	Gpu::set(2, 11, 18, 0x30F2);
}

void LinkMode::notifyClear(){
	// Gpu::set(2, 16, 17, 0x902F);
	// Gpu::set(2, 23, 17, 0x902F);
	Gpu::set(2, 16, 17, 0x0100);
	Gpu::set(2, 23, 17, 0x0100);
}
void LinkMode::notifyRead(){
	Gpu::set(2, 16, 17, 0xF08F);
	Gpu::set(2, 23, 17, 0x308F);
	notify_read = false;
	notify_timeout = (( sync_rate + 1) * (1 << sync_ticks )) >> 1;
}

void LinkMode::notifyWrite(){
	Gpu::set(2, 16, 17, 0x308D);
	Gpu::set(2, 23, 17, 0xF08D);
	notify_write = false;
	notify_timeout = (( sync_rate + 1) * (1 << sync_ticks )) >> 1;
}

extern "C" { int CheckBPMClock(int timerTarget); }; /* Since these are ASM instructions, we have to ensure CPP does not mangle them! */

void LinkMode::update(  ){
	
	Gpu::blinkUpdate(8);
	
	if(!CheckBPMClock( Sequencer::timerTarget )) return;
	
	if(( notify_timeout == 1 )){ LinkMode::notifyClear(); } else notify_timeout--;
	
	if( notify_read  ) LinkMode::notifyRead();
	if( notify_write ) LinkMode::notifyWrite();
	
	if( needs_redraw || RegionHandler::new_region || Config::menuindex_changed ){
		RegionHandler::new_region = false;
		Config::menuindex_changed = false;
		needs_redraw = false;
		notifyStatus();
	}
}

void LinkMode::toggleMaster(Control *c, bool bigstep, bool add, u32 *pointer){
	modify1BIT( c, bigstep, add, pointer);
	LinkMode::update();
	RegionHandler::drawControl(&LINKMODE_CONTROLS[ CONTROL_LINKMODE_MASTER]);
	switch( mode ){
		case LINKMODE_GBA: /*! let the user decide !*/break;
		case LINKMODE_IBM			: master_clock_enable = true; break;
		case LINKMODE_SYNC_SLAVE	: master_clock_enable = false; break;
		case LINKMODE_SYNC_MASTER	: master_clock_enable = true; break;
	}
	notifyStatus();
}

void LinkMode::songRecv( Control *c, bool bigstep, bool add, u32 *pointer ){
	
}

void LinkMode::songSend( Control *c, bool bigstep, bool add, u32 *pointer ){
	
}

void LinkMode::monitor(  ){
	HALT
}
