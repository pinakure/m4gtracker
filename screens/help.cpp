#include "help.hpp"

#include "../kernel/gpu/gpu.hpp"
#include "../kernel/key/key.hpp"
#include "../data/help.c"

u16 	Help::index_button 		= 0;
u16 	Help::index_topic  		= 0;
u16 	Help::index_last_topic  = 0;
bool	Help::button_redraw		= false;
bool 	Help::data_redraw 		= false;
u16 	Help::old_offset 		= 1;
u16 	Help::offset			= 0;
int 	Help::button_last_x		= -1;
int 	Help::button_last_y		= -1;
int 	Help::button_last_index	= -1;
int 	Help::last_offset		= -1;
int 	Help::button_moved		= -1;

void Help::blit( const u16 *help_data, int startx, int starty, int x, int y, int width, int height ){
	static int offsetSrc;
	static int offsetDst;
	
	width = width << 1; //double word per item!
	
	x<<=1;
	
	//R_DISPCNT = DISP_FORCE_HBLANK | (DISP_BG0_ON | DISP_BG1_ON |  DISP_BG2_ON) &0x0f00;			
	for(int sy=starty; sy<height+starty; sy++){
		offsetSrc = (sy*26)+startx;
		offsetDst = (y<<6)+x;
		
		DmaCopy(3, help_data + offsetSrc, (SCREEN_BASE2_ADDR) + offsetDst, width, 16);
		y++;
	}
	//R_DISPCNT = (DISP_BG0_ON | DISP_BG1_ON |  DISP_BG2_ON) &0x0f00;
}

void Help::drawButton(int x, int y, const char *t, bool active){
	y+=3;
	x+=1;
	for(int i=0, tx=x, li=strlen(t); i<li; i++){
		if(t[i] == ' '){tx++; continue;}
		Gpu::set(0,tx,y , active?0x17:0x0);
		//Gpu::set(0,tx,y , active?0x17:0x0);
		//TEXT(tx, y, , );
		//TEXT(tx, y, active?8:4, 0x10);
		tx++;
	}
}
	
void Help::eraseButton(){
	int o = Help::button_last_y - Help::offset;
	if(o >= 0){
		if(o <= 15){
			Help::drawButton( Help::button_last_x, o, "-------------------------", 0);	
		}
	}

	Help::button_last_index 	= Help::index_button;
	Help::button_last_x 		= Help::buttons[ Help::topics[ Help::index_topic ].button_list[ Help::button_last_index ] ].x;
	Help::button_last_y 		= Help::buttons[ Help::topics[ Help::index_topic ].button_list[ Help::button_last_index ] ].y;
}
	
void Help::scrollDown(){
	eraseButton();
	if( offset < 48 ) offset++;
	data_redraw = true;
}

void Help::scrollUp(){
	eraseButton();
	if( offset > 0) offset--;
	data_redraw = true;
}

void Help::prevButton(){
	if(index_button > 0){
		index_button--;
	}
	eraseButton();
	button_moved = true;
	button_redraw = true;
	// TODO: If current button if out of sight, scroll viewport until it is visible
}

void Help::nextButton(){
	if( index_button < Help::topics[ Help::index_topic ].button_count - 1 ){
		index_button++;
	}
	eraseButton();
	button_moved  = true;
	button_redraw = true;
	// TODO: If current button if out of sight, scroll viewport until it is visible
}

void Help::activateBack(){
	eraseButton();
	index_topic 		= index_last_topic;	
	index_last_topic 	= 0;
	index_button 		= 0;
	offset 				= last_offset;
	data_redraw 		= true;
	button_redraw 		= true;
}

void Help::activateButton(){
	eraseButton();
	index_last_topic 	= index_topic;
	index_topic 	 	= Help::buttons[ Help::topics[ index_topic ].button_list[ index_button ] ].destination_index;
	index_button 	 	= 0;
	last_offset 		= offset;
	offset 				= 0;
	data_redraw 		= true;
	button_redraw 		= true;
}

void Help::updateButtons(){
	int x,y;
	const char *t;
	int p =0;
	
	
	for(int i=0, o=0, il = Help::topics[ index_topic ].button_count; i<il; i++){
		p = Help::topics[ index_topic ].button_list[i];
		y = buttons[p].y;
		x = buttons[p].x;
		t = buttons[p].text;
		o = y - offset;
		if(o >= 0){
			if(o <= 15){
				Help::drawButton(x, o, t, i==index_button);
				continue;
			} 
		} 
		
		if( button_moved ){
			if( i == index_button ){
				offset 		= ( y > 48 ? 48 : y ) - 10;
				data_redraw = true;
			}
		}
	}
	button_moved = false;
}

void Help::update(){
	
	if( KEYDOWN_B		) { Help::activateBack(); }
	if( KEYDOWN_A		) { Help::activateButton(); }
	if( KEYDOWN_UP		) Help::scrollUp	(); else if( KEYDOWN_DOWN		) Help::scrollDown();
	if( KEYDOWN_LEFT	) Help::prevButton	(); else if( KEYDOWN_RIGHT	) Help::nextButton();

	// Propagate Region redraw flag
	if( RegionHandler::redraw ){
		data_redraw = true;
	}

	if( data_redraw ){
		Help::blit( Help::topics[ index_topic ].map_data, 0, offset, 1, 3, 26, 16 );
		
		// Erase old scrollbar knob
		Gpu::set( 1, 28, 3+( old_offset >> 2 ), 0x60);
		// Draw new scrollbar knob
		Gpu::set( 1, 28, 3+( offset >> 2 ), 0x6F);
		
		// Memorize current help_offset
		old_offset = offset;
		
		data_redraw = false;
		button_redraw = true;
	}
	
	if( button_redraw ){
		
		Help::updateButtons();
	//	HEXADECIMAL_DOUBLE(13,1, 7, sizeof(*Help::topics[index_topic].button_list[0]));
	//	HEXADECIMAL_DOUBLE(15,1, 7, index_button);
		button_redraw = false;
	}
}

void Help::dispatchMessage( u32 msg, u32 pointer){
	
	switch( msg ){
		
		/* Navigation */
		case MESSAGE_NAVIGATE_LEFT	: RegionHandler::load( ((Region*)pointer)->left ); break;
		case MESSAGE_NAVIGATE_RIGHT	: RegionHandler::load( ((Region*)pointer)->right); break;
		case MESSAGE_NAVIGATE_DOWN	: RegionHandler::load( ((Region*)pointer)->down ); break;
		case MESSAGE_NAVIGATE_UP	: RegionHandler::load( ((Region*)pointer)->up   ); break;
		case MESSAGE_ENTER			: break;
		case MESSAGE_EXIT			: break;
	}
}
