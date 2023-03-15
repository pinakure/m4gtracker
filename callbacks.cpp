#include "callbacks/cfg.hpp"
/* ------------------------------------------------------------------------------------------------------------------------------------- */
/*                                               SPECIFIC TYPE DISPLAY / INPUT METHODS													 */
/* ------------------------------------------------------------------------------------------------------------------------------------- */

/* Draws given string on screen */
void STRING(bool big, u8 x, u8 y, u8 *data) {
	int i =0;
	int limit = big?14:6;
	while(data[i]!=0xFF){
		Gpu::set(2, x+i, y, TABLE_TEXT[data[i]][0] | 0x4000);
		i++;
		if(i==limit)break;
	}
}

// Single 8x8 tile, 1 hexadecimal digit
void HEXADECIMAL(u8 x, u8 y, u16 color, u16 value) {
	value &=0xF;
	color <<= 12;
	Gpu::set(2,x,y,color | **(TABLE_HEXADECIMAL+value));
}

// Single 8x8 tile, 2 hexadecimal digits
void HEXADECIMAL_DOUBLE(u8 x, u8 y, u16 color, u16 value) {
	value &=0xFF;
	color <<= 12;
	Gpu::set(2,x,y,color | **(TABLE_HEXADECIMAL_DOUBLE+value));
}

static const u8 darker_color[16] = {
	COLOR_NONE		 , //COLOR_NONE,// Black
	COLOR_DARK_BLUE	 , //COLOR_DARK_CYAN,
	COLOR_DARK_CYAN	 , //COLOR_CYAN,
	COLOR_DARK_RED	 , //COLOR_RED,
	COLOR_BROWN		 , //COLOR_ORANGE,
	COLOR_DARK_RED	 , //COLOR_DARK_RED,
	COLOR_GRAY		 , //COLOR_WHITE,
	COLOR_OLIVE		 , //COLOR_YELLOW,
	COLOR_NONE		 , //COLOR_BLACK,//Transparent...
	COLOR_DARK_BLUE	 , //COLOR_DARK_BLUE,
	COLOR_DARK_RED	 , //COLOR_BROWN,
	COLOR_DARK_BLUE	 , //COLOR_OLIVE,
	COLOR_DARK_GREEN , //COLOR_DARK_GREEN,
	COLOR_DARK_BLUE	 , //COLOR_GRAY,
	COLOR_DARK_CYAN	 , //COLOR_BLUE,
	COLOR_DARK_GREEN , //COLOR_GREEN,
};

// Single 8x8 tile, 2 hexadecimal digits, but 0x00 is represented as a single '--' character
void HEXADECIMAL_DOUBLE_DASH(u8 x, u8 y, u16 color, u16 value) {
	value &=0xFF;
	color = COLOR_ORANGE;
	// if(!value) return Gpu::set(2,x,y, (darker_color[ color ]<<12) | 0x00B1);
	if(!value) return Gpu::set(2,x,y, (COLOR_DARK_BLUE<<12) | 0x00B1);
	Gpu::set(2,x,y, (color<<12) | ((value>0)?**(TABLE_HEXADECIMAL_DOUBLE+value):0xB1));
}

// Double 8x8 tile, 1 hexadecimal digit each
void HEXADECIMAL_INSTRUMENT(u8 x, u8 y, u16 color, u16 value) {
	value &=0xFF;
	color <<= 12;
	Gpu::set(2,x  ,y,color | **(TABLE_HEXADECIMAL+((value&0xF0)>>4)));
	Gpu::set(2,x+1,y,color | **(TABLE_HEXADECIMAL+(value&0x0F)));
}

// Two 8x8 tiles, 1 hexadecimal digit each, different color on zero
void HEXADECIMAL_TWOTILES(u8 x, u8 y, u16 color, u16 value) {
	value &=0xFF;
	color = value>0? color << 12 : 0x9000;
	Gpu::set(2,x  ,y,color | **(TABLE_HEXADECIMAL_TWOTILES+value));
	Gpu::set(2,x+1,y,color | *(*(TABLE_HEXADECIMAL_TWOTILES+value)+1));
}

// Single 8x8 tile, 1 decimal numeric digit
void DECIMAL(u8 x, u8 y, u16 color, u16 value) {
	value %=10;
	color <<= 12;
	Gpu::set(2,x,y,color | **(TABLE_DECIMAL+value));
}

// Single 8x8 tile displaying 2 decimal numeric digits
void DECIMAL_DOUBLE(u8 x, u8 y, u16 color, u16 value) {
	value %=100;
	color <<= 12;
	Gpu::set(2,x,y,color | **(TABLE_DECIMAL_DOUBLE+value));
}

