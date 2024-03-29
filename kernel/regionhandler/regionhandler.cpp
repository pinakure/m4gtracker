#include "regionhandler.hpp"
#include "../key/key.hpp"
#include "../gpu/gpu.hpp"
#include "../clip/clip.hpp"
#include "../sys/sys.hpp"
#include "../../debug.hpp"
#include "../../screens/live.hpp"
#include "../../screens/tracker.hpp"
#include "../../screens/patedit.hpp"
#include "../../screens/instedit.hpp"
#include "../../screens/config.hpp"
#include "../../screens/help.hpp"
#include "../../screens/snake.hpp"
#include "../../screens/songedit.hpp"
#include "../../screens/live/piano.hpp"
#include "../../screens/live/performance.hpp"
#include "../../data/data.hpp"

const unsigned short*	RegionHandler::map0;
const unsigned short*	RegionHandler::map1;
const unsigned short*	RegionHandler::map2;

Cache*					RegionHandler::dirty;
Region*					RegionHandler::region;
Control*				RegionHandler::control;
bool 					RegionHandler::redraw;
bool 					RegionHandler::new_region;
u32 					RegionHandler::messages[1024];
s32 					RegionHandler::messagecount;
u8 						RegionHandler::viewportLastValue;

void RegionHandler::dispatchCallback( const Callback *cb, const Control *ctl, u8 add, u8 bigstep, u16 msg ){
	const Callback *c = cb;	
	while(c){
		if(c->msg == msg){
			c->callback((Control*)ctl, add, bigstep, (u32*)c->var);
			return;
		}
		c=c->next;
	}	
}

void RegionHandler::init(){
	memset(messages, 0, sizeof(u32)*1024);
	region 			= NULL;
	redraw 			= true;
	messagecount 	= 0;
	Progress::disable();
}

RegionHandler::RegionHandler(){
}

void RegionHandler::sendMessageToRegion( Region *r, u32 msg , u32 pointer ){
	if(region == &REGION_MAP_1_LIVE1	) { Performance	::dispatchMessage( msg, pointer ); } else 
	if(region == &REGION_MAP_1_LIVE2	) { Piano		::dispatchMessage( msg, pointer ); } else 
	if(region == &REGION_MAP_1_HLP		) { Help		::dispatchMessage( msg, pointer ); } else 
	if(region == &REGION_MAP_2_PAT		) { PatEdit		::dispatchMessage( msg, pointer ); } else 
	if(region == &REGION_MAP_3_TRK		) { Tracker		::dispatchMessage( msg, pointer ); } else 
	if(region == &REGION_MAP_2_INS		) { InstEdit	::dispatchMessage( msg, pointer ); } else 
	if(region == &REGION_MAP_2_SNG		) { SongEdit	::dispatchMessage( msg, pointer ); } else 
	if(region == &REGION_MAP_4_CFG		) { Config		::dispatchMessage( msg, pointer ); } else 
	if(region == &REGION_MAP_4_SNK		) { SnakeGame	::dispatchMessage( msg, pointer ); } else 	
	Debug::panic("Cannot send message : Unknown Region", &pointer);
}

