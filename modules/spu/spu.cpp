Spu SPU;

/* Since these are ASM instructions, we have to ensure CPP does not mangle them! */
extern "C" {
	int CheckBPMClock(int timerTarget);
};


u8 transpose[6] = { 0, 0, 0, 0, 0, 0 };
bool reset_channel[6] = { false, false, false, false, false, false };
u8	 tsp_position[2];/*16x4*//*126*/
u8	 vol_position[2];/*16x4*//*190*/

void Spu::jumpToPatternAsync(int p){
	currentTicks = 0;
	currentBeats = 0;
	currentPattern = p;
}

bool Spu::setKey(	u8 channel, u8 key, u8 vol){ 
	reset_channel[channel] = key>0;
	VAR_CHANNEL[channel].key    = key; 	
	VAR_CHANNEL[channel].volume = vol; 
	VAR_CHANNEL[channel].lastpeak = VAR_CHANNEL[channel].peak; 
	VAR_CHANNEL[channel].peak = vol;
	return true;
}
void Spu::setInst(  u8 channel, u8 inst, u8 vol, bool retrig_note){ 
	VAR_CHANNEL[channel].inst   = inst;
	if(retrig_note) return; 	
	VAR_CHANNEL[channel].volume = vol; 
	VAR_CHANNEL[channel].lastpeak = VAR_CHANNEL[channel].peak; 
	VAR_CHANNEL[channel].peak = vol;	
}	
void Spu::setCmd(	u8 channel, u8 cmd, u8 value ){ 
	VAR_CHANNEL[channel].cmd    = cmd;	
	VAR_CHANNEL[channel].value  = value; 
}

void Spu::Init(int bpm){
	
	/* Set timer 2 prescaler to F/256 and reset it */
	*((volatile u16*)0x0400010A) = 0x0;    //Disable (bit 7)
	*((volatile u16*)0x04000108) = 0x0000; //Set counter to 0
	*((volatile u16*)0x0400010A) = 0x82;   //Enable (bit 7)
	
	/* Set timer 3 prescaler to F/256 and reset it */
	*((volatile u16*)0x0400010E) = 0x0;    //Disable (bit 7)
	*((volatile u16*)0x0400010C) = 0x0000; //Set counter to 0
	*((volatile u16*)0x0400010E) = 0x82;   //Enable (bit 7)
	
	currentTicks 	= 0;
	currentBeats 	= 0;
	currentPattern 	= 0;
	playing			= 0;
	beatsPerBar 	= 4;
	setTempo(bpm);
	enable();
}


#define SOUND_PWM_MASTER_RIGHT(vol)		(vol&0x7)
#define SOUND_PWM_MASTER_LEFT(vol)		((vol&0x7)<<4)
#define SOUND_PWM_ENABLE_RIGHT(channel)	((1<<8)<<channel)
#define SOUND_PWM_ENABLE_LEFT(channel)	((1<<12)<<channel)
#define SOUND_PWM_ENABLE_PSG_FIFO		0x0080




const u16 PWM_FREQ_TABLE[120]={ 0		,
								44 		,157	,263	,363	,457	,547	,631	,710	,786	,857	,923	,986	,
								1046	,1102	,1155	,1205	,1253	,1297	,1340	,1379	,1417	,1452	,1485	,1518	,
								1547	,1575	,1602	,1627	,1650	,1673	,1694	,1714	,1732	,1750	,1767	,1783	,
								1797	,1811	,1825	,1837	,1849	,1860	,1871	,1881	,1890	,1899	,1907	,1915	,
								1923	,1930	,1936	,1943	,1949	,1954	,1959	,1964	,1969	,1974	,1978	,1982	,
								1985	,1989	,1992	,1995	,1998	,2001	,2004	,2006	,2009	,2011	,2013	,2015	,
								2017	,2018	,2020	,2022	,2023	,2025	,2026	,2027	,2028	,2029	,2030	,2031	,
								2032	,2033	,2034	,2035	,2036	,2037	,2038	,2039	,2040	,2041	,2042	,2043	,
								2044	,2045	,2046	,2047	,2047	,2047	,2047	,2047	,2047	,2047	,2047	,2047	,
								2047	,2047	,2047	,2047	,2047	,2047	,2047	,2047	,2047	,2047	,2047	};

