// TBC on All Channel Data Update


void cellSync(void){
	for(int c=0; c<6; c++){
		for(int i=0, pattern; i<16; i++){
			pattern = VAR_SONG.PATTERNS[c].ORDER[VAR_CHANNEL[c].POSITION];
			VAR_CELLS[c].KEY[i] = VAR_DATA[pattern].KEY[i];
			VAR_CELLS[c].INS[i] = VAR_DATA[pattern].INS[i];
			VAR_CELLS[c].VOL[i] = VAR_DATA[pattern].VOL[i];
			VAR_CELLS[c].CMD[i] = VAR_DATA[pattern].CMD[i];
			VAR_CELLS[c].VAL[i] = VAR_DATA[pattern].VAL[i];
		}
	}
}
extern const Control CHAN0_CONTROLS[CONTROL_CHANNEL0_0_MAX]; 
extern const Control CHAN1_CONTROLS[CONTROL_CHANNEL1_0_MAX]; 
extern const Control CHAN2_CONTROLS[CONTROL_CHANNEL2_0_MAX]; 
extern const Control CHAN3_CONTROLS[CONTROL_CHANNEL3_0_MAX]; 
extern const Control CHAN4_CONTROLS[CONTROL_CHANNEL4_0_MAX]; 
extern const Control CHAN5_CONTROLS[CONTROL_CHANNEL5_0_MAX]; 

void cellSyncChannel(u8 c){
	for(int i=0, pattern; i<16; i++){
		pattern = VAR_SONG.PATTERNS[c].ORDER[VAR_CHANNEL[c].POSITION];
		VAR_CELLS[c].KEY[i] = VAR_DATA[pattern].KEY[i];
		VAR_CELLS[c].INS[i] = VAR_DATA[pattern].INS[i];
		VAR_CELLS[c].VOL[i] = VAR_DATA[pattern].VOL[i];
		VAR_CELLS[c].CMD[i] = VAR_DATA[pattern].CMD[i];
		VAR_CELLS[c].VAL[i] = VAR_DATA[pattern].VAL[i];

	}
	
	if(regHnd.region != &REGION_MAP_3_TRK) return;
	
	int  len = (VAR_CFG.CURRENTCHANNEL==c?(16*5):16);
	
	switch(VAR_CFG.CURRENTCHANNEL){
		case 0:	
			switch(c){
				case 0:	for(int i=0, li=len; i<li; i++){ regHnd.drawControl(&CHAN0_CONTROLS[i		]); } return;
				case 1:	for(int i=0, li=len; i<li; i++){ regHnd.drawControl(&CHAN0_CONTROLS[i+(16*5)]); } return;
				case 2:	for(int i=0, li=len; i<li; i++){ regHnd.drawControl(&CHAN0_CONTROLS[i+(16*6)]); } return;
				case 3:	for(int i=0, li=len; i<li; i++){ regHnd.drawControl(&CHAN0_CONTROLS[i+(16*7)]); } return;
				case 4:	for(int i=0, li=len; i<li; i++){ regHnd.drawControl(&CHAN0_CONTROLS[i+(16*8)]); } return;
				case 5:	for(int i=0, li=len; i<li; i++){ regHnd.drawControl(&CHAN0_CONTROLS[i+(16*9)]); } return;
			}				
		case 1:	
			switch(c){
				case 0:	for(int i=0, li=len; i<li; i++){ regHnd.drawControl(&CHAN1_CONTROLS[i+(16*5)]); } return;
				case 1:	for(int i=0, li=len; i<li; i++){ regHnd.drawControl(&CHAN1_CONTROLS[i		]); } return;
				case 2:	for(int i=0, li=len; i<li; i++){ regHnd.drawControl(&CHAN1_CONTROLS[i+(16*6)]); } return;
				case 3:	for(int i=0, li=len; i<li; i++){ regHnd.drawControl(&CHAN1_CONTROLS[i+(16*7)]); } return;
				case 4:	for(int i=0, li=len; i<li; i++){ regHnd.drawControl(&CHAN1_CONTROLS[i+(16*8)]); } return;
				case 5:	for(int i=0, li=len; i<li; i++){ regHnd.drawControl(&CHAN1_CONTROLS[i+(16*9)]); } return;
			}
		case 2:	
			switch(c){
				case 0:	for(int i=0, li=len; i<li; i++){ regHnd.drawControl(&CHAN2_CONTROLS[i+(16*5)]); } return;
				case 1:	for(int i=0, li=len; i<li; i++){ regHnd.drawControl(&CHAN2_CONTROLS[i+(16*6)]); } return;
				case 2:	for(int i=0, li=len; i<li; i++){ regHnd.drawControl(&CHAN2_CONTROLS[i		]); } return;
				case 3:	for(int i=0, li=len; i<li; i++){ regHnd.drawControl(&CHAN2_CONTROLS[i+(16*7)]); } return;
				case 4:	for(int i=0, li=len; i<li; i++){ regHnd.drawControl(&CHAN2_CONTROLS[i+(16*8)]); } return;
				case 5:	for(int i=0, li=len; i<li; i++){ regHnd.drawControl(&CHAN2_CONTROLS[i+(16*9)]); } return;
			}
		case 3:	
			switch(c){
				case 0:	for(int i=0, li=len; i<li; i++){ regHnd.drawControl(&CHAN3_CONTROLS[i+(16*5)]); } return;
				case 1:	for(int i=0, li=len; i<li; i++){ regHnd.drawControl(&CHAN3_CONTROLS[i+(16*6)]); } return;
				case 2:	for(int i=0, li=len; i<li; i++){ regHnd.drawControl(&CHAN3_CONTROLS[i+(16*7)]); } return;
				case 3:	for(int i=0, li=len; i<li; i++){ regHnd.drawControl(&CHAN3_CONTROLS[i		]); } return;
				case 4:	for(int i=0, li=len; i<li; i++){ regHnd.drawControl(&CHAN3_CONTROLS[i+(16*8)]); } return;
				case 5:	for(int i=0, li=len; i<li; i++){ regHnd.drawControl(&CHAN3_CONTROLS[i+(16*9)]); } return;
			}
		case 4:	
			switch(c){
				case 0:	for(int i=0, li=len; i<li; i++){ regHnd.drawControl(&CHAN4_CONTROLS[i+(16*5)]); } return;
				case 1:	for(int i=0, li=len; i<li; i++){ regHnd.drawControl(&CHAN4_CONTROLS[i+(16*6)]); } return;
				case 2:	for(int i=0, li=len; i<li; i++){ regHnd.drawControl(&CHAN4_CONTROLS[i+(16*7)]); } return;
				case 3:	for(int i=0, li=len; i<li; i++){ regHnd.drawControl(&CHAN4_CONTROLS[i+(16*8)]); } return;
				case 4:	for(int i=0, li=len; i<li; i++){ regHnd.drawControl(&CHAN4_CONTROLS[i		]); } return;
				case 5:	for(int i=0, li=len; i<li; i++){ regHnd.drawControl(&CHAN4_CONTROLS[i+(16*9)]); } return;
			}
		case 5:	
			switch(c){
				case 0:	for(int i=0, li=len; i<li; i++){ regHnd.drawControl(&CHAN5_CONTROLS[i+(16*5)]); } return;
				case 1:	for(int i=0, li=len; i<li; i++){ regHnd.drawControl(&CHAN5_CONTROLS[i+(16*6)]); } return;
				case 2:	for(int i=0, li=len; i<li; i++){ regHnd.drawControl(&CHAN5_CONTROLS[i+(16*7)]); } return;
				case 3:	for(int i=0, li=len; i<li; i++){ regHnd.drawControl(&CHAN5_CONTROLS[i+(16*8)]); } return;
				case 4:	for(int i=0, li=len; i<li; i++){ regHnd.drawControl(&CHAN5_CONTROLS[i+(16*9)]); } return;
				case 5:	for(int i=0, li=len; i<li; i++){ regHnd.drawControl(&CHAN5_CONTROLS[i		]); } return;
			}
	}
}

