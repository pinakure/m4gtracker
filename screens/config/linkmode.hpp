#ifndef LINKMODE_HPP
#define LINKMODE_HPP

#include "../../agb.h"

#include "../../data/control.hpp"
#include "../../modules/regionhandler/regionhandler.hpp"

enum E_LINKMODE_CONTROLS { 
	CONTROL_LINKMODE_MENU2,			
	CONTROL_LINKMODE_MODE,			
	CONTROL_LINKMODE_MASTER,			
	CONTROL_LINKMODE_SYNCRATE,
	CONTROL_LINKMODE_SYNCTICKS,		
	CONTROL_LINKMODE_RECEIVE,		
	CONTROL_LINKMODE_SEND,				
	CONTROL_LINKMODE_COUNT,
	CONTROL_LINKMODE_MAX
};

class LinkMode {
	public BACKUP:
		static BITFIELD( 2 ) 	mode;
		static BITFIELD( 1 )	master_clock_enable;
		static BITFIELD( 8 ) 	sync_rate;
		static BITFIELD( 8 ) 	sync_ticks;

	public TRANSIENT:
		static bool needs_redraw;
		static bool notify_read;
		static bool notify_write;
		static u16  notify_timeout;
		
		static void update			(  );
		static void monitor 		(  );
		static void songRecv		( Control *c, bool bigstep, bool add, u32 *pointer );
		static void songSend 		( Control *c, bool bigstep, bool add, u32 *pointer );
		static void toggleMaster	( Control *c, bool bigstep, bool add, u32 *pointer );
		static void alterMode		( Control *c, bool bigstep, bool add, u32 *pointer );
		static void notifyStatus 	( );
		static void notifyDisconnect( );
		static void notifyClear		( );
		static void notifyRead 		( );
		static void notifyWrite		( );
	
};

extern const Callback cb_cfg_linkmode;
extern const Callback cb_cfg_masterclock;
extern const Callback cb_cfg_syncrate;
extern const Callback cb_cfg_syncticks;
extern const Callback cb_cfg_receivesong;	
extern const Callback cb_cfg_sendsong;		

extern const Control LINKMODE_CONTROLS[ CONTROL_LINKMODE_MAX ];

#endif

