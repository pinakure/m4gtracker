
class Progress {
	private:
	public:
		u32 max;
		u32 current;
		u8 step;
		u8 laststep;
		E_StatusStrings upperLine;
		E_StatusStrings lowerLine;
		bool enabled;
		u8 redraw;
		u32 *var;
	
		Progress(){}

		void update(){
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
		
		void set(u32 value, u32 maxvalue, E_StatusStrings uLine, E_StatusStrings lLine, u32 *var){
			enabled = true;
			this->var = var;
			this->max = maxvalue;
			upperLine = uLine;
			lowerLine = lLine;
			current = *var;
			update();
			redraw = 2;
		}
	
};

class AlphaDialog {
	private:
		u8 buffer[16];
	public:	
		u8 maxlen;		// Max length of string
		u8 length;
		u8 position;
		u8 cursorX;
		u8 cursorY;
		u8 textX;
		u8 textY;
		u8 oldX;
		u8 oldY;
		u8 Y;
		u16 oldvalue;
		u8 *target;	// The string were to write buffered text at exit
		bool enabled;
		bool redraw;
		
		AlphaDialog(void){
			enabled = false;
			redraw = false;
		}
		
		void add(void){
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
		
		void rem(void){
			if(length>0){
				buffer[position-1] = 0;
				preview();
				
				length--; 
				position--;
			}else cancel();
		}
		
		void draw(void){			
			gpu.blit(MAP_CACHE, 0x0, 0x0, 0x0, Y, 0x08, 0x8);
			
			
			redraw = false;
		}
		
		
		
		void confirm(void){
			if( (!KEYPRESS_B) && (!KEYUP_B) ){
				enabled = false;
				buffer[length] = 0;
				for(int i=0; i<maxlen; i++){
					target[i] = buffer[i]-1;
				}
			}
		}
		
		void cancel(void){
			enabled = false;
		}
		
		void preview(void){
			int i;
			static u8 lastPosition;

			gpu.set(0, textX+lastPosition, textY, 0x0);	
			gpu.set(0, textX+(position>0?position-1:position), textY, 0x13);	

			lastPosition=position-1;
			
			for(i=0; i<maxlen; i++){
				if(buffer[i]==0) gpu.set(2, textX+i, textY, TABLE_TEXT[0x20][0] | 0xa000);
				else gpu.set(2, textX+i, textY, ((TABLE_TEXT[buffer[i]][0])-1) | 0x4000);
			}
		}
		
		void update(void){
			// If you want this to be multitask friendly, remove this while loop and put 'if(alphadialog->enabled)alphadialog->update();		
			while(enabled){
				if(redraw)draw();
				
				//KEY.Update();
				sys.update();
			
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
					u16 value = gpu.get(2, 1+cursorX, Y+1+cursorY)&0x0fff; 
					gpu.set(2, 1+cursorX, Y+1+cursorY, value | 0x2000);
					gpu.set(2, 1+oldX, Y+1+oldY, oldvalue | 0x9000);
					oldvalue = value;
					
					oldX = cursorX;
					oldY = cursorY;
				}
			}
		}
		
		void enable(bool bigString, u8* targetVariable, u8 vx, u8 vy){
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
};

class ReallyDialog {
	private:
	public:
		bool result;
		bool enabled;
		
		ReallyDialog(void){
			result = false;
			enabled = false;
		}
		
		void draw(void){
			gpu.blit(MAP_CACHE, 0x13, 0x0, 12, 6, 0x09, 0x5);
		}
		
		void confirm(void){
			if( (!KEYPRESS_B) && (!KEYUP_B) ){
				result = true;
				enabled = false;
			}
		}
		
		void cancel(void){
			result = false;
			enabled = false;
		}
		
		void enable(void){
			enabled = true;
			draw();
			while(enabled){
				
				/*!*/
				//KEY.Update();
				sys.update();
				/*!*/
				
				if(KEYUP_START) confirm();
				if(KEYUP_B    ) cancel();		
			}
		}
	
};

typedef struct s_Clipboard {
	u16 	*blockdata[16*5*6];		// 6 channels, 5 fields per channel row, 16 rows max ;; these are actually pointers to each control currently selected 
	u16 	blocklength;			// length of the block used currently (0 by default)
	u8 		blockstart;				// logical tile where the block starts
	u8 		blockwidth;				// width for the block displayed on the screen
	u8 		blockheight;			// height for the block displayed on the screen
	bool	blockclean;				// true if no copy operations were done yet
	bool	blockactive;			// true if block is visible and receiving arrow input, or waiting either B for cancel or A to copy, or B+A to cut 'cancel the cancel'. R + A, clone, R + arrows move the block by the other corner
} Clipboard;


void dispatchCallback(const Callback *cb, const Control *ctl, u8 add, u8 bigstep, u16 msg, u32 *pointer){
	const Callback *c = cb;	
	while(c){
		if(c->msg == msg){
			c->callback((Control*)ctl, add, bigstep, (u32*)c->var);//pointer);
			return;
		}
		c=c->next;
	}	
}


void trkDispatchMessage(u32);
void insDispatchMessage(u32);
void patDispatchMessage(u32);


class RegionHandler {
	private:
		