// TBC on channel data change
void cellCopy(u8 channel){
		
	for(int i=0, pattern; i<16;i++){		
		
		pattern = VAR_SONG.PATTERNS[channel].ORDER[VAR_CHANNEL[channel].POSITION];
		
		VAR_DATA[pattern].KEY[i] = VAR_CELLS[channel].KEY[i];
		VAR_DATA[pattern].INS[i] = VAR_CELLS[channel].INS[i];
		VAR_DATA[pattern].VOL[i] = VAR_CELLS[channel].VOL[i];
		VAR_DATA[pattern].CMD[i] = VAR_CELLS[channel].CMD[i];
		VAR_DATA[pattern].VAL[i] = VAR_CELLS[channel].VAL[i];
	}
	for(int i=0; i<6;i++){
		if(i==channel)continue;
		cellSyncChannel(i);
	}
}

void modifyNote(Control *c, bool bigstep, bool add, u32 *pointer){
	if(CURRENT_PATTERN == 0x00)return;
	#define VARIABLE	(*(u8*) c->var)
	if(KEY.press( KEY_R )) VARIABLE = 0;
	else {
		VARIABLE += (bigstep?0xC:0x1) * (add?1:-1);
		if(VARIABLE > 159) VARIABLE = 0;
		else if(VARIABLE > 119) VARIABLE = 119;
		if(VARIABLE == 0)VARIABLE = 1;
	}
	transientNote = VARIABLE;
	transientChanged = true;
	cellCopy(VAR_CFG.CURRENTCHANNEL);
	#undef VARIABLE
}
void trkDrawTransientInfo(void){
	NOTE(16,0,7,transientNote);
	HEXADECIMAL_TWOTILES(20,0,7,transientInstrument);
	HEXADECIMAL(23,0,7,transientVolume);
	COMMAND(27,0,7,transientCommand);
	HEXADECIMAL_TWOTILES(28,0,7,transientValue);
	transientChanged = false;
}