void Spu::enable(){
	// Sound 1-4 ON Flag, PSG/FIFO Master enable
	*((volatile u16*)0x04000084) = 0x0080;
	
	// Sound 1-4 Master Right 100%, Sound 1-4 Master Left 100%, Sound 1-4 Enable Right, Sound 1-4 Enable Left
	*((volatile u16*)0x04000080) = 0xFF77;

	// Sound 1-4 Volume 100%, DMA A 100%, DMA B 100% 
	// We wont enable DMA sound by the moment
	*((volatile u16*)0x04000082) |= 0xff0E;//PWM 100% DMA 100% (set volume)
}

void Spu::disable(void){
}

void Spu::setTempo(int bpm){
	timerTarget = (BPM_MAGIC / bpm)*10; 
	VAR_SONG.BPM = bpm;
	*((volatile u16*)0x0400010A) = 0x0; //Disable timer (bit 7)
	*((volatile u16*)0x04000108) = 0x0000; //'reset' timer counter
	*((volatile u16*)0x0400010A) = 0x82; //Enable (bit 7)		
}


void Spu::stop(void){
	playing = false;
}

void Spu::play(bool from_start){
	if(playing) return;

	currentBeats 	= 0;
	currentTicks 	= -1;
	
	if(from_start){
		currentPattern 	= 0;
		for(int i=0; i<6;i++){
			VAR_CHANNEL[i].STEP = 0;
			VAR_CHANNEL[i].POSITION = 0;
		}		
		playing = true;

		// Reset timer
		*((volatile u16*)0x0400010A) = 0x0; //Disable timer (bit 7)
		*((volatile u16*)0x04000108) = 0x0;
		*((volatile u16*)0x0400010A) = 0x82; //Enable (bit 7)
		return;
	}
	
	for(int i=0; i<6;i++){
		VAR_CHANNEL[i].STEP = 255;
	}
	
	playing = true;
	
	// Reset timer
	*((volatile u16*)0x0400010A) = 0x0; //Disable timer (bit 7)
	*((volatile u16*)0x04000108) = 0x0;
	*((volatile u16*)0x0400010A) = 0x82; //Enable (bit 7)
}


bool updateChannel(u8 chan){
	
	
	// DONT UPDATE CHANNELS WHEN THEY ARE ON PATTERN 0 (this is empty)
	if(VAR_SONG.PATTERNS[chan].ORDER[VAR_CHANNEL[chan].POSITION] == 0x00){			
		// Rewind until a pattern break (0x00) is found, or the beginning of the chain is reach
		while(VAR_CHANNEL[chan].POSITION > 0){
		
			VAR_CHANNEL[chan].POSITION--;
			
			if(VAR_SONG.PATTERNS[chan].ORDER[VAR_CHANNEL[chan].POSITION] == 0x00) {
				VAR_CHANNEL[chan].POSITION++;
				return true;
			}
		}
		// If after rewind, first pattern is still 0, stop this channel
		if(VAR_SONG.PATTERNS[chan].ORDER[0] == 0x00) return VAR_CHANNEL[chan].PLAYING = false;
		return true;
	}
	VAR_CHANNEL[chan].PLAYING = true;
		
	VAR_CHANNEL[chan].LASTSTEP =  VAR_CHANNEL[chan].STEP;
	VAR_CHANNEL[chan].STEP++;
	
	if(VAR_CHANNEL[chan].STEP == 16 /*VAR_CHANNEL[chan].LENGTH*/){
		VAR_CHANNEL[chan].STEP = 0;
		VAR_CHANNEL[chan].LASTPOSITION = VAR_CHANNEL[chan].POSITION;
		VAR_CHANNEL[chan].POSITION++;
		SPU.currentBeats=-1;
		if(VAR_SONG.PATTERNS[chan].ORDER[VAR_CHANNEL[chan].POSITION] == 0x00) return updateChannel(chan);
	}
	return true;
}

void Spu::mute(int channel){
	// ------------------------------------------------------------
	VAR_CHANNEL[channel].mute ^= 1;
	/* ------------------------------------------------------------
	Since a channel was unmuted, disable solo on every channel   */
	// Sync with audio registers
	/* ------------------------------------------------------------
	If a channel was unmuted, disable solo on every channel   	 */
	if(VAR_CHANNEL[channel].mute) return;
	for(int i=0; i<6;i++){
		VAR_CHANNEL[i].solo = false;
	}
}

