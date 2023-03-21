#include "coloreditor.hpp"
#include "../../debug.hpp"
#include "../../modules/gpu/gpu.hpp"
#include "../../modules/sys/sys.hpp"
#include "../../modules/key/key.hpp"
#include "../../modules/spu/sequencer.hpp"
#include "../../modules/sram/sram.hpp"

bool ColorEditor::enabled;

void ColorEditor::update(){

}

void ColorEditor::init(){
	enabled = false;
}

void ColorEditor::write(){
	for(int di=0; di<16; di++){
		// Read color from palette SRAM ( Two colors twice )
		getColor( di   , &VAR_CFG.PAL[ di ] );
		
		// Write palette data to SRAM
		u16 r = VAR_CFG.PAL[ di ].R;
		u16 g = VAR_CFG.PAL[ di ].G;
		u16 b = VAR_CFG.PAL[ di ].B;
		u16 c = ( b<<10 ) | ( g<<5 ) | ( r&0x1F );
		
		Sram::write16( c );
	}	
}

void ColorEditor::read(){
	for(int di=0;di<16; di++){
		
		// Read palette data stored at SRAM
		
		u16 c1 = Sram::read16();
		
		u8 r = (c1	    ) & 0x1F;
		u8 g = (c1 >>  5) & 0x1F;
		u8 b = (c1 >> 10) & 0x1F;
		VAR_CFG.PAL[  di  ].R = r;
		VAR_CFG.PAL[  di  ].G = g;
		VAR_CFG.PAL[  di  ].B = b;
		
		
		// Write to palette RAM ( Two colors twice ) 
		setColor( di, r, g, b );
	}	
}

void ColorEditor::setColor( u8 color, u8 r, u8 g, u8 b ){
	u16 packed = (b << 10) | (g<<5) | r;
	if( color == 8 ) {
		*(u16*) BG_PALETTE = packed;
	} else {
		*(u16*)(BG_PALETTE + ( color * 2)) = packed;
		*(u16*)(BG_PALETTE + (color*0x20)+0x10)	= packed;
	}
}

void ColorEditor::getColor( u8 color, Color *entry ){
	u16 *pcolor = (u16*)(BG_PALETTE + ( color * 2));
	if( color == 8 ) { pcolor = (u16*) BG_PALETTE;	}
	
	entry->R = ((*pcolor		) &0x1F);
	entry->G = ((*pcolor	>> 5) &0x1F);
	entry->B = ((*pcolor	>>10) &0x1F);
}

void ColorEditor::setColorChannel( u8 color, u8 index, s8 q){
	/*
	BIT  0-4 : Red
	BIT  5-9 : Green
	BIT 10-14: Blue
	*/
	
	u16 *pcolor = (u16*)(BG_PALETTE + ( color * 2));
	
	s16 r = ((*pcolor		) &0x1F);
	s16 g = ((*pcolor	>> 5) &0x1F);
	s16 b = ((*pcolor	>>10) &0x1F);
	
	s16 *var = (index==0?&r:index==1?&g:&b);
	
	if((*var==31) && (q>0))return;
	if((*var==0) && (q<0))return;
	if((*var<4) && (q<1))q=-*var;
	*var += q;
	if( *var<0) *var=0; 
	if( *var>31) *var=31; 
	
	*pcolor = (b << 10) | (g<<5) | r;
	if( color == 8 ) {
		*(u16*) BG_PALETTE = *pcolor;
	} else {
		*(u16*)(BG_PALETTE + ( color * 2)) = *pcolor;
		*(u16*)(BG_PALETTE+ (color*0x20)+0x10)	= *(u16*)(BG_PALETTE+(color*0x2));
	}
}

void ColorEditor::show(Control *c, bool bigstep, bool add, u32 *pointer){
	enabled = true;
	int dlg_x = 9;
	int dlg_y = 6;
	u8 	dx=0;
	u8 	dy=0;
	u8 position = 0;
	s16 active = 3;;
	Gpu::drawDialog(dlg_x, dlg_y, 21, 14, "Color Editor"); 
	s16 active_color;
	while( enabled ){
		Sequencer::update();
		Key::update();
		if(Key::press(KEY_START)){
			enabled = false;
		}
		if(Key::press(KEY_B)){
			Gpu::loadPalette();
			enabled = false;
		}
		if(Key::press(KEY_A)){
			if(Key::down(KEY_RIGHT	)) ColorEditor::setColorChannel( active / 3, active%3, 1);
			if(Key::down(KEY_LEFT 	)) ColorEditor::setColorChannel( active / 3, active%3,-1);
			if(Key::down(KEY_UP		)) ColorEditor::setColorChannel( active / 3, active%3, 4);
			if(Key::down(KEY_DOWN 	)) ColorEditor::setColorChannel( active / 3, active%3,-4);
		} else {
			if(Key::down(KEY_RIGHT	)) active++;
			if(Key::down(KEY_LEFT 	)) active--;
			if(Key::down(KEY_UP		)) active-=15;
			if(Key::down(KEY_DOWN 	)) active+=15;
		}
		while( active < 3 ) active=3;
		while( active > 47 ) active=47;
		position = 0;
		int color = 0;
		active_color = active / 3;
		static int time = 0;
		for(int y = 0; y<4; y++){
			for(int ix = 0; ix<4; ix++,color++){
				if(color==0)continue;
				dy = position / 5;
				dx = position % 5;
				position++;
				time++;
				u16 pcolor = *(u16*)(BG_PALETTE + ( color * 2));
	
				u16 r = ((pcolor		) &0x1F);
				u16 g = ((pcolor	>> 5) &0x1F);
				u16 b = ((pcolor	>>10) &0x1F);
				
				u8 action_color = Key::press(KEY_A)?0x14:(((time&0xFFF)>0x800)?0x17:0x0B);
				Gpu::set(0, dlg_x+2+(dx<<2), dlg_y+2+(dy<<2), (color==active_color) ? action_color:0x19);
				Gpu::set(0, dlg_x+1+(dx<<2), dlg_y+3+(dy<<2), (color==active_color) ? ((active % 3) ==0)? action_color : 0x15 : 0x19);
				Gpu::set(0, dlg_x+2+(dx<<2), dlg_y+3+(dy<<2), (color==active_color) ? ((active % 3) ==1)? action_color : 0x0C : 0x19);
				Gpu::set(0, dlg_x+3+(dx<<2), dlg_y+3+(dy<<2), (color==active_color) ? ((active % 3) ==2)? action_color : 0x11 : 0x19);
				
				Gpu::set(2, dlg_x+1+(dx<<2), dlg_y+3+(dy<<2), (COLOR_RED	<<12) | (0x300+r) );
				Gpu::set(2, dlg_x+2+(dx<<2), dlg_y+3+(dy<<2), (COLOR_GREEN  <<12) | (0x300+g) );
				Gpu::set(2, dlg_x+3+(dx<<2), dlg_y+3+(dy<<2), (COLOR_BLUE	<<12) | (0x300+b) );
				Gpu::set(2, dlg_x+2+(dx<<2), dlg_y+2+(dy<<2), ((color!=8?color:0)<<12) | 0x45);
			}
		}
	}
	Key::forceNoInput();
	RegionHandler::redraw = 1;
}
