#include "debug.hpp"

#include "../modules/gpu/gpu.hpp"
#include "../modules/sys/sys.hpp"
#include "../modules/key/key.hpp"
#include "../modules/sram/sram.hpp"
#include "../data/helpers.hpp"
#include "../callbacks/cfg.hpp"
#include "../data/enum.h"

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

int 		Debug::counter	 	= 0;
void*		Debug::watch_var 	= NULL;
const char* Debug::watch_name 	= NULL;
u8		 	Debug::watch_size 	= 0;
u32 		Debug::last_var 	= 0xFFFF;
bool 		Debug::step_by_step = false;


#include "../modules/clip/clip.hpp"
	

void Debug::clear( u8 color ){
	for(int x=0; x<30; x++){
		for(int y=0; y<20; y++){
			gpu.set( 0	, x	, y , color );
			gpu.set( 1	, x	, y , 0x0000);
			gpu.set( 2	, x	, y , 0x00FC);
		}
	}
}	
	
#ifndef NDEBUG

static bool redraw;
static bool watch_hex;	
static u16 	color_timer;
void Debug::init(){
	color_timer = 0;
	watch_var 	= NULL;
	counter 	= 0;
	watch_size  = 0;
	watch_name  = NULL;
	watch_hex   = false;
	step_by_step= false;
	last_var 	= 0xFFFFFFFF;
	WATCH( Clipboard::type );
}

void Debug::updateWatch(){
	if( !watch_var ) return;
	
	if(KEY.up(KEY_SELECT)){
		watch_hex ^=1;
		redraw = true;
	}
	
	switch(watch_size){
		case  8: return watch( watch_name, *((    u8*) watch_var), watch_size );
		case 16: return watch( watch_name, *((   u16*) watch_var), watch_size );
		case 32: return watch( watch_name, *((   u32*) watch_var), watch_size );
		case 64: return watch( watch_name, *((size_t*) watch_var), watch_size );
	}
}

void Debug::runTests(){
	//error(0xCAFECAFE, true);
	//error(0xCAFECAFE, false);
	//HALT
}

static void inverse_text(int y, const char *message, u8 fg, u8 bg){
	size_t len = strlen(message)>>1;
	for(int i=1; i<29; i++){
		gpu.set(0, i, y, 0x10 + fg );
	}
	Debug::ascii( 15-(len), y, message, bg);
}

void Debug::ascii( u8 x, u8 y, const char *data, u8 color ){
	for(int i=x, l=x+strlen(data); i<l; i++){
		char d = *data;
		if(!( i & 0x1)) gpu.set(1, x + ( i>>1 ), y, (color << 12) |  (0x0100 + d) );
		else gpu.set(2, x + ( i>>1 ), y, (color << 12) |  (0x0100 + d) );
		data++;
	}
}

void Debug::watch( const char *varname	, u8 var 	 ){ 	watch(varname, var,  8); }
void Debug::watch( const char *varname	, u16 var 	 ){		watch(varname, var, 16); }
void Debug::watch( const char *varname	, u32 var 	 ){ 	watch(varname, var, 32); }
void Debug::watch( const char *varname	, size_t var ){ 	watch(varname, var, 64); }