void RegionHandler::dispatchMessages(){
	int max = 0;
	u32 m;
	u32 pv;
	u32 msg;
	
	//const Region *r;
	//u8 i=0;
	
	while(messagecount > 0){
		messagecount--;
		msg = messages[messagecount];
		m  = msg&0xF0000000;
		pv = msg&0x0FFFFFFF;
		
		switch(m){
			
			/* Navigation */
			case MESSAGE_NAVIGATE_LEFT	: 
			case MESSAGE_NAVIGATE_RIGHT	: 
			case MESSAGE_NAVIGATE_DOWN	: 
			case MESSAGE_NAVIGATE_UP	: 
				sendMessageToRegion( region, m, pv );
				return;
					
			/* Direct callback */
			case MESSAGE_ACTIVATE		: 
				if( Clip::visible ) 
					Clip::activate();
				else 
					controlTrigger( ((Control*)pv), 1); 
				break;
			
			/* Variable modification */
			case MESSAGE_CANCEL			:
				if( Clip::visible ) 
					Clip::hide(); 
				else 
					if(region == &REGION_MAP_3_TRK) { Tracker::dispatchMessage ( m , pv ); } else 
					if(region == &REGION_MAP_2_PAT) { PatEdit::dispatchMessage ( m , pv ); } else 
					if(region == &REGION_MAP_2_INS) { InstEdit::dispatchMessage( m , pv ); } else 
					RegionHandler::controlClear	( (Control*)pv );
				break;
				
			case MESSAGE_MODIFY_ADD		: controlModify	( ((Control*)pv) , true  , false ); break;
			case MESSAGE_MODIFY_SUB 	: controlModify	( ((Control*)pv) , false , false ); break;
			case MESSAGE_MODIFY_BIGADD	: controlModify	( ((Control*)pv) , true	 , true	 ); break;
			case MESSAGE_MODIFY_BIGSUB	: controlModify	( ((Control*)pv) , false , true	 ); break;
				
			/* Redraw */
			case MESSAGE_REDRAW_REGION	: redraw = true; break;
			case MESSAGE_REDRAW_CONTROL	: drawControl( ((Control*)pv) ); break;
			case MESSAGE_REDRAW_DISPLAY	: drawDisplay( ((Display*)pv) ); break;
			case MESSAGE_REDRAW_VIEWPORT: 
				#define V ((Viewport*)pv)
				#define VAR *V->var
				#define REG  (const Region*)&(V->regions[VAR])
				
				drawViewport(V, (REG)->xadd, (REG)->yadd, REG);
				loadControls(REG);
				loadDisplays(REG);
				viewportLastValue = VAR;
				
				#undef V 
				#undef VAR 
				#undef REG
				break;
			
			case MESSAGE_OTHER:						
				sendMessageToRegion( region, pv, 0);
				/*// This is where to attach custom message dispatchers for each screen. 
				// Notice you can only send notifications, neither pointers can be passed, 
				// unless we speak of offsets (16 bit)
				if(region == &REGION_MAP_2_INS) { InstEdit::dispatchMessage( pv ,0 ); break; }
				if(region == &REGION_MAP_3_TRK) { Tracker::dispatchMessage ( pv ,0 ); break; }
				if(region == &REGION_MAP_2_PAT) { PatEdit::dispatchMessage ( pv ,0 ); break; }
				*/
				break;

			case MESSAGE_KEYPRESS:
				if( Clip::visible ) return Clip::processInput();
				if(KEYDOWN_LEFT ) jumpToControl(control->left ); else 
				if(KEYDOWN_RIGHT) jumpToControl(control->right); else
				if(KEYDOWN_UP   ) jumpToControl(control->up   ); else 
				if(KEYDOWN_DOWN ) jumpToControl(control->down );						
				break;
		}
		max++;
		if(max == 4)break;
	}
}

void RegionHandler::sendMessage(u32 message){
	messages[messagecount] = message;
	messagecount++;
}

void RegionHandler::load(const Region *r){
	//if( Clip::visible )return;
	if(region) RegionHandler::sendMessageToRegion( region, MESSAGE_EXIT );
	region = (Region*)r;
	control = r->focus?(Control*)r->focus:NULL;
	redraw = true;
	new_region = true;
	// Set new dirty zone
	dirty = (Cache*)region->dirty;
	viewportLastValue = 0xFF;
	if( r ) RegionHandler::sendMessageToRegion( (Region*)r, MESSAGE_ENTER );
}

void RegionHandler::drawCache(u8 x, u8 y, const Cache *c, u8 var, bool highlight){
	map0 = 	MAPDATA + 
			((c->mapIndex * 3) << 12) + 
			(c->x) + 
			(c->y<<6)+
			var*c->itemwidth;
			
	int cx, cy;
	for(cy=0; cy<c->itemheight; cy++){
		map1 = map0 + 4096;
		map2 = map1 + 4096;
		
		
		for(cx=0; cx<c->itemwidth; cx++){
			#ifdef BGCURSORS
				Gpu::set(0, x+cx, cy+y, highlight?0x13:0x0);
			#endif 
			Gpu::set(1, x+cx, cy+y, *map1);
			Gpu::set(2, x+cx, cy+y, *map2);
			map0++;
			map1++;
			map2++;
		}
		map0+=64-c->itemwidth;
	}
}

