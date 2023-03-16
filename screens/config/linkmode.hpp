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
	public:
		static void update			(  );
		static void monitor 		(  );
		static void songRecv		( Control *c, bool bigstep, bool add, u32 *pointer );
		static void songSend 		( Control *c, bool bigstep, bool add, u32 *pointer );
		static void toggleMaster	( Control *c, bool bigstep, bool add, u32 *pointer );
};

extern const Callback cb_cfg_linkmode;
extern const Callback cb_cfg_masterclock;
extern const Callback cb_cfg_syncrate;
extern const Callback cb_cfg_syncticks;
extern const Callback cb_cfg_receivesong;	
extern const Callback cb_cfg_sendsong;		

extern const Control LINKMODE_CONTROLS[ CONTROL_LINKMODE_MAX ];

#endif

