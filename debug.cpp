#include "debug.hpp"
#include "helpers.hpp"

#include "screens/songedit.hpp"
#include "modules/gpu/gpu.hpp"
#include "modules/sys/sys.hpp"
#include "modules/key/key.hpp"
#include "modules/sram/sram.hpp"
#include "screens/config.hpp"
#include "data/enum.h"
#include "data/data.hpp"
#include "modules/spu/mixer.hpp"
#include "modules/spu/synth.hpp"
#include "modules/spu/sequencer.hpp"

#include "modules/clip/clip.hpp"

#ifndef NDEBUG

static const u16 color_hex = COLOR_ORANGE;
static const u16 color_dec = COLOR_WHITE;
static const u16 color_bef = COLOR_RED;
static const u16 color_aft = COLOR_GREEN;

#include "data/song.hpp"


void Debug::runTests(){

	return;
	memoryTest( NULL, 0, 0, NULL);
		
	Mixer::start();
	Sequencer::playing = true;
	Gpu::clear();
	
	while(1){
		Sequencer::update();	
	}
	//error(0xCAFECAFE, true);
	//error(0xCAFECAFE, false);
	//HALT
}

VariableWatch	Debug::variables[4];
ArrayWatch		Debug::arrays[4];
u8 				Debug::variable_count;
u8 				Debug::array_count;

int 			Debug::counter	 	= 0;
bool 			Debug::step_by_step = false;
bool 			Debug::watch_hex 	= false;

void Debug::init(){
	counter 	 	= 0;
	watch_hex    	= false;
	step_by_step 	= false;
	variable_count 	= 0;
	array_count		= 0;
	
	for( int i=0; i<4; i++){
		variables[ i ].variable = NULL;
		arrays[ i ].array		= NULL;
	}
	
	//Debug::watchArray("test", (u8**)&test[0] );
}

void Debug::updateWatch(){
	
	bool redraw = false;
	
	if( RegionHandler::new_region || RegionHandler::redraw ) redraw = true;
	
	if( KEYPRESS_L && KEYUP_R ) {
		redraw 		  = true;
		watch_hex 	 ^= 1;
	}
	if( KEYPRESS_R && KEYUP_L ) {
		redraw 		  = true;
		step_by_step ^= 1;
	}
	
	if( redraw ){
		for( int i=0; i<4; i++){
			variables	[ i ].redraw = true;
			arrays		[ i ].redraw = true;
		}
		redraw = false;
	}
	
	for( int i = 0; i < variable_count; i++ ){
		watchUpdate( i );
	}
	
	for( int i = 0; i < array_count; i++ ){
		watchUpdateArray( i );
	}
}

void Debug::watchArray	( const char *varname , u8**	   	  var ){ watchArray	( varname, ( void** ) var,  8 ); }
void Debug::watchArray	( const char *varname , u16**		  var ){ watchArray	( varname, ( void** ) var, 16 ); }
void Debug::watchArray	( const char *varname , u32**		  var ){ watchArray	( varname, ( void** ) var, 32 ); }
void Debug::watchArray	( const char *varname , size_t** 	  var ){ watchArray	( varname, ( void** ) var, 64 ); }
void Debug::watch		( const char *varname , u8*			  var ){ watch		( varname, ( void*  ) var,  8 ); }
void Debug::watch		( const char *varname , u16*		  var ){ watch		( varname, ( void*  ) var, 16 ); }
void Debug::watch		( const char *varname , u32*  		  var ){ watch		( varname, ( void*  ) var, 32 ); }
void Debug::watch		( const char *varname , size_t* 	  var ){ watch		( varname, ( void*  ) var, 64 ); }

