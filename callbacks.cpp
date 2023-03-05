/* ------------------------------------------------------------------------------------------------------------------------------------- */
/*                                                 SPECIFIC TYPE VARIABLE MODIFIERS														 */
/* ------------------------------------------------------------------------------------------------------------------------------------- */

#define CURRENT_PATTERN 		VAR_SONG.PATTERNS[VAR_CFG.CURRENTCHANNEL].ORDER[VAR_CHANNEL[VAR_CFG.CURRENTCHANNEL].POSITION]
#define VARIABLE	(*(u8*) c->var)
void modifyCHAR(Control *c, bool bigstep, bool add, u32 *pointer){}
void modify1BIT(Control *c, bool bigstep, bool add, u32 *pointer){	VARIABLE^=0x1; }
void modifyTempo(Control *c, bool bigstep, bool add, u32 *pointer){	modify8BIT(c, bigstep, add, pointer); Sequencer::setTempo( VAR_SONG.BPM ); }


static u8 transient2BIT;
static u8 transient3BIT;
static u8 transient4BIT;
static u8 transient5BIT;
static u8 transient6BIT;
static u8 transient7BIT;
static u8 transient8BIT;

static u8 transientNote;
static u8 transientValue;
static u8 transientVolume;
static u8 transientInstrument;
static u8 transientCommand;

static u8 transientChanged;

void Net::alterMode(Control *c, bool bigstep, bool add, u32 *pointer){	
	VARIABLE 	 	 = ( bigstep ? (add?0x3:0) : (VARIABLE + (add?1:-1)) ) & 0x3; 	
	transient2BIT 	 = VARIABLE; 
	transientChanged = true;
	setMode( (eNetMode) VARIABLE );
}

void modify2BIT(Control *c, bool bigstep, bool add, u32 *pointer){	VARIABLE = ( bigstep ? (add?0x3:0) : (VARIABLE + (add?1:-1)) ) & 0x3; 	transient2BIT = VARIABLE; transientChanged=true;}
void modify3BIT(Control *c, bool bigstep, bool add, u32 *pointer){	VARIABLE = ( bigstep ? (add?0x7:0) : (VARIABLE + (add?1:-1)) ) & 0x7; 	transient3BIT = VARIABLE; transientChanged=true;}
void modify4BIT(Control *c, bool bigstep, bool add, u32 *pointer){	VARIABLE = ( bigstep ? (add?0xF:0) : (VARIABLE + (add?1:-1)) ) & 0xF; 	transient4BIT = VARIABLE; transientChanged=true;}	
void modify5BIT(Control *c, bool bigstep, bool add, u32 *pointer){	VARIABLE = (VARIABLE + ((bigstep?0x4:0x1) * (add?1:-1))      ) & 0x1F;	transient5BIT = VARIABLE; transientChanged=true;}
void modify6BIT(Control *c, bool bigstep, bool add, u32 *pointer){	VARIABLE = (VARIABLE + ((bigstep?0x4:0x1) * (add?1:-1))      ) & 0x3F;	transient6BIT = VARIABLE; transientChanged=true;}
void modify7BIT(Control *c, bool bigstep, bool add, u32 *pointer){	VARIABLE = (VARIABLE + ((bigstep?0x4:0x1) * (add?1:-1))      ) & 0x7F;	transient7BIT = VARIABLE; transientChanged=true;}
void modify8BIT(Control *c, bool bigstep, bool add, u32 *pointer){	VARIABLE = (VARIABLE + ((bigstep?0x10:0x1)* (add?1:-1))      ) & 0xFF;	transient8BIT = VARIABLE; transientChanged=true;}