// Two 8x8 tiles displaying a decimal numeric digit each
void DECIMAL_TWOTILES(u8 x, u8 y, u16 color, u16 value) {
	value %=100;
	color <<= 12;
	Gpu::set(2,x  ,y,color | **(TABLE_DECIMAL_TWOTILES+value));
	Gpu::set(2,x+1,y,color | *(*(TABLE_DECIMAL_TWOTILES+value)+1));
}

// Double 8x8 tile displaying 4 decimal numeric digits
void DECIMAL_DOUBLE_TWOTILES(u8 x, u8 y, u16 color, u16 value) {
	value %=320;
	color <<= 12;
	Gpu::set(2,x  ,y,color | **(TABLE_DECIMAL_DOUBLE_TWOTILES+value));
	Gpu::set(2,x+1,y,color | *(*(TABLE_DECIMAL_DOUBLE_TWOTILES+value)+1));
}

// Double 8x8 tile displaying 4 binary digits
void NIBBLE(u8 x, u8 y, u16 color, u16 value) {
	value &=0xF;
	color <<= 12;
	Gpu::set(2,x  ,y,color | **(TABLE_NIBBLE+value));
	Gpu::set(2,x+1,y,color | *(*(TABLE_NIBBLE+value)+1));
}

// 9 Tile progress bar (36 steps)
void PROGRESS(u8 x, u8 y, u16 color, u16 value) {
	value %=36;
	color <<= 12;
	Gpu::set(1,x  ,y, **(TABLE_PROGRESS_LINE+value));
	Gpu::set(1,x+1,y, *(*(TABLE_PROGRESS_LINE+value)+1));
	Gpu::set(1,x+2,y, *(*(TABLE_PROGRESS_LINE+value)+2));
	Gpu::set(1,x+3,y, *(*(TABLE_PROGRESS_LINE+value)+3));
	Gpu::set(1,x+4,y, *(*(TABLE_PROGRESS_LINE+value)+4));
	Gpu::set(1,x+5,y, *(*(TABLE_PROGRESS_LINE+value)+5));
	Gpu::set(1,x+6,y, *(*(TABLE_PROGRESS_LINE+value)+6));
	Gpu::set(1,x+7,y, *(*(TABLE_PROGRESS_LINE+value)+7));		
	Gpu::set(1,x+8,y, *(*(TABLE_PROGRESS_LINE+value)+8));
}

// Mirrored tile wave display (8 pixels wide/sample, 16 pixels height)
void WAVE_DOUBLE(u8 x, u8 y, u16 color, u16 value) {
	value &=0x3F;
	color <<= 12;
	
	Gpu::set(1,x,y  , **(TABLE_WAVE_DOUBLE+value));
	Gpu::set(1,x,y+1, *(*(TABLE_WAVE_DOUBLE+value)+1));
}

/* NOT USED */
void PIXEL(u8 x, u8 y, u16 color, u16 value) {
	//;nop
}

// Single tile wave display (8 pixels wide/sample)
void WAVE_SINGLE(u8 x, u8 y, u16 color, u16 value) {
	value %=49;
	color <<= 12;
	Gpu::set(1,x,y  , **(TABLE_WAVE_SINGLE+value));
}

// 8x16 Big Hexadecimal Character
void BIGHEX(u8 x, u8 y, u16 color, u16 value) {
	value &=0xF;
	color <<= 12;
	Gpu::set(2,x,y  , color | **(TABLE_BIGFONT_HEX+value));
	Gpu::set(2,x,y+1, color | *(*(TABLE_BIGFONT_HEX+value)+1));
}

// 8x16 Big Numeric Character
void BIGDEC(u8 x, u8 y, u16 color, u16 value) {
	value %=10;
	color <<= 12;
	Gpu::set(2,x,y  , color | **(TABLE_BIGFONT_HEX+value));
	Gpu::set(2,x,y+1, color | *(*(TABLE_BIGFONT_HEX+value)+1));
}

// 8x16 Big Text Character
void BIGTEXT(u8 x, u8 y, u16 color, u16 value) {
	value %=40;
	color <<= 12;
	Gpu::set(2,x,y  , color | **(TABLE_BIGFONT+value));
	Gpu::set(2,x,y+1, color | *(*(TABLE_BIGFONT+value)+1));
}

// 8x8 Text Character
void TEXT(u8 x, u8 y, u16 color, u16 value) {
	return;
	
	value %=0x54;
	color <<= 12;
	color = 0x4000;
	
	Gpu::set(2,x,y  , color | **(TABLE_TEXT+value));
}

// Triple tile Vertical VUMeter
void VUMETER_V3(u8 x, u8 y, u16 color, u16 value) {
	value %=13;
	color <<= 12;
	Gpu::set(1,x,y  , **(TABLE_VU3_V+value));
	Gpu::set(1,x,y+1, *(*(TABLE_VU3_V+value)+1));
	Gpu::set(1,x,y+2, *(*(TABLE_VU3_V+value)+2));
}