void Debug::watchArray	( const char *varname , void** array, u8 size ){
	
	// Ignore watch if it is already been watched
	for( int i=0; i<4; i++){
		if( arrays[ i ].array == array ) return;
	}
	
	if( array_count > 2 ) arrays[ 3 ] = arrays[ 2 ];
	if( array_count > 1 ) arrays[ 2 ] = arrays[ 1 ];
	if( array_count > 0 ) arrays[ 1 ] = arrays[ 0 ];
	
	arrays[ 0 ].name  		= varname;
	arrays[ 0 ].size  		= size;
	arrays[ 0 ].array 	 	= array; 
	arrays[ 0 ].timer 		= 0; 
	arrays[ 0 ].redraw 		= true; 
	
	for( int i = 0; i < 0x10 ; i++ ){
		arrays[ 0 ].last_value[ i ] = 0xFFFF; 
	}
	
	if( array_count < 3 ) array_count++;
}

void Debug::watch( const char *varname	, void* 	var , u8 size ){
	
	// Ignore watch if it is already been watched
	for( int i=0; i<4; i++){
		if( variables[ i ].variable == var ) return;
	}
	
	if( variable_count > 2 ) variables[3] = variables[2];
	if( variable_count > 1 ) variables[2] = variables[1];
	if( variable_count > 0 ) variables[1] = variables[0];

	variables[ 0 ].name  	= varname;
	variables[ 0 ].size  	= size;
	variables[ 0 ].variable = var; 
	variables[ 0 ].timer 	= 0; 
	variables[ 0 ].redraw 	= true; 
	
	if( variable_count < 3 ) variable_count++;
}

void Debug::drawFrame( u8 pos_x, u8 pos_y, u8 width, u8 height, const char *title, u8 size, bool alt_color ){
	
	u16 	border 	  	= ( ( step_by_step 	? COLOR_DARK_RED 	: ( step_by_step ? COLOR_DARK_RED : watch_hex ? COLOR_BROWN : COLOR_DARK_BLUE)	) << 12 );
	u16 	primary   	= ( ( alt_color 	? COLOR_DARK_RED	: ( step_by_step ? COLOR_DARK_RED : watch_hex ? COLOR_BROWN : COLOR_DARK_BLUE)	) << 12 );

	// Draw Watch Window Frame
	for(int y=0; y < height; y++){
		for(int x=0; x < width; x++){
			
			if( (y > 0) && (y<height-1)) Gpu::set( 0, pos_x + x, pos_y + y, primary  	| 0x0D );
			
			Gpu::set( 1, pos_x + x, pos_y + y, 0x100);
			Gpu::set( 2, pos_x + x, pos_y + y, 0x100);
			if( ( x != 0 ) && ( y != 0 ) && ( x != width - 1 ) && ( y != height - 1 ) ) continue;
			Gpu::set( 0, pos_x + x, pos_y + y, border | 0x10);
			Gpu::set( 1, pos_x + x, pos_y + y, 0x100);
			Gpu::set( 2, pos_x + x, pos_y + y, 0x100);
		}		
	}

		// Draw variable name and bit width
	Gpu::ascii 		( pos_x - (( pos_x & 0x1 ) ? 6 : 5 ), pos_y	+ height - 1, title			, COLOR_WHITE				);
	DECIMAL_DOUBLE	( pos_x + 1							, pos_y 			, COLOR_YELLOW	, size 						);
	Gpu::set			( 2,		pos_x										, pos_y 		, (COLOR_CYAN<<12) | 0x175	);
	if( step_by_step )                            		
		Gpu::ascii	( pos_x - (( pos_x & 0x1 ) ? 5 : 4 ), pos_y				, "Step"		, COLOR_RED 				);
}

