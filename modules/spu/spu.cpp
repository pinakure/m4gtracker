#include "spu.hpp"
#include "../../data/variables.hpp"

int  SPU::timerTarget;
int  SPU::currentPattern; //NOT USED! 
int  SPU::currentTicks;
int  SPU::targetTick[6];
int  SPU::currentBeats;
int  SPU::beatsPerBar;
int  SPU::secPerBeat;
bool SPU::retrig_note[6];
bool SPU::enable_metronome;	
bool SPU::playing;


/* Since these are ASM instructions, we have to ensure CPP does not mangle them! */
extern "C" {
	int CheckBPMClock(int timerTarget);
};


void SPU::jumpToPatternAsync(int p){
	currentTicks = 0;
	currentBeats = 0;
	currentPattern = p;
}

void SPU::setKey(	u8 index, u8 key	){ channel[ index ].key    = key; 	}
void SPU::setInst(  u8 index, u8 inst	){ channel[ index ].inst   = inst; 	}
void SPU::setCmd(	u8 index, u8 cmd	){ channel[ index ].cmd    = cmd;	}
void SPU::setValue( u8 index, u8 value	){ channel[ index ].value  = value; }
void SPU::setVolume(u8 index, u8 volume	){ 
	channel[ index ].volume 	= volume; 
	channel[ index ].last_peak	= channel[ index ].peak; 
	channel[ index ].peak 		= volume;
}

void SPU::init(int bpm){
	
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
	enable(0);
	enable(1);
	enable(2);
	enable(3);
}

#define SOUND_PWM_MASTER_RIGHT(vol)		(vol&0x7)
#define SOUND_PWM_MASTER_LEFT(vol)		((vol&0x7)<<4)
#define SOUND_PWM_ENABLE_RIGHT(index)	((1<<8)<<index)
#define SOUND_PWM_ENABLE_LEFT(index)	((1<<12)<<index)
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

void SPU::enable(int index){
	
	
	
	// TODO: handle channel to enable arbitrary channels
	
	*((volatile u16*)0x04000084) |= SOUND_PWM_ENABLE_PSG_FIFO;
	*((volatile u16*)0x04000080) |= SOUND_PWM_ENABLE_LEFT(index) | SOUND_PWM_ENABLE_RIGHT(index) | SOUND_PWM_MASTER_LEFT(7) | SOUND_PWM_MASTER_RIGHT(7);
	//Enable SOUND 1 and 2
	*((volatile u16*)0x04000082) |= 0xE;//PWM 100% DMA 100% (set volume)
}

void SPU::disable(void){
}

void SPU::setTempo(int bpm){
	timerTarget = (BPM_MAGIC / bpm)*10; 
	song.bpm = bpm;
	*((volatile u16*)0x0400010A) = 0x0; //Disable timer (bit 7)
	*((volatile u16*)0x04000108) = 0x0000; //'reset' timer counter
	*((volatile u16*)0x0400010A) = 0x82; //Enable (bit 7)		
}


void SPU::stop(void){
	playing = false;
}

void SPU::play(bool from_start){
	if(playing) return;

	currentBeats 	= 0;
	currentTicks 	= -1;
	
	if(from_start){
		currentPattern 	= 0;
		for( int i=0; i < 6; i++ ){
			channel[ i ].step		= 0;
			channel[ i ].position	= 0;
		}		
		playing = true;

		// Reset timer
		*((volatile u16*)0x0400010A) = 0x0; //Disable timer (bit 7)
		*((volatile u16*)0x04000108) = 0x0;
		*((volatile u16*)0x0400010A) = 0x82; //Enable (bit 7)
		return;
	}
	
	for( int i=0; i < 6; i++ ){
		channel[ i ].step = 255;
	}
	
	playing = true;
	
	// Reset timer
	*((volatile u16*)0x0400010A) = 0x0; //Disable timer (bit 7)
	*((volatile u16*)0x04000108) = 0x0;
	*((volatile u16*)0x0400010A) = 0x82; //Enable (bit 7)
}


bool updateChannel(u8 chan){
	
	
	// DONT UPDATE CHANNELS WHEN THEY ARE ON PATTERN 0 (this is empty)
	if( song.patterns[ chan ].order[ channel[chan].position ] == 0x00 ){
		// Rewind until a pattern break (0x00) is found, or the beginning of the chain is reach
		while( channel[chan].position > 0 ){
		
			channel[chan].position--;
			
			if( song.patterns[ chan ].order[ channel[chan].position ] == 0x00 ) {
				channel[chan].position++;
				return true;
			}
		}
		// If after rewind, first pattern is still 0, stop this channel
		if( song.patterns[ chan ].order[ 0 ] == 0x00 ) return channel[chan].playing = false;
		return true;
	}
	channel[chan].playing = true;
		
	channel[chan].last_step =  channel[chan].step;
	channel[chan].step++;
	
	if(channel[chan].step == 16 /*channel[chan].LENGTH*/){
		channel[chan].step = 0;
		channel[chan].last_position = channel[chan].position;
		channel[chan].position++;
		SPU::currentBeats=-1;
		if( song.patterns[ chan ].order[ channel[ chan ].position ] == 0x00 ) return updateChannel( chan );
	}
	return true;
}

