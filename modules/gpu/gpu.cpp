#include "gpu.hpp"
#include "virtualscreen.hpp"
#include "../../debug.hpp"
#include "../../data/helpers.hpp"
#include "../../data/caches.hpp"

const unsigned short*	Gpu::MAP0;
const unsigned short*	Gpu::MAP1;
const unsigned short*	Gpu::MAP2;
vu8  						Gpu::redraw;
bool 						Gpu::blink;
u16 						Gpu::vcount;
const	u16				Gpu::colors[16][2] = {
	{ COLOR_NONE			<< 12 , COLOR_NONE 			<< 12 },
	{ COLOR_DARK_CYAN	<< 12 , COLOR_CYAN 			<< 12 },
	{ COLOR_CYAN			<< 12 , COLOR_DARK_CYAN 	<< 12 },
	{ COLOR_RED			<< 12 , COLOR_DARK_RED 	<< 12 },
	{ COLOR_ORANGE		<< 12 , COLOR_BROWN 		<< 12 },
	{ COLOR_DARK_RED	<< 12 , COLOR_RED 			<< 12 },
	{ COLOR_WHITE		<< 12 , COLOR_GRAY 			<< 12 },
	{ COLOR_YELLOW		<< 12 , COLOR_OLIVE 		<< 12 },
	{ COLOR_BLACK		<< 12 , COLOR_BLACK 		<< 12 },
	{ COLOR_DARK_BLUE	<< 12 , COLOR_BLUE 			<< 12 },
	{ COLOR_BROWN		<< 12 , COLOR_ORANGE 		<< 12 },
	{ COLOR_OLIVE		<< 12 , COLOR_YELLOW 		<< 12 },
	{ COLOR_DARK_GREEN	<< 12 , COLOR_GREEN 		<< 12 },
	{ COLOR_GRAY			<< 12 , COLOR_WHITE 		<< 12 },
	{ COLOR_BLUE			<< 12 , COLOR_DARK_BLUE 	<< 12 },
	{ COLOR_GREEN		<< 12 , COLOR_DARK_GREEN 	<< 12 },
};

const u32 LAYERS[3] = {
	SCREEN_BASE0_ADDR,
	SCREEN_BASE1_ADDR,
	SCREEN_BASE2_ADDR
};
	
void Gpu::loadPalette(){
	// Load palette
	DmaArrayCopy(3, Palette, BG_PALETTE, 16);
	
	/* txt layer's palette trick */
	for(int i=0;i<16;i++)
	{
	//	 NUMBERS[i] = i;
	
		 *(u16*)(BG_PALETTE+(i*0x20))		=	*(u16*)(BG_PALETTE);
		 *(u16*)(BG_PALETTE+(i*0x20)+0x10)	=	*(u16*)(BG_PALETTE+(i*0x2));
	 }					
}

void Gpu::update(u8 delta) {
	if(redraw>0){
		//redraw level = redraw value
		redraw--;
	} 
}

void Gpu::clear( u16 color, u16 foreground ){
	for(int x=0; x<30; x++){
		for(int y=0; y<20; y++){
			set( 0	, x	, y , color );
			set( 1	, x	, y , 0x0000);
			set( 2	, x	, y , foreground);
		}
	}
}

bool Gpu::isVblank() {
	return (R_VCOUNT&0x00FF >=160);
}

void Gpu::init(){

	redraw = 0;
	vcount = 0;
	blink  = false;

	// Force HBLANK to access VRAM
	R_DISPCNT = DISP_FORCE_HBLANK; 
	
		
	loadPalette();
	
	// Load tilesets
	DmaCopy(3, TILESET_0,CHAR_BASE0_ADDR, 8192, 16);//256 tiles
	DmaCopy(3, TILESET_1,CHAR_BASE1_ADDR, 16384, 16);//512 tiles
	DmaCopy(3, TILESET_2,CHAR_BASE2_ADDR, 16384*2, 16);//1024 tiles
	
	// Initialize layers
	R_BG0CNT = SCR_BASE4 | CHAR_BASE_0 | BG_PRIORITY_2 | BG_COLOR_16 | BG_SCREEN_SIZE_0;
	R_BG1CNT = SCR_BASE5 | CHAR_BASE_1 | BG_PRIORITY_1 | BG_COLOR_16 | BG_SCREEN_SIZE_0;
	R_BG2CNT = SCR_BASE6 | CHAR_BASE_2 | BG_PRIORITY_0 | BG_COLOR_16 | BG_SCREEN_SIZE_0;
	
	// Set LCD Video Mode 2 (240x160, 3 layers, 1024 tiles per layer, 64x64 mcg)
	R_DISPCNT = DISP_MODE_0;
	
	R_DISPCNT = (DISP_BG0_ON | DISP_BG1_ON |  DISP_BG2_ON) &0x0f00;			
	
	VirtualScreen::init();
}

