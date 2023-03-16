#ifndef REGIONHANDLER_HPP
#define REGIONHANDLER_HPP

#include "../../agb.h"

#include "../../data/enum.h"
#include "../../data/caches.hpp"
#include "../../data/controls.hpp"
#include "../../data/regions.hpp"
#include "../../dialogs/progress.hpp"
#include "../../dialogs/alpha.hpp"
#include "../../dialogs/really.hpp"

#include "cursor.hpp"

class RegionHandler {
	private:
		
	public:
		static const unsigned short*	map0;
		static const unsigned short*	map1;
		static const unsigned short*	map2;
		
		static Cache*					dirty;
		static Region*					region;
		static Control*					control; 		//focused control
		static bool 					redraw;  		// used to notify the loaded region it must be redrawn
		static bool 					new_region;  	// turned on @ load, screen responsible of reading this var must turn it off
		static u32 						messages[1024];
		static s32 						messagecount;
		static u8 						viewportLastValue;// <--- warning!: this disrupts recursive functionality!
		
		RegionHandler					( );
		static void	init				( );
		static void dispatchMessages			( );
		static void draw						( );
		
		static void sendMessage			( u32 message										);
		static void load				( const Region *r									);
		static void drawCache			( u8 x, u8 y, const Cache *c, u8 var, bool highlight);
		static void drawVerticalCache	( u8 x, u8 y, const Cache *c, u8 var, bool highlight);
		static void drawControl			( const Control *c									);
		static void drawDisplay			( const Display *d									);
		static void updateViewport		( const Viewport *v, u8 x, u8 y						);
		static void loadControls		( const Region *r									);
		static void loadDisplays		( const Region *r									);
		static void drawViewport		( const Viewport *v, u8 x, u8 y, const Region *r	);
		static void jumpToControl		( const Control *c									);
		static void controlTrigger		( const Control *c, u16 q							);
		static void controlClear		( const Control *c									);
		static void controlModify		( const Control *c, bool big, bool add				);
		static void update				( u8 delta											);

		static void dispatchCallback	( const Callback *cb, const Control *ctl, u8 add, u8 bigstep, u16 msg );
};

#define OnNewRegion( f ) if(RegionHandler::redraw){ f };

#endif