void SPU::mute(int index){
	// ------------------------------------------------------------
	channel[index].mute ^= 1;
	/* ------------------------------------------------------------
	Since a channel was unmuted, disable solo on every channel   */
	// Sync with audio registers
	/* ------------------------------------------------------------
	If a channel was unmuted, disable solo on every channel   	 */
	if(channel[index].mute) return;
	for(int i=0; i<6;i++){
		channel[i].solo = false;
	}
}

void SPU::solo(int index){
	/* ------------------------------------------------------------
	if channel has solo enabled unmute channels and disable solo */
	if(channel[index].solo){
		for(int i=0; i<6;i++){
			channel[index].solo = false;
			channel[i].mute = false;
		}
		return;
	}
	/* ------------------------------------------------------------
	Mute all channels 											 */
	for(int i=0; i<6;i++){
		channel[i].mute = true;
	}
	/* ------------------------------------------------------------
	Unmute and enable solo on selected channel 				     */
	channel[index].solo = true;
	channel[index].mute = false;
	
	// Sync with audio registers
}

void SPU::noteOnWAV(void){
}

void SPU::noteOnNZE(void){
}

void SPU::noteOnPWM1(void){
	*(u16*)(0x04000064) = 0x8000 | PWM_FREQ_TABLE[channel[0].key];
	retrig_note[0] = false;			
}

void SPU::noteOnPWM2(void){
	*(u16*)(0x0400006C) = 0x8000 | PWM_FREQ_TABLE[channel[1].key];	
	retrig_note[1] = false;			
}

void SPU::triggerChannel(int channel_index){
	
	u8 vol = channel[channel_index].volume;
	u8 ins = channel[channel_index].inst;
	
	// Fill correspondant VAR_XXX setting temporal struct
	Instrument *i = &VAR_INSTRUMENTS[ins];
	

	switch(channel_index){
		case 0:
			
			vol = ((i->SETTINGS[2] & 0xF) * vol) >> 4;
			*(u16*)(0x04000062) = (vol<<12) | 0x0100 | 0x003F;				
			if(retrig_note[channel_index]) return noteOnPWM1();
			return;
			
		case 1: 
			vol = ((i->SETTINGS[2] & 0xF) * vol) >> 4;
			*(u16*)(0x04000068) = (vol<<12) | 0x0100 | 0x003F;				
			if(retrig_note[channel_index]) return noteOnPWM2();
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

extern void DECIMAL_DOUBLE(u8 x, u8 y, u16 color, u16 value);

void SPU::update(){
	bool newBeat, newTick;

	if(!playing){
		return;
	}
	
	if(CheckBPMClock(timerTarget)){
		/* --------------------------------------------------------------------
		Restore timer, set it to exceeded time 								 */
		*((volatile u16*)0x0400010A) = 0x0; //Disable timer (bit 7)
		*((volatile u16*)0x04000108) = *((volatile u16*)0x04000108) - timerTarget; //'reset' timer counter
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
			currentBeats++;		
		}	
		
		/* ----------------------------------------------------------------
		If current tick%4 == 0, time to trigger notes					 */
		if(!(currentTicks&0x3)){
			for(int i=0, s = 0; i<6; i++){
				s = channel[i].step;
				retrig_note[i] = false;
				if(VAR_CELLS[i].key[s] > 0x0) {
					retrig_note[i] = true;
					setKey(i, VAR_CELLS[i].key[s]); 
					setVolume(i	, VAR_CELLS[i].vol[s]); 
				}
				if(VAR_CELLS[i].ins[s] > 0x0) {				
					setInst(i	, VAR_CELLS[i].ins[s] ); 
					if(!retrig_note[i]) setVolume(i	, VAR_CELLS[i].vol[s]); 
				}
				
				if(VAR_CELLS[i].cmd[s]  > 0x0) {
					setCmd(i 	, VAR_CELLS[i].cmd[s] ); 
					setValue(i 	, VAR_CELLS[i].val[s]); 
				}
				
				
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
	//GPU::set(1,0,0, ((SPU::currentTicks>>2)&1) == 0?0x6F : 0x60);
	//GPU::set(1,0,1, 0x5);

	//DECIMAL_DOUBLE(0,0,9, SPU::currentTicks>>2);
	DECIMAL_DOUBLE(2,2,1, SPU::currentBeats);
	/*
	DECIMAL_DOUBLE(3,2,2, SPU::currentPattern);
	VUMETER_V1(0,0,4, (SPU::currentBeats % 4) < 1);				
	WAVE_SINGLE(0,1,4, (SPU::currentBeats % 4));				*/
}