void Debug::watchUpdateArray( u8 index ){
	/*
	DO NOT USE
	*/
	/*!
	ArrayWatch *aw = &arrays[ index ];
	
	if( aw->timer > 1 ) aw->timer--;
	else if( aw->timer == 1 ) {
		aw->timer 	= 0;
		aw->redraw 	= true;
	}
	
	// Cast variables in array to every posible value
	static u8	u8value		[0x10];
	static u16	u16value	[0x10]; 
	static u32	u32value	[0x10]; 
	static u32 last_value	[0x10]; 
	
	// Only let updation when variable value has change
	for( int i=0; i < 0x10 ; i++ ){
		u8value			[ i ]  = 0;// *((  u8** ) aw->array)[i];
		u16value		[ i ]  = VAR_CELLS[ 1 ].KEY[ i ];// *(( u16** ) aw->array)[i];
		u32value		[ i ]  = 0;// *(( u32** ) aw->array)[i];
		last_value		[ i ]  = aw->last_value[ i ];
		aw->last_value	[ i ]  = u32value[ i ];
		if( last_value	[ i ] != u32value[ i ] ) {
			aw->timer = 0x7FF;
			aw->redraw = true;
		} 
	}
	
	// Begin redraw / update 
	if( !aw->redraw ) return;	
	aw->redraw = false;

	bool alt_color 	= aw->timer > 0;
	int  pos_x 		= 26 - ( index * 4 );
	int  pos_y 		=  3;
	int  width 		=  4;
	int  height 	= 18; 

	// Draw Watch Window Frame
	drawFrame( pos_x, pos_y, width, height, aw->name, aw->size, alt_color);
	
	for( int i=0; i<16; i++, pos_y++){
		
		// Draw variable representation, indicating when it changes
		if( watch_hex ){
			
			switch( aw->size ){
				case 8:
					// Print 0x
					Gpu::set( 2, 		pos_x + width-3 , pos_y+1 	,((	alt_color ? color_aft : color_hex 	) << 12 ) | 0xB4	);
					// Print Hexadecimal representation 
					HEXADECIMAL_DOUBLE(	pos_x + width-2	, pos_y+1	,  	alt_color ? color_aft : color_hex 	, u8value[i]		);
					break;
				case 16:
					// Print 0x
					Gpu::set( 2, 		pos_x + width-4	, pos_y+1	,((	alt_color ? color_aft : color_hex 	) << 12 ) | 0xB4	);
					// Print Hexadecimal representation 
					HEXADECIMAL_DOUBLE(	pos_x + width-3	, pos_y+1	,   alt_color ? color_aft : color_hex 	, u16value[i] >> 8	);
					HEXADECIMAL_DOUBLE(	pos_x + width-2	, pos_y+1	,   alt_color ? color_aft : color_hex 	, u16value[i]		);
					break;
				case 32:
				case 64:
					// Print 0x
					Gpu::set( 2, 		pos_x + width-6	, pos_y+1	,((	alt_color ? color_aft : color_hex 	) << 12 ) | 0xB4	);
					// Print Hexadecimal representation 
					HEXADECIMAL_DOUBLE(	pos_x + width-5	, pos_y+1	,   alt_color ? color_aft : color_hex 	, u32value[i] >> 24	);
					HEXADECIMAL_DOUBLE(	pos_x + width-4	, pos_y+1	,   alt_color ? color_aft : color_hex 	, u32value[i] >> 16	);
					HEXADECIMAL_DOUBLE(	pos_x + width-3	, pos_y+1	,   alt_color ? color_aft : color_hex 	, u32value[i] >>  8	);
					HEXADECIMAL_DOUBLE(	pos_x + width-2	, pos_y+1	,   alt_color ? color_aft : color_hex 	, u32value[i] 		);
					break;
			}
		} else {
			
			if( aw->size == 8 ){
				DECIMAL_DOUBLE(	pos_x + width - 2, pos_y + 1, alt_color ? color_aft : color_dec , u8value[i]	);
			} else if( aw->size == 16 ){
				DECIMAL_DOUBLE(	pos_x + width - 2, pos_y + 1, alt_color ? color_aft : color_dec , u16value[i]	);
			} else {
				Gpu::number( pos_x + width - 5 , pos_y + 1 , u32value[i]		, alt_color ? color_aft : color_dec );
			}
			
		}
	}
	*/
}