void paste2BIT(Control *c, bool bigstep, bool add, u32 *pointer){ if(VARIABLE == 0x00){ VARIABLE = transient2BIT; return; } transient2BIT = VARIABLE; transientChanged=true; }
void paste3BIT(Control *c, bool bigstep, bool add, u32 *pointer){ if(VARIABLE == 0x00){ VARIABLE = transient3BIT; return; } transient3BIT = VARIABLE; transientChanged=true; }
void paste4BIT(Control *c, bool bigstep, bool add, u32 *pointer){ if(VARIABLE == 0x00){ VARIABLE = transient4BIT; return; } transient4BIT = VARIABLE; transientChanged=true; }
void paste5BIT(Control *c, bool bigstep, bool add, u32 *pointer){ if(VARIABLE == 0x00){ VARIABLE = transient5BIT; return; } transient5BIT = VARIABLE; transientChanged=true; }
void paste6BIT(Control *c, bool bigstep, bool add, u32 *pointer){ if(VARIABLE == 0x00){ VARIABLE = transient6BIT; return; } transient6BIT = VARIABLE; transientChanged=true; }
void paste7BIT(Control *c, bool bigstep, bool add, u32 *pointer){ if(VARIABLE == 0x00){ VARIABLE = transient7BIT; return; } transient7BIT = VARIABLE; transientChanged=true; }
void paste8BIT(Control *c, bool bigstep, bool add, u32 *pointer){ if(VARIABLE == 0x00){ VARIABLE = transient8BIT; return; } transient7BIT = VARIABLE; transientChanged=true; }


void modify3VAL(Control *c, bool bigstep, bool add, u32 *pointer){	VARIABLE = ( bigstep ? (add?0x2:0) : (VARIABLE + (add?1:-1)) ) % 3; if(VARIABLE>2) VARIABLE = 2; 	}
void modify6VAL(Control *c, bool bigstep, bool add, u32 *pointer){  VARIABLE = ( bigstep ? (add?0x5:0) : (VARIABLE + (add?1:-1)) ) % 6;		}

void modify5VAL(Control *c, bool bigstep, bool add, u32 *pointer){  
	VARIABLE = (add ? (VARIABLE+1) : (VARIABLE-1)) % 5;	
	if(VARIABLE > 4)VARIABLE = 4;
}

void cellCopy(u8 channel);
void modifyCommand(Control *c, bool bigstep, bool add, u32 *pointer){
	if(CURRENT_PATTERN == 0x00)return;
	
	if(bigstep&!add)VARIABLE = 0;
	else {
		VARIABLE += (add?1:-1);
		if((VARIABLE > 26) && (VARIABLE < 0x80)) VARIABLE = 26;
		else if(VARIABLE > 0x26) VARIABLE = 0;
	}
	transientCommand = VARIABLE;
	transientChanged = true;
	
	cellCopy(VAR_CFG.CURRENTCHANNEL);
}
void modifyNote(Control *c, bool bigstep, bool add, u32 *pointer);


void modifyInst		(Control *c, bool bigstep, bool add, u32 *pointer){if(CURRENT_PATTERN == 0x00)return;VARIABLE = (VARIABLE + ((bigstep?0x4:0x1) * (add?1:-1))      ) & 0x3F;	transientInstrument = VARIABLE; transientChanged=true; cellCopy(VAR_CFG.CURRENTCHANNEL);}
void modifyVolume	(Control *c, bool bigstep, bool add, u32 *pointer){if(CURRENT_PATTERN == 0x00)return;VARIABLE = ( bigstep ? (add?0xF:0) : (VARIABLE + (add?1:-1)) ) & 0xF; 	transientVolume 	= VARIABLE; transientChanged=true; cellCopy(VAR_CFG.CURRENTCHANNEL);}	
void modifyValue	(Control *c, bool bigstep, bool add, u32 *pointer){if(CURRENT_PATTERN == 0x00)return;VARIABLE = (VARIABLE + ((bigstep?0x10:0x1)* (add?1:-1))      ) & 0xFF;	transientValue		= VARIABLE; transientChanged=true; cellCopy(VAR_CFG.CURRENTCHANNEL);}

