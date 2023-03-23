#include "control.hpp"
#include "../kernel/spu/sequencer.hpp"
#include "../screens/tracker.hpp"
#include "data.hpp"

u8 sTransient::bit1;
u8 sTransient::bit2;
u8 sTransient::bit3;
u8 sTransient::bit4;
u8 sTransient::bit5;
u8 sTransient::bit6;
u8 sTransient::bit7;
u8 sTransient::bit8;
u8 sTransient::note;
u8 sTransient::value;
u8 sTransient::volume;
u8 sTransient::instrument;
u8 sTransient::command;
u8 sTransient::changed;

/* GENERIC CONTROL CALLBACKS */ 

#define VARIABLE			(*(u8*) c->var)
void modify1BIT	(Control *c, bool bigstep, bool add, u32 *pointer){	VARIABLE^=0x1; Transient::bit1 = VARIABLE; Transient::changed = true; }
void modify2BIT	(Control *c, bool bigstep, bool add, u32 *pointer){	VARIABLE = ( bigstep ? ( add	?0x03:0x0) : (VARIABLE + (add?1:-1)) ) & 0x03; Transient::bit2 = VARIABLE; Transient::changed = true; }
void modify3BIT	(Control *c, bool bigstep, bool add, u32 *pointer){	VARIABLE = ( bigstep ? ( add	?0x07:0x0) : (VARIABLE + (add?1:-1)) ) & 0x07; Transient::bit3 = VARIABLE; Transient::changed = true; }
void modify4BIT	(Control *c, bool bigstep, bool add, u32 *pointer){	VARIABLE = ( bigstep ? ( add	?0x0F:0x0) : (VARIABLE + (add?1:-1)) ) & 0x0F; Transient::bit4 = VARIABLE; Transient::changed = true; }	
void modify5BIT	(Control *c, bool bigstep, bool add, u32 *pointer){	VARIABLE = (VARIABLE + ((bigstep?0x04:0x1) * ( add ? 1 : -1 ) )      ) & 0x1F; Transient::bit5 = VARIABLE; Transient::changed = true; }
void modify6BIT	(Control *c, bool bigstep, bool add, u32 *pointer){	VARIABLE = (VARIABLE + ((bigstep?0x04:0x1) * ( add ? 1 : -1 ) )      ) & 0x3F; Transient::bit6 = VARIABLE; Transient::changed = true; }
void modify7BIT	(Control *c, bool bigstep, bool add, u32 *pointer){	VARIABLE = (VARIABLE + ((bigstep?0x04:0x1) * ( add ? 1 : -1 ) )      ) & 0x7F; Transient::bit7 = VARIABLE; Transient::changed = true; }
void modify8BIT	(Control *c, bool bigstep, bool add, u32 *pointer){	VARIABLE = (VARIABLE + ((bigstep?0x10:0x1) * ( add ? 1 : -1 ) )      ) & 0xFF; Transient::bit8 = VARIABLE; Transient::changed = true; }
void modifyCHAR	(Control *c, bool bigstep, bool add, u32 *pointer){ }
void modifyTempo(Control *c, bool bigstep, bool add, u32 *pointer){	modify8BIT(c, bigstep, add, pointer); Sequencer::setTempo( VAR_SONG.BPM ); }

void modifyValue(Control *c, bool bigstep, bool add, u32 *pointer, u8 quant){	
	VARIABLE = ( bigstep ? (add ? quant - 1 : 0) : (VARIABLE + (add ? 1 : -1 )) ) % quant; 
	if( VARIABLE > quant - 1 ) VARIABLE = quant - 1; 	
}	
void modify3VAL (Control *c, bool bigstep, bool add, u32 *pointer){ return modifyValue(c, bigstep, add, pointer,  3); }
void modify5VAL (Control *c, bool bigstep, bool add, u32 *pointer){ return modifyValue(c, bigstep, add, pointer,  5); }
void modify6VAL (Control *c, bool bigstep, bool add, u32 *pointer){ return modifyValue(c, bigstep, add, pointer,  6); }
void modify27VAL(Control *c, bool bigstep, bool add, u32 *pointer){ return modifyValue(c, bigstep, add, pointer, 27); }

void modifyCommand (Control *c, bool bigstep, bool add, u32 *pointer){
	if(CURRENT_PATTERN == 0x00)return;
	
	if( bigstep &! add) VARIABLE = 0; else {
		VARIABLE += (add?1:-1);
		if((VARIABLE > 26) && (VARIABLE < 0x80)) VARIABLE = 26;
		else if(VARIABLE > 0x26) VARIABLE = 0;
	}
	Transient::command = VARIABLE;
	Transient::changed = true;
	
	Tracker::copyChannel( VAR_CFG.CURRENTCHANNEL );
}

