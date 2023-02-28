#include "regionhandler.hpp"
#include "../gpu/gpu.hpp"
#include "../key/key.hpp"
#include "../sys/sys.hpp"
#include "../../data/tables.hpp"
#include "../../data/regions.hpp"
#include "../../data/layers.hpp"
#include "../../data/variables.hpp"

extern LIVE VAR_LIVE;
extern void STATUS(u8 x, u8 y, u16 color, u16 value);
extern void PROGRESS(u8 x, u8 y, u16 color, u16 value);
void trkDispatchMessage(u32);
void insDispatchMessage(u32);
void patDispatchMessage(u32);

s_Clipboard Clipboard;


u32 			Progress::max;
u32 			Progress::current=0;
u8 				Progress::step;
u8 				Progress::laststep;
E_StatusStrings Progress::upperLine;
E_StatusStrings Progress::lowerLine;
bool 			Progress::enabled;
u8 				Progress::redraw;
u32*			Progress::var=NULL;


Progress 				REGHND::progress;
const unsigned short*	REGHND::map0;
const unsigned short*	REGHND::map1;
const unsigned short*	REGHND::map2;
Cache* 					REGHND::dirty;
Region* 				REGHND::region;
Control* 				REGHND::control; //focused control
bool 					REGHND::redraw;
u32 					REGHND::messages[1024];
s32 					REGHND::messagecount;
u8 						REGHND::viewportLastValue;// <--- warning!: this disrupts recursive functionality!
		
void Progress::update(){
	if(current != *var){
		current = *var;
	
		step = (current * 0x24) / max;		
		if(step != laststep){
			laststep = step;
			redraw = 1;
		}
		if(current >= max)enabled = false;
	}
}

void Progress::set(u32 value, u32 maxvalue, E_StatusStrings uLine, E_StatusStrings lLine, u32 *var){
	enabled = true;
	Progress::var = var;
	max = maxvalue;
	upperLine = uLine;
	lowerLine = lLine;
	current = *var;
	update();
	redraw = 2;
}
		
AlphaDialog::AlphaDialog(){
	enabled = false;
	redraw = false;
}

void AlphaDialog::add(){
	u8 index = (cursorY * 6) + cursorX;
	buffer[position] = index+1;
	if(length<maxlen){
		length++;
		position++;
	} else {
		rem();
		add();
	}
	preview();
}

void AlphaDialog::rem(){
	if(length>0){
		buffer[position-1] = 0;
		preview();
		
		length--; 
		position--;
	}else cancel();
}

void AlphaDialog::draw(){
	GPU::blit(GPU::MAP_CACHE, 0x0, 0x0, 0x0, Y, 0x08, 0x8);
	redraw = false;
}

void AlphaDialog::confirm(){
	if( (!KEYPRESS_B) && (!KEYUP_B) ){
		enabled = false;
		buffer[length] = 0;
		for(int i=0; i<maxlen; i++){
			target[i] = buffer[i]-1;
		}
	}
}

void AlphaDialog::cancel(){
	enabled = false;
}

void AlphaDialog::preview(){
	int i;
	static u8 lastPosition;

	GPU::set(0, textX+lastPosition, textY, 0x0);	
	GPU::set(0, textX+(position>0?position-1:position), textY, 0x13);	

	lastPosition=position-1;
	
	for(i=0; i<maxlen; i++){
		if(buffer[i]==0) GPU::set(2, textX+i, textY, Tables::TEXT[0x20][0] | 0xa000);
		else GPU::set(2, textX+i, textY, ((Tables::TEXT[buffer[i]][0])-1) | 0x4000);
	}
}

void AlphaDialog::update(){
	// If you want this to be multitask friendly, remove this while loop and put 'if(alphadialog->enabled)alphadialog->update();		
	while(enabled){
		if(redraw)draw();
		
		//KEY::Update();
		SYS::update();
	
		if(KEYDOWN_LEFT ) cursorX--; else
		if(KEYDOWN_RIGHT) cursorX++; else 
		if(KEYDOWN_DOWN ) cursorY++; else 
		if(KEYDOWN_UP   ) cursorY--;
		
		if(KEYUP_START  ) confirm();
		
		if(KEYDOWN_A    ) add();				
		if(KEYDOWN_B    ) rem();
		
		cursorX %= 6;
		cursorY %= 6;
	
		// Redraw if cursor changed
		if((oldX != cursorX)||(oldY != cursorY)){
			u16 value = GPU::get(2, 1+cursorX, Y+1+cursorY)&0x0fff; 
			GPU::set(2, 1+cursorX, Y+1+cursorY, value | 0x2000);
			GPU::set(2, 1+oldX, Y+1+oldY, oldvalue | 0x9000);
			oldvalue = value;
			
			oldX = cursorX;
			oldY = cursorY;
		}
	}
}