void pasteCommand	(Control *c, bool bigstep, bool add, u32 *pointer){ if(VARIABLE == 0x00){if(CURRENT_PATTERN == 0x00)return; VARIABLE = transientCommand; 	cellCopy(VAR_CFG.CURRENTCHANNEL); return; } transientCommand 	= VARIABLE; transientChanged=true; }
void pasteInst		(Control *c, bool bigstep, bool add, u32 *pointer){ if(VARIABLE == 0x00){if(CURRENT_PATTERN == 0x00)return; VARIABLE = transientInstrument; cellCopy(VAR_CFG.CURRENTCHANNEL); return; } transientInstrument = VARIABLE; transientChanged=true; }
void pasteNote		(Control *c, bool bigstep, bool add, u32 *pointer){ if(VARIABLE == 0x00){if(CURRENT_PATTERN == 0x00)return; VARIABLE = transientNote; 		cellCopy(VAR_CFG.CURRENTCHANNEL); return; } transientNote 		= VARIABLE; transientChanged=true; }
void pasteVolume	(Control *c, bool bigstep, bool add, u32 *pointer){ if(VARIABLE == 0x00){if(CURRENT_PATTERN == 0x00)return; VARIABLE = transientVolume;		cellCopy(VAR_CFG.CURRENTCHANNEL); return; } transientVolume 	= VARIABLE; transientChanged=true; }
void pasteValue		(Control *c, bool bigstep, bool add, u32 *pointer){ if(VARIABLE == 0x00){if(CURRENT_PATTERN == 0x00)return; VARIABLE = transientValue; 		cellCopy(VAR_CFG.CURRENTCHANNEL); return; } transientValue 		= VARIABLE; transientChanged=true; }

#undef VARIABLE

/* ------------------------------------------------------------------------------------------------------------------------------------- */
/*                                               SPECIFIC TYPE DISPLAY / INPUT METHODS													 */
/* ------------------------------------------------------------------------------------------------------------------------------------- */

/* Draws given string on screen */
void STRING(bool big, u8 x, u8 y, u8 *data) {
	int i =0;
	int limit = big?14:6;
	while(data[i]!=0xFF){
		gpu.set(2, x+i, y, TABLE_TEXT[data[i]][0] | 0x4000);
		i++;
		if(i==limit)break;
	}
}

/* Invokes AlphaNumeric input modal dialog, and sets its return var */
// Also Sets regionHander in modal alphanumeric dialog mode, max string len 14
void ALPHA14(Control *c, bool bigstep, bool add, u32 *pointer){

	// TODO: Rename to getTitle and getArtist
	AlphaDialog a;
	a.enable(true, c->var, c->x, c->y);
	
	regHnd.redraw=true;
}

/* Invokes AlphaNumeric input modal dialog, and sets its return var */
// Also Sets regionHander in modal alphanumeric dialog mode, max string len 6
void ALPHA6(Control *c, bool bigstep, bool add, u32 *pointer){
	
	AlphaDialog a;
	a.enable(false, c->var, c->x, c->y);
	
	regHnd.redraw=true;
	
}

// Single 8x8 tile, 1 hexadecimal digit
void HEXADECIMAL(u8 x, u8 y, u16 color, u16 value) {
	value &=0xF;
	color <<= 12;
	gpu.set(2,x,y,color | **(TABLE_HEXADECIMAL+value));
}

// Single 8x8 tile, 2 hexadecimal digits
void HEXADECIMAL_DOUBLE(u8 x, u8 y, u16 color, u16 value) {
	value &=0xFF;
	color <<= 12;
	gpu.set(2,x,y,color | **(TABLE_HEXADECIMAL_DOUBLE+value));
}

// Double 8x8 tile, 1 hexadecimal digit each
void HEXADECIMAL_INSTRUMENT(u8 x, u8 y, u16 color, u16 value) {
	value &=0xFF;
	color <<= 12;
	gpu.set(2,x  ,y,color | **(TABLE_HEXADECIMAL+((value&0xF0)>>4)));
	gpu.set(2,x+1,y,color | **(TABLE_HEXADECIMAL+(value&0x0F)));
}