void Debug::watchUpdate( u8 index ){
	
	VariableWatch *vw = &variables[ index ];
	
	if( vw->timer > 1 ) vw->timer--;
	else if( vw->timer == 1 ) {
		vw->timer 	= 0;
		vw->redraw 	= true;
	}
	
	// Cast variable to every posible value
	u8	u8value 	= *(  u8* ) vw->variable;
	u16	u16value 	= *( u16* ) vw->variable;
	u32	u32value 	= *( u32* ) vw->variable;
	u32 last_value	= vw->last_value;
	
	// Only let updation when variable value has change
	if( ( vw->last_value == u32value ) && !( vw->redraw ) ) return;
	
	// Begin redraw / update 
	vw->redraw = false;
	
	bool 	alt_color 	= vw->timer > 0;
	int 	pos_x 		= 16;
	int 	pos_y 		= 16 - ( index * 4 );
	int 	width 		= 10;
	int 	height 		=  4; 

	// Draw Watch Window Frame
	drawFrame( pos_x, pos_y, width, height, vw->name, vw->size, alt_color);
	
	// Draw variable representation, indicating when it changes
	if( watch_hex ){
		switch( vw->size ){
			case 8:
				// Print binary representation
				NIBBLE(				pos_x + 1		, pos_y+1	,  	alt_color ? color_bef : color_hex 	, last_value >> 4 	);
				NIBBLE(				pos_x + 3		, pos_y+1	,  	alt_color ? color_bef : color_hex 	, last_value		);
				NIBBLE(				pos_x + 1		, pos_y+2	,  	alt_color ? color_aft : color_hex 	, u8value >> 4		);
				NIBBLE(				pos_x + 3		, pos_y+2	,  	alt_color ? color_aft : color_hex 	, u8value			);
				// Print 0x
				Gpu::set( 2, 		pos_x + width-3 , pos_y+1 	,((	alt_color ? color_bef : color_hex 	) << 12 ) | 0xB4	);
				Gpu::set( 2, 		pos_x + width-3 , pos_y+2 	,((	alt_color ? color_aft : color_hex 	) << 12 ) | 0xB4	);
				// Print Hexadecimal representation 
				HEXADECIMAL_DOUBLE(	pos_x + width-2	, pos_y+1	,  	alt_color ? color_bef : color_hex 	, last_value		);
				HEXADECIMAL_DOUBLE(	pos_x + width-2	, pos_y+2	,  	alt_color ? color_aft : color_hex 	, u8value			);
				break;
			case 16:
				// Print 0x
				Gpu::set( 2, 		pos_x + width-4	, pos_y+1	,((	alt_color ? color_bef : color_hex 	) << 12 ) | 0xB4	);
				Gpu::set( 2, 		pos_x + width-4	, pos_y+2	,((	alt_color ? color_aft : color_hex 	) << 12 ) | 0xB4	);
				// Print Hexadecimal representation 
				HEXADECIMAL_DOUBLE(	pos_x + width-3	, pos_y+1	,   alt_color ? color_bef : color_hex 	, last_value >> 8	);
				HEXADECIMAL_DOUBLE(	pos_x + width-2	, pos_y+1	,   alt_color ? color_bef : color_hex 	, last_value		);
				HEXADECIMAL_DOUBLE(	pos_x + width-3	, pos_y+2	,   alt_color ? color_aft : color_hex 	, u16value >> 8		);
				HEXADECIMAL_DOUBLE(	pos_x + width-2	, pos_y+2	,   alt_color ? color_aft : color_hex 	, u16value			);
				break;
			case 32:
			case 64:
				// Print 0x
				Gpu::set( 2, 		pos_x + width-6	, pos_y+1	,((	alt_color ? color_bef : color_hex 	) << 12 ) | 0xB4	);
				Gpu::set( 2, 		pos_x + width-6	, pos_y+2	,((	alt_color ? color_aft : color_hex 	) << 12 ) | 0xB4	);
				// Print Hexadecimal representation 
				HEXADECIMAL_DOUBLE(	pos_x + width-5	, pos_y+1	,   alt_color ? color_bef : color_hex 	, last_value >> 24	);
				HEXADECIMAL_DOUBLE(	pos_x + width-4	, pos_y+1	,   alt_color ? color_bef : color_hex 	, last_value >> 16	);
				HEXADECIMAL_DOUBLE(	pos_x + width-3	, pos_y+1	,   alt_color ? color_bef : color_hex 	, last_value >>  8	);
				HEXADECIMAL_DOUBLE(	pos_x + width-2	, pos_y+1	,   alt_color ? color_bef : color_hex 	, last_value		);
				HEXADECIMAL_DOUBLE(	pos_x + width-5	, pos_y+2	,   alt_color ? color_aft : color_hex 	, u32value >> 24	);
				HEXADECIMAL_DOUBLE(	pos_x + width-4	, pos_y+2	,   alt_color ? color_aft : color_hex 	, u32value >> 16	);
				HEXADECIMAL_DOUBLE(	pos_x + width-3	, pos_y+2	,   alt_color ? color_aft : color_hex 	, u32value >>  8	);
				HEXADECIMAL_DOUBLE(	pos_x + width-2	, pos_y+2	,   alt_color ? color_aft : color_hex 	, u32value 			);
				break;
		}
	} else {
		if( vw->size == 8 ){
			DECIMAL_DOUBLE(	pos_x + width - 2, pos_y + 1, alt_color ? color_bef : color_dec , vw->last_value	);
			DECIMAL_DOUBLE(	pos_x + width - 2, pos_y + 2, alt_color ? color_aft : color_dec , u8value			);
		} else if( vw->size == 16 ){
			DECIMAL_DOUBLE(	pos_x + width - 2, pos_y + 1, alt_color ? color_bef : color_dec , vw->last_value	);
			DECIMAL_DOUBLE(	pos_x + width - 2, pos_y + 2, alt_color ? color_aft : color_dec , u16value			);
		} else {
			Gpu::number( pos_x + width - 5 , pos_y + 1 , vw->last_value	, alt_color ? color_bef : color_dec );
			Gpu::number( pos_x + width - 5 , pos_y + 2 , u32value		, alt_color ? color_aft : color_dec );
		}
	}
	
	// If value of the var has changed since last iteration, update its value and increase redraw timer
	if(	vw->last_value != u32value ){
		
		vw->timer = 0x7FF;
		
		if( step_by_step )while( !KEYDOWN_SELECT ) {
			KEYUPDATE();
		}
		
		vw->last_value 	= u32value;
		vw->redraw 		= true;
	}
}	


