#ifndef HELPERS_HPP
#define HELPERS_HPP

#include "../agb.h"

#define BREAK while(1){ static int freeze = 0; freeze++; STATUS(12, 8, (freeze>>8)&0xFF, 0x7);}
	
/* Draws given string on screen */
void STRING					( bool big, u8 x, u8 y, u8 *data );

void HEXADECIMAL			( u8 x, u8 y, u16 color, u16 value );// Single 8x8 tile, 1 hexadecimal digit
void HEXADECIMAL_DOUBLE		( u8 x, u8 y, u16 color, u16 value );// Single 8x8 tile, 2 hexadecimal digits
void HEXADECIMAL_INSTRUMENT	( u8 x, u8 y, u16 color, u16 value );// Double 8x8 tile, 1 hexadecimal digit each
void HEXADECIMAL_TWOTILES	( u8 x, u8 y, u16 color, u16 value );// Two 8x8 tiles, 1 hexadecimal digit each, different color on zero
void DECIMAL				( u8 x, u8 y, u16 color, u16 value );// Single 8x8 tile, 1 decimal numeric digit
void DECIMAL_DOUBLE			( u8 x, u8 y, u16 color, u16 value );// Single 8x8 tile displaying 2 decimal numeric digits
void DECIMAL_TWOTILES		( u8 x, u8 y, u16 color, u16 value );// Two 8x8 tiles displaying a decimal numeric digit each
void DECIMAL_DOUBLE_TWOTILES( u8 x, u8 y, u16 color, u16 value );// Double 8x8 tile displaying 4 decimal numeric digits
void NIBBLE					( u8 x, u8 y, u16 color, u16 value );// Double 8x8 tile displaying 4 binary digits
void PROGRESS				( u8 x, u8 y, u16 color, u16 value );// 9 Tile progress bar ( 36 steps)
void WAVE_DOUBLE			( u8 x, u8 y, u16 color, u16 value );// Mirrored tile wave display ( 8 pixels wide/sample, 16 pixels height)
void PIXEL					( u8 x, u8 y, u16 color, u16 value );
void WAVE_SINGLE			( u8 x, u8 y, u16 color, u16 value );// Single tile wave display ( 8 pixels wide/sample)
void BIGHEX					( u8 x, u8 y, u16 color, u16 value );// 8x16 Big Hexadecimal Character
void BIGDEC					( u8 x, u8 y, u16 color, u16 value );// 8x16 Big Numeric Character
void BIGTEXT				( u8 x, u8 y, u16 color, u16 value );// 8x16 Big Text Character
void TEXT					( u8 x, u8 y, u16 color, u16 value );// 8x8 Text Character
void VUMETER_V3				( u8 x, u8 y, u16 color, u16 value );// Triple tile Vertical VMeter
void VUMETER_H3				( u8 x, u8 y, u16 color, u16 value );// Triple tile Horizontal VUMeter
void VUMETER_H1				( u8 x, u8 y, u16 color, u16 value );// Single tile Horizontal VUMeter
void VUMETER_V1				( u8 x, u8 y, u16 color, u16 value );// Single tile Vertical VUMeter
void COMMAND				( u8 x, u8 y, u16 color, u16 value );// Command type display function
void NOTE					( u8 x, u8 y, u16 color, u16 value );// Note type display function
void TABLEPOS				( u8 x, u8 y, u16 color, u16 value );// Vertical cursor for the instrument tables
void VISPOS1				( u8 x, u8 y, u16 color, u16 value );// Upper instrument arrow cursor ( 16 steps)
void VISPOS2				( u8 x, u8 y, u16 color, u16 value );// Lower instrument arrow cursor ( 16 steps)
void STATUS					( u8 x, u8 y, u16 color, u16 value );

#endif