// Two 8x8 tiles, 1 hexadecimal digit each, different color on zero
void HEXADECIMAL_TWOTILES(u8 x, u8 y, u16 color, u16 value) {
	value &=0xFF;
	color = value>0? color << 12 : 0x9000;
	gpu.set(2,x  ,y,color | **(TABLE_HEXADECIMAL_TWOTILES+value));
	gpu.set(2,x+1,y,color | *(*(TABLE_HEXADECIMAL_TWOTILES+value)+1));
}

// Single 8x8 tile, 1 decimal numeric digit
void DECIMAL(u8 x, u8 y, u16 color, u16 value) {
	value %=10;
	color <<= 12;
	gpu.set(2,x,y,color | **(TABLE_DECIMAL+value));
}

// Single 8x8 tile displaying 2 decimal numeric digits
void DECIMAL_DOUBLE(u8 x, u8 y, u16 color, u16 value) {
	value %=100;
	color <<= 12;
	gpu.set(2,x,y,color | **(TABLE_DECIMAL_DOUBLE+value));
}

// Two 8x8 tiles displaying a decimal numeric digit each
void DECIMAL_TWOTILES(u8 x, u8 y, u16 color, u16 value) {
	value %=100;
	color <<= 12;
	gpu.set(2,x  ,y,color | **(TABLE_DECIMAL_TWOTILES+value));
	gpu.set(2,x+1,y,color | *(*(TABLE_DECIMAL_TWOTILES+value)+1));
}

// Double 8x8 tile displaying 4 decimal numeric digits
void DECIMAL_DOUBLE_TWOTILES(u8 x, u8 y, u16 color, u16 value) {
	value %=320;
	color <<= 12;
	gpu.set(2,x  ,y,color | **(TABLE_DECIMAL_DOUBLE_TWOTILES+value));
	gpu.set(2,x+1,y,color | *(*(TABLE_DECIMAL_DOUBLE_TWOTILES+value)+1));
}

// Double 8x8 tile displaying 4 binary digits
void NIBBLE(u8 x, u8 y, u16 color, u16 value) {
	value &=0xF;
	color <<= 12;
	gpu.set(2,x  ,y,color | **(TABLE_NIBBLE+value));
	gpu.set(2,x+1,y,color | *(*(TABLE_NIBBLE+value)+1));
}

// 9 Tile progress bar (36 steps)
void PROGRESS(u8 x, u8 y, u16 color, u16 value) {
	value %=36;
	color <<= 12;
	gpu.set(1,x  ,y, **(TABLE_PROGRESS_LINE+value));
	gpu.set(1,x+1,y, *(*(TABLE_PROGRESS_LINE+value)+1));
	gpu.set(1,x+2,y, *(*(TABLE_PROGRESS_LINE+value)+2));
	gpu.set(1,x+3,y, *(*(TABLE_PROGRESS_LINE+value)+3));
	gpu.set(1,x+4,y, *(*(TABLE_PROGRESS_LINE+value)+4));
	gpu.set(1,x+5,y, *(*(TABLE_PROGRESS_LINE+value)+5));
	gpu.set(1,x+6,y, *(*(TABLE_PROGRESS_LINE+value)+6));
	gpu.set(1,x+7,y, *(*(TABLE_PROGRESS_LINE+value)+7));		
	gpu.set(1,x+8,y, *(*(TABLE_PROGRESS_LINE+value)+8));
}

// Mirrored tile wave display (8 pixels wide/sample, 16 pixels height)
void WAVE_DOUBLE(u8 x, u8 y, u16 color, u16 value) {
	value &=0x3F;
	color <<= 12;
	
	gpu.set(1,x,y  , **(TABLE_WAVE_DOUBLE+value));
	gpu.set(1,x,y+1, *(*(TABLE_WAVE_DOUBLE+value)+1));
}

/* NOT USED */
void PIXEL(u8 x, u8 y, u16 color, u16 value) {
	//;nop
}