static void inverse_text(int y, const char *message, u8 fg, u8 bg){
	size_t len = strlen(message)>>1;
	for(int i=1; i<29; i++){
		Gpu::set(0, i, y, 0x10 + fg );
	}
	Gpu::ascii( 30-len, y, message, bg);
}

void Debug::bsod( const char *title, const char *message1,const char *message2, const char *message3 , u32 *pointer){
	Console console("KERNEL PANIC");
	//Gpu::bigString(9, 0, "KERNEL PANIC");
	inverse_text( 3, title, COLOR_WHITE, COLOR_BLUE );
	
	Gpu::ascii( 1, 5, message1 , COLOR_WHITE);
	Gpu::ascii( 1, 6, message2 , COLOR_WHITE);
	Gpu::ascii( 1, 7, message3 , COLOR_WHITE);
	
	if( pointer > 0 ){	
		Gpu::ascii( 1, 9, "There is some extra information:"			, COLOR_WHITE);
		Gpu::ascii( 1,11, "  VALUE :  0x"								, COLOR_WHITE);
		// Pointer address
		HEXADECIMAL_DOUBLE	(  8 , 11 , COLOR_WHITE		, ( (unsigned)pointer &0xFF000000 ) >> 24	); 	
		HEXADECIMAL_DOUBLE	(  9 , 11 , COLOR_WHITE		, ( (unsigned)pointer &0x00FF0000 ) >> 16	); 	
		HEXADECIMAL_DOUBLE	( 10 , 11 , COLOR_WHITE		, ( (unsigned)pointer &0x0000FF00 ) >>  8	); 	
		HEXADECIMAL_DOUBLE	( 11 , 11 , COLOR_WHITE		, ( (unsigned)pointer &0x000000FF ) 	    );	
		Gpu::ascii		(  1 , 12 , "    *U8 :  0x"	, COLOR_WHITE					);
		Gpu::ascii		(  1 , 13 , "   *U16 :  0x"	, COLOR_WHITE					);
		Gpu::ascii		(  1 , 14 , "   *U32 :  0x"	, COLOR_WHITE					);
		// U8 interpretation
		HEXADECIMAL_DOUBLE	(  8 , 12 , COLOR_WHITE		, ((( u8)*pointer ) & 0x000000FF ) 	  );
		// U16 Interpretation
		HEXADECIMAL_DOUBLE	(  8 , 13 , COLOR_WHITE		, (((u16)*pointer ) & 0x0000FF00 ) >>  8 );
		HEXADECIMAL_DOUBLE	(  9 , 13 , COLOR_WHITE		, (((u16)*pointer ) & 0x000000FF ) 	  );
		// U32 Interpretation
		HEXADECIMAL_DOUBLE	(  8 , 14 , COLOR_WHITE		, (((u32)*pointer ) & 0xFF000000 ) >> 24 );
		HEXADECIMAL_DOUBLE	(  9 , 14 , COLOR_WHITE		, (((u32)*pointer ) & 0x00FF0000 ) >> 16 );
		HEXADECIMAL_DOUBLE	( 10 , 14 , COLOR_WHITE		, (((u32)*pointer ) & 0x0000FF00 ) >>  8 );
		HEXADECIMAL_DOUBLE	( 11 , 14 , COLOR_WHITE		, (((u32)*pointer ) & 0x000000FF )       );
		
	} else Gpu::ascii( 1, 9, "No additional information was provided."	, COLOR_WHITE);
	
	console.setCursor(1,15);
	console.print(" Press START to reboot this unit.", COLOR_WHITE);
	
	while(1){
		KEYUPDATE();
		if( KEYDOWN_START ) asm("swi 00");
		
		static bool bm;
		if(bm != Gpu::blink){
			bm = Gpu::blink;
			Gpu::set(2, 16, 16, (COLOR_WHITE << 12) | (Gpu::blink ? 0x0116 : 0x0100) );
		}
		Gpu::blinkUpdate(8);
	}
}

