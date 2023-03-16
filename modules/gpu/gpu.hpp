#ifndef _GPU_HPP
#define _GPU_HPP

#include "../../agb.h" 
#include "../dma/dma.h"
#include "../../data/tileset.hpp"
#include "../../data/layers.hpp"
#include "../../data/tables.hpp"

#define VLOCK() R_DISPCNT = DISP_FORCE_HBLANK | (DISP_BG0_ON | DISP_BG1_ON |  DISP_BG2_ON) &0x0f00
#define VUNLOCK() R_DISPCNT = (DISP_BG0_ON | DISP_BG1_ON |  DISP_BG2_ON) &0x0f00
	
#define BG_BG					0
#define BG_HUD					1
#define BG_TEXT					2

#define BG_COLOR_16             0x0000      // Select 16 colors
#define BG_COLOR_256            0x0080      // Select 256 colors
#define BG_SCREEN_SIZE_0        0x0000      // Screen Size (256x256)
#define BG_SCREEN_SIZE_1        0x4000      // Screen Size (512x256)
#define BG_SCREEN_SIZE_2        0x8000      // Screen Size (256x512)
#define BG_SCREEN_SIZE_3        0xc000      // Screen Size (512x512)

#define BG_PRIORITY_0           0x0000      // BG 0 Priority
#define BG_PRIORITY_1           0x0001      // BG 1 Priority
#define BG_PRIORITY_2           0x0002      // BG 2 Priority
#define BG_PRIORITY_3           0x0003      // BG 3 Priority

#define BG_PRIORITY_SHIFT       0
#define BG_CHAR_BASE_SHIFT      2
#define BG_SCREEN_BASE_SHIFT    8
#define BG_SCREEN_SIZE_SHIFT    14

#define DISP_MODE_0             0x0000      // BG Mode 0
#define DISP_MODE_1             0x0001      // BG Mode 0
#define DISP_MODE_2             0x0002      // BG Mode 0
#define DISP_MODE_3             0x0003      // BG Mode 0
#define DISP_MODE_4             0x0004      // BG Mode 0
#define DISP_MODE_5             0x0005      // BG Mode 0
#define DISP_FORCE_HBLANK       0x0080      // Force HBLANK Flag
#define DISP_BG0_ON             0x0100      // BG0 ON
#define DISP_BG1_ON             0x0200      // BG1 ON
#define DISP_BG2_ON             0x0400      // BG2 ON
#define DISP_BG3_ON             0x0800      // BG3 ON
#define DISP_BG_ALL_ON          0x0f00      // All BG ON

// Screen & Character Database Address
#define CHAR_BASE0_ADDR		(VRAM+0x0000)
#define CHAR_BASE1_ADDR		(VRAM+0x4000)
#define CHAR_BASE2_ADDR		(VRAM+0x8000)
#define CHAR_BASE3_ADDR		(VRAM+0xc000)
#define SCREEN_BASE0_ADDR	(CHAR_BASE0_ADDR+0x2000) //screen base block #4
#define SCREEN_BASE1_ADDR	(CHAR_BASE0_ADDR+0x2800) //screen base block #5
#define SCREEN_BASE2_ADDR	(CHAR_BASE0_ADDR+0x3000) //screen base block #6

// Character Base Block Specification
#define CHAR_BASE_0  0x0
#define CHAR_BASE_1  0x4
#define CHAR_BASE_2  0x8
#define CHAR_BASE_3  0xC

// Screen Base Block Specification
#define SCR_BASE0  0x0000
#define SCR_BASE1  0x0100
#define SCR_BASE2  0x0200
#define SCR_BASE3  0x0300
#define SCR_BASE4  0x0400
#define SCR_BASE5  0x0500
#define SCR_BASE6  0x0600
#define SCR_BASE7  0x0700

