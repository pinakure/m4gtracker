#include "debug.hpp"

#include "../modules/gpu/gpu.hpp"
#include "../modules/sys/sys.hpp"
#include "../modules/key/key.hpp"
#include "../modules/sram/sram.hpp"
#include "../data/helpers.hpp"
#include "../data/enum.h"

int Debug::counter=0;

void Debug::runTests(){
//	error(0xCAFECAFE, true);
//	error(0xCAFECAFE, false);
}

void Debug::clear( ){
	for(int x=0; x<30; x++){
		for(int y=0; y<20; y++){
			gpu.set( 0	, x	, y , 0x0000);
			gpu.set( 1	, x	, y , 0x0000);
			gpu.set( 2	, x	, y , 0x00FC);
		}
	}
}	
	
void Debug::error( int error_code, bool recoverable ){
	// Clear unused palette color
	*(u32*)(BG_PALETTE+16) = 0x00000000;
	
	// Clear screen
	clear();
	
	// Select corresponding error color
	u16 color = recoverable ? 0x03E0 : 0x001F; 
		
	// Set red/green palette
	*(u16*)(BG_PALETTE +4) = color;
	*(u16*)(BG_PALETTE+80) = color;
	
	// Draw guru rectangle
	for( int x=1; x < 29 ; x++ ){
		for( int y=1; y <  6 ; y++ ){
			gpu.set( 1	,  0, y, 0x67);
			gpu.set( 1	,  x, 0, 0x68);
			gpu.set( 1	,  x, 6, 0x6A);
			gpu.set( 1	, 29, y, 0x69);
		}
	}
	gpu.set( 1	, 0 , 0, 0x62);
	gpu.set( 1	, 0 , 6, 0x61);	
	gpu.set( 1	,29 , 0, 0x63);
	gpu.set( 1	,29 , 6, 0x64);
	
	// Draw error text
	string(  2, 2, "Software Failure", COLOR_CYAN );
	string(  2, 4, "Guru Meditation ", COLOR_CYAN );
	gpu.set( 2,18,4,(COLOR_CYAN<<12) | 0xb4 );
	hexnum( 19, 4, error_code		, COLOR_CYAN );

	while(1){
		// Palette blink
		counter++;
		if( gpu.isVblank()){
			if( recoverable){
				sys.update();
				if( KEY.activity() ){
					clear();
					gpu.loadPalette();
					return;
				}
			}
			if((counter & 0xFFFF) > 0x8000) 
				*(u16*)(BG_PALETTE +4) = 0; 
			else 
				*(u16*)(BG_PALETTE +4) = color;
		}
	}
}

void Debug::halt( const char *filename, int line ){
	clear();
	int x;
	
	for(x=0; x<30; x++){
		gpu.set( 0	, x , -x , 0x002A);
		gpu.set( 0	, x , (-x)+1 , 0x003A);
		gpu.set( 0	, x , (-x)+2 , 0x0013);
		gpu.set( 0	, x , (-x)+3 , 0x0003);
		gpu.set( 0	, x , (-x)+4 , 0x0015);
		gpu.set( 0	, x , (-x)+5 , 0x0005);
	}
	
	static u16 counter=0;
	while(1){
		gpu.set( 2	, 5, 1 , (((counter+0x200)&0xfff) > 0x800 ?0x4000:0x7000) | (((counter+0x200)&0x7ff) > 0x400 ?0xb3:0xa2));
		string		( 1 , 5 , "FILE"			, COLOR_YELLOW);
		string		( 0 , 6 , filename			, COLOR_OLIVE);
		string		( 3 , 3 , "LINENUMBER"		, COLOR_YELLOW);
		number 		( 2 , 4 , line				, COLOR_OLIVE);
		bigString	( 6 , 0 , "PROGRAM HALTED"	, (counter&0x7ff) > 0x400 ? COLOR_WHITE : COLOR_RED);
		counter++;
	};
}

void Debug::number( u8 x, u8 y, int number, u8 color ){
	DECIMAL_DOUBLE( x  , y, color, (number/1000000	)%100 );// 16
	DECIMAL_DOUBLE( x+1, y, color, (number/10000	)%100 );// 65
	DECIMAL_DOUBLE( x+2, y, color, (number/100		)%100 );// 53
	DECIMAL_DOUBLE( x+3, y, color, (number			)%100 );// 5
}

void Debug::hexnum( u8 x, u8 y, int number, u8 color ){
	HEXADECIMAL_DOUBLE( x  , y, color, number>> 24 );// 16
	HEXADECIMAL_DOUBLE( x+1, y, color, number>> 16 );// 65
	HEXADECIMAL_DOUBLE( x+2, y, color, number>>  8 );// 53
	HEXADECIMAL_DOUBLE( x+3, y, color, number      );// 5
}

void Debug::bigString( u8 x, u8 y, const char *data, u8 color ){
	color = ((color&0xf) << 4 ) | (color&0xf);
	for(int i=x, l=x+strlen(data); i<l; i++){
		if( data[0] != ' ' ) BIGTEXT( i, y, color, data[0] - 0x41); 
		data++;
	}
}