// Triple tile Horizontal VUMeter
void VUMETER_H3(u8 x, u8 y, u16 color, u16 value) {
	value %=13;
	color <<= 12;
	Gpu::set(1,x  ,y,**(TABLE_VU3_H+value));
	Gpu::set(1,x+1,y,*(*(TABLE_VU3_H+value)+1));
	Gpu::set(1,x+2,y,*(*(TABLE_VU3_H+value)+2));
}

// Single tile Horizontal VUMeter
void VUMETER_H1(u8 x, u8 y, u16 color, u16 value) {
	value &=7;
	color <<= 12;
	Gpu::set(1,x  ,y,**(TABLE_VU1_H+value));
}

// Single tile Vertical VUMeter
void VUMETER_V1(u8 x, u8 y, u16 color, u16 value) {
	value &=7;
	color <<= 12;
	Gpu::set(1,x  ,y,**(TABLE_VU1_V+value));
}
		
// Command type display function
void COMMAND(u8 x, u8 y, u16 color, u16 value) {
	value %=27;
	color = value?0x2000:0x9000;
	Gpu::set(2,x,y , color | **(TABLE_COMMANDS+value));
}

// Note type display function
void NOTE(u8 x, u8 y, u16 color, u16 value) {
	value %=120;
	color = value?0x6000:0xD000;
	Gpu::set(2,x  ,y, color | **(TABLE_NOTES+value));
	Gpu::set(2,x+1,y, color | *(*(TABLE_NOTES+value)+1));
	Gpu::set(2,x+2,y, color | *(*(TABLE_NOTES+value)+2));
}

// Vertical cursor for the instrument tables
void TABLEPOS(u8 x, u8 y, u16 color, u16 value) {
	value &=0xF;
	//color <<= 12;
	Gpu::set(2,x,y, **(TABLE_TABLEPOSITION+value));
	Gpu::set(2,x,y+1, *(*(TABLE_TABLEPOSITION+value)+1));
	Gpu::set(2,x,y+2, *(*(TABLE_TABLEPOSITION+value)+2));
	Gpu::set(2,x,y+3, *(*(TABLE_TABLEPOSITION+value)+3));
	Gpu::set(2,x,y+4, *(*(TABLE_TABLEPOSITION+value)+4));
	Gpu::set(2,x,y+5, *(*(TABLE_TABLEPOSITION+value)+5));
	Gpu::set(2,x,y+6, *(*(TABLE_TABLEPOSITION+value)+6));
	Gpu::set(2,x,y+7, *(*(TABLE_TABLEPOSITION+value)+7));
	Gpu::set(2,x,y+8, *(*(TABLE_TABLEPOSITION+value)+8));
	Gpu::set(2,x,y+9, *(*(TABLE_TABLEPOSITION+value)+9));
	Gpu::set(2,x,y+10,*(*(TABLE_TABLEPOSITION+value)+10));
	Gpu::set(2,x,y+11,*(*(TABLE_TABLEPOSITION+value)+11));
	Gpu::set(2,x,y+12,*(*(TABLE_TABLEPOSITION+value)+12));
	Gpu::set(2,x,y+13,*(*(TABLE_TABLEPOSITION+value)+13));
	Gpu::set(2,x,y+14,*(*(TABLE_TABLEPOSITION+value)+14));
	Gpu::set(2,x,y+15,*(*(TABLE_TABLEPOSITION+value)+15));
}

// Upper instrument arrow cursor (16 steps)
void VISPOS1(u8 x, u8 y, u16 color, u16 value) {
	value &=0xF;
	//color <<= 12;
	Gpu::set(2,x,y,   **(TABLE_VISPOSITION1+value));
	Gpu::set(2,x+1,y, *(*(TABLE_VISPOSITION1+value)+1));
	Gpu::set(2,x+2,y, *(*(TABLE_VISPOSITION1+value)+2));
	Gpu::set(2,x+3,y, *(*(TABLE_VISPOSITION1+value)+3));
	Gpu::set(2,x+4,y, *(*(TABLE_VISPOSITION1+value)+4));
	Gpu::set(2,x+5,y, *(*(TABLE_VISPOSITION1+value)+5));
	Gpu::set(2,x+6,y, *(*(TABLE_VISPOSITION1+value)+6));
	Gpu::set(2,x+7,y, *(*(TABLE_VISPOSITION1+value)+7));
	Gpu::set(2,x+8,y, *(*(TABLE_VISPOSITION1+value)+8));
	Gpu::set(2,x+9,y, *(*(TABLE_VISPOSITION1+value)+9));
	Gpu::set(2,x+10,y,*(*(TABLE_VISPOSITION1+value)+10));
	Gpu::set(2,x+11,y,*(*(TABLE_VISPOSITION1+value)+11));
	Gpu::set(2,x+12,y,*(*(TABLE_VISPOSITION1+value)+12));
	Gpu::set(2,x+13,y,*(*(TABLE_VISPOSITION1+value)+13));
	Gpu::set(2,x+14,y,*(*(TABLE_VISPOSITION1+value)+14));
	Gpu::set(2,x+15,y,*(*(TABLE_VISPOSITION1+value)+15));
}