const u8 tracker_positions_x[6] = { 15, 20, 25, 15, 20, 25 };    
const u8 tracker_positions_y[6] = {  1,  1,  1,  2,  2,  2 };    
const u16 channel_symbols[6][6] = {
	{ 0x7035, 0xB035, 0xB036, 0xB037, 0xB038, 0xB039 },
	{ 0xB035, 0x7035, 0xB036, 0xB037, 0xB038, 0xB039 },
	{ 0xB035, 0xB035, 0x7036, 0xB037, 0xB038, 0xB039 },
	{ 0xB035, 0xB035, 0xB036, 0x7037, 0xB038, 0xB039 },
	{ 0xB035, 0xB035, 0xB036, 0xB037, 0x7038, 0xB039 },
	{ 0xB035, 0xB035, 0xB036, 0xB037, 0xB038, 0x7039 }	
};
void trkDrawPosition(int channel, bool hl){
	gpu.set(2, tracker_positions_x[channel], tracker_positions_y[channel], channel_symbols[VAR_CFG.CURRENTCHANNEL][channel]);
	HEXADECIMAL(tracker_positions_x[channel]+1	, tracker_positions_y[channel]	, 0x1+hl , VAR_CHANNEL[channel].POSITION>>4);
	HEXADECIMAL(tracker_positions_x[channel]+2	, tracker_positions_y[channel]	, 0x1+hl , VAR_CHANNEL[channel].POSITION&0xf);
	HEXADECIMAL_TWOTILES(tracker_positions_x[channel]+3	, tracker_positions_y[channel]	, hl?0x6:0xD , VAR_SONG.PATTERNS[channel].ORDER[VAR_CHANNEL[channel].POSITION]);
	VAR_CHANNEL[channel].LASTPOSITION =  VAR_CHANNEL[channel].POSITION;	
}

const u8 tracker_cols[6][6] = {
	{ 1, 11, 15, 19, 23, 27 },
	{ 1,  5, 15, 19, 23, 27 },
	{ 1,  5,  9, 19, 23, 27 },
	{ 1,  5,  9, 13, 23, 27 },
	{ 1,  5,  9, 13, 17, 27 },
	{ 1,  5,  9, 13, 17, 21 }
};
void trkDrawLine(int channel){
	int y = 4+VAR_CHANNEL[channel].STEP;
	int x = tracker_cols[VAR_CFG.CURRENTCHANNEL][channel];
	for(int i=0, li=(channel==VAR_CFG.CURRENTCHANNEL?9:3); i<li; i++){
		gpu.set(0, x+i, 4+VAR_CHANNEL[channel].LASTSTEP, 0x0);
		gpu.set(0, x+i, y, 0x15);
	}	
	VAR_CHANNEL[channel].LASTSTEP =  VAR_CHANNEL[channel].STEP;	
}

void trkDispatchMessage(u32 msg){
	switch(msg){
		/*case MESSAGE_OTHER_REFRESH_DATA:
			cellSyncChannel();	
			return;*/
			
		case MESSAGE_OTHER_PREV:
			VAR_CFG.CURRENTCHANNEL = VAR_CFG.CURRENTCHANNEL>0?VAR_CFG.CURRENTCHANNEL-1:5;
			for(int i=0; i<6;i++){
				trkDrawPosition(i, i==VAR_CFG.CURRENTCHANNEL);
			}	
			regHnd.updateViewport(&VIEWPORT_TRK, regHnd.region->xadd,regHnd.region->yadd);
			return;
		
		case MESSAGE_OTHER_NEXT:
			VAR_CFG.CURRENTCHANNEL = VAR_CFG.CURRENTCHANNEL<5?VAR_CFG.CURRENTCHANNEL+1:0;
			for(int i=0; i<6;i++){
				trkDrawPosition(i, i==VAR_CFG.CURRENTCHANNEL);
			}	
			regHnd.updateViewport(&VIEWPORT_TRK, regHnd.region->xadd, regHnd.region->yadd);
			return;
	}
}

void trkGlobalUpdater(RegionHandler* rh){
	static bool tracker_clean = false;

	gpu.set(2,0,1, SPU.playing ? 0xF08D  : 0x31FE );
	gpu.set(1,0,0, SPU.playing?((SPU.currentBeats) == 0?0x32 : ((SPU.currentBeats&3) == 0?0x34 : 0x33)):0x33);
		
	if(transientChanged || rh->redraw) trkDrawTransientInfo();
	if(rh->redraw){
		for(int i=0; i<6;i++){
			
			trkDrawPosition(i,i==VAR_CFG.CURRENTCHANNEL);
			
		}
	}
	
	if(SPU.playing){
		for(int i=0; i<6;i++){
			
			// Update vumeters
			u8 last_peak = VAR_CHANNEL[i].lastpeak;
			u8 peak = VAR_CHANNEL[i].peak;
			if(last_peak!=peak) VUMETER_V3(8+i, 0, 0x0, peak>12?12:peak);			
			
			// Update next step (cell) reactive elements
			if(VAR_CHANNEL[i].LASTSTEP != VAR_CHANNEL[i].STEP) trkDrawLine(i);
			
			// Update next pattern reactive elements
			if((VAR_CHANNEL[i].LASTPOSITION != VAR_CHANNEL[i].POSITION) || rh->redraw) {
				trkDrawPosition(i, i==VAR_CFG.CURRENTCHANNEL);
				cellSyncChannel(i);
				VAR_CHANNEL[i].LASTPOSITION =  VAR_CHANNEL[i].POSITION;
			}
		}
		tracker_clean = false;
		return;
	}
	
	// Tracker screen is dirty; Clean and redraw active control
	if(!tracker_clean) {
		for(int y=4; y<20;y++){
			for(int i=1; i<30; i++){
				gpu.set(0, i, y, 0x0);
			}
		}
		regHnd.sendMessage(MESSAGE_REDRAW_CONTROL | (unsigned)(regHnd.control)&0x0fffffff);
		tracker_clean = true;
	}
}