void RegionHandler::drawVerticalCache(u8 x, u8 y, const Cache *c, u8 var, bool highlight){
	map0 = 	MAPDATA + 
			((c->mapIndex * 3) << 12) + 
			(c->x) + 
			(c->y<<6)+
			(var*(c->itemheight<<6));
			
	int cx, cy;
	for(cy=0; cy<c->itemheight; cy++){
		map1 = map0 + 4096;
		map2 = map1 + 4096;
		
		for(cx=0; cx<c->itemwidth; cx++){
			#ifdef BGCURSORS
				Gpu::set(0, x+cx, cy+y, highlight?0x13:*map0);
			#endif
			Gpu::set(0, x+cx, cy+y, *map0);
			Gpu::set(1, x+cx, cy+y, *map1);
			Gpu::set(2, x+cx, cy+y, *map2);
			map0++;
			map1++;
			map2++;
		}
		map0+=64 - c->itemwidth;
	}
}

void RegionHandler::drawControl(const Control *c){
#define y	c->y
#define  x	c->x
	if(c->cache) {
		#ifdef BGCURSORS
		int color = c == control? c->cache->palActive : c->cache->palInactive;
		#else 
		int color = c->cache->palInactive;
		#endif 
		
		if(c->cache->mapIndex == 0xFF){
			// Cache is located on a table dumper funcion				
			
			//c->cache->element(x, y, color, *(u16*)c->var);    // WARNING: If you uncomment this line, bad alignment exceptions will happen
			c->cache->element(x, y, color, *(u8*)c->var);		// so try to survive with this one instead!
			
			// Draw the block below the control
			#ifdef BGCURSORS
				int cx = 0;
				for(; cx<c->cache->itemwidth; cx++){
					Gpu::set(0, x+cx, y, c==control?0x13:0x0);
				}				
			#endif
		} else {
			// Cache is located on map rom data
			u8 val = !c->var?c==control ? 1 : 0:*c->var;
			
			if(c->cache->vertical){
				drawVerticalCache(x, y, c->cache, val, c==control);
			}
			else {
				drawCache(x, y, c->cache, val, c==control);
			}
		}
	}
	
#undef x
#undef y
}

void RegionHandler::drawDisplay(const Display *d){
	#define y	d->y
	#define  x	d->x
	if(d->cache) {
		int color = d->cache->palInactive;
		
		if(d->cache->mapIndex == 0xFF){
			// Cache is located on a table dumper funcion				
			d->cache->element(x, y, color, *(u16*)d->var);
			
			// Draw the block below the control
			#ifdef BGCURSORS
				int cx = 0;
				for(; cx<d->cache->itemwidth;cx++){
					Gpu::set(0, x+cx, y, 0x0);
				}
			#endif
			
		} else {
			// Cache is located on map rom data
			// u8 val = !c->var?c==control ? 1 : 0:*c->var;
			u8 val = (d->invert?!*d->var:*d->var);
			
			if(d->cache->vertical)drawVerticalCache(x, y, d->cache, val, false);
			else drawCache(x, y, d->cache, val, false);
		}
	}
	
#undef x
#undef y
}

void RegionHandler::updateViewport(const Viewport *v, u8 x, u8 y){
	if(!v)return;
	if(!v->var)return;
	if(!v->regions)return;
	
	if(v->regions[*v->var % v->count]->updater)
		v->regions[*v->var % v->count]->updater();
	
	if(viewportLastValue != *v->var){
		drawViewport(v, x<0xFF?x:v->x, y<0xFF?y:v->y, v->regions[*v->var]);
		
		/***/
		loadControls(v->regions[*v->var]);			
		loadDisplays(v->regions[*v->var]);			
		
		/***/
		//sendMessage(MESSAGE_REDRAW_VIEWPORT | (unsigned)v);
		
		viewportLastValue = *v->var;
	}
}

void RegionHandler::loadControls(const Region *r){			
	// 
	// Caution with this!
	// It prevents loading a viewport from a region which 
	// is already in a viewport
	//
	messagecount = 0;
	if(r->viewport) viewportLastValue = 0xFF;
	
	//
	// Because we don't want to stress the engine 
	// redrawing too much controls at a time, we 
	// will send redraw messages instead for the
	// new control list, and we'll set focusControl to 
	// r->focus
	//
	const Control* c = r->controls;
	
	control = r->focus;
	
	if(c){
		//
		// Loop until the terminator value is found (c->x == 0xFF)
		//
		while(c->x < 0xFF){
			sendMessage(MESSAGE_REDRAW_CONTROL | (unsigned)c);
			
			c++;
		}
		//dispatchCallback(control->callback, control, false, false, EVENT_FOCUS);
	}
}