void Spu::solo(int channel){
	/* ------------------------------------------------------------
	if channel has solo enabled unmute channels and disable solo */
	if(VAR_CHANNEL[channel].solo){
		for(int i=0; i<6;i++){
			VAR_CHANNEL[channel].solo = false;
			VAR_CHANNEL[i].mute = false;
		}
		return;
	}
	/* ------------------------------------------------------------
	Mute all channels 											 */
	for(int i=0; i<6;i++){
		VAR_CHANNEL[i].mute = true;
	}
	/* ------------------------------------------------------------
	Unmute and enable solo on selected channel 				     */
	VAR_CHANNEL[channel].solo = true;
	VAR_CHANNEL[channel].mute = false;
	
	// Sync with audio registers
}

void Spu::noteOnWAV(void){
}

void Spu::noteOnNZE(void){
}

void Spu::noteOnPWM1(void){
	*(u16*)(0x04000064) = 0x8000 | PWM_FREQ_TABLE[VAR_CHANNEL[0].key + transpose[0] + (VAR_CFG.TRACKER.TRANSPOSE-0x80) + VAR_SONG.TRANSPOSE ]+(VAR_CFG.TRACKER.FINETUNE<<1);
	retrig_note[0] = false;			
}

void Spu::noteOnPWM2(void){
	*(u16*)(0x0400006C) = 0x8000 | PWM_FREQ_TABLE[VAR_CHANNEL[1].key + transpose[1] + (VAR_CFG.TRACKER.TRANSPOSE-0x80) + VAR_SONG.TRANSPOSE ]+(VAR_CFG.TRACKER.FINETUNE<<1);
	retrig_note[1] = false;			
}

extern SETTINGS_PWM unpackPWM(INSTRUMENT *i);

void Spu::triggerChannel(int channel_index){
	
	u8 vol = VAR_CHANNEL[channel_index].volume;
	u8 ins = VAR_CHANNEL[channel_index].inst;
	
	// Fill correspondant VAR_XXX setting temporal struct
	INSTRUMENT *i = &VAR_INSTRUMENTS[ins];
	
	SETTINGS_PWM pwm;
	switch(channel_index){
		case 0:			
			pwm = unpackPWM( i );
			if(reset_channel[0]) {
				tsp_position[0]=0;				
				vol_position[0]=0;
				reset_channel[0] = false;				
				transpose[0] = 0;
			}
			if(pwm.TSP_ENABLE){				
				if(tsp_position[0] < pwm.TSP_ENVELOPE){
					transpose[0] = pwm.TSP[ tsp_position[0] ];
					tsp_position[0]++;
					retrig_note[0] = true;
				}
			}
			if(pwm.VOL_ENABLE){				
				if( vol_position[0] < pwm.VOL_ENVELOPE){
					vol = (vol + pwm.VOL[ vol_position[0] ]) >> 1;
					vol_position[0]++;
					retrig_note[0] = true;
				}
			}
			if(retrig_note[0]){
				//vol = ((i->SETTINGS[2] & 0xF) * vol) >> 4;
				*(u16*)(0x04000060) = (pwm.SWEEPSTEPS << 4) | (pwm.SWEEPDIR << 3) | ( pwm.SWEEPSPEED );				
				*(u16*)(0x04000062) = ((pwm.LEVEL*vol>>4) << 12) | (!pwm.ENVELOPEDIR << 11) | (pwm.VOLUMEFADE<<8) | ( pwm.DUTYCYCLE << 6 );				
				return noteOnPWM1();
			}
			return;
			
		case 1: 
			pwm = unpackPWM( i );
			if(reset_channel[1]) {
				tsp_position[1]=0;				
				vol_position[1]=0;
				reset_channel[1] = false;				
				transpose[1] = 0;
			}
			if(pwm.TSP_ENABLE){				
				if(tsp_position[1] < pwm.TSP_ENVELOPE){
					transpose[1] = pwm.TSP[ tsp_position[1] ];
					tsp_position[1]++;
					retrig_note[1] = true;
				}
			}
			if(pwm.VOL_ENABLE){				
				if( vol_position[1] < pwm.VOL_ENVELOPE){
					vol = (vol + pwm.VOL[ vol_position[1] ]) >> 1;
					vol_position[1]++;
					retrig_note[1] = true;
				}
			}
			if(retrig_note[1]){
				//vol = ((i->SETTINGS[2] & 0xF) * vol) >> 4;
				*(u16*)(0x04000068) = ((pwm.LEVEL*vol>>4) << 12) | (!pwm.ENVELOPEDIR << 11) | (pwm.VOLUMEFADE<<8) | ( pwm.DUTYCYCLE << 6 );				
				return noteOnPWM2();
			}
			return;
			
		case 2: 
			return;
			
		case 3: 
			return;
			
		case 4: 
			return;
			
		case 5: 
			return;
	}
}

