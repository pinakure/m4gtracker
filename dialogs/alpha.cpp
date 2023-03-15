#include "alpha.hpp"
#include "../modules/gpu/gpu.hpp"
#include "../modules/key/key.hpp"
#include "../modules/spu/sequencer.hpp"
#include "../modules/regionhandler/regionhandler.hpp"

u8 		AlphaDialog::buffer[16] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
u8 		AlphaDialog::maxlen		= 0;	
u8 		AlphaDialog::length		= 0;
u8 		AlphaDialog::position	= 0;
u8 		AlphaDialog::cursorX	= 0;
u8 		AlphaDialog::cursorY	= 0;
u8 		AlphaDialog::textX		= 0;
u8 		AlphaDialog::textY		= 0;
u8 		AlphaDialog::oldX		= 0;
u8 		AlphaDialog::oldY		= 0;
u8 		AlphaDialog::Y			= 0;
u16 	AlphaDialog::oldvalue	= 0;
u8*		AlphaDialog::target		= NULL;
bool 	AlphaDialog::enabled	= false;
bool 	AlphaDialog::redraw		= false;

/* Invokes AlphaNumeric input modal dialog, and sets its return var */
// Also Sets regionHander in modal alphanumeric dialog mode, max string len 14
void AlphaDialog::getBigString(Control *c, bool bigstep, bool add, u32 *pointer){
	AlphaDialog::enable(true, c->var, c->x, c->y);
	KEYFORCENOINPUT();
	regHnd.redraw=true;
}

/* Invokes AlphaNumeric input modal dialog, and sets its return var */
// Also Sets regionHander in modal alphanumeric dialog mode, max string len 6
void AlphaDialog::getString(Control *c, bool bigstep, bool add, u32 *pointer){
	AlphaDialog::enable(false, c->var, c->x, c->y);
	KEYFORCENOINPUT();
	regHnd.redraw=true;
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
	} else cancel();
}

void AlphaDialog::draw(){
	Gpu::blit(MAP_CACHE, 0x0, 0x0, 0x0, Y, 0x08, 0x8);
	
	
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

	Gpu::set(0, textX+lastPosition, textY, 0x0);	
	Gpu::set(0, textX+(position>0?position-1:position), textY, 0x13);	

	lastPosition=position-1;
	
	for(i=0; i<maxlen; i++){
		if(buffer[i]==0) Gpu::set(2, textX+i, textY, TABLE_TEXT[0x20][0] | 0xa000);
		else Gpu::set(2, textX+i, textY, ((TABLE_TEXT[buffer[i]][0])-1) | 0x4000);
	}
}

void AlphaDialog::update(){
	// If you want this to be multitask friendly, remove this while loop and put 'if(alphadialog->enabled)alphadialog->update();		
	while(enabled){
		if(redraw)draw();
		
		Sequencer::update();	
		KEYUPDATE();
	
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
			u16 value = Gpu::get(2, 1+cursorX, Y+1+cursorY)&0x0fff; 
			Gpu::set(2, 1+cursorX, Y+1+cursorY, value | 0x2000);
			Gpu::set(2, 1+oldX, Y+1+oldY, oldvalue | 0x9000);
			oldvalue = value;
			
			oldX = cursorX;
			oldY = cursorY;
		}
	}
}

void AlphaDialog::enable(bool bigString, u8* targetVariable, u8 vx, u8 vy){
	memset( &buffer, 0, sizeof(u8)*16);
	enabled = false;
	redraw 	= false;

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
	
	return update();
}