// Single tile wave display (8 pixels wide/sample)
void WAVE_SINGLE(u8 x, u8 y, u16 color, u16 value) {
	value %=49;
	color <<= 12;
	gpu.set(1,x,y  , **(TABLE_WAVE_SINGLE+value));
}

// 8x16 Big Hexadecimal Character
void BIGHEX(u8 x, u8 y, u16 color, u16 value) {
	value &=0xF;
	color <<= 12;
	gpu.set(2,x,y  , color | **(TABLE_BIGFONT_HEX+value));
	gpu.set(2,x,y+1, color | *(*(TABLE_BIGFONT_HEX+value)+1));
}

// 8x16 Big Numeric Character
void BIGDEC(u8 x, u8 y, u16 color, u16 value) {
	value %=10;
	color <<= 12;
	gpu.set(2,x,y  , color | **(TABLE_BIGFONT_HEX+value));
	gpu.set(2,x,y+1, color | *(*(TABLE_BIGFONT_HEX+value)+1));
}

// 8x16 Big Text Character
void BIGTEXT(u8 x, u8 y, u16 color, u16 value) {
	value %=40;
	color <<= 12;
	gpu.set(2,x,y  , color | **(TABLE_BIGFONT+value));
	gpu.set(2,x,y+1, color | *(*(TABLE_BIGFONT+value)+1));
}

// 8x8 Text Character
void TEXT(u8 x, u8 y, u16 color, u16 value) {
	return;
	
	value %=0x54;
	color <<= 12;
	color = 0x4000;
	
	gpu.set(2,x,y  , color | **(TABLE_TEXT+value));
}

// Triple tile Vertical VUMeter
void VUMETER_V3(u8 x, u8 y, u16 color, u16 value) {
	value %=13;
	color <<= 12;
	gpu.set(1,x,y  , **(TABLE_VU3_V+value));
	gpu.set(1,x,y+1, *(*(TABLE_VU3_V+value)+1));
	gpu.set(1,x,y+2, *(*(TABLE_VU3_V+value)+2));
}

// Triple tile Horizontal VUMeter
void VUMETER_H3(u8 x, u8 y, u16 color, u16 value) {
	value %=13;
	color <<= 12;
	gpu.set(1,x  ,y,**(TABLE_VU3_H+value));
	gpu.set(1,x+1,y,*(*(TABLE_VU3_H+value)+1));
	gpu.set(1,x+2,y,*(*(TABLE_VU3_H+value)+2));
}

// Single tile Horizontal VUMeter
void VUMETER_H1(u8 x, u8 y, u16 color, u16 value) {
	value &=7;
	color <<= 12;
	gpu.set(1,x  ,y,**(TABLE_VU1_H+value));
}

// Single tile Vertical VUMeter
void VUMETER_V1(u8 x, u8 y, u16 color, u16 value) {
	value &=7;
	color <<= 12;
	gpu.set(1,x  ,y,**(TABLE_VU1_V+value));
}
		
// Command type display function
void COMMAND(u8 x, u8 y, u16 color, u16 value) {
	value %=27;
	color = value?0x2000:0x9000;
	gpu.set(2,x,y , color | **(TABLE_COMMANDS+value));
}

// Note type display function
void NOTE(u8 x, u8 y, u16 color, u16 value) {
	value %=120;
	color = value?0x6000:0xD000;
	gpu.set(2,x  ,y, color | **(TABLE_NOTES+value));
	gpu.set(2,x+1,y, color | *(*(TABLE_NOTES+value)+1));
	gpu.set(2,x+2,y, color | *(*(TABLE_NOTES+value)+2));
}