void updateCHANNEL0(RegionHandler* rh){
	trkGlobalUpdater(rh);
}
void updateCHANNEL1(RegionHandler* rh){
	trkGlobalUpdater(rh);
}
void updateCHANNEL2(RegionHandler* rh){
	trkGlobalUpdater(rh);
}
void updateCHANNEL3(RegionHandler* rh){
	trkGlobalUpdater(rh);
}
void updateCHANNEL4(RegionHandler* rh){
	trkGlobalUpdater(rh);
}
void updateCHANNEL5(RegionHandler* rh){
	trkGlobalUpdater(rh);
}

void updateVIS(RegionHandler* rh){
}
void updateHEADER1(RegionHandler* rh){
}
void updateHEADER0(RegionHandler* rh){
}
void updateTABLE(RegionHandler* rh){
}

#define CB_CHAN_KEY(c,a)	CALLBACK( cb_ch##c##_keypaste_0##a 	, pasteNote	    , EVENT_KEYDOWN_B 	, &VAR_CELLS[ 0x##c ].KEY[ 0x##a ], NULL						);	\
							CALLBACK( cb_ch##c##_key_0##a 		, modifyNote	, EVENT_MODIFY_B 	, &VAR_CELLS[ 0x##c ].KEY[ 0x##a ], &cb_ch##c##_keypaste_0##a 	);
#define CB_CHAN_INS(c,a)	CALLBACK( cb_ch##c##_inspaste_0##a 	, pasteInst		,EVENT_KEYDOWN_B 	, &VAR_CELLS[ 0x##c ].INS[ 0x##a ], NULL						);	\
							CALLBACK( cb_ch##c##_ins_0##a 		, modifyInst	, EVENT_MODIFY_B 	, &VAR_CELLS[ 0x##c ].INS[ 0x##a ], &cb_ch##c##_inspaste_0##a 	);
#define CB_CHAN_VOL(c,a)	CALLBACK( cb_ch##c##_volpaste_0##a 	, pasteVolume	, EVENT_KEYDOWN_B 	, &VAR_CELLS[ 0x##c ].VOL[ 0x##a ], NULL						);	\
							CALLBACK( cb_ch##c##_vol_0##a 		, modifyVolume	, EVENT_MODIFY_B 	, &VAR_CELLS[ 0x##c ].VOL[ 0x##a ], &cb_ch##c##_volpaste_0##a	); 
#define CB_CHAN_CMD(c,a)	CALLBACK( cb_ch##c##_cmdpaste_0##a 	, pasteCommand	, EVENT_KEYDOWN_B 	, &VAR_CELLS[ 0x##c ].CMD[ 0x##a ], NULL						);	\
							CALLBACK( cb_ch##c##_cmd_0##a 		, modifyCommand	, EVENT_MODIFY_B 	, &VAR_CELLS[ 0x##c ].CMD[ 0x##a ], &cb_ch##c##_cmdpaste_0##a 	); 
#define CB_CHAN_VAL(c,a)	CALLBACK( cb_ch##c##_valpaste_0##a 	, pasteValue	, EVENT_KEYDOWN_B 	, &VAR_CELLS[ 0x##c ].VAL[ 0x##a ], NULL						);	\
							CALLBACK( cb_ch##c##_val_0##a 		, modifyValue	, EVENT_MODIFY_B 	, &VAR_CELLS[ 0x##c ].VAL[ 0x##a ], &cb_ch##c##_valpaste_0##a	); 
CB_CHAN_VAL(0, 0);	CB_CHAN_CMD(0, 0);	CB_CHAN_VOL(0, 0);	CB_CHAN_INS(0, 0);	CB_CHAN_KEY(0, 0);
CB_CHAN_VAL(0, 1);	CB_CHAN_CMD(0, 1);	CB_CHAN_VOL(0, 1);	CB_CHAN_INS(0, 1);	CB_CHAN_KEY(0, 1);
CB_CHAN_VAL(0, 2);	CB_CHAN_CMD(0, 2);	CB_CHAN_VOL(0, 2);	CB_CHAN_INS(0, 2);	CB_CHAN_KEY(0, 2);
CB_CHAN_VAL(0, 3);	CB_CHAN_CMD(0, 3);	CB_CHAN_VOL(0, 3);	CB_CHAN_INS(0, 3);	CB_CHAN_KEY(0, 3);
CB_CHAN_VAL(0, 4);	CB_CHAN_CMD(0, 4);	CB_CHAN_VOL(0, 4);	CB_CHAN_INS(0, 4);	CB_CHAN_KEY(0, 4);
CB_CHAN_VAL(0, 5);	CB_CHAN_CMD(0, 5);	CB_CHAN_VOL(0, 5);	CB_CHAN_INS(0, 5);	CB_CHAN_KEY(0, 5);
CB_CHAN_VAL(0, 6);	CB_CHAN_CMD(0, 6);	CB_CHAN_VOL(0, 6);	CB_CHAN_INS(0, 6);	CB_CHAN_KEY(0, 6);
CB_CHAN_VAL(0, 7);	CB_CHAN_CMD(0, 7);	CB_CHAN_VOL(0, 7);	CB_CHAN_INS(0, 7);	CB_CHAN_KEY(0, 7);
CB_CHAN_VAL(0, 8);	CB_CHAN_CMD(0, 8);	CB_CHAN_VOL(0, 8);	CB_CHAN_INS(0, 8);	CB_CHAN_KEY(0, 8);
CB_CHAN_VAL(0, 9);	CB_CHAN_CMD(0, 9);	CB_CHAN_VOL(0, 9);	CB_CHAN_INS(0, 9);	CB_CHAN_KEY(0, 9);
CB_CHAN_VAL(0, A);	CB_CHAN_CMD(0, A);	CB_CHAN_VOL(0, A);	CB_CHAN_INS(0, A);	CB_CHAN_KEY(0, A);
CB_CHAN_VAL(0, B);	CB_CHAN_CMD(0, B);	CB_CHAN_VOL(0, B);	CB_CHAN_INS(0, B);	CB_CHAN_KEY(0, B);
CB_CHAN_VAL(0, C);	CB_CHAN_CMD(0, C);	CB_CHAN_VOL(0, C);	CB_CHAN_INS(0, C);	CB_CHAN_KEY(0, C);
CB_CHAN_VAL(0, D);	CB_CHAN_CMD(0, D);	CB_CHAN_VOL(0, D);	CB_CHAN_INS(0, D);	CB_CHAN_KEY(0, D);
CB_CHAN_VAL(0, E);	CB_CHAN_CMD(0, E);	CB_CHAN_VOL(0, E);	CB_CHAN_INS(0, E);	CB_CHAN_KEY(0, E);
CB_CHAN_VAL(0, F);	CB_CHAN_CMD(0, F);	CB_CHAN_VOL(0, F);	CB_CHAN_INS(0, F);	CB_CHAN_KEY(0, F);

CB_CHAN_VAL(1, 0);	CB_CHAN_CMD(1, 0);	CB_CHAN_VOL(1, 0);	CB_CHAN_INS(1, 0);	CB_CHAN_KEY(1, 0);
CB_CHAN_VAL(1, 1);	CB_CHAN_CMD(1, 1);	CB_CHAN_VOL(1, 1);	CB_CHAN_INS(1, 1);	CB_CHAN_KEY(1, 1);
CB_CHAN_VAL(1, 2);	CB_CHAN_CMD(1, 2);	CB_CHAN_VOL(1, 2);	CB_CHAN_INS(1, 2);	CB_CHAN_KEY(1, 2);
CB_CHAN_VAL(1, 3);	CB_CHAN_CMD(1, 3);	CB_CHAN_VOL(1, 3);	CB_CHAN_INS(1, 3);	CB_CHAN_KEY(1, 3);
CB_CHAN_VAL(1, 4);	CB_CHAN_CMD(1, 4);	CB_CHAN_VOL(1, 4);	CB_CHAN_INS(1, 4);	CB_CHAN_KEY(1, 4);
CB_CHAN_VAL(1, 5);	CB_CHAN_CMD(1, 5);	CB_CHAN_VOL(1, 5);	CB_CHAN_INS(1, 5);	CB_CHAN_KEY(1, 5);
CB_CHAN_VAL(1, 6);	CB_CHAN_CMD(1, 6);	CB_CHAN_VOL(1, 6);	CB_CHAN_INS(1, 6);	CB_CHAN_KEY(1, 6);
CB_CHAN_VAL(1, 7);	CB_CHAN_CMD(1, 7);	CB_CHAN_VOL(1, 7);	CB_CHAN_INS(1, 7);	CB_CHAN_KEY(1, 7);
CB_CHAN_VAL(1, 8);	CB_CHAN_CMD(1, 8);	CB_CHAN_VOL(1, 8);	CB_CHAN_INS(1, 8);	CB_CHAN_KEY(1, 8);
CB_CHAN_VAL(1, 9);	CB_CHAN_CMD(1, 9);	CB_CHAN_VOL(1, 9);	CB_CHAN_INS(1, 9);	CB_CHAN_KEY(1, 9);
CB_CHAN_VAL(1, A);	CB_CHAN_CMD(1, A);	CB_CHAN_VOL(1, A);	CB_CHAN_INS(1, A);	CB_CHAN_KEY(1, A);
CB_CHAN_VAL(1, B);	CB_CHAN_CMD(1, B);	CB_CHAN_VOL(1, B);	CB_CHAN_INS(1, B);	CB_CHAN_KEY(1, B);
CB_CHAN_VAL(1, C);	CB_CHAN_CMD(1, C);	CB_CHAN_VOL(1, C);	CB_CHAN_INS(1, C);	CB_CHAN_KEY(1, C);
CB_CHAN_VAL(1, D);	CB_CHAN_CMD(1, D);	CB_CHAN_VOL(1, D);	CB_CHAN_INS(1, D);	CB_CHAN_KEY(1, D);
CB_CHAN_VAL(1, E);	CB_CHAN_CMD(1, E);	CB_CHAN_VOL(1, E);	CB_CHAN_INS(1, E);	CB_CHAN_KEY(1, E);
CB_CHAN_VAL(1, F);	CB_CHAN_CMD(1, F);	CB_CHAN_VOL(1, F);	CB_CHAN_INS(1, F);	CB_CHAN_KEY(1, F);

CB_CHAN_VAL(2, 0);	CB_CHAN_CMD(2, 0);	CB_CHAN_VOL(2, 0);	CB_CHAN_INS(2, 0);	CB_CHAN_KEY(2, 0);
CB_CHAN_VAL(2, 1);	CB_CHAN_CMD(2, 1);	CB_CHAN_VOL(2, 1);	CB_CHAN_INS(2, 1);	CB_CHAN_KEY(2, 1);
CB_CHAN_VAL(2, 2);	CB_CHAN_CMD(2, 2);	CB_CHAN_VOL(2, 2);	CB_CHAN_INS(2, 2);	CB_CHAN_KEY(2, 2);
CB_CHAN_VAL(2, 3);	CB_CHAN_CMD(2, 3);	CB_CHAN_VOL(2, 3);	CB_CHAN_INS(2, 3);	CB_CHAN_KEY(2, 3);
CB_CHAN_VAL(2, 4);	CB_CHAN_CMD(2, 4);	CB_CHAN_VOL(2, 4);	CB_CHAN_INS(2, 4);	CB_CHAN_KEY(2, 4);
CB_CHAN_VAL(2, 5);	CB_CHAN_CMD(2, 5);	CB_CHAN_VOL(2, 5);	CB_CHAN_INS(2, 5);	CB_CHAN_KEY(2, 5);
CB_CHAN_VAL(2, 6);	CB_CHAN_CMD(2, 6);	CB_CHAN_VOL(2, 6);	CB_CHAN_INS(2, 6);	CB_CHAN_KEY(2, 6);
CB_CHAN_VAL(2, 7);	CB_CHAN_CMD(2, 7);	CB_CHAN_VOL(2, 7);	CB_CHAN_INS(2, 7);	CB_CHAN_KEY(2, 7);
CB_CHAN_VAL(2, 8);	CB_CHAN_CMD(2, 8);	CB_CHAN_VOL(2, 8);	CB_CHAN_INS(2, 8);	CB_CHAN_KEY(2, 8);
CB_CHAN_VAL(2, 9);	CB_CHAN_CMD(2, 9);	CB_CHAN_VOL(2, 9);	CB_CHAN_INS(2, 9);	CB_CHAN_KEY(2, 9);
CB_CHAN_VAL(2, A);	CB_CHAN_CMD(2, A);	CB_CHAN_VOL(2, A);	CB_CHAN_INS(2, A);	CB_CHAN_KEY(2, A);
CB_CHAN_VAL(2, B);	CB_CHAN_CMD(2, B);	CB_CHAN_VOL(2, B);	CB_CHAN_INS(2, B);	CB_CHAN_KEY(2, B);
CB_CHAN_VAL(2, C);	CB_CHAN_CMD(2, C);	CB_CHAN_VOL(2, C);	CB_CHAN_INS(2, C);	CB_CHAN_KEY(2, C);
CB_CHAN_VAL(2, D);	CB_CHAN_CMD(2, D);	CB_CHAN_VOL(2, D);	CB_CHAN_INS(2, D);	CB_CHAN_KEY(2, D);
CB_CHAN_VAL(2, E);	CB_CHAN_CMD(2, E);	CB_CHAN_VOL(2, E);	CB_CHAN_INS(2, E);	CB_CHAN_KEY(2, E);
CB_CHAN_VAL(2, F);	CB_CHAN_CMD(2, F);	CB_CHAN_VOL(2, F);	CB_CHAN_INS(2, F);	CB_CHAN_KEY(2, F);

CB_CHAN_VAL(3, 0);	CB_CHAN_CMD(3, 0);	CB_CHAN_VOL(3, 0);	CB_CHAN_INS(3, 0);	CB_CHAN_KEY(3, 0);
CB_CHAN_VAL(3, 1);	CB_CHAN_CMD(3, 1);	CB_CHAN_VOL(3, 1);	CB_CHAN_INS(3, 1);	CB_CHAN_KEY(3, 1);
CB_CHAN_VAL(3, 2);	CB_CHAN_CMD(3, 2);	CB_CHAN_VOL(3, 2);	CB_CHAN_INS(3, 2);	CB_CHAN_KEY(3, 2);
CB_CHAN_VAL(3, 3);	CB_CHAN_CMD(3, 3);	CB_CHAN_VOL(3, 3);	CB_CHAN_INS(3, 3);	CB_CHAN_KEY(3, 3);
CB_CHAN_VAL(3, 4);	CB_CHAN_CMD(3, 4);	CB_CHAN_VOL(3, 4);	CB_CHAN_INS(3, 4);	CB_CHAN_KEY(3, 4);
CB_CHAN_VAL(3, 5);	CB_CHAN_CMD(3, 5);	CB_CHAN_VOL(3, 5);	CB_CHAN_INS(3, 5);	CB_CHAN_KEY(3, 5);
CB_CHAN_VAL(3, 6);	CB_CHAN_CMD(3, 6);	CB_CHAN_VOL(3, 6);	CB_CHAN_INS(3, 6);	CB_CHAN_KEY(3, 6);
CB_CHAN_VAL(3, 7);	CB_CHAN_CMD(3, 7);	CB_CHAN_VOL(3, 7);	CB_CHAN_INS(3, 7);	CB_CHAN_KEY(3, 7);
CB_CHAN_VAL(3, 8);	CB_CHAN_CMD(3, 8);	CB_CHAN_VOL(3, 8);	CB_CHAN_INS(3, 8);	CB_CHAN_KEY(3, 8);
CB_CHAN_VAL(3, 9);	CB_CHAN_CMD(3, 9);	CB_CHAN_VOL(3, 9);	CB_CHAN_INS(3, 9);	CB_CHAN_KEY(3, 9);
CB_CHAN_VAL(3, A);	CB_CHAN_CMD(3, A);	CB_CHAN_VOL(3, A);	CB_CHAN_INS(3, A);	CB_CHAN_KEY(3, A);
CB_CHAN_VAL(3, B);	CB_CHAN_CMD(3, B);	CB_CHAN_VOL(3, B);	CB_CHAN_INS(3, B);	CB_CHAN_KEY(3, B);
CB_CHAN_VAL(3, C);	CB_CHAN_CMD(3, C);	CB_CHAN_VOL(3, C);	CB_CHAN_INS(3, C);	CB_CHAN_KEY(3, C);
CB_CHAN_VAL(3, D);	CB_CHAN_CMD(3, D);	CB_CHAN_VOL(3, D);	CB_CHAN_INS(3, D);	CB_CHAN_KEY(3, D);
CB_CHAN_VAL(3, E);	CB_CHAN_CMD(3, E);	CB_CHAN_VOL(3, E);	CB_CHAN_INS(3, E);	CB_CHAN_KEY(3, E);
CB_CHAN_VAL(3, F);	CB_CHAN_CMD(3, F);	CB_CHAN_VOL(3, F);	CB_CHAN_INS(3, F);	CB_CHAN_KEY(3, F);

CB_CHAN_VAL(4, 0);	CB_CHAN_CMD(4, 0);	CB_CHAN_VOL(4, 0);	CB_CHAN_INS(4, 0);	CB_CHAN_KEY(4, 0);
CB_CHAN_VAL(4, 1);	CB_CHAN_CMD(4, 1);	CB_CHAN_VOL(4, 1);	CB_CHAN_INS(4, 1);	CB_CHAN_KEY(4, 1);
CB_CHAN_VAL(4, 2);	CB_CHAN_CMD(4, 2);	CB_CHAN_VOL(4, 2);	CB_CHAN_INS(4, 2);	CB_CHAN_KEY(4, 2);
CB_CHAN_VAL(4, 3);	CB_CHAN_CMD(4, 3);	CB_CHAN_VOL(4, 3);	CB_CHAN_INS(4, 3);	CB_CHAN_KEY(4, 3);
CB_CHAN_VAL(4, 4);	CB_CHAN_CMD(4, 4);	CB_CHAN_VOL(4, 4);	CB_CHAN_INS(4, 4);	CB_CHAN_KEY(4, 4);
CB_CHAN_VAL(4, 5);	CB_CHAN_CMD(4, 5);	CB_CHAN_VOL(4, 5);	CB_CHAN_INS(4, 5);	CB_CHAN_KEY(4, 5);
CB_CHAN_VAL(4, 6);	CB_CHAN_CMD(4, 6);	CB_CHAN_VOL(4, 6);	CB_CHAN_INS(4, 6);	CB_CHAN_KEY(4, 6);
CB_CHAN_VAL(4, 7);	CB_CHAN_CMD(4, 7);	CB_CHAN_VOL(4, 7);	CB_CHAN_INS(4, 7);	CB_CHAN_KEY(4, 7);
CB_CHAN_VAL(4, 8);	CB_CHAN_CMD(4, 8);	CB_CHAN_VOL(4, 8);	CB_CHAN_INS(4, 8);	CB_CHAN_KEY(4, 8);
CB_CHAN_VAL(4, 9);	CB_CHAN_CMD(4, 9);	CB_CHAN_VOL(4, 9);	CB_CHAN_INS(4, 9);	CB_CHAN_KEY(4, 9);
CB_CHAN_VAL(4, A);	CB_CHAN_CMD(4, A);	CB_CHAN_VOL(4, A);	CB_CHAN_INS(4, A);	CB_CHAN_KEY(4, A);
CB_CHAN_VAL(4, B);	CB_CHAN_CMD(4, B);	CB_CHAN_VOL(4, B);	CB_CHAN_INS(4, B);	CB_CHAN_KEY(4, B);
CB_CHAN_VAL(4, C);	CB_CHAN_CMD(4, C);	CB_CHAN_VOL(4, C);	CB_CHAN_INS(4, C);	CB_CHAN_KEY(4, C);
CB_CHAN_VAL(4, D);	CB_CHAN_CMD(4, D);	CB_CHAN_VOL(4, D);	CB_CHAN_INS(4, D);	CB_CHAN_KEY(4, D);
CB_CHAN_VAL(4, E);	CB_CHAN_CMD(4, E);	CB_CHAN_VOL(4, E);	CB_CHAN_INS(4, E);	CB_CHAN_KEY(4, E);
CB_CHAN_VAL(4, F);	CB_CHAN_CMD(4, F);	CB_CHAN_VOL(4, F);	CB_CHAN_INS(4, F);	CB_CHAN_KEY(4, F);

CB_CHAN_VAL(5, 0);	CB_CHAN_CMD(5, 0);	CB_CHAN_VOL(5, 0);	CB_CHAN_INS(5, 0);	CB_CHAN_KEY(5, 0);
CB_CHAN_VAL(5, 1);	CB_CHAN_CMD(5, 1);	CB_CHAN_VOL(5, 1);	CB_CHAN_INS(5, 1);	CB_CHAN_KEY(5, 1);
CB_CHAN_VAL(5, 2);	CB_CHAN_CMD(5, 2);	CB_CHAN_VOL(5, 2);	CB_CHAN_INS(5, 2);	CB_CHAN_KEY(5, 2);
CB_CHAN_VAL(5, 3);	CB_CHAN_CMD(5, 3);	CB_CHAN_VOL(5, 3);	CB_CHAN_INS(5, 3);	CB_CHAN_KEY(5, 3);
CB_CHAN_VAL(5, 4);	CB_CHAN_CMD(5, 4);	CB_CHAN_VOL(5, 4);	CB_CHAN_INS(5, 4);	CB_CHAN_KEY(5, 4);
CB_CHAN_VAL(5, 5);	CB_CHAN_CMD(5, 5);	CB_CHAN_VOL(5, 5);	CB_CHAN_INS(5, 5);	CB_CHAN_KEY(5, 5);
CB_CHAN_VAL(5, 6);	CB_CHAN_CMD(5, 6);	CB_CHAN_VOL(5, 6);	CB_CHAN_INS(5, 6);	CB_CHAN_KEY(5, 6);
CB_CHAN_VAL(5, 7);	CB_CHAN_CMD(5, 7);	CB_CHAN_VOL(5, 7);	CB_CHAN_INS(5, 7);	CB_CHAN_KEY(5, 7);
CB_CHAN_VAL(5, 8);	CB_CHAN_CMD(5, 8);	CB_CHAN_VOL(5, 8);	CB_CHAN_INS(5, 8);	CB_CHAN_KEY(5, 8);
CB_CHAN_VAL(5, 9);	CB_CHAN_CMD(5, 9);	CB_CHAN_VOL(5, 9);	CB_CHAN_INS(5, 9);	CB_CHAN_KEY(5, 9);
CB_CHAN_VAL(5, A);	CB_CHAN_CMD(5, A);	CB_CHAN_VOL(5, A);	CB_CHAN_INS(5, A);	CB_CHAN_KEY(5, A);
CB_CHAN_VAL(5, B);	CB_CHAN_CMD(5, B);	CB_CHAN_VOL(5, B);	CB_CHAN_INS(5, B);	CB_CHAN_KEY(5, B);
CB_CHAN_VAL(5, C);	CB_CHAN_CMD(5, C);	CB_CHAN_VOL(5, C);	CB_CHAN_INS(5, C);	CB_CHAN_KEY(5, C);
CB_CHAN_VAL(5, D);	CB_CHAN_CMD(5, D);	CB_CHAN_VOL(5, D);	CB_CHAN_INS(5, D);	CB_CHAN_KEY(5, D);
CB_CHAN_VAL(5, E);	CB_CHAN_CMD(5, E);	CB_CHAN_VOL(5, E);	CB_CHAN_INS(5, E);	CB_CHAN_KEY(5, E);
CB_CHAN_VAL(5, F);	CB_CHAN_CMD(5, F);	CB_CHAN_VOL(5, F);	CB_CHAN_INS(5, F);	CB_CHAN_KEY(5, F);

#undef CB_CHAN_VAL
#undef CB_CHAN_CMD
#undef CB_CHAN_VOL
#undef CB_CHAN_INS
#undef CB_CHAN_KEY
