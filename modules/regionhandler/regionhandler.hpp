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

class RegionHandler {
	private:
		
	public:
		const unsigned short *map0;
		const unsigned short *map1;
		const unsigned short *map2;
		
		Cache*		dirty;
		Region*		region;
		Control*	control; 		//focused control
		bool 		redraw;  		// used to notify the loaded region it must be redrawn
		bool 		new_region;  	// turned on @ load, screen responsible of reading this var must turn it off
		
		u32 		messages[1024];
		s32 		messagecount;
		
		u8 			viewportLastValue;// <--- warning!: this disrupts recursive functionality!
		
		RegionHandler();
		void dispatchMessages();
		void dispatchCallback( const Callback *cb, const Control *ctl, u8 add, u8 bigstep, u16 msg, u32 *pointer );
		void sendMessage(u32 message);
		void load(const Region *r);
		void drawCache(u8 x, u8 y, const Cache *c, u8 var, bool highlight);
		void drawVerticalCache(u8 x, u8 y, const Cache *c, u8 var, bool highlight);
		void drawControl(const Control *c);
		void drawDisplay(const Display *d);
		void updateViewport(const Viewport *v, u8 x, u8 y);
		void loadControls(const Region *r);
		void loadDisplays(const Region *r);
		void drawViewport(const Viewport *v, u8 x, u8 y, const Region *r);
		void draw();
		void jumpToControl(const Control *c);
		void controlTrigger(const Control *c, u16 q);
		void controlClear(const Control *c);
		void controlModify(const Control *c, bool big, bool add);
		void update(u8 delta);
};

#define OnNewRegion( f ) if(rh->redraw){ f };

extern RegionHandler regHnd;

#endif