void Gpu::blitSafe(E_Maps mapIndex, int startx, int starty, int x, int y, int width, int height){
	//copy area being to be written to a safe place first (safe place would be defined dirty area)
}

void Gpu::blitBg(E_Maps mapIndex, u8 startx, int starty, u8 x, u8 y, u8 width, u8 height){
	static int offsetSrc;
	static int offsetDst;
	
	width = width << 1; //double word per item!
	x<<=1;
	
	MAP0 = MAPDATA + ((mapIndex * 3) << 12);
		
	for(int sy=starty; sy<height+starty; sy++){
		offsetSrc = (sy<<6)+startx;
		offsetDst = (y<<6)+x;
		
		DmaCopy(3, MAP0+offsetSrc, (SCREEN_BASE0_ADDR)+offsetDst, width, 32);
		y++;
	}		
}

void Gpu::blit(E_Maps mapIndex, int startx, int starty, int x, int y, int width, int height){
	static int offsetSrc;
	static int offsetDst;
	
	width = width << 1; //double word per item!
	x<<=1;
	
	MAP0 = MAPDATA + ((mapIndex * 3) << 12);
	MAP1 = MAP0 + 4096;
	MAP2 = MAP1 + 4096;
		
	//R_DISPCNT = DISP_FORCE_HBLANK | (DISP_BG0_ON | DISP_BG1_ON |  DISP_BG2_ON) &0x0f00;			
	for(int sy=starty; sy<height+starty; sy++){
		offsetSrc = (sy<<6)+startx;
		offsetDst = (y<<6)+x;
		
		DmaCopy(3, MAP0+offsetSrc, (SCREEN_BASE0_ADDR)+offsetDst, width, 32);
		DmaCopy(3, MAP1+offsetSrc, (SCREEN_BASE1_ADDR)+offsetDst, width, 32);
		DmaCopy(3, MAP2+offsetSrc, (SCREEN_BASE2_ADDR)+offsetDst, width, 32);
		y++;
	}
	//R_DISPCNT = (DISP_BG0_ON | DISP_BG1_ON |  DISP_BG2_ON) &0x0f00;
}

void Gpu::set(u8 layer, u8 x, u8 y, u16 index) {
	*(u16*)(*(((const u32 *) LAYERS)+ layer) + ( (y << 6 /*x64*/) + (x << 1) )) = index;
}

u16 Gpu::get(u8 layer, u8 x, u8 y) {
	return *(u16*)(*(((const u32 *) LAYERS)+ layer) + ( (y << 6 /*x64*/) + (x << 1) ));
}

void Gpu::blinkUpdate( int speed){
	static bool blink_lock = false;
	if( ((R_VCOUNT&0x00FF) >=160) && (!blink_lock)){
		blink_lock ^= 1;
		vcount++;
	} else blink_lock = false;
	
	/*
	HEXADECIMAL_DOUBLE(6,4,0xFF, ( vcount >>8  	) & 0xFF );
	HEXADECIMAL_DOUBLE(7,4,0xFF, ( vcount 		) & 0xFF );
	*/
	blink = (vcount & 0xFFFF>>speed) > (0x8000>>speed);
}

void Gpu::blitAlt(const u16 *map_address, int startx, int starty, int x, int y, int width, int height){
	int offsetSrc;
	int offsetDst;
	
	width = width << 1; //double word per item!
	
	x<<=1;
	
	//R_DISPCNT = DISP_FORCE_HBLANK | (DISP_BG0_ON | DISP_BG1_ON |  DISP_BG2_ON) &0x0f00;			
	for(int sy=starty; sy<height+starty; sy++){
		offsetSrc = (sy<<6)+startx;
		offsetDst = (y<<6)+x;
		
		DmaCopy(3, map_address + offsetSrc, (SCREEN_BASE0_ADDR)+offsetDst, width, 16);
		DmaCopy(3, map_address + 4096 + offsetSrc, (SCREEN_BASE1_ADDR)+offsetDst, width, 16);
		DmaCopy(3, map_address + 8192 + offsetSrc, (SCREEN_BASE2_ADDR)+offsetDst, width, 16);
		y++;
	}
	//R_DISPCNT = (DISP_BG0_ON | DISP_BG1_ON |  DISP_BG2_ON) &0x0f00;
}