#define R_BGCNT       *(u16*)(REG_BASE + 0x8)    // BG Control
#define R_BG0CNT      *(u16*)(REG_BASE + 0x8)    // BG 0 Control
#define R_BG1CNT      *(u16*)(REG_BASE + 0xa)    // BG 1 Control
#define R_BG2CNT      *(u16*)(REG_BASE + 0xc)    // BG 2 Control
#define R_BG3CNT      *(u16*)(REG_BASE + 0xe)    // BG 3 Control
#define R_BGOFS       (REG_BASE + 0x10)   // BG Offset
#define R_BG0HOFS     (REG_BASE + 0x10)   // BG 0 H Offset
#define R_BG0VOFS     (REG_BASE + 0x12)   // BG 0 V Offset
#define R_BG1HOFS     (REG_BASE + 0x14)   // BG 1 H Offset
#define R_BG1VOFS     (REG_BASE + 0x16)   // BG 1 V Offset
#define R_BG2HOFS     (REG_BASE + 0x18)   // BG 2 H Offset
#define R_BG2VOFS     (REG_BASE + 0x1a)   // BG 2 V Offset
#define R_BG3HOFS     (REG_BASE + 0x1c)   // BG 3 H Offset
#define R_BG3VOFS     (REG_BASE + 0x1e)   // BG 3 V Offset
#define R_BG2AFFINE   (REG_BASE + 0x20)   // BG 2 Affin Transformation Parameters
#define R_BG2PA       (REG_BASE + 0x20)   // BG 2 Line Direction X Coordinate Difference
#define R_BG2PB       (REG_BASE + 0x22)   // BG 3 Vertical Direction X Coordinate Difference
#define R_BG2PC       (REG_BASE + 0x24)   // BG 2 Line Direction Y coordinate Difference
#define R_BG2PD       (REG_BASE + 0x26)   // BG 2 Vertical Direction Y Coordinate Difference
#define R_BG2X        (REG_BASE + 0x28)   // BG 2 Start X Coordinate
#define R_BG2X_L      (REG_BASE + 0x28)
#define R_BG2X_H      (REG_BASE + 0x2a)
#define R_BG2Y        (REG_BASE + 0x2c)   // BG 2 Start Y Coordinate
#define R_BG2Y_L      (REG_BASE + 0x2c)
#define R_BG2Y_H      (REG_BASE + 0x2e)
#define R_BG3AFFINE   (REG_BASE + 0x30)   // BG 3 Affin Transformation Parameters
#define R_BG3PA       (REG_BASE + 0x30)   // BG 3 Line Direction X Coordinate Difference
#define R_BG3PB       (REG_BASE + 0x32)   // BG 3 Vertical Direction X Coordinate Difference
#define R_BG3PC       (REG_BASE + 0x34)   // BG 3 Line Direction Y Coordinate Difference
#define R_BG3PD       (REG_BASE + 0x36)   // BG 3 Vertical Direction Y Coordinate Difference
#define R_BG3X        (REG_BASE + 0x38)   // BG 3 Start X Coordinate
#define R_BG3X_L      (REG_BASE + 0x38)
#define R_BG3X_H      (REG_BASE + 0x3a)
#define R_BG3Y        (REG_BASE + 0x3c)   // BG 3 Start Y Coordinate
#define R_BG3Y_L      (REG_BASE + 0x3c)
#define R_BG3Y_H      (REG_BASE + 0x3e)

enum E_Maps {
	MAP_HLP = 0,
	MAP_INS,
	MAP_TRK,
	MAP_CFG,
	MAP_CACHE,
	MAP_NONE 
};

extern const u32 LAYERS[3];
extern const unsigned short Palette[256];

class Gpu {	
  private:
	static vu8 					redraw;

  public:
	static const unsigned short*		MAP0;
	static const unsigned short*		MAP1;
	static const unsigned short*		MAP2;
  	static const	u16			colors[ 16 ][ 2 ];
	static bool 					blink;
	static u16 					vcount;
		
