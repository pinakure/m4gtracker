#ifndef _GPU_HPP
#define _GPU_HPP

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

class VirtualScreen;

enum E_Maps {
	MAP_HLP = 0,
	MAP_INS,
	MAP_TRK,
	MAP_CFG,
	MAP_CACHE,
	MAP_NONE };

const u32 LAYERS[3] = {
	SCREEN_BASE0_ADDR,
	SCREEN_BASE1_ADDR,
	SCREEN_BASE2_ADDR
};

class GPU {	
  private:
  
	vu8 redraw;

  public:
	const unsigned short *MAP0;
	const unsigned short *MAP1;
	const unsigned short *MAP2;

	VirtualScreen *vs;
	
	GPU(void){
		redraw = 0;
	}
	
	bool isVblank(void) {
		return (R_VCOUNT&0x00FF >=160);
	}

	void start(void){
		
		// Force HBLANK to access VRAM
		R_DISPCNT = DISP_FORCE_HBLANK; 
		
			
		// Load palette
		DmaArrayCopy(3, Palette, BG_PALETTE, 16);
		
		/* txt layer's palette trick */
		 for(int i=0;i<16;i++)
		 {
		//	 NUMBERS[i] = i;
		
			 *(u16*)(BG_PALETTE+(i*0x20))		=	*(u16*)(BG_PALETTE);
			 *(u16*)(BG_PALETTE+(i*0x20)+0x10)	=	*(u16*)(BG_PALETTE+(i*0x2));
		 }					
		
		
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
		
	}
	
	void safeblit(E_Maps mapIndex, int startx, int starty, int x, int y, int width, int height){
		//copy area being to be written to a safe place first (safe place would be defined dirty area)
	}
	
	void blit_0(E_Maps mapIndex, u8 startx, int starty, u8 x, u8 y, u8 width, u8 height){
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
	
	// void blit(E_Maps mapIndex, u8 startx, int starty, u8 x, u8 y, u8 width, u8 height){
	void blit(E_Maps mapIndex, int startx, int starty, int x, int y, int width, int height){
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

	void set(u8 layer, u8 x, u8 y, u16 index) {
		*(u16*)(*(((const u32 *) LAYERS)+ layer) + ( (y << 6 /*x64*/) + (x << 1) )) = index;
	}
	
	u16 get(u8 layer, u8 x, u8 y) {
		return *(u16*)(*(((const u32 *) LAYERS)+ layer) + ( (y << 6 /*x64*/) + (x << 1) ));
	}
	
	void update(u8 delta) {
		if(redraw>0){
			//redraw level = redraw value
			redraw--;
		} 
	}
};

extern GPU gpu;

#include "virtualscreen.cpp"

#endif 