void AlphaDialog::enable( bool bigString, u8* targetVariable, u8 vx, u8 vy ){
	textX = vx;
	textY = vy;
	cursorX = 0;
	cursorY = 0;
	oldX = 0x30;
	oldY = 0x30;
	memset(buffer, 0x0, 16);
	length = 0;
	position = 0;
	enabled = true;
	redraw = true;
	maxlen = bigString?14:6;
	Y = bigString?3:0xc;
	target = targetVariable;
	
	// Read variable
	for(int i=0; i<maxlen;i++){
		buffer[i] = targetVariable[i]+1;
		if(buffer[i]==0x0)break;
		length++;
		position++;
	}
	
	preview();
	
	update();
}

ReallyDialog::ReallyDialog(void){
	result = false;
	enabled = false;
}

void ReallyDialog::draw(){
	GPU::blit(GPU::MAP_CACHE, 0x13, 0x0, 12, 6, 0x09, 0x5);
}

void ReallyDialog::confirm(){
	if( (!KEYPRESS_B) && (!KEYUP_B) ){
		result = true;
		enabled = false;
	}
}

void ReallyDialog::cancel(){
	result  = false;
	enabled = false;
}

void ReallyDialog::enable(){
	enabled = true;
	draw();
	while(enabled){
		
		/*!*/
		//KEY::Update();
		SYS::update();
		/*!*/
	

		if(KEYUP_START) confirm();
		if(KEYUP_B    ) cancel();		
	}
}


void dispatchCallback(const Callback *cb, const Control *ctl, u8 add, u8 bigstep, u16 msg){
	const Callback *c = cb;	
	while(c){
		if(c->msg == msg){
			c->callback((Control*)ctl, add, bigstep, (u32*)c->var);//pointer);
			return;
		}
		c=c->next;
	}	
}


void REGHND::init(){
	region = NULL;
	redraw = true;
	memset(messages, 0, sizeof(u32)*1024);
	messagecount = 0;
	progress.enabled = false;
}