	static void 	blinkUpdate	( int speed = 0 );
	static void 	blit			( E_Maps mapIndex, int startx, int starty, int x, int y, int width, int height);
	static void 	blitBg		( E_Maps mapIndex,  u8 startx, int starty,  u8 x,  u8 y,  u8 width,  u8 height);
	static void 	blitSafe		( E_Maps mapIndex, int startx, int starty, int x, int y, int width, int height);
	static void 	blitAlt		( const u16 *map_address, int startx, int starty, int x, int y, int width, int height);
	static void	clear			( u16 color = 0x10, u16 foreground=0x00FC);	
	static u16 	get			( u8 layer, u8 x, u8 y );
	static void 	init			( );	
	static bool 	isVblank		( );
	static void 	loadPalette	( );
	static void 	set			( u8 layer, u8 x, u8 y, u16 index );

	static void	update		( u8 delta );
	
	// Data output helpers
	static void	string	 	( u8 x, u8 y, const char *filename	, u8 color=6 );
	static void	ascii	 		( u8 x, u8 y, const char *filename	, u8 color=6 );
	static void	number	 	( u8 x, u8 y, u32 number 				, u8 color=6 );
	static void	hexnum	 	( u8 x, u8 y, u32 number 				, u8 color=6 );
	static void	bigString		( u8 x, u8 y, const char *filename	, u8 color=6 );	
	static void	drawDialog	( u8 x, u8 y, u8 width					, u8 height	, const char *caption=NULL );
};

void STRING						( bool big, u8 x , u8 y , u8* data				);
void HEXADECIMAL				( 			u8 x , u8 y	, u16 color	, u16 value );
void HEXADECIMAL_DOUBLE			( 			u8 x , u8 y	, u16 color	, u16 value );
void HEXADECIMAL_DOUBLE_DASH	( 			u8 x , u8 y	, u16 color	, u16 value );
void HEXADECIMAL_INSTRUMENT		( 			u8 x , u8 y	, u16 color	, u16 value );
void HEXADECIMAL_TWOTILES		( 			u8 x , u8 y	, u16 color	, u16 value );
void DECIMAL					( 			u8 x , u8 y	, u16 color	, u16 value );
void DECIMAL_DOUBLE				( 			u8 x , u8 y	, u16 color	, u16 value );
void DECIMAL_TWOTILES			( 			u8 x , u8 y	, u16 color	, u16 value );
void DECIMAL_DOUBLE_TWOTILES	( 			u8 x , u8 y	, u16 color	, u16 value );
void NIBBLE						( 			u8 x , u8 y	, u16 color	, u16 value );
void PROGRESS					( 			u8 x , u8 y	, u16 color	, u16 value );
void WAVE_DOUBLE				( 			u8 x , u8 y	, u16 color	, u16 value );
void PIXEL						( 			u8 x , u8 y	, u16 color	, u16 value );
void WAVE_SINGLE				( 			u8 x , u8 y	, u16 color	, u16 value );
void BIGHEX						( 			u8 x , u8 y	, u16 color	, u16 value );
void BIGDEC						( 			u8 x , u8 y	, u16 color	, u16 value );
void BIGTEXT					( 			u8 x , u8 y	, u16 color	, u16 value );
void TEXT						( 			u8 x , u8 y	, u16 color	, u16 value );
void VUMETER_V3					( 			u8 x , u8 y	, u16 color	, u16 value );
void VUMETER_H3					( 			u8 x , u8 y	, u16 color	, u16 value );
void VUMETER_H1					( 			u8 x , u8 y	, u16 color	, u16 value );
void VUMETER_V1					( 			u8 x , u8 y	, u16 color	, u16 value );
void COMMAND					( 			u8 x , u8 y	, u16 color	, u16 value );
void NOTE						( 			u8 x , u8 y	, u16 color	, u16 value );
void TABLEPOS					( 			u8 x , u8 y	, u16 color	, u16 value );
void VISPOS1					( 			u8 x , u8 y	, u16 color	, u16 value );
void VISPOS2					( 			u8 x , u8 y	, u16 color	, u16 value );
void STATUS						( 			u8 x , u8 y	, u16 color	, u16 value );


#endif 
