#include "gpu.hpp"
#include "virtualscreen.hpp"
#include "../../data/caches.hpp"
#include "../../debug.hpp"

const unsigned short*	Gpu::MAP0;
const unsigned short*	Gpu::MAP1;
const unsigned short*	Gpu::MAP2;
vu8  						Gpu::redraw;
bool 						Gpu::blink;
u16 						Gpu::vcount;


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

const u16 Gpu::colors[16][2] = {
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

/* ----------------------------------------------------------------------------------------------------------------------------------------------------------------	*/
/* TODO : MOVE THESE TO STATIC GPU METHODS 																															*/
/*-----------------------------------------------------------------------------------------------------------------------------------------------------------------	*/
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