// Vertical cursor for the instrument tables
void TABLEPOS(u8 x, u8 y, u16 color, u16 value) {
	value &=0xF;
	//color <<= 12;
	gpu.set(2,x,y, **(TABLE_TABLEPOSITION+value));
	gpu.set(2,x,y+1, *(*(TABLE_TABLEPOSITION+value)+1));
	gpu.set(2,x,y+2, *(*(TABLE_TABLEPOSITION+value)+2));
	gpu.set(2,x,y+3, *(*(TABLE_TABLEPOSITION+value)+3));
	gpu.set(2,x,y+4, *(*(TABLE_TABLEPOSITION+value)+4));
	gpu.set(2,x,y+5, *(*(TABLE_TABLEPOSITION+value)+5));
	gpu.set(2,x,y+6, *(*(TABLE_TABLEPOSITION+value)+6));
	gpu.set(2,x,y+7, *(*(TABLE_TABLEPOSITION+value)+7));
	gpu.set(2,x,y+8, *(*(TABLE_TABLEPOSITION+value)+8));
	gpu.set(2,x,y+9, *(*(TABLE_TABLEPOSITION+value)+9));
	gpu.set(2,x,y+10,*(*(TABLE_TABLEPOSITION+value)+10));
	gpu.set(2,x,y+11,*(*(TABLE_TABLEPOSITION+value)+11));
	gpu.set(2,x,y+12,*(*(TABLE_TABLEPOSITION+value)+12));
	gpu.set(2,x,y+13,*(*(TABLE_TABLEPOSITION+value)+13));
	gpu.set(2,x,y+14,*(*(TABLE_TABLEPOSITION+value)+14));
	gpu.set(2,x,y+15,*(*(TABLE_TABLEPOSITION+value)+15));
}

// Upper instrument arrow cursor (16 steps)
void VISPOS1(u8 x, u8 y, u16 color, u16 value) {
	value &=0xF;
	//color <<= 12;
	gpu.set(2,x,y,   **(TABLE_VISPOSITION1+value));
	gpu.set(2,x+1,y, *(*(TABLE_VISPOSITION1+value)+1));
	gpu.set(2,x+2,y, *(*(TABLE_VISPOSITION1+value)+2));
	gpu.set(2,x+3,y, *(*(TABLE_VISPOSITION1+value)+3));
	gpu.set(2,x+4,y, *(*(TABLE_VISPOSITION1+value)+4));
	gpu.set(2,x+5,y, *(*(TABLE_VISPOSITION1+value)+5));
	gpu.set(2,x+6,y, *(*(TABLE_VISPOSITION1+value)+6));
	gpu.set(2,x+7,y, *(*(TABLE_VISPOSITION1+value)+7));
	gpu.set(2,x+8,y, *(*(TABLE_VISPOSITION1+value)+8));
	gpu.set(2,x+9,y, *(*(TABLE_VISPOSITION1+value)+9));
	gpu.set(2,x+10,y,*(*(TABLE_VISPOSITION1+value)+10));
	gpu.set(2,x+11,y,*(*(TABLE_VISPOSITION1+value)+11));
	gpu.set(2,x+12,y,*(*(TABLE_VISPOSITION1+value)+12));
	gpu.set(2,x+13,y,*(*(TABLE_VISPOSITION1+value)+13));
	gpu.set(2,x+14,y,*(*(TABLE_VISPOSITION1+value)+14));
	gpu.set(2,x+15,y,*(*(TABLE_VISPOSITION1+value)+15));
}

// Lower instrument arrow cursor (16 steps)
void VISPOS2(u8 x, u8 y, u16 color, u16 value) {
	value &=0xF;
	//color <<= 12;
	gpu.set(2,x,y,   **(TABLE_VISPOSITION2+value));
	gpu.set(2,x+1,y, *(*(TABLE_VISPOSITION2+value)+1));
	gpu.set(2,x+2,y, *(*(TABLE_VISPOSITION2+value)+2));
	gpu.set(2,x+3,y, *(*(TABLE_VISPOSITION2+value)+3));
	gpu.set(2,x+4,y, *(*(TABLE_VISPOSITION2+value)+4));
	gpu.set(2,x+5,y, *(*(TABLE_VISPOSITION2+value)+5));
	gpu.set(2,x+6,y, *(*(TABLE_VISPOSITION2+value)+6));
	gpu.set(2,x+7,y, *(*(TABLE_VISPOSITION2+value)+7));
	gpu.set(2,x+8,y, *(*(TABLE_VISPOSITION2+value)+8));
	gpu.set(2,x+9,y, *(*(TABLE_VISPOSITION2+value)+9));
	gpu.set(2,x+10,y,*(*(TABLE_VISPOSITION2+value)+10));
	gpu.set(2,x+11,y,*(*(TABLE_VISPOSITION2+value)+11));
	gpu.set(2,x+12,y,*(*(TABLE_VISPOSITION2+value)+12));
	gpu.set(2,x+13,y,*(*(TABLE_VISPOSITION2+value)+13));
	gpu.set(2,x+14,y,*(*(TABLE_VISPOSITION2+value)+14));
	gpu.set(2,x+15,y,*(*(TABLE_VISPOSITION2+value)+15));
}

