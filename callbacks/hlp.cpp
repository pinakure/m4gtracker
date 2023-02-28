
static void helpBlit(const u16 *help_data, int startx, int starty, int x, int y, int width, int height){
	static int offsetSrc;
	static int offsetDst;
	
	width = width << 1; //double word per item!
	
	x<<=1;
	
	//R_DISPCNT = DISP_FORCE_HBLANK | (DISP_BG0_ON | DISP_BG1_ON |  DISP_BG2_ON) &0x0f00;			
	for(int sy=starty; sy<height+starty; sy++){
		offsetSrc = (sy*26)+startx;
		offsetDst = (y<<6)+x;
		
		DmaCopy(3, help_data+offsetSrc, (SCREEN_BASE2_ADDR)+offsetDst, width, 16);
		y++;
	}
	//R_DISPCNT = (DISP_BG0_ON | DISP_BG1_ON |  DISP_BG2_ON) &0x0f00;
}

static u16 index_help_button = 0;
static u16 index_help_topic  = 0;
static u16 index_help_last_topic  = 0;
static bool help_button_redraw= false;
static bool help_data_redraw 	= false;
static u16 old_help_offset 	= 1;
static u16 help_offset		= 0;

void helpDrawButton(int x, int y, const char *t, bool active){
	y+=3;
	x+=1;
	for(int i=0, tx=x, li=strlen(t); i<li; i++){
		if(t[i] == ' '){tx++; continue;}
		GPU::set(0,tx,y , active?0x17:0x0);
		//GPU::set(0,tx,y , active?0x17:0x0);
		//TEXT(tx, y, , );
		//TEXT(tx, y, active?8:4, 0x10);
		tx++;
	}
}

static int help_button_last_x;
static int help_button_last_y;
static int help_button_last_index;
static int help_last_offset;
static int help_button_moved;
	
void eraseButton(){
	int o = help_button_last_y - help_offset;
	if(o >= 0){
		if(o <= 15){
			helpDrawButton(	help_button_last_x, o, "-------------------------", 0);	
		}
	}

	help_button_last_index = index_help_button;
	help_button_last_x = help_buttons[help_topics[index_help_topic].button_list[help_button_last_index]].x;
	help_button_last_y = help_buttons[help_topics[index_help_topic].button_list[help_button_last_index]].y;
}
	

void helpScrollDown(void){
	eraseButton();
	if(help_offset < 48) help_offset++;
	help_data_redraw = true;
}

void helpScrollUp(void){
	eraseButton();
	if(help_offset > 0) help_offset--;
	help_data_redraw = true;
}

void helpPrevButton(void){
	if(index_help_button > 0){
		index_help_button--;
	}
	eraseButton();
	help_button_moved = true;
	help_button_redraw = true;
	// TODO: If current button if out of sight, scroll viewport until it is visible
}

void helpNextButton(void){
	if(index_help_button < help_topics[index_help_topic].button_count-1){
		index_help_button++;
	}
	eraseButton();
	help_button_moved = true;
	help_button_redraw = true;
	// TODO: If current button if out of sight, scroll viewport until it is visible
}

void helpActivateBack(void){
	eraseButton();
	index_help_topic = index_help_last_topic;	
	index_help_last_topic = 0;
	index_help_button = 0;
	help_offset = help_last_offset;
	help_data_redraw = true;
	help_button_redraw = true;
}

void helpActivateButton(void){
	eraseButton();
	index_help_last_topic = index_help_topic;
	index_help_topic = help_buttons[help_topics[index_help_topic].button_list[index_help_button]].destination_index;
	index_help_button = 0;
	help_last_offset = help_offset;
	help_offset = 0;
	help_data_redraw = true;
	help_button_redraw = true;
}

void helpUpdateButtons(void){
	int x,y;
	const char *t;
	int p =0;
	
	
	for(int i=0, o=0, il= help_topics[index_help_topic].button_count; i<il; i++){
		p = help_topics[index_help_topic].button_list[i];
		y = help_buttons[p].y;
		x = help_buttons[p].x;
		t = help_buttons[p].text;
		o = y - help_offset;
		if(o >= 0){
			if(o <= 15){
				helpDrawButton(x, o, t, i==index_help_button);
				continue;
			} 
		} 
		
		if(help_button_moved){
			if(i==index_help_button){
				help_offset = (y>48?48:y)-10;
				help_data_redraw = true;
			}
		}
	}
	help_button_moved = false;
}

void updateHLP(){
	
	if(KEY::down(KEY_B)) { helpActivateBack(); }
	if(KEY::down(KEY_A)) { helpActivateButton(); }
	if(KEY::down(KEY_UP	) ) helpScrollUp(); else if(KEY::down(KEY_DOWN) ) helpScrollDown();
	if(KEY::down(KEY_LEFT)) helpPrevButton();else if(KEY::down(KEY_RIGHT)) helpNextButton();

	// Propagate Region redraw flag
	if(REGHND::redraw){
		help_data_redraw = true;
	}

	if(help_data_redraw){
		helpBlit(help_topics[index_help_topic].map_data, 0, help_offset, 1, 3, 26, 16);
		
		// Erase old scrollbar knob
		GPU::set(1, 28, 3+(old_help_offset>>2), 0x60);
		// Draw new scrollbar knob
		GPU::set(1, 28, 3+(help_offset>>2), 0x6F);
		
		// Memorize current help_offset
		old_help_offset = help_offset;
		
		help_data_redraw = false;
		help_button_redraw = true;
	}
	
	if(help_button_redraw){
		
		helpUpdateButtons();
	//	HEXADECIMAL_DOUBLE(13,1, 7, sizeof(*help_topics[index_help_topic].button_list[0]));
	//	HEXADECIMAL_DOUBLE(15,1, 7, index_help_button);
		help_button_redraw = false;
	}
}