void Debug::panic( const char *message, u32 *pointer){
	bsod( 
		message, 
		"System has encountered a problem and has been halted.", 
		"Please report this error if it's not the first time  ",  
		"and it can be replicated by following specific steps.",
		pointer
	);
}

void Debug::error( int error_code, bool recoverable ){

	if( recoverable) while( KEYPRESS_B ) KEYUPDATE();

	// Clear unused palette color
	*(u32*)(BG_PALETTE+16) = 0x00000000;
	
	// Clear screen
	for(int x=0; x<30; x++){
		for(int y=0; y<6; y++){
			Gpu::set( 0	, x	, y , 0 );
			Gpu::set( 1	, x	, y , 0x0000);
			Gpu::set( 2	, x	, y , 0x00FC);
		}
	}
	
	// Select corresponding error color
	u16 color = recoverable ? 0x03E0 : 0x001F; 
		
	// Set red/green palette
	*(u16*)(BG_PALETTE +4) = color;
	*(u16*)(BG_PALETTE+80) = color;
	
	// Draw guru rectangle
	for( int x=1; x < 29 ; x++ ){
		for( int y=1; y <  7 ; y++ ){
			Gpu::set( 1	,  0, y, 0x67);
			Gpu::set( 1	,  x, 0, 0x68);
			Gpu::set( 1	,  x, 6, 0x6A);
			Gpu::set( 1	, 29, y, 0x69);
		}
	}
	Gpu::set( 1	, 0 , 0, 0x62);
	Gpu::set( 1	, 0 , 6, 0x61);	
	Gpu::set( 1	,29 , 0, 0x63);
	Gpu::set( 1	,29 , 6, 0x64);
	
	// Draw error text
	if(!recoverable)
		Gpu::ascii(  2, 2, "Software Failure.    Press START to restart GBA.", COLOR_CYAN );
	else 
		Gpu::ascii(  2, 2, "Breakpoint raised.   Press B button to continue.", COLOR_CYAN );
	Gpu::ascii(  5, 4, "Guru Meditation ", COLOR_CYAN );
	Gpu::set(2, 16, 4, (COLOR_CYAN <<12) | 0x2D );
	HEXADECIMAL_DOUBLE( 17, 4, COLOR_CYAN , error_code>>24);
	HEXADECIMAL_DOUBLE( 18, 4, COLOR_CYAN , error_code>>16);
	Gpu::set(2, 19, 4, (COLOR_CYAN <<12) | 0x2C );
	HEXADECIMAL_DOUBLE( 20, 4, COLOR_CYAN , error_code>>8);
	HEXADECIMAL_DOUBLE( 21, 4, COLOR_CYAN , error_code	 );

	while(1){
		// Palette blink
		counter++;
		if( Gpu::isVblank()){
			KEYUPDATE();
		}
		if( recoverable){
			if( KEYDOWN_B ){
				while( !KEYUP_B ){ KEYUPDATE(); };
				for(int x=0; x<30; x++){
					for(int y=0; y<7; y++){
						Gpu::set( 0	, x	, y , 0 );
						Gpu::set( 1	, x	, y , 0x0000);
						Gpu::set( 2	, x	, y , 0x00FC);
					}
				}			
				Gpu::loadPalette();
				return;
			}
		} else {
			if( KEYDOWN_START ) {
				asm("swi 00");
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
	Gpu::clear();
	int x;
	
	for(x=0; x<30; x++){
		Gpu::set( 0	, x , -x , 0x002A);
		Gpu::set( 0	, x , (-x)+1 , 0x003A);
		Gpu::set( 0	, x , (-x)+2 , 0x0013);
		Gpu::set( 0	, x , (-x)+3 , 0x0003);
		Gpu::set( 0	, x , (-x)+4 , 0x0015);
		Gpu::set( 0	, x , (-x)+5 , 0x0005);
	}
	
	static u16 counter=0;
	while(1){
		Gpu::set( 2	,  5, 1 , (((counter+0x200)&0xfff) > 0x800 ?0x4000:0x7000) | (((counter+0x200)&0x7ff) > 0x400 ?0xb3:0xa2));
		Gpu::bigString	(  6 , 0 , "PROGRAM HALTED"	, (counter&0x7ff) > 0x400 ? COLOR_WHITE : COLOR_RED);
		Gpu::string		(  3 , 3 , "LINE"			, COLOR_YELLOW);
		Gpu::number	(  8 , 3 , line				, COLOR_OLIVE);
		Gpu::string		(  3 , 5 , "FILE"			, COLOR_YELLOW);
		Gpu::ascii		(  5 , 5 , filename			, COLOR_OLIVE);
		counter++;
	};
}

// Stress test for GUI Display Functions
void Debug::benchmark( ){
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

void Debug::updateMemTest(){
}

ProgressBar::ProgressBar(){
	redraw();
}
		
void ProgressBar::redraw(){
	for(int s=0; s<0x30; s++){
		Gpu::set(0, s,19, 0x29);
	}
}

void ProgressBar::render(float q, bool highlight ){
	Gpu::set(0, (u8)(q*30),19, highlight ? 0x25 : 0x23);
}

void Debug::memoryTest(Control* c, bool bigstep, bool add, u32* pointer ){

	u16 p = 0;
	
	Console 	console("MEMORY TEST", 0x0010 + COLOR_DARK_CYAN);
	ProgressBar progressbar;

	console.print("");
	
	// Fill SRAM with known values 
	console.print("Filling SRAM 0x0000 ~ "STRINGIFY( SRAM_SIZE ) );
	SRAM.seek(0);
	for( int i=0, p=0; i < SRAM_SIZE ; i++,p++){
		SRAM.write(0x80 + (i&0xF));
		if( (i &0xF) == 0){
			float q = ( float( p ) / float( SRAM_SIZE ) );
			progressbar.render( q, false );
			console.percent(q);
		}
		console.update();
	}
	console.percent(1.0f);
	
	// Check values written persistence
	console.print("Checking SRAM Integrity...");
	SRAM.seek(0);
	for( int i=0, p=0; i < SRAM_SIZE ; i++,p++){
		ASSERT(SRAM.read() == (0x80 + (i&0xF)));
		if( (i &0xF) == 0){
			float q = ( float( p ) / float( SRAM_SIZE ) );
			progressbar.render( q, true );
			console.percent(q);
		}
		console.update();
	}
	console.percent(1.0f);
	progressbar.redraw();
	
	console.print("SRAM Integrity check OK", COLOR_GREEN);
	console.wait( 1 );
	
	console.print("Filling song memory...");

	Song *s = &VAR_SONG;

	for( VAR_CFG.SLOT=0, p=0; VAR_CFG.SLOT < SONG_SLOT_COUNT; VAR_CFG.SLOT++ ){
		
		// Set title 
		for(int a=0; a<14;a++){
			s->TITLE	[ a ] = VAR_CFG.SLOT;
			s->ARTIST	[ a ] = a;
		}
		
		s->NOTEMPTY = true;
		
		// Write groove table
		for(int i=0; i<0xF; i++){
			s->GROOVE.STEP[ i ] = (0x80 + (i&0xF));
		}
		
		for( int c=0; c < CHANNEL_COUNT; c++){
			for(int i=0; i < ORDER_COUNT; i++ ){
				u8 val = 0x40 + (p & 0x3f);
				s->PATTERNS[ c ].ORDER[ i ] = val;
				s->PATTERNS[ c ].TRANSPOSE[ i ] = val;
				console.update();
				if( (p&0xF) == 0){
					float q = ( float( p ) / float( ( ORDER_COUNT-1 ) * SONG_SLOT_COUNT * CHANNEL_COUNT ) );
					console.percent(q);
					progressbar.render(q, false);
				}
				p++;
			}
		}		

		SRAM.songSave(false);
	}
	console.percent(1.0f);
	
	console.print( "Reading song memory..." );
	for( VAR_CFG.SLOT=0, p = 0; VAR_CFG.SLOT < SONG_SLOT_COUNT; VAR_CFG.SLOT++){
		
		SRAM.songLoad(false);
		
		for(int a=0; a < 14 ; a++){
			ASSERT( s->TITLE[a]==VAR_CFG.SLOT);
			ASSERT( s->ARTIST[a]==a);
		}
		
		ASSERT(s->NOTEMPTY);
		
		// Check groove table
		for(int i=0; i<0xF; i++){
			ASSERT( s->GROOVE.STEP[ i ] == (0x80 + (i&0xF)));
		}
		
		for( int c=0; c<CHANNEL_COUNT; c++){
			for(int i=0; i<ORDER_COUNT; i++ ){
				u8 val = 0x40 + (p & 0x3f);
				ASSERT( s->PATTERNS[c].ORDER[ i ] == val );
				ASSERT( s->PATTERNS[c].TRANSPOSE[i] == val);
				console.update();
				if( (p&0xF)==0){
					float q = ( float( p ) / float( ( ORDER_COUNT-1 ) * SONG_SLOT_COUNT * CHANNEL_COUNT ) );
					console.percent( q );
					progressbar.render(q, true);
				}
				p++;
			}
		}
	}
	console.percent( 1.0f );

	// Reset slot to first item
	VAR_CFG.SLOT = 0;
	
	console.print("Checks passed successfully."	, COLOR_GREEN );
	console.wait( 1 );
	RegionHandler::redraw = true;
	RegionHandler::update(0);
}

#else

void Debug::init			( ){}
void Debug::benchmark		( ){}
void Debug::halt			( const char *filename, int line 	 				){}
void Debug::panic			( const char *filename, u32 *pointer 				){}
void Debug::error			( int error_code , bool recoverable  				){}
void Debug::watch			( const char *varname	, u8 var 	 				){}
void Debug::watch			( const char *varname	, size_t var 				){}
void Debug::watch			( const char *varname	, u32 var 	 				){}
void Debug::watch			( const char *varname	, u16 var 	 				){}
void Debug::watch			( const char *varname	, u32 var 	, u8 size		){}
void Debug::runTests		( ){}
void Debug::updateWatch		( ){}
void Debug::updateMemTest  	( ){}
void Debug::memoryTest		( Control* c, bool bigstep, bool add, u32* pointer 	){}
void Debug::watchArray		( ){}
	
#endif