void Debug::watch( const char *varname	, u32 var 	 , u8 size){
	if( color_timer > 1 ) color_timer--;
	else if( color_timer == 1 ) {
		color_timer = 0;
		redraw = true;
	}
	
	if( ( last_var == var ) && !redraw ) return;
	bool alt_color = color_timer > 0;
	redraw = false;
	int pos_x 	= 20;
	int pos_y 	= 16;
	int width 	= 10;
	int height 	=  4; 

	watch_name = varname;
	watch_size = size;
	
	
	
	u16 border 	  = ( ( step_by_step 	? COLOR_DARK_RED 	: ( step_by_step ? COLOR_DARK_RED : watch_hex ? COLOR_BROWN : COLOR_DARK_BLUE)	) << 12 );
	u16 primary   = ( ( alt_color 		? COLOR_DARK_RED	: ( step_by_step ? COLOR_DARK_RED : watch_hex ? COLOR_BROWN : COLOR_DARK_BLUE)	) << 12 );
	u16 secondary = ( ( alt_color 		? COLOR_DARK_GREEN	: ( step_by_step ? COLOR_DARK_RED : watch_hex ? COLOR_BROWN : COLOR_DARK_BLUE)	) << 12 );
	
	for(int y=0; y < height; y++){
		for(int x=0; x < width; x++){

			if( y == 1 ) gpu.set( 0, pos_x + x, pos_y + y, primary  	| 0x0D );
			else 
			if( y == 2 ) gpu.set( 0, pos_x + x, pos_y + y, secondary 	| 0x0D );
			gpu.set( 1, pos_x + x, pos_y + y, 0x100);
			gpu.set( 2, pos_x + x, pos_y + y, 0x100);

			if( ( x != 0 ) && ( y != 0 ) && ( x != width - 1 ) && ( y != height - 1 ) ) continue;
			gpu.set( 0, pos_x + x, pos_y + y, border | 0x10);
			gpu.set( 1, pos_x + x, pos_y + y, 0x100);
			gpu.set( 2, pos_x + x, pos_y + y, 0x100);
		}		
	}
	
	ascii ( 			14, pos_y			, varname		, COLOR_WHITE		);
	DECIMAL_DOUBLE(pos_x+1, pos_y+height-1	, COLOR_YELLOW	, size				);
	ascii(  			15, pos_y+height-1	, "bits"		, COLOR_CYAN 		);
	
	
	u16 color_hex = COLOR_ORANGE;
	u16 color_dec = COLOR_WHITE;
	u16 color_bef = COLOR_RED;
	u16 color_aft = COLOR_GREEN;
	
	if( watch_hex ){
		switch(size){
			case 8:
				NIBBLE(				pos_x + 1		, pos_y+1	,  	alt_color ? color_bef : color_hex 	, last_var >> 4		);
				NIBBLE(				pos_x + 3		, pos_y+1	,  	alt_color ? color_bef : color_hex 	, last_var			);
				gpu.set( 2, 		pos_x + width-3 , pos_y+1 	,((	alt_color ? color_bef : color_hex 	) <<12) | 0xB4		);
				HEXADECIMAL_DOUBLE(	pos_x + width-2	, pos_y+1	,  	alt_color ? color_bef : color_hex 	, last_var			);
				NIBBLE(				pos_x + 1		, pos_y+2	,  	alt_color ? color_aft : color_hex 	, var >> 4			);
				NIBBLE(				pos_x + 3		, pos_y+2	,  	alt_color ? color_aft : color_hex 	, var 				);
				gpu.set( 2, 		pos_x + width-3 , pos_y+2 	,((	alt_color ? color_aft : color_hex 	) <<12) | 0xB4		);
				HEXADECIMAL_DOUBLE(	pos_x + width-2	, pos_y+2	,  	alt_color ? color_aft : color_hex 	, var 				);
				break;
			case 16:
				gpu.set( 2, 		pos_x + width-4	, pos_y+1	,((	alt_color ? color_bef : color_hex 	) <<12 ) | 0xB4		);
				HEXADECIMAL_DOUBLE(	pos_x + width-3	, pos_y+1	,   alt_color ? color_bef : color_hex 	, last_var	>> 8	);
				HEXADECIMAL_DOUBLE(	pos_x + width-2	, pos_y+1	,   alt_color ? color_bef : color_hex 	, last_var			);
				gpu.set( 2, 		pos_x + width-4	, pos_y+2	,((	alt_color ? color_aft : color_hex 	) <<12 ) | 0xB4		);
				HEXADECIMAL_DOUBLE(	pos_x + width-3	, pos_y+2	,   alt_color ? color_aft : color_hex 	, var 		>> 8	);
				HEXADECIMAL_DOUBLE(	pos_x + width-2	, pos_y+2	,   alt_color ? color_aft : color_hex 	, var 				);
				break;
			case 32:
			case 64:
				gpu.set( 2, 		pos_x + width-6	, pos_y+1	,((	alt_color ? color_bef : color_hex 	) <<12 ) | 0xB4		);
				HEXADECIMAL_DOUBLE(	pos_x + width-5	, pos_y+1	,   alt_color ? color_bef : color_hex 	, last_var	>> 24	);
				HEXADECIMAL_DOUBLE(	pos_x + width-4	, pos_y+1	,   alt_color ? color_bef : color_hex 	, last_var	>> 16	);
				HEXADECIMAL_DOUBLE(	pos_x + width-3	, pos_y+1	,   alt_color ? color_bef : color_hex 	, last_var	>> 8	);
				HEXADECIMAL_DOUBLE(	pos_x + width-2	, pos_y+1	,   alt_color ? color_bef : color_hex 	, last_var			);
				gpu.set( 2, 		pos_x + width-6	, pos_y+2	,((	alt_color ? color_aft : color_hex 	) <<12 ) | 0xB4		);
				HEXADECIMAL_DOUBLE(	pos_x + width-5	, pos_y+2	,   alt_color ? color_aft : color_hex 	, var 		>> 24	);
				HEXADECIMAL_DOUBLE(	pos_x + width-4	, pos_y+2	,   alt_color ? color_aft : color_hex 	, var 		>> 16	);
				HEXADECIMAL_DOUBLE(	pos_x + width-3	, pos_y+2	,   alt_color ? color_aft : color_hex 	, var 		>> 8	);
				HEXADECIMAL_DOUBLE(	pos_x + width-2	, pos_y+2	,   alt_color ? color_aft : color_hex 	, var 				);
				break;
		}
	} else {
		switch(size){
			case 8:
				DECIMAL_DOUBLE(pos_x+width-2, pos_y+1, alt_color ? color_bef : color_dec, last_var);
				DECIMAL_DOUBLE(pos_x+width-2, pos_y+2, alt_color ? color_aft : color_dec, var 	);
				break;
			case 32:
			case 64:
			case 16:
				number( pos_x+width-5	, pos_y+1	, last_var	, alt_color ? color_bef : color_dec );
				number( pos_x+width-5	, pos_y+2	, var		, alt_color ? color_aft : color_dec );
		}
	}
	
	
	if(	last_var != var){
		color_timer = 0xFFF;
		if( step_by_step )
		while( !KEY.down( KEY_START )) KEY.update();
		last_var = var;
		redraw = true;
	}
}
	