/* ########################################################################## */
/* DATA OUTPUT HELPERS 														  */
/* ########################################################################## */

void Gpu::ascii( u8 x, u8 y, const char *data, u8 color ){
	for(int i=x, l=i + strlen(data), x=x%1; i<l; i++){
		char d = *data;
		
		if(( i & 0x1)) 
			Gpu::set(2, x+( i>>1 ), y, (color << 12) |  (0x0100 + d) );
		else 
			Gpu::set(1, x+( i>>1 ), y, (color << 12) |  (0x0100 + d) );
		data++;
	}
}

void Gpu::number( u8 x, u8 y, u32 num, u8 color ){
	DECIMAL_DOUBLE( x  , y, color, (num/1000000	)%100 );// 16
	DECIMAL_DOUBLE( x+1, y, color, (num/10000	)%100 );// 65
	DECIMAL_DOUBLE( x+2, y, color, (num/100		)%100 );// 53
	DECIMAL_DOUBLE( x+3, y, color, (num			)%100 );// 5
}

void Gpu::hexnum( u8 x, u8 y, u32 num, u8 color ){
	HEXADECIMAL_DOUBLE( x  , y, color, num>> 24 );// 16
	HEXADECIMAL_DOUBLE( x+1, y, color, num>> 16 );// 65
	HEXADECIMAL_DOUBLE( x+2, y, color, num>>  8 );// 53
	HEXADECIMAL_DOUBLE( x+3, y, color, num      );// 5
}

void Gpu::bigString( u8 x, u8 y, const char *data, u8 color ){
	color = ((color&0xf) << 4 ) | (color&0xf);
	for(int i=x, l=x+strlen(data); i<l; i++){
		if( data[0] != ' ' ) BIGTEXT( i, y, color, data[0] - 0x41); 
		data++;
	}
}

void Gpu::string( u8 x, u8 y, const char *data, u8 color ){
	
	const char OFFSET = 'a' - 'A';
	
	for(int i=x, l=x+strlen(data), c = 0; i<l; i++){
		char d = ( ( *data >= 'a' ) && ( *data <= 'z' ) ) ? *data - OFFSET : *data;
		if( ( d >= '0' ) && ( d <= '9' ) ) 
			c = colors[ color&0xf][ 1 ] | ( d - 0x30);
		else 
			c = colors[ color&0xf ][ 0 ] | TABLE_TEXT[ d - 0x41 ][ 0 ];
		if( data[0] != ' ' ) Gpu::set(2, i, y, c );
		data++;
	}
}

void Gpu::drawDialog( u8 x, u8 y, u8 width, u8 height, const char *caption ){
	for(int dx = x, ldx=width+x; dx<ldx; dx++){
		for(int dy = y, ldy=height+y; dy<ldy; dy++){
			Gpu::set(0, dx, dy, 0x0019	);
			Gpu::set(1, dx, dy, 0x0000	);
			Gpu::set(2, dx, dy, 0x0100	);
			if( dx == x){
				if( dy == ldy - 1 	) { Gpu::set( 1, dx, dy, 0x08 ); continue; }//LOW LEFT
				if( dy == y			) { Gpu::set( 1, dx, dy, 0x06 ); continue; }//TOP LEFT
				Gpu::set( 1, dx, dy, 0x0A );
				continue;
			}
			if( dx == ldx - 1 ){
				if( dy == ldy - 1	) { Gpu::set( 1, dx, dy, 0x07 ); continue; }//LOW RIGHT
				if( dy == y			) { Gpu::set( 1, dx, dy, 0x05 ); continue; }//TOP RIGHT
				Gpu::set( 1, dx, dy, 0x0A );
				continue;
			}
			if(( dy == ldy - 1 )||( dy == y )){
				Gpu::set( 1, dx, dy, 0x09 );
				continue;
			}
			//Gpu::set(0, dx, dy, 0x0);
		}
	}
	if(!caption) return;
	ascii( (x<<1) + 2, y, caption, COLOR_CYAN );
	for(int i=0; i<(u8)(strlen(caption)>>1); i++){
		Gpu::set(0, x+1+i, y, 0x11);
	}
}
