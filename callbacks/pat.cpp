

// TBC on value change @ PATtern editor
void patternCopy(u8 channel){
	
	CFG::CURRENTCHANNEL = channel;
	// *REGION_MAP_3_TRK.viewport->var = channel;	
	
	for(int i=0; i<16;i++){		
		VAR_SONG.PATTERNS[channel].ORDER[CFG::ORDERPOSITION + i] = VAR_PATTERN[channel].ORDER[i];
	}
	
	cellSync();
}

// TBC each time CFG::ORDERPOSITION CHANGES
void patternSync(u8 position){
	for(int i=0, p; i<16; i++){
		p = i+position;
		VAR_PATTERN[0].ORDER[i] = VAR_SONG.PATTERNS[0].ORDER[p];
		VAR_PATTERN[1].ORDER[i] = VAR_SONG.PATTERNS[1].ORDER[p];
		VAR_PATTERN[2].ORDER[i] = VAR_SONG.PATTERNS[2].ORDER[p];
		VAR_PATTERN[3].ORDER[i] = VAR_SONG.PATTERNS[3].ORDER[p];
		VAR_PATTERN[4].ORDER[i] = VAR_SONG.PATTERNS[4].ORDER[p];
		VAR_PATTERN[5].ORDER[i] = VAR_SONG.PATTERNS[5].ORDER[p];
	}
}

void patternCH0_alter(Control *c, bool bigstep, bool add, u32 *pointer){	modify7BIT(c, bigstep, add, pointer); patternCopy(0);}
void patternCH1_alter(Control *c, bool bigstep, bool add, u32 *pointer){	modify7BIT(c, bigstep, add, pointer); patternCopy(1);}
void patternCH2_alter(Control *c, bool bigstep, bool add, u32 *pointer){	modify7BIT(c, bigstep, add, pointer); patternCopy(2);}
void patternCH3_alter(Control *c, bool bigstep, bool add, u32 *pointer){	modify7BIT(c, bigstep, add, pointer); patternCopy(3);}
void patternCH4_alter(Control *c, bool bigstep, bool add, u32 *pointer){	modify7BIT(c, bigstep, add, pointer); patternCopy(4);}
void patternCH5_alter(Control *c, bool bigstep, bool add, u32 *pointer){	modify7BIT(c, bigstep, add, pointer); patternCopy(5);}

void patternCH0_paste(Control *c, bool bigstep, bool add, u32 *pointer){	paste7BIT(c, bigstep, add, pointer); patternCopy(0);}
void patternCH1_paste(Control *c, bool bigstep, bool add, u32 *pointer){	paste7BIT(c, bigstep, add, pointer); patternCopy(1);}
void patternCH2_paste(Control *c, bool bigstep, bool add, u32 *pointer){	paste7BIT(c, bigstep, add, pointer); patternCopy(2);}
void patternCH3_paste(Control *c, bool bigstep, bool add, u32 *pointer){	paste7BIT(c, bigstep, add, pointer); patternCopy(3);}
void patternCH4_paste(Control *c, bool bigstep, bool add, u32 *pointer){	paste7BIT(c, bigstep, add, pointer); patternCopy(4);}
void patternCH5_paste(Control *c, bool bigstep, bool add, u32 *pointer){	paste7BIT(c, bigstep, add, pointer); patternCopy(5);}

static u8 pattern_bookmark_row = 0;

void patternCH0_focus(Control *c, bool bigstep, bool add, u32 *pointer){	CFG::CURRENTCHANNEL = 0; pattern_bookmark_row = *(u8*)pointer;}
void patternCH1_focus(Control *c, bool bigstep, bool add, u32 *pointer){	CFG::CURRENTCHANNEL = 1; pattern_bookmark_row = *(u8*)pointer;}
void patternCH2_focus(Control *c, bool bigstep, bool add, u32 *pointer){	CFG::CURRENTCHANNEL = 2;	pattern_bookmark_row = *(u8*)pointer;}
void patternCH3_focus(Control *c, bool bigstep, bool add, u32 *pointer){	CFG::CURRENTCHANNEL = 3;	pattern_bookmark_row = *(u8*)pointer;}
void patternCH4_focus(Control *c, bool bigstep, bool add, u32 *pointer){	CFG::CURRENTCHANNEL = 4;	pattern_bookmark_row = *(u8*)pointer;}
void patternCH5_focus(Control *c, bool bigstep, bool add, u32 *pointer){	CFG::CURRENTCHANNEL = 5;	pattern_bookmark_row = *(u8*)pointer;}


