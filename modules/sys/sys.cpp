Sys sys;

void Sys::reset(){
	//SPU.stop();
	var_reset = true;
}



void Sys::init(){
	KEY.init();
	SPU.Init(VAR_SONG.BPM);		
	Net::init();
	var_reset = false;
	
	//keyrate  = 40;
	//retrig   = false;
	keyboard = 0x0000; // Clear Keypress, down and up buffer (AB, SL, ST, arrows)
	cursor = 0x00; 
	
	/*for(int i=0; i<6; i++){
		VAR_PATTERN[i].POINTER = VAR_PATTERN[i].ORDER;
	}*/
}

#define BUTTON_A		(kb & 0x0001)
#define BUTTON_B		(kb & 0x0002)
#define BUTTON_SELECT	(kb & 0x0004)
#define BUTTON_START	(kb & 0x0008)
#define BUTTON_R		(kb & 0x0100)
#define BUTTON_L		(kb & 0x0200)
#define BUTTON_RIGHT	(kb & 0x0010)
#define BUTTON_LEFT		(kb & 0x0020)
#define BUTTON_UP		(kb & 0x0040)
#define BUTTON_DOWN		(kb & 0x0080)

#define KEYBOARD_A		0x0001
#define KEYBOARD_B		0x0004
#define KEYBOARD_SELECT	0x0010
#define KEYBOARD_START	0x0040
#define KEYBOARD_R		0x0100
#define KEYBOARD_L		0x0400
#define CURSOR_RIGHT	0x01
#define CURSOR_LEFT		0x04
#define CURSOR_UP		0x10
#define CURSOR_DOWN		0x40

void Sys::updateInput(){
	u32 msg = 0;

	if(VAR_LIVE.PERFORM.LOCK)return;
	// Handle Navigation
	if(KEY.press(KEY_SELECT)){
		msg  = KEY.down(KEY_LEFT ) ? MESSAGE_NAVIGATE_LEFT  :
			   KEY.down(KEY_RIGHT) ? MESSAGE_NAVIGATE_RIGHT :
			   KEY.down(KEY_UP   ) ? MESSAGE_NAVIGATE_UP    :
			   KEY.down(KEY_DOWN ) ? MESSAGE_NAVIGATE_DOWN  :
			   0x0;
		if(regHnd.region && msg){
			regHnd.sendMessage(msg | (unsigned)regHnd.region);
			return;
		}
	}
	
	// Handle ACTIVATE message
	if(KEY.down(KEY_A) && regHnd.control) {
		regHnd.sendMessage(MESSAGE_ACTIVATE | (unsigned)regHnd.control);
		return;		
	}
	
	if(KEY.down(KEY_START)) {
		if(SPU.playing) {
			SPU.stop();
			return;
		}
		
		if(KEY.press(KEY_SELECT)) {
			SPU.play(true);
			return;
		}		
		if(!SPU.playing) SPU.play(false);
		return;
	}
	
	
	// Handle Copy Command (B+A)
	if(KEY.press(KEY_B) && regHnd.control) {
		if(KEY.down(KEY_A)) {
			//regHnd.sendMessage(MESSAGE_COPY | (unsigned)regHnd.control);
			return;		
		}
		
		msg = KEY.down(KEY_LEFT ) ? MESSAGE_OTHER_PREV : 
			  KEY.down(KEY_RIGHT) ? MESSAGE_OTHER_NEXT : 
			  KEY.down(KEY_DOWN ) ? MESSAGE_OTHER_BIGPREV : 
			  KEY.down(KEY_UP   ) ? MESSAGE_OTHER_BIGNEXT : 
			  0;
		
		if(msg)regHnd.sendMessage(msg);
		return;
	}
	
	// Handle control modifiers
	if(KEY.press(KEY_A)){
		msg  = KEY.down(KEY_LEFT ) ? MESSAGE_MODIFY_SUB 	:
			   KEY.down(KEY_RIGHT) ? MESSAGE_MODIFY_ADD 	:
			   KEY.down(KEY_UP   ) ? MESSAGE_MODIFY_BIGADD  :
			   KEY.down(KEY_DOWN ) ? MESSAGE_MODIFY_BIGSUB  :
			   0x0;
		if(regHnd.control && msg){
			regHnd.sendMessage(msg | (unsigned)regHnd.control);
			return;
		}
		
		// Handle A Cancel (A, then B)
		if(KEY.down(KEY_B) && regHnd.control) {
			regHnd.sendMessage(MESSAGE_CANCEL | (unsigned)regHnd.control);
			return;
		}
	}
		
	// Handle other arbitrary keypresses (MESSAGE_KEYPRESS)
	msg =  KEY.down(KEY_LEFT  ) ? 1 :
		   KEY.down(KEY_RIGHT ) ? 1 :
		   KEY.down(KEY_UP    ) ? 1 :
		   KEY.down(KEY_DOWN  ) ? 1 :
		/*   KEY.down(KEY_A     ) ? 1 :
		   KEY.down(KEY_B     ) ? 1 :*/	
		   KEY.down(KEY_SELECT) ? 1 :
		   KEY.down(KEY_START ) ? 1 :
		   0;
		   
	if(regHnd.control && msg ){
		regHnd.sendMessage(MESSAGE_KEYPRESS | (unsigned)regHnd.control);
	}
	
}

void adsr_view(){
	if(regHnd.region != &REGION_MAP_2_INS)return;
	if(VAR_INSTRUMENT.TYPE != INSTRUMENT_TYPE_WAV) return;
	gpu.vs->clear();
	
	for(int x=0;x<0x40; x+=2){
		//gpu.vs->set(x>>1, 15 - ( SPU.adsr_table[ 0 ][ x ] ) );
		
		gpu.vs->set(x>>1,  7 - ( SPU.adsr_table[ 0 ][ x ] >> 1 ) );
		gpu.vs->set(x>>1, 15 - ( SPU.adsr_table[ 1 ][ x ] >> 1 ) );
		gpu.vs->set(x>>1, 23 - ( SPU.adsr_table[ 2 ][ x ] >> 1 ) );
		gpu.vs->set(x>>1, 31 - ( SPU.adsr_table[ 3 ][ x ] >> 1 ) );
		
	}
	DECIMAL_DOUBLE(14,2, 7, SPU.adsr_position);

	if(SPU.adsr_position<0x3F){
		VISPOS1(14, 1,	0xFF, SPU.adsr_position>>2);
		VISPOS2(14, 18, 0xFF, SPU.adsr_position>>2);
	}
	gpu.vs->draw(14,6);
}
void overloadTest(RegionHandler &regHnd);


void Sys::update(){
	
	SPU.update();	
	KEY.update();
	updateInput();
	
	
	if(KEY.press(KEY_L)) adsr_view();//overloadTest(regHnd);
	// if(KEY.Press(KEY_R))gpu.vs->draw(14,2);
}