void STATUS(u8 x, u8 y, u16 color, u16 value) {
	value %=21;
	color <<= 12;
	gpu.set(2,x  ,y  , color | **(TABLE_STATUS_STRINGS+value));
	gpu.set(2,x+1,y  , color | *(*(TABLE_STATUS_STRINGS+value)+1));
	gpu.set(2,x+2,y  , color | *(*(TABLE_STATUS_STRINGS+value)+2));
	gpu.set(2,x+3,y  , color | *(*(TABLE_STATUS_STRINGS+value)+3));
	gpu.set(2,x+4,y  , color | *(*(TABLE_STATUS_STRINGS+value)+4));
	gpu.set(2,x+5,y  , color | *(*(TABLE_STATUS_STRINGS+value)+5));
	gpu.set(2,x+6,y  , color | *(*(TABLE_STATUS_STRINGS+value)+6));
	gpu.set(2,x+7,y  , color | *(*(TABLE_STATUS_STRINGS+value)+7));
	gpu.set(2,x+8,y  , color | *(*(TABLE_STATUS_STRINGS+value)+8));
	gpu.set(2,x  ,y+1, color | *(*(TABLE_STATUS_STRINGS+value)+9));
	gpu.set(2,x+1,y+1, color | *(*(TABLE_STATUS_STRINGS+value)+10));
	gpu.set(2,x+2,y+1, color | *(*(TABLE_STATUS_STRINGS+value)+11));
	gpu.set(2,x+3,y+1, color | *(*(TABLE_STATUS_STRINGS+value)+12));
	gpu.set(2,x+4,y+1, color | *(*(TABLE_STATUS_STRINGS+value)+13));
	gpu.set(2,x+5,y+1, color | *(*(TABLE_STATUS_STRINGS+value)+14));
	gpu.set(2,x+6,y+1, color | *(*(TABLE_STATUS_STRINGS+value)+15));
	gpu.set(2,x+7,y+1, color | *(*(TABLE_STATUS_STRINGS+value)+16));
	gpu.set(2,x+8,y+1, color | *(*(TABLE_STATUS_STRINGS+value)+17));
}

/* ------------------------------------------------------------------------------------------------------------------------------------- */
/*                                               EXCLUSIVE SCREEN CALLBACKS 															 */
/* ------------------------------------------------------------------------------------------------------------------------------------- */
#define CALLBACK(n, c, t, v, nx)			const Callback n = { c , t , v, nx}

CALLBACK( cb_no_callback	, NULL			, 0x0000 /* No key presses*/, NULL /* No var */		, NULL /* Last element */	);
CALLBACK( cb_reset			, RESET			, EVENT_PANIC 				, NULL 					, NULL 						);

#include "callbacks/debug.cpp"
#include "callbacks/hlp.cpp"
#include "callbacks/ins.cpp"
#include "callbacks/pat.cpp"
#include "callbacks/trk.cpp"
#include "callbacks/cfg.cpp"
#include "callbacks/sng.cpp"
#include "callbacks/snk.cpp"
#include "callbacks/liv1.cpp"
#include "callbacks/liv2.cpp"

#undef CALLBACK	