void RegionHandler::loadDisplays(const Region *r){
	const Display* d = r->displays;
	
	if(d){
		//
		// Loop until the terminator value is found (d->x == 0xFF)
		//
		while(d->x < 0xFF){
			sendMessage(MESSAGE_REDRAW_DISPLAY | (unsigned)d);
			
			d++;
		}
		
	}
}

void RegionHandler::drawViewport(const Viewport *v, u8 x, u8 y, const Region *r){
	// Substract original region coordinates to viewport coordinates
	
	Gpu::blit((E_Maps)region->mapIndex, r->x, r->y, r->xadd<0xFF?r->xadd:x, y, r->width, r->height);
}

void RegionHandler::draw(){
	Region *r = region;
	Gpu::blit((E_Maps)r->mapIndex, r->x, r->y, 0,0, r->width, r->height);

	loadControls(r);
	loadDisplays(r);
}

void RegionHandler::jumpToControl(const Control *c){
	
	if( Live::PERFORM.LOCK || Clip::visible || !c ) return;
	
	Control *o = control;
	control = (Control*)c;
	
	dispatchCallback(c->callback, c, false, false, EVENT_FOCUS);
	sendMessage(MESSAGE_REDRAW_CONTROL | (unsigned)o);
	sendMessage(MESSAGE_REDRAW_CONTROL | (unsigned)control);
}

void RegionHandler::controlTrigger(const Control *c, u16 q){
	ASSERT( c );
	ASSERT( c->callback );
		
	switch( c->callback->msg ){
		default					: //Debug::error(c->callback->msg);
								  break;
		
		case EVENT_KEY_A		: dispatchCallback(c->callback, c, true, true, EVENT_KEYDOWN_A	);
								  break;
		case EVENT_KEY_B		: dispatchCallback(c->callback, c, true, true, EVENT_KEYDOWN_B	);
								  break;
		
		case EVENT_KEYUP_A		: Sys::forceNoInput(); dispatchCallback(c->callback, c, true, true, EVENT_KEYUP_A  	); Sys::forceNoInput(); break;
		case EVENT_KEYUP_B		: Sys::forceNoInput(); dispatchCallback(c->callback, c, true, true, EVENT_KEYUP_B  	); Sys::forceNoInput(); break;
		case EVENT_KEYDOWN_A	: Sys::forceNoInput(); dispatchCallback(c->callback, c, true, true, EVENT_KEYDOWN_A	); break;
		case EVENT_KEYDOWN_B	: Sys::forceNoInput(); dispatchCallback(c->callback, c, true, true, EVENT_KEYDOWN_B	); break;
	}
	
	/*control->callback(control, true, true, ); */
	sendMessage(MESSAGE_REDRAW_CONTROL | (unsigned)c);
}

void RegionHandler::controlClear(const Control *c){
	ASSERT( c );
	if( !c->callback || Clip::visible ) return;
	if( ! c->var ) return;
	ASSERT( c->callback );
	(*(u8*) c->var) = 0;
	sendMessage(MESSAGE_REDRAW_CONTROL | (unsigned)c);
}

void RegionHandler::controlModify(const Control *c, bool big, bool add){
	if( !c->callback || Clip::visible ) return;
	dispatchCallback(c->callback, c, add, big, EVENT_MODIFY_B );
	sendMessage(MESSAGE_REDRAW_CONTROL | (unsigned)c);
}

void RegionHandler::update( u8 delta ){
	if(redraw) draw();
	// Handle modal progress dialog, if enableds
	if(Progress::update()) return;
	
	if(region->updater)region->updater();
	
	// If there is any viewport, it will override message queu and control list
	updateViewport(region->viewport, region->xadd, region->yadd);			
	
	//this allows a updater to detect wether or not the region has just been redrawn (useful on entry)
	redraw=false; //<--- on redraw, move back to that method if you notice any redraw bug or glitch. 
	
	dispatchMessages();
}