void cellSyncChannel(u8 c);


void Spu::update(void){
	bool newBeat, newTick;

	if(!playing){
		return;
	}
	
	if(CheckBPMClock(this->timerTarget)){
		/* --------------------------------------------------------------------
		Restore timer, set it to exceeded time 								 */
		*((volatile u16*)0x0400010A) = 0x0; //Disable timer (bit 7)
		*((volatile u16*)0x04000108) = *((volatile u16*)0x04000108) - this->timerTarget; //'reset' timer counter
		*((volatile u16*)0x0400010A) = 0x82; //Enable (bit 7)
		// --------------------------------------------------------------------
		newTick = true;
	} else newTick = false;
	
	if(newTick){
		/* ----------------------------------------------------------------
		Jump to next pattern, or find previous loop entry			     */
		if(!(currentTicks % (beatsPerBar*4) )){
			if(updateChannel(0))cellSyncChannel(0); 
			if(updateChannel(1))cellSyncChannel(1);
			if(updateChannel(2))cellSyncChannel(2);
			if(updateChannel(3))cellSyncChannel(3);
			if(updateChannel(4))cellSyncChannel(4);
			if(updateChannel(5))cellSyncChannel(5);			
			//if(stop_it)return stop(); 
			
			currentTicks=0;
			newBeat = true;
		} else newBeat = false;		
		
		
		if(newBeat){			
			/* --------------------------------------------------------------------
			Do metronome noise 													 */			
			/*
			if(enable_metronome){
				if(currentBeats+1 == 0){
					*((volatile u16*)0x04000068) = 0x8181;
					*((volatile u16*)0x0400006C) = 0xC7b7;				
				} else 
				if(!((currentBeats+1) % beatsPerBar)){								
					*((volatile u16*)0x04000068) = 0x8181;
					*((volatile u16*)0x0400006C) = 0xC770;
				}
			}
			*/
			currentBeats++;		
		}	
		
		/* ----------------------------------------------------------------
		If current tick%4 == 0, time to trigger notes					 */
		if(!(currentTicks&0x3)){
			for(int i=0, s = 0; i<6; i++){
				s = VAR_CHANNEL[i].STEP;
				retrig_note[i] = false;
				if(VAR_CELLS[i].KEY[s] > 0x0) retrig_note[i] = setKey(i, VAR_CELLS[i].KEY[s], VAR_CELLS[i].VOL[s]); 
				if(VAR_CELLS[i].INS[s] > 0x0) setInst(i	, VAR_CELLS[i].INS[s], VAR_CELLS[i].VOL[s], retrig_note[i]); 
				if(VAR_CELLS[i].CMD[s] > 0x0) setCmd( i , VAR_CELLS[i].CMD[s], VAR_CELLS[i].VAL[s]); 
				
				
				// Handle Command
				// Handle Groove Table
				targetTick[i] = 0;
				
				if((currentTicks&0x3) == targetTick[i]){
					// trigger sound settings
					triggerChannel(i);
					targetTick[i] = 0;
				}
		
			}
		}
		
		currentTicks++;		
	} 
		
	// Debug output
	//gpu.set(1,0,0, ((SPU.currentTicks>>2)&1) == 0?0x6F : 0x60);
	//gpu.set(1,0,1, 0x5);

	//DECIMAL_DOUBLE(0,0,9, SPU.currentTicks>>2);
	DECIMAL_DOUBLE(2,2,1, SPU.currentBeats);
	/*
	DECIMAL_DOUBLE(3,2,2, SPU.currentPattern);
	VUMETER_V1(0,0,4, (SPU.currentBeats % 4) < 1);				
	WAVE_SINGLE(0,1,4, (SPU.currentBeats % 4));				*/
}

