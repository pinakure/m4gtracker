#include "gpu.hpp"

#include "../../data/tileset.hpp"
#include "../../data/layers.hpp"
#include "../../data/palette.hpp"

vu8 					GPU::redraw;
VirtualScreen*			GPU::vs;
const unsigned short*	GPU::MAP0;
const unsigned short*	GPU::MAP1;
const unsigned short*	GPU::MAP2;
const u32 				GPU::LAYERS[3] = {
	SCREEN_BASE0_ADDR,
	SCREEN_BASE1_ADDR,
	SCREEN_BASE2_ADDR
};

bool GPU::isVblank(){
    return (R_VCOUNT & 0x00FF) >= 160;
}

void GPU::start(){
	redraw = 0;

    // Force HBLANK to access VRAM
    R_DISPCNT = DISP_FORCE_HBLANK; 
            
    // Load palette
    DmaArrayCopy(3, Palette::DATA, BG_PALETTE, 16);
    
    /* txt layer's palette trick */
        for(int i=0;i<16;i++)
        {
    //	 NUMBERS[i] = i;
    
            *(u16*)(BG_PALETTE+(i*0x20))		=	*(u16*)(BG_PALETTE);
            *(u16*)(BG_PALETTE+(i*0x20)+0x10)	=	*(u16*)(BG_PALETTE+(i*0x2));
        }					
        
    // Load tilesets
    DmaCopy(3, Tiles::SET_0,CHAR_BASE0_ADDR, 8192, 16);//256 tiles
    DmaCopy(3, Tiles::SET_1,CHAR_BASE1_ADDR, 16384, 16);//512 tiles
    DmaCopy(3, Tiles::SET_2,CHAR_BASE2_ADDR, 16384*2, 16);//1024 tiles
    
    // Initialize layers
    R_BG0CNT = SCR_BASE4 | CHAR_BASE_0 | BG_PRIORITY_2 | BG_COLOR_16 | BG_SCREEN_SIZE_0;
    R_BG1CNT = SCR_BASE5 | CHAR_BASE_1 | BG_PRIORITY_1 | BG_COLOR_16 | BG_SCREEN_SIZE_0;
    R_BG2CNT = SCR_BASE6 | CHAR_BASE_2 | BG_PRIORITY_0 | BG_COLOR_16 | BG_SCREEN_SIZE_0;
    
    // Set LCD Video Mode 2 (240x160, 3 layers, 1024 tiles per layer, 64x64 mcg)
    R_DISPCNT = DISP_MODE_0;
    
    R_DISPCNT = (DISP_BG0_ON | DISP_BG1_ON |  DISP_BG2_ON) &0x0f00;			
}

void GPU::safeblit(E_Maps mapIndex, int startx, int starty, int x, int y, int width, int height){
    //copy area being to be written to a safe place first (safe place would be defined dirty area)
}

void GPU::blit_0(E_Maps mapIndex, u8 startx, int starty, u8 x, u8 y, u8 width, u8 height){
    static int offsetSrc;
    static int offsetDst;
    
    width = width << 1; //double word per item!
    x<<=1;
    
    MAP0 = Layers::DATA + ((mapIndex * 3) << 12);
        
    for(int sy=starty; sy<height+starty; sy++){
        offsetSrc = (sy<<6)+startx;
        offsetDst = (y<<6)+x;
        
        DmaCopy(3, MAP0+offsetSrc, (SCREEN_BASE0_ADDR)+offsetDst, width, 32);
        y++;
    }		
}

void GPU::blit(E_Maps mapIndex, int startx, int starty, int x, int y, int width, int height){
    static int offsetSrc;
    static int offsetDst;
    
    width = width << 1; //double word per item!
    x<<=1;
    
    MAP0 = Layers::DATA + ((mapIndex * 3) << 12);
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

void GPU::set(u8 layer, u8 x, u8 y, u16 index){
    *(u16*)(*(((const u32 *) LAYERS)+ layer) + ( (y << 6 /*x64*/) + (x << 1) )) = index;
}


u16 GPU::get(u8 layer, u8 x, u8 y){
    return *(u16*)(*(((const u32 *) LAYERS)+ layer) + ( (y << 6 /*x64*/) + (x << 1) ));
}

void GPU::update(u8 delta){
    if(redraw>0){
        //redraw level = redraw value
        redraw--;
    } 
}