const u16 debug_colors[16][2] = {
	{ COLOR_NONE		<< 12 , COLOR_NONE 			<< 12 },
	{ COLOR_DARK_CYAN	<< 12 , COLOR_CYAN 			<< 12 },
	{ COLOR_CYAN		<< 12 , COLOR_DARK_CYAN 	<< 12 },
	{ COLOR_RED			<< 12 , COLOR_DARK_RED 		<< 12 },
	{ COLOR_ORANGE		<< 12 , COLOR_BROWN 		<< 12 },
	{ COLOR_DARK_RED	<< 12 , COLOR_RED 			<< 12 },
	{ COLOR_WHITE		<< 12 , COLOR_GRAY 			<< 12 },
	{ COLOR_YELLOW		<< 12 , COLOR_OLIVE 		<< 12 },
	{ COLOR_BLACK		<< 12 , COLOR_BLACK 		<< 12 },
	{ COLOR_DARK_BLUE	<< 12 , COLOR_BLUE 			<< 12 },
	{ COLOR_BROWN		<< 12 , COLOR_ORANGE 		<< 12 },
	{ COLOR_OLIVE		<< 12 , COLOR_YELLOW 		<< 12 },
	{ COLOR_DARK_GREEN	<< 12 , COLOR_GREEN 		<< 12 },
	{ COLOR_GRAY		<< 12 , COLOR_WHITE 		<< 12 },
	{ COLOR_BLUE		<< 12 , COLOR_DARK_BLUE 	<< 12 },
	{ COLOR_GREEN		<< 12 , COLOR_DARK_GREEN  	<< 12 },
};

void Debug::string( u8 x, u8 y, const char *data, u8 color ){
	
	const char OFFSET = 'a' - 'A';
	
	for(int i=x, l=x+strlen(data), c = 0; i<l; i++){
		char d = ( ( *data >= 'a' ) && ( *data <= 'z' ) ) ? *data - OFFSET : *data;
		if( ( d >= '0' ) && ( d <= '9' ) ) 
			c = debug_colors[ color&0xf][ 1 ] | ( d - 0x30);
		else 
			c = debug_colors[ color&0xf ][ 0 ] | TABLE_TEXT[ d - 0x41 ][ 0 ];
		if( data[0] != ' ' ) gpu.set(2, i, y, c );
		data++;
	}
}

// Stress test for GUI Display Functions
void Debug::benchmark( RegionHandler &regHnd ){
	static u16 i, o;
	// M4G0 code offers a great collection of macros which you
	// can use to display a value in a bunch of different formats:
	
	// Hexadecimal values
	HEXADECIMAL(1,4,4, i);
	HEXADECIMAL_DOUBLE(1,5,4, i);
	HEXADECIMAL_TWOTILES(1,6,4, i);
	BIGHEX(3,4,4,i);
	
	// Decimal values
	DECIMAL(1,7,7, i);
	DECIMAL_DOUBLE(1,8,7, i);
	DECIMAL_TWOTILES(1,9,7, i);
	DECIMAL_DOUBLE_TWOTILES(4,9,7, i);
	BIGDEC(3,7,4,i);
	
	// Special values
	NIBBLE(1,10,6, i);
	BIGTEXT(3,10,6,i);
	TEXT(1,11,6,i);
	NOTE(1,12,0xFF, i);
	STATUS(1,13,6,i);
	
	// Waves and vumeters
	PROGRESS(1,15, 0xFF,i); 
	WAVE_DOUBLE(1,16,0xFF, i); 
	WAVE_SINGLE(3,16,0xFF, i);				
	COMMAND(3,17, 0xFF,i);
	VUMETER_V3(6,16,0xFF, i);				
	VUMETER_V1(5,18,0xFF, i);				
	VUMETER_H3(2,18,0xFF, i);				
	VUMETER_H1(1,18,0xFF, i);
	
	// Position Markers
	TABLEPOS(14, 2,0xFF, i);
	VISPOS1(14, 1,0xFF, i);
	VISPOS2(14, 18,0xFF, i);
	// Lets change the value little by little
	i++;
	return;
	o&=15;
	if(o==0)i++;
	o++;
}

void Debug::updateMemory( RegionHandler* rh ){
}

void Debug::updateMemTest(RegionHandler* rh){
}

void Debug::memoryTest(Control* c, bool bigstep, bool add, u32* pointer ){
	u8 	a1,a2,a3;
	int i;
	
	for( i = 0 ; i < 0x8000 ; i++ ){
		SRAM.seek( i ); a1 = SRAM.read();
		SRAM.seek( i ); a2 = SRAM.read();
		SRAM.seek( i ); a3 = SRAM.read();
		
		if( ( ( a1 != a2 ) || ( a2 != a3 ) ) || a1 != a3 ){
			SRAM.drawPosition( 27, 1, 7 );
			return;
		}
	}
	SRAM.drawPosition( 27, 1, 0xF );
}