void REGHND::dispatchMessages(){
	int max = 0;
	u32 m;
	u32 pv;
	
	//const Region *r;
	//u8 i=0;
	
	while(messagecount > 0){
		messagecount--;
		m  = messages[messagecount]&0xF0000000;
		pv = messages[messagecount]&0x0FFFFFFF;
		
		switch(m){
			
			/* Navigation */
			case MESSAGE_NAVIGATE_LEFT	: load( ((Region*)pv)->left ); break;
			case MESSAGE_NAVIGATE_RIGHT	: load( ((Region*)pv)->right); break;
			case MESSAGE_NAVIGATE_DOWN	: load( ((Region*)pv)->down ); break;
			case MESSAGE_NAVIGATE_UP	: load( ((Region*)pv)->up   ); break;
			
			/* Direct callback */
			case MESSAGE_ACTIVATE		: controlTrigger( ((Control*)pv), 1); break;
			
			/* Variable modification */
			case MESSAGE_MODIFY_ADD		: controlModify( ((Control*)pv)	, true  , false ); break;
			case MESSAGE_MODIFY_SUB 	: controlModify( ((Control*)pv)	, false , false ); break;
			case MESSAGE_MODIFY_BIGADD	: controlModify( ((Control*)pv)	, true	, true	); break;
			case MESSAGE_MODIFY_BIGSUB	: controlModify( ((Control*)pv)	, false , true	); break;
				
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
				// This is where to attach custom message dispatchers for each screen. 
				// Notice you can only send notifications, neither pointers can be passed, 
				// unless we speak of offsets (16 bit)
				if(region == &REGION_MAP_2_INS) { insDispatchMessage(pv); break; }
				if(region == &REGION_MAP_3_TRK) { trkDispatchMessage(pv); break; }
				if(region == &REGION_MAP_2_PAT) { patDispatchMessage(pv); break; }
				break;

			case MESSAGE_KEYPRESS:
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

void REGHND::sendMessage( u32 message ){
	messages[messagecount] = message;
	messagecount++;
}

void REGHND::load( const Region *r ){
	region = (Region*)r;
	control = r->focus?(Control*)r->focus:NULL;
	redraw = true;
	// Set new dirty zone
	dirty = (Cache*)region->dirty;
	viewportLastValue = 0xFF;
}

void REGHND::drawCache( u8 x, u8 y, const Cache *c, u8 var, bool highlight ){
	map0 = 	Layers::DATA + 
			((c->mapIndex * 3) << 12) + 
			(c->x) + 
			(c->y<<6)+
			var*c->itemwidth;
			
	int cx, cy;
	for(cy=0; cy<c->itemheight; cy++){
		map1 = map0 + 4096;
		map2 = map1 + 4096;
		
		for(cx=0; cx<c->itemwidth; cx++){
			GPU::set(0, x+cx, cy+y, highlight?0x13:0x0);
			GPU::set(1, x+cx, cy+y, *map1);
			GPU::set(2, x+cx, cy+y, *map2);
			map0++;
			map1++;
			map2++;
		}
		map0+=64-c->itemwidth;
	}
}

void REGHND::drawVerticalCache( u8 x, u8 y, const Cache *c, u8 var, bool highlight ){
	map0 = 	Layers::DATA + 
			((c->mapIndex * 3) << 12) + 
			(c->x) + 
			(c->y<<6)+
			(var*(c->itemheight<<6));
			
	int cx, cy;
	for(cy=0; cy<c->itemheight; cy++){
		map1 = map0 + 4096;
		map2 = map1 + 4096;
		
		for(cx=0; cx<c->itemwidth; cx++){
			GPU::set(0, x+cx, cy+y, highlight?0x13:*map0);
			GPU::set(1, x+cx, cy+y, *map1);
			GPU::set(2, x+cx, cy+y, *map2);
			map0++;
			map1++;
			map2++;
		}
		map0+=64 - c->itemwidth;
	}
}

void REGHND::drawControl( const Control *c ){
#define y	c->y
#define  x	c->x
	if(c->cache) {
		int color = c == control? c->cache->palActive : c->cache->palInactive;
		
		if(c->cache->mapIndex == 0xFF){
			// Cache is located on a table dumper funcion				
			
			//c->cache->element(x, y, color, *(u16*)c->var);    // WARNING: If you uncomment this line, bad alignment exceptions will happen
			c->cache->element(x, y, color, *(u8*)c->var);		// so try to survive with this one instead!
			
			// Draw the block below the control
			int cx = 0;
			for(; cx<c->cache->itemwidth; cx++){
				GPU::set(0, x+cx, y, c==control?0x13:0x0);
			}				
			
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

void REGHND::drawDisplay( const Display *d ){
#define y	d->y
#define  x	d->x
	if(d->cache) {
		int color = d->cache->palInactive;
		
		if(d->cache->mapIndex == 0xFF){
			// Cache is located on a table dumper funcion				
			d->cache->element(x, y, color, *(u16*)d->var);
			
			// Draw the block below the control
			int cx = 0;
			for(; cx<d->cache->itemwidth;cx++){
				GPU::set(0, x+cx, y, 0x0);
			}				
			
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

void REGHND::updateViewport( const Viewport *v, u8 x, u8 y ){
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

void REGHND::loadControls( const Region *r ){			
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

void REGHND::loadDisplays( const Region *r ){
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

void REGHND::drawViewport( const Viewport *v, u8 x, u8 y, const Region *r ){
	// Substract original region coordinates to viewport coordinates
	
	GPU::blit( ( GPU::E_Maps ) region->mapIndex, r->x, r->y, r->xadd<0xFF?r->xadd:x, y, r->width, r->height);
}

void REGHND::draw(){
	Region *r = region;
	GPU::blit( ( GPU::E_Maps ) r->mapIndex, r->x, r->y, 0,0, r->width, r->height);

	loadControls(r);
	loadDisplays(r);
	
}

void REGHND::jumpToControl( const Control *c ){
	if(VAR_LIVE.PERFORM.LOCK)return;
	if(!c)return;
	Control *o = control;
	control = (Control*)c;
	
	dispatchCallback(c->callback, c, false, false, EVENT_FOCUS);
	sendMessage(MESSAGE_REDRAW_CONTROL | (unsigned)o);
	sendMessage(MESSAGE_REDRAW_CONTROL | (unsigned)control);
}

void REGHND::controlTrigger( const Control *c, u16 q ){
	if(!c->callback)return;
	dispatchCallback(c->callback, c, true, true, EVENT_KEYDOWN_B);
	dispatchCallback(c->callback, c, true, true, EVENT_KEYUP_B);
	/*control->callback(control, true, true, (u32*)this); */
	sendMessage(MESSAGE_REDRAW_CONTROL | (unsigned)c);
	
	
}

void REGHND::controlModify( const Control *c, bool big, bool add ){
	if(!c->callback)return;
	dispatchCallback(c->callback, c, add, big, EVENT_MODIFY_B);
	/*control->modifier(control, big, add, (u32*)this); */
	sendMessage(MESSAGE_REDRAW_CONTROL | (unsigned)c);
}

extern const Cache CACHE_DLG_PROGRESS_SMALL;

void REGHND::updateProgress(){
	if(progress.redraw){
		if(progress.redraw>1){
			drawCache(10, 5, &CACHE_DLG_PROGRESS_SMALL, 0, 0);
			STATUS(11, 6, 6, progress.upperLine);
			STATUS(11, 8, 7, progress.lowerLine);
		}
		PROGRESS(11, 11, 0, progress.step);
		progress.redraw = 0;
	}			
	progress.update();
}

void REGHND::update( u8 delta ){
	if(redraw) draw();
	// Handle modal progress dialog, if enableds
	if(progress.enabled){
		updateProgress();
		return;
	}
	
	if( region->updater ) region->updater();
	
	// If there is any viewport, it will override message queu and control list
	updateViewport(region->viewport, region->xadd, region->yadd);			
	
	//this allows a updater to detect wether or not the region has just been redrawn (useful on entry)
	redraw=false; //<--- on redraw, move back to that method if you notice any redraw bug or glitch. 
	
	dispatchMessages();
}