// Lower instrument arrow cursor (16 steps)
void VISPOS2(u8 x, u8 y, u16 color, u16 value) {
	value &=0xF;
	//color <<= 12;
	Gpu::set(2,x,y,   **(TABLE_VISPOSITION2+value));
	Gpu::set(2,x+1,y, *(*(TABLE_VISPOSITION2+value)+1));
	Gpu::set(2,x+2,y, *(*(TABLE_VISPOSITION2+value)+2));
	Gpu::set(2,x+3,y, *(*(TABLE_VISPOSITION2+value)+3));
	Gpu::set(2,x+4,y, *(*(TABLE_VISPOSITION2+value)+4));
	Gpu::set(2,x+5,y, *(*(TABLE_VISPOSITION2+value)+5));
	Gpu::set(2,x+6,y, *(*(TABLE_VISPOSITION2+value)+6));
	Gpu::set(2,x+7,y, *(*(TABLE_VISPOSITION2+value)+7));
	Gpu::set(2,x+8,y, *(*(TABLE_VISPOSITION2+value)+8));
	Gpu::set(2,x+9,y, *(*(TABLE_VISPOSITION2+value)+9));
	Gpu::set(2,x+10,y,*(*(TABLE_VISPOSITION2+value)+10));
	Gpu::set(2,x+11,y,*(*(TABLE_VISPOSITION2+value)+11));
	Gpu::set(2,x+12,y,*(*(TABLE_VISPOSITION2+value)+12));
	Gpu::set(2,x+13,y,*(*(TABLE_VISPOSITION2+value)+13));
	Gpu::set(2,x+14,y,*(*(TABLE_VISPOSITION2+value)+14));
	Gpu::set(2,x+15,y,*(*(TABLE_VISPOSITION2+value)+15));
}

void STATUS(u8 x, u8 y, u16 color, u16 value) {
	value %=21;
	color <<= 12;
	Gpu::set(2,x  ,y  , color | **(TABLE_STATUS_STRINGS+value));
	Gpu::set(2,x+1,y  , color | *(*(TABLE_STATUS_STRINGS+value)+1));
	Gpu::set(2,x+2,y  , color | *(*(TABLE_STATUS_STRINGS+value)+2));
	Gpu::set(2,x+3,y  , color | *(*(TABLE_STATUS_STRINGS+value)+3));
	Gpu::set(2,x+4,y  , color | *(*(TABLE_STATUS_STRINGS+value)+4));
	Gpu::set(2,x+5,y  , color | *(*(TABLE_STATUS_STRINGS+value)+5));
	Gpu::set(2,x+6,y  , color | *(*(TABLE_STATUS_STRINGS+value)+6));
	Gpu::set(2,x+7,y  , color | *(*(TABLE_STATUS_STRINGS+value)+7));
	Gpu::set(2,x+8,y  , color | *(*(TABLE_STATUS_STRINGS+value)+8));
	Gpu::set(2,x  ,y+1, color | *(*(TABLE_STATUS_STRINGS+value)+9));
	Gpu::set(2,x+1,y+1, color | *(*(TABLE_STATUS_STRINGS+value)+10));
	Gpu::set(2,x+2,y+1, color | *(*(TABLE_STATUS_STRINGS+value)+11));
	Gpu::set(2,x+3,y+1, color | *(*(TABLE_STATUS_STRINGS+value)+12));
	Gpu::set(2,x+4,y+1, color | *(*(TABLE_STATUS_STRINGS+value)+13));
	Gpu::set(2,x+5,y+1, color | *(*(TABLE_STATUS_STRINGS+value)+14));
	Gpu::set(2,x+6,y+1, color | *(*(TABLE_STATUS_STRINGS+value)+15));
	Gpu::set(2,x+7,y+1, color | *(*(TABLE_STATUS_STRINGS+value)+16));
	Gpu::set(2,x+8,y+1, color | *(*(TABLE_STATUS_STRINGS+value)+17));
}

/* ------------------------------------------------------------------------------------------------------------------------------------- */
/*                                               EXCLUSIVE SCREEN CALLBACKS 															 */
/* ------------------------------------------------------------------------------------------------------------------------------------- */

const Callback cb_no_callback	= { NULL			, 0x0000 /* No key presses*/, NULL /* No var */		, NULL /* Last element */	};
const Callback cb_reset			= { Config::reset 	, EVENT_PANIC 				, NULL 					, NULL 						};