void modifyInst ( Control *c, bool bigstep, bool add, u32 *pointer ){
	if( CURRENT_PATTERN == 0x00 ) return;
	VARIABLE 			= (VARIABLE + ((bigstep?0x4:0x1) * (add?1:-1)) ) & 0x3F;	
	Transient::instrument= VARIABLE; 
	Transient::changed	= true; 
	if( VARIABLE == 0 ) VARIABLE = 1;
	Tracker::copyChannel( VAR_CFG.CURRENTCHANNEL ); 
}

void modifyVolume (Control *c, bool bigstep, bool add, u32 *pointer){
	if(CURRENT_PATTERN == 0x00) return;
	VARIABLE 			= ( bigstep ? (add?0xF:0) : (VARIABLE + (add?1:-1)) ) & 0xF; 	
	Transient::volume 	= VARIABLE; 
	Transient::changed 	= true; 
	Tracker::copyChannel( VAR_CFG.CURRENTCHANNEL ); 
}	

void modifyValue (Control *c, bool bigstep, bool add, u32 *pointer){
	if(CURRENT_PATTERN == 0x00) return;
	VARIABLE 			= (VARIABLE + ((bigstep?0x10:0x1)* (add?1:-1)) ) & 0xFF;
	Transient::value		= VARIABLE; 
	Transient::changed 	= true; 
	Tracker::copyChannel( VAR_CFG.CURRENTCHANNEL ); 
}

void paste2BIT(Control *c, bool bigstep, bool add, u32 *pointer){ if(VARIABLE == 0x00){ VARIABLE = Transient::bit2; return; } Transient::bit2 = VARIABLE; Transient::changed = true; }
void paste3BIT(Control *c, bool bigstep, bool add, u32 *pointer){ if(VARIABLE == 0x00){ VARIABLE = Transient::bit3; return; } Transient::bit3 = VARIABLE; Transient::changed = true; }
void paste4BIT(Control *c, bool bigstep, bool add, u32 *pointer){ if(VARIABLE == 0x00){ VARIABLE = Transient::bit4; return; } Transient::bit4 = VARIABLE; Transient::changed = true; }
void paste5BIT(Control *c, bool bigstep, bool add, u32 *pointer){ if(VARIABLE == 0x00){ VARIABLE = Transient::bit5; return; } Transient::bit5 = VARIABLE; Transient::changed = true; }
void paste6BIT(Control *c, bool bigstep, bool add, u32 *pointer){ if(VARIABLE == 0x00){ VARIABLE = Transient::bit6; return; } Transient::bit6 = VARIABLE; Transient::changed = true; }
void paste7BIT(Control *c, bool bigstep, bool add, u32 *pointer){ if(VARIABLE == 0x00){ VARIABLE = Transient::bit7; return; } Transient::bit7 = VARIABLE; Transient::changed = true; }
void paste8BIT(Control *c, bool bigstep, bool add, u32 *pointer){ if(VARIABLE == 0x00){ VARIABLE = Transient::bit8; return; } Transient::bit7 = VARIABLE; Transient::changed = true; }

void pasteCommand (Control *c, bool bigstep, bool add, u32 *pointer){ 
	if(VARIABLE == 0x00){
		if(CURRENT_PATTERN == 0x00) return; 
		VARIABLE = Transient::command; 		
		Tracker::copyChannel( VAR_CFG.CURRENTCHANNEL ); 
		return; 
	} 
	Transient::command = VARIABLE; 
	Transient::changed = true; 
}

void pasteInst (Control *c, bool bigstep, bool add, u32 *pointer){ 
	if(VARIABLE == 0x00){
		if(CURRENT_PATTERN == 0x00)return; 
		VARIABLE = Transient::instrument; 	
		Tracker::copyChannel( VAR_CFG.CURRENTCHANNEL ); 
		return; 
	} 
	Transient::instrument = VARIABLE; 
	Transient::changed 	 = true; 
}

void pasteNote (Control *c, bool bigstep, bool add, u32 *pointer){ 
	if(VARIABLE == 0x00){
		if(CURRENT_PATTERN == 0x00) return; 
		VARIABLE = Transient::note; 			
		Tracker::copyChannel( VAR_CFG.CURRENTCHANNEL ); 
		return; 
	} 
	Transient::note 		= VARIABLE; 
	Transient::changed 	= true; 
}

void pasteVolume (Control *c, bool bigstep, bool add, u32 *pointer){ 
	if(VARIABLE == 0x00){
		if(CURRENT_PATTERN == 0x00) return; 
		VARIABLE = Transient::volume;
		Tracker::copyChannel( VAR_CFG.CURRENTCHANNEL );
		return; 
	} 
	Transient::volume	= VARIABLE; 
	Transient::changed   = true;
}

void pasteValue	(Control *c, bool bigstep, bool add, u32 *pointer){ 
	if(VARIABLE == 0x00){
		if(CURRENT_PATTERN == 0x00) return; 
		VARIABLE = Transient::value;
		Tracker::copyChannel( VAR_CFG.CURRENTCHANNEL ); 
		return; 
	} 
	Transient::value 	= VARIABLE; 
	Transient::changed 	= true; 
}
#undef VARIABLE