void Debug::panic( const char *message, u32 *pointer){
	clear( COLOR_BLUE );
	Debug::bigString(9, 0, "KERNEL PANIC");
	
	inverse_text( 3, message, COLOR_WHITE, COLOR_BLUE );
	
	Debug::ascii( 1, 5, "System has encountered a problem and has been halted." , COLOR_WHITE);
	Debug::ascii( 1, 6, "Please report this error if it's not the first time  "	, COLOR_WHITE);
	Debug::ascii( 1, 7, "and it can be replicated by following specific steps."	, COLOR_WHITE);
	Debug::ascii( 1, 9, "There is some extra information:"			, COLOR_WHITE);
	Debug::ascii( 1,11, "ADDRESS :  0x"								, COLOR_WHITE);
	// Pointer address
	HEXADECIMAL_DOUBLE(  8 , 11 , COLOR_WHITE, ((( unsigned ) pointer ) &0xFF000000 ) >> 24	); 	
	HEXADECIMAL_DOUBLE(  9 , 11 , COLOR_WHITE, ((( unsigned ) pointer ) &0x00FF0000 ) >> 16	); 	
	HEXADECIMAL_DOUBLE( 10 , 11 , COLOR_WHITE, ((( unsigned ) pointer ) &0x0000FF00 ) >>  8	); 	
	HEXADECIMAL_DOUBLE( 11 , 11 , COLOR_WHITE, ((( unsigned ) pointer ) &0x000000FF ) 	    );	
	
	Debug::ascii( 1,12, "    *U8 :  0x"								, COLOR_WHITE);
	Debug::ascii( 1,13, "   *U16 :  0x"								, COLOR_WHITE);
	Debug::ascii( 1,14, "   *U32 :  0x"								, COLOR_WHITE);
	
	if(!pointer){
		Debug::ascii( 1,12, "No additional information was provided."	, COLOR_WHITE);
	} else {
		// U8 interpretation
		HEXADECIMAL_DOUBLE(  8 , 12 , COLOR_WHITE, (((u8)*pointer ) & 0x000000FF ) 	  );
		// U16 Interpretation
		HEXADECIMAL_DOUBLE(  8 , 13 , COLOR_WHITE, (((u16)*pointer ) & 0x0000FF00 ) >>  8 );
		HEXADECIMAL_DOUBLE(  9 , 13 , COLOR_WHITE, (((u16)*pointer ) & 0x000000FF ) 	  );
		// U32 Interpretation
		HEXADECIMAL_DOUBLE(  8 , 14 , COLOR_WHITE, (((u32)*pointer ) & 0xFF000000 ) >> 24 );
		HEXADECIMAL_DOUBLE(  9 , 14 , COLOR_WHITE, (((u32)*pointer ) & 0x00FF0000 ) >> 16 );
		HEXADECIMAL_DOUBLE( 10 , 14 , COLOR_WHITE, (((u32)*pointer ) & 0x0000FF00 ) >>  8 );
		HEXADECIMAL_DOUBLE( 11 , 14 , COLOR_WHITE, (((u32)*pointer ) & 0x000000FF )       );
	}
	
	Debug::ascii( 1,16, "Press START to reboot this unit.", COLOR_WHITE);
	while(1){
		KEY.update();
		if( KEY.down( KEY_START ) ) asm("swi 00");
		
		static bool bm;
		if(bm != gpu.blink){
			bm = gpu.blink;
			gpu.set(2, 17, 16, (COLOR_WHITE << 12) | (gpu.blink ? 0x0116 : 0x0100) );
		}
		gpu.blinkUpdate(8);
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
	if(!recoverable)
		ascii(  2, 2, "Software Failure.    Press START to restart GBA.", COLOR_CYAN );
	else 
		ascii(  2, 2, "Software Failure.    Press B button to continue.", COLOR_CYAN );
	ascii(  4, 4, "Guru Meditation ", COLOR_CYAN );
	gpu.set(2, 14, 4, (COLOR_CYAN <<12) | 0x2D );
	hexnum( 15, 4, error_code		, COLOR_CYAN );
	gpu.set(2, 19, 4, (COLOR_CYAN <<12) | 0x2C );
	hexnum( 20, 4, error_code		, COLOR_CYAN );

	while(1){
		// Palette blink
		counter++;
		if( gpu.isVblank()){
			KEY.update();
		}
		if( recoverable){
			if( KEY.down( KEY_B ) ){
				clear();
				gpu.loadPalette();
				return;
			}
		} else {
			if( KEY.down( KEY_START ) ) {
				asm("swi 26");
			}
		}
		if((counter & 0xFFF) > 0x800) {
			*(u16*)(BG_PALETTE +4) = 0; 
		} else {
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
		gpu.set( 2	,  5, 1 , (((counter+0x200)&0xfff) > 0x800 ?0x4000:0x7000) | (((counter+0x200)&0x7ff) > 0x400 ?0xb3:0xa2));
		bigString	(  6 , 0 , "PROGRAM HALTED"	, (counter&0x7ff) > 0x400 ? COLOR_WHITE : COLOR_RED);
		string		(  3 , 3 , "LINE"			, COLOR_YELLOW);
		number 		(  8 , 3 , line				, COLOR_OLIVE);
		string		(  3 , 5 , "FILE"			, COLOR_YELLOW);
		ascii		(  5 , 5 , filename			, COLOR_OLIVE);
		counter++;
	};
}

void Debug::number( u8 x, u8 y, u32 num, u8 color ){
	DECIMAL_DOUBLE( x  , y, color, (num/1000000	)%100 );// 16
	DECIMAL_DOUBLE( x+1, y, color, (num/10000	)%100 );// 65
	DECIMAL_DOUBLE( x+2, y, color, (num/100		)%100 );// 53
	DECIMAL_DOUBLE( x+3, y, color, (num			)%100 );// 5
}

void Debug::hexnum( u8 x, u8 y, u32 num, u8 color ){
	HEXADECIMAL_DOUBLE( x  , y, color, num>> 24 );// 16
	HEXADECIMAL_DOUBLE( x+1, y, color, num>> 16 );// 65
	HEXADECIMAL_DOUBLE( x+2, y, color, num>>  8 );// 53
	HEXADECIMAL_DOUBLE( x+3, y, color, num      );// 5
}

void Debug::bigString( u8 x, u8 y, const char *data, u8 color ){
	color = ((color&0xf) << 4 ) | (color&0xf);
	for(int i=x, l=x+strlen(data); i<l; i++){
		if( data[0] != ' ' ) BIGTEXT( i, y, color, data[0] - 0x41); 
		data++;
	}
}

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
#else

void Debug::init			( 									 				){}
void Debug::benchmark		( RegionHandler &regHnd 			 				){}
void Debug::halt			( const char *filename, int line 	 				){}
void Debug::panic			( const char *filename, u32 *pointer 				){}
void Debug::error			( int error_code , bool recoverable  				){}
void Debug::watch			( const char *varname	, u8 var 	 				){}
void Debug::watch			( const char *varname	, size_t var 				){}
void Debug::watch			( const char *varname	, u32 var 	 				){}
void Debug::watch			( const char *varname	, u16 var 	 				){}
void Debug::watch			( const char *varname	, u32 var 	, u8 size		){}
void Debug::string			( u8 x, u8 y, const char *filename	, u8 color 		){}
void Debug::ascii			( u8 x, u8 y, const char *filename	, u8 color 		){}
void Debug::number			( u8 x, u8 y, u32 number 			, u8 color 		){}
void Debug::hexnum			( u8 x, u8 y, u32 number 			, u8 color 		){}
void Debug::bigString		( u8 x, u8 y, const char *filename	, u8 color 		){}
void Debug::runTests		( 													){}
void Debug::updateMemory	( RegionHandler* rh 								){}
void Debug::updateWatch		( 													){}
void Debug::updateMemTest  	( RegionHandler* rh 								){}
void Debug::memoryTest		( Control* c, bool bigstep, bool add, u32* pointer 	){}
	
#endif