//PATTERNS
const u8 NUMBERS[16] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 };
#define CB_PATTERNS(c, a)	CALLBACK( cb_patterns_focus_##c##_0##a 	, patternCH##c##_focus	, EVENT_FOCUS 		, (u32*)&NUMBERS[0x0##a] 					, NULL							);\
							CALLBACK( cb_patterns_paste_##c##_0##a 	, patternCH##c##_paste	, EVENT_KEYDOWN_B 	, &VAR_PATTERN[0x##c].ORDER[0x##a] 	, &cb_patterns_focus_##c##_0##a );\
							CALLBACK( cb_patterns_##c##_0##a 		, patternCH##c##_alter	, EVENT_MODIFY_B 	, &VAR_PATTERN[0x##c].ORDER[0x##a] 	, &cb_patterns_paste_##c##_0##a );  
CB_PATTERNS(0, 0);		CB_PATTERNS(1, 0);		CB_PATTERNS(2, 0);		CB_PATTERNS(3, 0);		CB_PATTERNS(4, 0);		CB_PATTERNS(5, 0);
CB_PATTERNS(0, 1);		CB_PATTERNS(1, 1);		CB_PATTERNS(2, 1);		CB_PATTERNS(3, 1);		CB_PATTERNS(4, 1);		CB_PATTERNS(5, 1);
CB_PATTERNS(0, 2);		CB_PATTERNS(1, 2);		CB_PATTERNS(2, 2);		CB_PATTERNS(3, 2);		CB_PATTERNS(4, 2);		CB_PATTERNS(5, 2);
CB_PATTERNS(0, 3);		CB_PATTERNS(1, 3);		CB_PATTERNS(2, 3);		CB_PATTERNS(3, 3);		CB_PATTERNS(4, 3);		CB_PATTERNS(5, 3);
CB_PATTERNS(0, 4);		CB_PATTERNS(1, 4);		CB_PATTERNS(2, 4);		CB_PATTERNS(3, 4);		CB_PATTERNS(4, 4);		CB_PATTERNS(5, 4);
CB_PATTERNS(0, 5);		CB_PATTERNS(1, 5);		CB_PATTERNS(2, 5);		CB_PATTERNS(3, 5);		CB_PATTERNS(4, 5);		CB_PATTERNS(5, 5);
CB_PATTERNS(0, 6);		CB_PATTERNS(1, 6);		CB_PATTERNS(2, 6);		CB_PATTERNS(3, 6);		CB_PATTERNS(4, 6);		CB_PATTERNS(5, 6);
CB_PATTERNS(0, 7);		CB_PATTERNS(1, 7);		CB_PATTERNS(2, 7);		CB_PATTERNS(3, 7);		CB_PATTERNS(4, 7);		CB_PATTERNS(5, 7);
CB_PATTERNS(0, 8);		CB_PATTERNS(1, 8);		CB_PATTERNS(2, 8);		CB_PATTERNS(3, 8);		CB_PATTERNS(4, 8);		CB_PATTERNS(5, 8);
CB_PATTERNS(0, 9);		CB_PATTERNS(1, 9);		CB_PATTERNS(2, 9);		CB_PATTERNS(3, 9);		CB_PATTERNS(4, 9);		CB_PATTERNS(5, 9);
CB_PATTERNS(0, A);		CB_PATTERNS(1, A);		CB_PATTERNS(2, A);		CB_PATTERNS(3, A);		CB_PATTERNS(4, A);		CB_PATTERNS(5, A);
CB_PATTERNS(0, B);		CB_PATTERNS(1, B);		CB_PATTERNS(2, B);		CB_PATTERNS(3, B);		CB_PATTERNS(4, B);		CB_PATTERNS(5, B);
CB_PATTERNS(0, C);		CB_PATTERNS(1, C);		CB_PATTERNS(2, C);		CB_PATTERNS(3, C);		CB_PATTERNS(4, C);		CB_PATTERNS(5, C);
CB_PATTERNS(0, D);		CB_PATTERNS(1, D);		CB_PATTERNS(2, D);		CB_PATTERNS(3, D);		CB_PATTERNS(4, D);		CB_PATTERNS(5, D);
CB_PATTERNS(0, E);		CB_PATTERNS(1, E);		CB_PATTERNS(2, E);		CB_PATTERNS(3, E);		CB_PATTERNS(4, E);		CB_PATTERNS(5, E);
CB_PATTERNS(0, F);		CB_PATTERNS(1, F);		CB_PATTERNS(2, F);		CB_PATTERNS(3, F);		CB_PATTERNS(4, F);		CB_PATTERNS(5, F);
#undef CB_PATTERNS

CALLBACK( cb_pat_solo_0	, SOLO0	, EVENT_KEYUP_B	, NULL 	, NULL 	);
CALLBACK( cb_pat_solo_1	, SOLO1	, EVENT_KEYUP_B	, NULL 	, NULL 	);
CALLBACK( cb_pat_solo_2	, SOLO2	, EVENT_KEYUP_B	, NULL 	, NULL 	);
CALLBACK( cb_pat_solo_3	, SOLO3	, EVENT_KEYUP_B	, NULL 	, NULL 	);
CALLBACK( cb_pat_solo_4	, SOLO4	, EVENT_KEYUP_B	, NULL 	, NULL 	);
CALLBACK( cb_pat_solo_5	, SOLO5	, EVENT_KEYUP_B	, NULL 	, NULL 	);
CALLBACK( cb_pat_mute_0	, MUTE0	, EVENT_KEYUP_B	, NULL 	, NULL 	);
CALLBACK( cb_pat_mute_1	, MUTE1	, EVENT_KEYUP_B	, NULL 	, NULL 	);
CALLBACK( cb_pat_mute_2	, MUTE2	, EVENT_KEYUP_B	, NULL 	, NULL 	);
CALLBACK( cb_pat_mute_3	, MUTE3	, EVENT_KEYUP_B	, NULL 	, NULL 	);
CALLBACK( cb_pat_mute_4	, MUTE4	, EVENT_KEYUP_B	, NULL 	, NULL 	);
CALLBACK( cb_pat_mute_5	, MUTE5	, EVENT_KEYUP_B	, NULL 	, NULL 	);
	
	
const u8 pattern_arrow_position[6] = { 3, 7, 11, 15, 19, 23 };
	
bool pat_clean = false;
bool pat_solo_clean = false;

const u16 pattern_channel_symbols[6] = { 0x7035, 0x7035, 0x7036, 0x7037, 0x7038, 0x7039 };

u8 pattern_bookmarks[6] = {0,0,0,0,0,0};

void patGlobalUpdater(){
	static int bookmark_timer = 0;
		
	if(KEY::press(KEY_B)) {
		bookmark_timer++;
		if(bookmark_timer > 2000) {			
			pattern_bookmarks[CFG::CURRENTCHANNEL] = CFG::ORDERPOSITION + pattern_bookmark_row;
			pat_clean = false;
			bookmark_timer = 0;
		}
		return;
	}
	bookmark_timer = 0;
}
	
void updatePAT(){
	static int last_position;

	patGlobalUpdater();
	
	// Draw position arrows
	for(int c=0, x=0, y=0; c < 6; c++) {
		if((!pat_clean)|| (REGHND::redraw) || (VAR_CHANNEL[c].POSITION != VAR_CHANNEL[c].LASTPOSITION)){
			
			if(pattern_bookmarks[c]>0) {
				y = pattern_bookmarks[c] - CFG::ORDERPOSITION;
				if((y<=15)&&(y>=0)){
					gpu.set(0, pattern_arrow_position[c]+1, 4+y, 0x8);
					gpu.set(0, pattern_arrow_position[c]+2, 4+y, 0x8);
				}
			}
			y = VAR_CHANNEL[c].POSITION - CFG::ORDERPOSITION;
			x = VAR_CHANNEL[c].LASTPOSITION - CFG::ORDERPOSITION;
			if((x<=15)&&(x>=0)) gpu.set(2, pattern_arrow_position[c], 4+x, 0x00FC);
			if((y<=15)&&(y>=0)) gpu.set(2, pattern_arrow_position[c], 4+y, 0x408D);
		}
		pat_solo_clean = false;
	}

	if(!pat_solo_clean){
		/* ------------------------------------------------------------
		Redraw mute / solo icons 									 */
		for(int c=0; c < 6; c++) {
			/* ------------------------------------------------------------
			If mute is enabled, draw MUTE on High Red color 			 */
			if(VAR_CHANNEL[c].mute) { 
				gpu.set(2, pattern_arrow_position[c]+1, 3, 0x3057);
				gpu.set(2, pattern_arrow_position[c]+2, 3, 0x3058); 
				continue; 
			}
			/* ------------------------------------------------------------
			If solo is enabled, draw SOLO on High green color	   		 */
			if(VAR_CHANNEL[c].solo) { 
				gpu.set(2, pattern_arrow_position[c]+1, 3, 0xF055);
				gpu.set(2, pattern_arrow_position[c]+2, 3, 0xF056); 
				continue;
			}
			/* ------------------------------------------------------------
			Else, draw channel icon		 								 */
				gpu.set(2, pattern_arrow_position[c]+1, 3, pattern_channel_symbols[c]); 
				gpu.set(2, pattern_arrow_position[c]+2, 3, 0x00FC); 
			// ------------------------------------------------------------
		}
		// ------------------------------------------------------------
		pat_solo_clean = true;
	}
	
	// Draw position tags (Sides)
	if((!pat_clean)||(last_position != CFG::ORDERPOSITION)||(REGHND::redraw)){		
		for(int c=0, h=0, y=4, v=CFG::ORDERPOSITION; c < 16; c++) {
			h = (c & 1) ? 0x3 : 0x5;
			HEXADECIMAL( 1, y, h, v>>4);
			HEXADECIMAL( 2, y, h, v&0xf);
			HEXADECIMAL(27, y, h, v>>4);
			HEXADECIMAL(28, y, h, v&0xf);
			
			v++;
			y++;
		}
		last_position = CFG::ORDERPOSITION;
		pat_clean = true;
		
		
	}
}
//----------------------------------------------------------------------------------------
// PAT SCREEN CALLBACKS
//----------------------------------------------------------------------------------------
void SOLO0(Control *c, bool bigstep, bool add, u32 *pointer){	SPU::solo(0);	pat_solo_clean = false;/*REGHND::drawControl(&PAT_CONTROLS[CONTROL_PAT_SOLO_LEFT_00 ]);*/ }
void SOLO1(Control *c, bool bigstep, bool add, u32 *pointer){	SPU::solo(1);	pat_solo_clean = false;/*REGHND::drawControl(&PAT_CONTROLS[CONTROL_PAT_SOLO_LEFT_01 ]);*/ }
void SOLO2(Control *c, bool bigstep, bool add, u32 *pointer){	SPU::solo(2);	pat_solo_clean = false;/*REGHND::drawControl(&PAT_CONTROLS[CONTROL_PAT_SOLO_LEFT_02 ]);*/ }
void SOLO3(Control *c, bool bigstep, bool add, u32 *pointer){	SPU::solo(3);	pat_solo_clean = false;/*REGHND::drawControl(&PAT_CONTROLS[CONTROL_PAT_SOLO_RIGHT_00]);*/ }
void SOLO4(Control *c, bool bigstep, bool add, u32 *pointer){	SPU::solo(4);	pat_solo_clean = false;/*REGHND::drawControl(&PAT_CONTROLS[CONTROL_PAT_SOLO_RIGHT_01]);*/ }
void SOLO5(Control *c, bool bigstep, bool add, u32 *pointer){	SPU::solo(5);	pat_solo_clean = false;/*REGHND::drawControl(&PAT_CONTROLS[CONTROL_PAT_SOLO_RIGHT_02]);*/ }
void MUTE0(Control *c, bool bigstep, bool add, u32 *pointer){	SPU::mute(0);	pat_solo_clean = false;/*REGHND::drawControl(&PAT_CONTROLS[CONTROL_PAT_MUTE_LEFT_00]);*/ }
void MUTE1(Control *c, bool bigstep, bool add, u32 *pointer){	SPU::mute(1);	pat_solo_clean = false;/*REGHND::drawControl(&PAT_CONTROLS[CONTROL_PAT_MUTE_LEFT_01]);*/ }
void MUTE2(Control *c, bool bigstep, bool add, u32 *pointer){	SPU::mute(2);	pat_solo_clean = false;/*REGHND::drawControl(&PAT_CONTROLS[CONTROL_PAT_MUTE_LEFT_02]);*/ }
void MUTE3(Control *c, bool bigstep, bool add, u32 *pointer){	SPU::mute(3);	pat_solo_clean = false;/*REGHND::drawControl(&PAT_CONTROLS[CONTROL_PAT_MUTE_RIGHT_00]);*/ }
void MUTE4(Control *c, bool bigstep, bool add, u32 *pointer){	SPU::mute(4);	pat_solo_clean = false;/*REGHND::drawControl(&PAT_CONTROLS[CONTROL_PAT_MUTE_RIGHT_01]);*/ }
void MUTE5(Control *c, bool bigstep, bool add, u32 *pointer){	SPU::mute(5);	pat_solo_clean = false;/*REGHND::drawControl(&PAT_CONTROLS[CONTROL_PAT_MUTE_RIGHT_02]);*/ }

// Reloads VAR_SONG.PATTERN[6][--16--] into visible VARIABLE lookup vars (VAR_PATTERN[6][16])
void patSync(void){
	int o = 0;
	
	for(int c=0; c<6;c++){
		for(int i=0; i<16;i++){
			/* ------------------------------------------------------------
			Copy data from VISIBLE CONTROL VARS -> Song Data Order Memory*/
			VAR_PATTERN[c].ORDER[i] = VAR_SONG.PATTERNS[c].ORDER[i+CFG::ORDERPOSITION];
			/* ------------------------------------------------------------
			Manually draw the controls (good to avoid message overflow)  */
			REGHND::drawControl(&PAT_CONTROLS[o]);
			/* ------------------------------------------------------------
			Ensure the pattern order tags at the sides get updated later */
			pat_clean = false;
			// ------------------------------------------------------------
			o++;
		}
	}
}

void patDispatchMessage(u32 msg){
	switch(msg){
		/* Scroll up */
		case MESSAGE_OTHER_PREV:
			if(CFG::ORDERPOSITION>0){
				/* ------------------------------------------------------------
				Clear Arrows when pattern scrolls down 						 */
				for(int c=0, x=0; c<6;c++){
					x = VAR_CHANNEL[c].POSITION - CFG::ORDERPOSITION;
					if((x<=15)&&(x>=0))gpu.set(2, pattern_arrow_position[c], 4+x, 0x00FC);
				}
				// ------------------------------------------------------------
				CFG::ORDERPOSITION--;
				/* ------------------------------------------------------------
				Copy cells from VAR_DATA(Memory)->VAR_PATTERN (Visible Vars) */
				patSync();
				/* ------------------------------------------------------------
				Ensure the pattern order tags at the sides get updated later */
				pat_clean = false;				
				// ------------------------------------------------------------
			}
			return;
			
		/* Scroll down */
		case MESSAGE_OTHER_NEXT: 
			/* about the 240 below, it is 256 patterns - 16 visible entries */
			if(CFG::ORDERPOSITION < 240){ 
				/* ------------------------------------------------------------
				Clear Arrows when pattern scrolls down 						 */
				for(int c=0, x=0; c<6;c++){
					x = VAR_CHANNEL[c].POSITION - CFG::ORDERPOSITION;
					if((x<=15)&&(x>=0))gpu.set(2, pattern_arrow_position[c], 4+x, 0x00FC);
				}
				// ------------------------------------------------------------
				CFG::ORDERPOSITION++;
				/* ------------------------------------------------------------
				Copy cells from VAR_DATA(Memory)->VAR_PATTERN (Visible Vars) */
				patSync();
				/* ------------------------------------------------------------
				Ensure the pattern order tags at the sides get updated later */
				pat_clean = false;				
				// ------------------------------------------------------------
			}
			return;		
	}
}
