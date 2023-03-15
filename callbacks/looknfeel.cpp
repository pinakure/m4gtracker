#include "looknfeel.hpp"
#include "../modules/spu/mixer.hpp"
#include "../modules/spu/synth.hpp"
#include "../modules/sys/sys.hpp"
#include "../modules/gpu/gpu.hpp"
#include "../modules/key/key.hpp"
#include "../data/data.hpp"

#define MOSAIC_CNT *(u16*)0x400004C
#define REG_BG0 *(u16*)0x4000008
#define REG_BG1 *(u16*)0x400000A
#define REG_BG2 *(u16*)0x400000C
#define REG_BG3 *(u16*)0x400000E

void rotateGfx(){
	static u32 bigIndex = 0;
	static u8 index = 3;
	#define ROWSIZE 		(16*32)
	#define FONTSIZE 		(ROWSIZE * 3)
	
	//#define CYCLEPOS 		CHAR_BASE0_ADDR+(ROWSIZE*4)
	#define CYCLEPOS 		CHAR_BASE0_ADDR+(ROWSIZE*4)
	#define CYCLEPOS1 		CYCLEPOS+(index*32)
	#define CYCLEPOS2 		CYCLEPOS+(32*6)+(index*32)
	#define CYCLEBAK		CHAR_BASE0_ADDR+(ROWSIZE*10)
	if(bigIndex < 50) bigIndex++; else {
		// Draw M4G logo
		DmaCopy(3, CYCLEPOS1, CYCLEBAK , 32, 16);
		DmaCopy(3, CYCLEPOS2, CYCLEPOS1, 32, 16);
		DmaCopy(3, CYCLEBAK , CYCLEPOS2, 32, 16);
	
		index++;
		if(index == 6)index=0;

		bigIndex = 0x0;
	}
}

void LookNFeel::logoFade(){
	int freeze=0;
	REG_BG0 |= 0x0040;
	REG_BG1 |= 0x0040;
	REG_BG2 |= 0x0040;
	REG_BG3 |= 0x0040;
	
	if( VAR_CFG.LOOKNFEEL.STARTUPSFX ) Mixer::start();
	
	Synth::lfo = 0x00;
	
	while( freeze < 0xfff>>4 ){
		regHnd.drawVerticalCache(8,7,&CACHE_LOGOTYPE,0,0);
		//KEYUPDATE();
		MOSAIC_CNT = 0xFF - ((freeze )&0xFF);
		if( VAR_CFG.LOOKNFEEL.STARTUPSFX ) {
			Synth::polysynth( 0xffffffff - ((freeze>>4)+(freeze>>2)) );
		}
		if( ((R_VCOUNT&0x00FF) >=160)){
		//if(!gpu.isVblank()) {
			freeze++;
			for(int i=0;i<16;i++) {
				int c1 = Palette[4+((freeze)&0x7)];
				int r, g, b;
				if(!(freeze&7)||!(freeze&3)){
					r = (u8)( ( (c1 & 0x001f)	   )  * ((freeze*3) / (0xf * 3))); 
					g = (u8)( ( (c1 & 0x02e0) >> 5  ) * ((freeze*3) / (0xf * 3)));
					b = (u8)( ( (c1 & 0x3d00) >> 10 ) * ((freeze*3) / (0xf * 3)));
				} else {
					r = 0;
					g = 0;
					b = 0;
				}
				c1 = (b << 10) | (g << 5) | r;
				*(u8*)(BG_PALETTE+(i*0x8)) = c1;
				*(u8*)(BG_PALETTE+(i*0x8)+0x04) = 0xFFFF-c1;
			}		
		}
			rotateGfx();
	}
	if( VAR_CFG.LOOKNFEEL.STARTUPSFX ) Mixer::stop();
	
	REG_BG0 	^= 0x0040;
	REG_BG1 	^= 0x0040;
	REG_BG2 	^= 0x0040;
	REG_BG3 	^= 0x0040;
	MOSAIC_CNT 	 = 0x0000;
}

void LookNFeel::logoWait(){
	int freeze=0;
	
	gpu.loadPalette();
	while( (freeze < 2048) && !KEYACTIVITY() ){
		regHnd.drawVerticalCache(8,7,&CACHE_LOGOTYPE,0,0);
		KEYUPDATE();
		rotateGfx(); 
		freeze++;
	}
	gpu.loadPalette();
}

void LookNFeel::init(){
	if( VAR_CFG.LOOKNFEEL.SHOWLOGO ){
		logoFade();
		logoWait();
	}
}

// Called when a look and feel value is changed
void LookNFeel::update( RegionHandler* rh ){
	static u8 lastBorder = VAR_CFG.LOOKNFEEL.BORDER;
	static u8 lastFont = VAR_CFG.LOOKNFEEL.FONT;
	
	rotateGfx();
	
	// Reload HUD style
	if((VAR_CFG.LOOKNFEEL.BORDER != lastBorder)||(VAR_CFG.RELOADSKIN)){
		lastBorder = VAR_CFG.LOOKNFEEL.BORDER;
		DmaCopy(3, CHAR_BASE1_ADDR+ROWSIZE+(ROWSIZE*lastBorder),CHAR_BASE1_ADDR, ROWSIZE, 16);
	}

	// Reload font		
	if((VAR_CFG.LOOKNFEEL.FONT != lastFont)||(VAR_CFG.RELOADSKIN)){
		lastFont = VAR_CFG.LOOKNFEEL.FONT;
		DmaCopy(3, CHAR_BASE2_ADDR+(ROWSIZE*36)+(FONTSIZE*lastFont),CHAR_BASE2_ADDR, FONTSIZE, 16);
		VAR_CFG.RELOADSKIN = false;
	}
	#undef FONTSIZE
	#undef ROWSIZE	
}

void LookNFeel::credits( RegionHandler* rh ){

}