	public:
		Progress progress;
		const unsigned short *map0;
		const unsigned short *map1;
		const unsigned short *map2;
		
		Cache *dirty;
		Region *region;
		Control *control; //focused control
		bool redraw;
		
		u32 messages[1024];
		s32 messagecount;
		
		
		
		u8 viewportLastValue;// <--- warning!: this disrupts recursive functionality!
		
		RegionHandler(void){
			region = NULL;
			redraw = true;
			memset(messages, 0, sizeof(u32)*1024);
			messagecount = 0;
			progress.enabled = false;
		}

		void dispatchMessages(void){
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
					case MESSAGE_REDRAW_REGION	: this->redraw = true; break;
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
		
		void sendMessage(u32 message){
			messages[messagecount] = message;
			messagecount++;
		}
		
		void load(const Region *r){
			region = (Region*)r;
			control = r->focus?(Control*)r->focus:NULL;
			redraw = true;
			// Set new dirty zone
			dirty = (Cache*)region->dirty;
			viewportLastValue = 0xFF;
		}
		
		void drawCache(u8 x, u8 y, const Cache *c, u8 var, bool highlight){
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
					gpu.set(0, x+cx, cy+y, highlight?0x13:0x0);
					gpu.set(1, x+cx, cy+y, *map1);
					gpu.set(2, x+cx, cy+y, *map2);
					map0++;
					map1++;
					map2++;
				}
				map0+=64-c->itemwidth;
			}
		}
		
		void drawVerticalCache(u8 x, u8 y, const Cache *c, u8 var, bool highlight){
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
					gpu.set(0, x+cx, cy+y, highlight?0x13:*map0);
					gpu.set(1, x+cx, cy+y, *map1);
					gpu.set(2, x+cx, cy+y, *map2);
					map0++;
					map1++;
					map2++;
				}
				map0+=64 - c->itemwidth;
			}
		}
		
		void drawControl(const Control *c){
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
						gpu.set(0, x+cx, y, c==control?0x13:0x0);
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
		
		void drawDisplay(const Display *d){
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
						gpu.set(0, x+cx, y, 0x0);
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
		
		void updateViewport(const Viewport *v, u8 x, u8 y){
			if(!v)return;
			if(!v->var)return;
			if(!v->regions)return;
			
			if(v->regions[*v->var % v->count]->updater)
				v->regions[*v->var % v->count]->updater(this);
			
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
		
		void loadControls(const Region *r){			
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
				//dispatchCallback(control->callback, control, false, false, EVENT_FOCUS, NULL);
			}
		}
		
		void loadDisplays(const Region *r){
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

		void drawViewport(const Viewport *v, u8 x, u8 y, const Region *r){
			// Substract original region coordinates to viewport coordinates
			
			gpu.blit((E_Maps)region->mapIndex, r->x, r->y, r->xadd<0xFF?r->xadd:x, y, r->width, r->height);
		}

		void draw(void){
			Region *r = region;
			gpu.blit((E_Maps)r->mapIndex, r->x, r->y, 0,0, r->width, r->height);
	
			loadControls(r);
			loadDisplays(r);
			
		}
		
		void jumpToControl(const Control *c){
			if(VAR_LIVE.PERFORM.LOCK)return;
			if(!c)return;
			Control *o = control;
			control = (Control*)c;
			
			dispatchCallback(c->callback, c, false, false, EVENT_FOCUS, (u32*)this);
			sendMessage(MESSAGE_REDRAW_CONTROL | (unsigned)o);
			sendMessage(MESSAGE_REDRAW_CONTROL | (unsigned)control);
		}
		
		void controlTrigger(const Control *c, u16 q){
			if(!c->callback)return;
			dispatchCallback(c->callback, c, true, true, EVENT_KEYDOWN_B, (u32*)this);
			dispatchCallback(c->callback, c, true, true, EVENT_KEYUP_B, (u32*)this);
			/*control->callback(control, true, true, (u32*)this); */
			sendMessage(MESSAGE_REDRAW_CONTROL | (unsigned)c);
			
			
		}
		
		void controlModify(const Control *c, bool big, bool add){
			if(!c->callback)return;
			dispatchCallback(c->callback, c, add, big, EVENT_MODIFY_B, (u32*)this);
			/*control->modifier(control, big, add, (u32*)this); */
			sendMessage(MESSAGE_REDRAW_CONTROL | (unsigned)c);
		}
		
		void updateProgress(void){
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
		
		void update(u8 delta){
			if(redraw) draw();
			// Handle modal progress dialog, if enableds
			if(progress.enabled){
				updateProgress();
				return;
			}
			
			if(region->updater)region->updater(this);
			
			// If there is any viewport, it will override message queu and control list
			this->updateViewport(region->viewport, region->xadd, region->yadd);			
			
			//this allows a updater to detect wether or not the region has just been redrawn (useful on entry)
			redraw=false; //<--- on redraw, move back to that method if you notice any redraw bug or glitch. 
			
			dispatchMessages();
		}
};
extern RegionHandler regHnd;

