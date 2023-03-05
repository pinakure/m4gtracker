#include "mixer.hpp"
#include "synth.hpp"
#include "sequencer.hpp"

Spu SPU;

/* Since these are ASM instructions, we have to ensure CPP does not mangle them! */
extern "C" {
	int CheckBPMClock(int timerTarget);
};

const u16 PWM_FREQ_TABLE[ 120 ]={ 
	0		,
	44 		,157	,263	,363	,457	,547	,631	,710	,786	,857	,923	,986	,
	1046	,1102	,1155	,1205	,1253	,1297	,1340	,1379	,1417	,1452	,1485	,1518	,
	1547	,1575	,1602	,1627	,1650	,1673	,1694	,1714	,1732	,1750	,1767	,1783	,
	1797	,1811	,1825	,1837	,1849	,1860	,1871	,1881	,1890	,1899	,1907	,1915	,
	1923	,1930	,1936	,1943	,1949	,1954	,1959	,1964	,1969	,1974	,1978	,1982	,
	1985	,1989	,1992	,1995	,1998	,2001	,2004	,2006	,2009	,2011	,2013	,2015	,
	2017	,2018	,2020	,2022	,2023	,2025	,2026	,2027	,2028	,2029	,2030	,2031	,
	2032	,2033	,2034	,2035	,2036	,2037	,2038	,2039	,2040	,2041	,2042	,2043	,
	2044	,2045	,2046	,2047	,2047	,2047	,2047	,2047	,2047	,2047	,2047	,2047	,
	2047	,2047	,2047	,2047	,2047	,2047	,2047	,2047	,2047	,2047	,2047	
};

const u8 operators[8][16] = {
		/* SAWx1 	*/ { 0x8, 0x9, 0xA, 0xB, 0xC, 0xD, 0xE, 0xF, 0x0, 0x1, 0x2, 0x3, 0x4, 0x5, 0x6, 0x7 },
		/* SAWx2 	*/ { 0x9, 0xB, 0xD, 0xF, 0x0, 0x2, 0x4, 0x6, 0x8, 0xA, 0xC, 0xE, 0x1, 0x3, 0x5, 0x7 },
		/* SAWx4 	*/ { 0x9, 0xD, 0x2, 0xC, 0x9, 0xD, 0x1, 0x5, 0x9, 0xD, 0x2, 0xC, 0x9, 0xD, 0x1, 0x5 },
		/* SHARK 	*/ { 0x8, 0xB, 0xC, 0xD, 0xE, 0xE, 0xF, 0xF, 0x0, 0x0, 0x1, 0x1, 0x2, 0x3, 0x4, 0x6 },
		/* SINE  	*/ { 0xA, 0xC, 0xE, 0xF, 0xF, 0xE, 0xB, 0x9, 0x6, 0x3, 0x1, 0x0, 0x0, 0x1, 0x4, 0x7 },
		/* TRIANGLE */ { 0x8, 0xA, 0xC, 0xE, 0xF, 0xD, 0xB, 0x9, 0x7, 0x5, 0x3, 0x1, 0x0, 0x2, 0x4, 0x6 },
		/* TRIx2	*/ { 0x8, 0xC, 0xF, 0xB, 0x7, 0x3, 0x0, 0x4, 0x8, 0xC, 0xF, 0xB, 0x7, 0x3, 0x0, 0x4 },
		/* SQUARE	*/ { 0xC, 0xF, 0xF, 0xF, 0xF, 0xF, 0xF, 0xB, 0x4, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x3 },
};

bool 	Spu::retrig_note[6]		= { 0, 0, 0, 0, 0, 0 };
int 	Spu::targetTick[6] 		= { 0, 0, 0, 0, 0, 0 };
u8 		Spu::transpose[6] 		= { 0, 0, 0, 0, 0, 0 };
bool 	Spu::initialized 		= false;
bool 	Spu::reset_channel[6] 	= { false, false, false, false, false, false };
u8	 	Spu::tsp_position[6]	= { 0, 0, 0, 0, 0, 0 };/*16x4*//*126*/
u8	 	Spu::vol_position[6]	= { 0, 0, 0, 0, 0, 0 };/*16x4*//*190*/
u8 		Spu::wav_adsr_position 	= 0; // WAVE CHANNEL ADSR TABLES COMMON INDEX
u8 		Spu::fmw_adsr_position 	= 0; // FMW CHANNEL ADSR TABLES COMMON INDEX
u8 		Spu::smp_adsr_position 	= 0; // SMP CHANNEL ADSR TABLES COMMON INDEX
u8 		Spu::wav_adsr_table[4][0x40];
u8 		Spu::fmw_adsr_table[4][0x40];
u8 		Spu::smp_adsr_table[0x40];

bool swap_bank=0;

extern void cellSyncChannel(u8 c);
extern void adsr_view();

extern inline SETTINGS_PWM unpackPWM(INSTRUMENT *i);
extern inline SETTINGS_WAV unpackWAV(INSTRUMENT *i);
extern inline SETTINGS_SMP unpackSMP(INSTRUMENT *i);
extern inline SETTINGS_FMW unpackFMW(INSTRUMENT *i);

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
	/* ----------------------------------------------------------*/
	/* INITIALIZE SOUND CHIP 									 */
	/* ----------------------------------------------------------*/
	// Sound 1-4 ON Flag, PSG/FIFO Master enable
	*((volatile u16*)0x04000084) = 0x0080;
	
	// Sound 1-4 Master Right 100%, Sound 1-4 Master Left 100%, Sound 1-4 Enable Right, Sound 1-4 Enable Left
	*((volatile u16*)0x04000080) = 0xFF77;

	// Sound 1-4 Volume 100%, DMA A 100%, DMA B 100% 
	// We wont enable DMA sound by the moment
	*((volatile u16*)0x04000082) |= 0x0B0E;//PWM 100% DMA 100% (set volume)

	REG_SOUND3CNT_H = SOUND3OUTPUT1;
	
	/*-----------------------------------------------------------*/
	/* WORKAROUND TBF											 */
	/* ----------------------------------------------------------*/
	VAR_CFG.CURRENTINSTRUMENT = 0x1; // 0x0 is not accesible / usable
	/*----------------------------------------------------------*/
	
	/* ----------------------------------------------------------*/
	/* INITIALIZE DIRECTSOUND MIXER								 */
	/* ----------------------------------------------------------*/
	Mixer::init();
}
/*-------------------------------------------------------------------------*/
void Spu::setTempo(int bpm){
	timerTarget = (BPM_MAGIC / bpm)*10; 
	VAR_SONG.BPM = bpm;
	*((volatile u16*)0x0400010A) = 0x0; //Disable timer (bit 7)
	*((volatile u16*)0x04000108) = 0x0000; //'reset' timer counter
	*((volatile u16*)0x0400010A) = 0x82; //Enable (bit 7)		
}
/*-------------------------------------------------------------------------*/
void Spu::enablePWM1(void){ (*(vu16*)0x4000080)|= 0x1177; }
void Spu::enablePWM2(void){	(*(vu16*)0x4000080)|= 0x2277; }
void Spu::enableWAV( void){	(*(vu16*)0x4000080)|= 0x4477; }
void Spu::enableNZE( void){	(*(vu16*)0x4000080)|= 0x8877; }
void Spu::enableFMW(void){  }
void Spu::enableSMP(void){  }
void Spu::enable(){
	enablePWM1();
	enablePWM2();
	enableNZE();
	enableWAV();
	enableFMW();
	enableSMP();
}
/*-------------------------------------------------------------------------*/
void Spu::disablePWM1(){ (*(vu16*)0x4000080) &= ~0x1100; }
void Spu::disablePWM2(){ (*(vu16*)0x4000080) &= ~0x2200; }
void Spu::disableWAV() { (*(vu16*)0x4000080) &= ~0x4400; }
void Spu::disableNZE() { (*(vu16*)0x4000080) &= ~0x8800; }
void Spu::disableFMW(){
	#ifdef SMP_DMA
		*(u16*)0x4000102 /*REG_TM0CNT_H*/ =0; //disable timer 0
		*(u16*)0x40000C6 /*REG_DMA1CNT_H*/=0; //stop DMA1
		*(u16*)0x4000106 /*REG_TM1CNT_H*/ =0; //disable timer 1
		*(u16*)0x4000104 = 0;	//Reset Timer 2 count value (sample count)
		(*(vu16*)0x4000082)=0x0002; //disable dsound 1
	#endif
}
void Spu::disableSMP(void){

}
void Spu::disable(){
	disablePWM1();
	disablePWM2();
	disableNZE();
	disableWAV();
	disableFMW();
	disableSMP();
}
/*-------------------------------------------------------------------------*/
void Spu::stop(void){
	playing = false;
	disable();
	//if( VAR_CFG.BEHAVIOR.AUTOSAVE) SRAM.songSave();
}

void Spu::play(bool from_start){
	if(playing) return;
	
	enable();
	
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

/*-------------------------------------------------------------------------*/
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

/*-------------------------------------------------------------------------*/
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

/*-------------------------------------------------------------------------*/

void Spu::noteOnPWM1(){
	u16 freq 	= PWM_FREQ_TABLE[ VAR_CHANNEL[ 0 ].key 
				+ transpose[ 0 ] 
				+ ( VAR_CFG.TRACKER.TRANSPOSE - 0x80 ) 
				+ VAR_SONG.TRANSPOSE ] 
				+ ( VAR_CFG.TRACKER.FINETUNE << 1 );
	SOUND1CNT_H = 0x8000 | freq;
	retrig_note[ 0 ] = false;			
}

void Spu::noteOnPWM2(){
	u16 freq 	= PWM_FREQ_TABLE[ VAR_CHANNEL[ 1 ].key 
				+ transpose[ 1 ] 
				+ ( VAR_CFG.TRACKER.TRANSPOSE - 0x80 ) 
				+ VAR_SONG.TRANSPOSE ] 
				+ ( VAR_CFG.TRACKER.FINETUNE << 1 );
	SOUND2CNT_H = 0x8000 | freq;
	retrig_note[ 1 ] = false;			
}

void Spu::noteOnNZE(){
	u16 freq 	= PWM_FREQ_TABLE[ VAR_CHANNEL[ 2 ].key 
				+ transpose[ 2 ] 
				+ ( VAR_CFG.TRACKER.TRANSPOSE - 0x80 ) 
				+ VAR_SONG.TRANSPOSE ] 
				+ ( VAR_CFG.TRACKER.FINETUNE << 1 );
	SOUND4CNT_H = 0xc000 | freq;
	retrig_note[ 2 ] = false;
}

void Spu::noteOnWAV(){
	u16 freq 	= PWM_FREQ_TABLE[ VAR_CHANNEL[ 3 ].key 
				+ transpose[ 3 ] 
				+ ( VAR_CFG.TRACKER.TRANSPOSE - 0x80 ) 
				+ VAR_SONG.TRANSPOSE ] 
				+ ( VAR_CFG.TRACKER.FINETUNE << 1 );
	REG_SOUND3CNT_X = SOUND3INIT | SOUND3PLAYLOOP | freq; 
	//REG_SOUNDCNT_L = 0xFFff;			
	retrig_note[ 3 ] = false;
}

void Spu::noteOnFMW(){
	u16 freq 	= PWM_FREQ_TABLE[ VAR_CHANNEL[ 4 ].key 
				+ transpose[ 4 ] 
				+ ( VAR_CFG.TRACKER.TRANSPOSE - 0x80 ) 
				+ VAR_SONG.TRANSPOSE ] 
				+ ( VAR_CFG.TRACKER.FINETUNE << 1 );
	DECIMAL_DOUBLE_TWOTILES(0,2,0xFF,freq);
	Mixer::noteOn1( freq );
	retrig_note[ 4 ] = false;
}

void Spu::noteOnSMP(){
	u16 freq 	= PWM_FREQ_TABLE[ VAR_CHANNEL[ 5 ].key 
				+ transpose[ 5 ] 
				+ ( VAR_CFG.TRACKER.TRANSPOSE - 0x80 ) 
				+ VAR_SONG.TRANSPOSE ] 
				+ ( VAR_CFG.TRACKER.FINETUNE << 1 );
	Mixer::noteOn2( freq );
	retrig_note[ 5 ] = false;
}

/*-------------------------------------------------------------------------*/

void Spu::triggerChannel(int channel_index){
	
	u8 vol = VAR_CHANNEL[channel_index].volume;
	u8 ins = VAR_CHANNEL[channel_index].inst;
	
	// Fill correspondant VAR_XXX setting temporal struct
	INSTRUMENT *i = &VAR_INSTRUMENTS[ins];
	
	SETTINGS_PWM pwm;
	SETTINGS_WAV wav;
	SETTINGS_FMW fmw;
	SETTINGS_SMP smp;
	
	switch(channel_index){
		
		case 0:	// PULSE 1
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
				SOUND1CNT_X = ( pwm.SWEEPSTEPS << 4) | (pwm.SWEEPDIR << 3) | ( pwm.SWEEPSPEED );				
				SOUND1CNT_L = ((pwm.LEVEL*vol>>4) << 12) | (!pwm.ENVELOPEDIR << 11) | (pwm.VOLUMEFADE<<8) | ( pwm.DUTYCYCLE << 6 );				
				return noteOnPWM1();
			}
			return;
		
		case 1: // PULSE 2	
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
				SOUND2CNT_L = ((pwm.LEVEL*vol>>4) << 12) | (!pwm.ENVELOPEDIR << 11) | (pwm.VOLUMEFADE<<8) | ( pwm.DUTYCYCLE << 6 );				
				return noteOnPWM2();
			}
			return;
		
		case 2: // NOISE CHANNEL
			pwm = unpackPWM( i );
			if( reset_channel[ 2 ] ) {
				tsp_position[ 2 ] = 0;				
				vol_position[ 2 ] = 0;
				reset_channel[ 2 ] = false;				
				transpose[ 2 ] = 0;
			}
			if( pwm.TSP_ENABLE ){				
				if( tsp_position[ 2 ] < pwm.TSP_ENVELOPE ){
					transpose[ 2 ] = pwm.TSP[ tsp_position[ 2 ] ];
					tsp_position[ 2 ]++;
					retrig_note[ 2 ] = true;
				}
			}
			if( pwm.VOL_ENABLE ){				
				if( vol_position[ 2 ] < pwm.VOL_ENVELOPE ){
					vol = ( vol + pwm.VOL[ vol_position[ 2 ] ] ) >> 1;
					vol_position[ 2 ]++;
					retrig_note[ 2 ] = true;
				}
			}
			if( retrig_note[ 2 ] ){
				SOUND4CNT_L = ( ( pwm.LEVEL * vol >> 4 ) << 12 ) | ( !pwm.ENVELOPEDIR << 11 ) | ( pwm.VOLUMEFADE << 8 ) | ( pwm.DUTYCYCLE << 6 );				
				return noteOnNZE();
			}
			return;
			
		case 3: // WAVE CHANNEL	
			wav = unpackWAV( i );
			if( reset_channel[ 3 ] ) {
				tsp_position[ 3 ] 	= 0;				
				vol_position[ 3 ] 	= 0;
				reset_channel[ 3 ] 	= false;				
				transpose[ 3 ] 		= 0;
			}
			
			if( retrig_note[ 3 ] ){
				// Regenerate WAV ADSR Tables
				wav_adsr_position = 0;
				updateWavADSR( &wav );
			} 
			// Regenerate WAVE shape (scaled to channel volume)
			renderWavWAVE( &wav , vol );
			return noteOnWAV();
			
		case 4: // FM Wave CHANNEL
			fmw = unpackFMW( i );
			if( reset_channel[4] ){
				tsp_position[ 4 ]	= 0;
				vol_position[ 4 ] 	= 0;
				reset_channel[ 4 ]  = false;
				transpose[ 4 ]		= 0;
			}
			
			if( retrig_note[ 4 ] ){
				// Regenerate FM ADSR Tables
				fmw_adsr_position = 0;
				updateFmwADSR( &fmw );
			}
			// Regenerate FMW shape (scaled to channel volume)
			renderFmwWAVE( &fmw, vol );
			return noteOnFMW();
			
		case 5: // SMP Channel
			smp = unpackSMP( i );
			if( reset_channel[5] ){
				tsp_position[ 5 ]	= 0;
				vol_position[ 5 ] 	= 0;
				reset_channel[ 5 ]  = false;
				transpose[ 5 ]		= 0;
			}
			
			if( retrig_note[ 5 ] ){
				// Regenerate SMP ADSR Table
				smp_adsr_position = 0;
				updateSmpADSR( &smp );
			}
			// Regenerate FMW shape (scaled to channel volume)
			renderSmpWAVE( &smp, vol );
			return noteOnSMP();
	}
}

/*-------------------------------------------------------------------------*/

void Spu::updateWavADSR( SETTINGS_WAV *wav ){
	renderADSR( wav->OP1_ADSR, wav_adsr_table[0] );
	renderADSR( wav->OP2_ADSR, wav_adsr_table[1] );
	renderADSR( wav->OP3_ADSR, wav_adsr_table[2] );
	renderADSR( wav->OP4_ADSR, wav_adsr_table[3] );
	adsr_view();
}

void Spu::updateFmwADSR( SETTINGS_FMW *fmw ){
	renderADSR( fmw->OP1_ADSR, fmw_adsr_table[0] );
	renderADSR( fmw->OP2_ADSR, fmw_adsr_table[1] );
	renderADSR( fmw->OP3_ADSR, fmw_adsr_table[2] );
	renderADSR( fmw->OP4_ADSR, fmw_adsr_table[3] );
	adsr_view();
}

void Spu::updateSmpADSR( SETTINGS_SMP *smp ){
	renderADSR( smp->ADSR, smp_adsr_table );
	adsr_view();
}

void Spu::renderADSR( u8 adsr[ 4 ], u8 adsr_table[0x40] ){
	#define ATTACK 			adsr[0]
	#define DECAY  			adsr[1]
	#define SUSTAIN 		adsr[2]
	#define RELEASE 		adsr[3]
	#define TABLE			adsr_table[ position ]
	for( int position=0; position<0x40; position++){
		TABLE = 0;
	}
	
	u8 len = ATTACK + DECAY + SUSTAIN + RELEASE;;
		
	u8 quantum;
	u8 level = ATTACK > 0 ? 0 : 0xF;
	
	for( int position = 0, index = 0; position < len ; position++ ){
			
		if( position < ATTACK ){
			// Attack Phase
			quantum = (0xF<<2) / (ATTACK); // Attack will never be 0 here, so no zerodiv danger.
			index	= position+1;
			level 	= (quantum>>2) * index;		
		} else if(position < (ATTACK+DECAY)){
			// Decay Phase
			//index	= position - ATTACK;
			quantum = (level<<2) / (DECAY+SUSTAIN);
			level  -= (quantum>>2);
		} else if(position < (ATTACK+DECAY+SUSTAIN)){
			// Sustain Phase
			//index	= position - (ATTACK+DECAY);
			// Sustain..do nothing
		} else {
			// Release Phase
			//index	= position - (ATTACK+DECAY+SUSTAIN);
			quantum = ( ((SUSTAIN>0)?SUSTAIN:0xF)<<2) / RELEASE;
			if((quantum>>2) > level)level = 0;
			else level  -= (quantum>>2);
		}
		TABLE = level;
	}
	#undef ATTACK
	#undef DECAY
	#undef SUSTAIN
	#undef RELEASE
	#undef TABLE
}

/*-------------------------------------------------------------------------*/

void Spu::renderFmwWAVE( SETTINGS_FMW *fmw, u8 vol){
	static u8 operator_volume[4];
	
	// Get volume level for each of the 4 ADSR envelopes
	operator_volume[ 0 ] = (fmw_adsr_table[ 0 ][ fmw_adsr_position ] * vol)>>4;
	operator_volume[ 1 ] = (fmw_adsr_table[ 1 ][ fmw_adsr_position ] * vol)>>4;
	operator_volume[ 2 ] = (fmw_adsr_table[ 2 ][ fmw_adsr_position ] * vol)>>4;
	operator_volume[ 3 ] = (fmw_adsr_table[ 3 ][ fmw_adsr_position ] * vol)>>4;

	// Advance adsr table common index
	fmw_adsr_position = ( fmw_adsr_position < 0x3F) ? fmw_adsr_position+1 : 0x3F;
	
	#define OPERATOR1( a )	((u8)((u32)( operators[ fmw->OP1_TYPE ][ a ] * operator_volume[0] ) >> 4))
	#define OPERATOR2( a )	((u8)((u32)( operators[ fmw->OP2_TYPE ][ a ] * operator_volume[1] ) >> 4))
	#define OPERATOR3( a )	((u8)((u32)( operators[ fmw->OP3_TYPE ][ a ] * operator_volume[2] ) >> 4))
	#define OPERATOR4( a )	((u8)((u32)( operators[ fmw->OP4_TYPE ][ a ] * operator_volume[3] ) >> 4))
	
	// Mix shapes
	fmw->WAVEDATA[ 0] = (u8)((u32)( OPERATOR1( 0x0 ) + OPERATOR2( 0x0 ) + OPERATOR3( 0x0 ) + OPERATOR4( 0x0 ) ) >> 2);
	fmw->WAVEDATA[ 1] = (u8)((u32)( OPERATOR1( 0x1 ) + OPERATOR2( 0x1 ) + OPERATOR3( 0x1 ) + OPERATOR4( 0x1 ) ) >> 2);
	fmw->WAVEDATA[ 2] = (u8)((u32)( OPERATOR1( 0x2 ) + OPERATOR2( 0x2 ) + OPERATOR3( 0x2 ) + OPERATOR4( 0x2 ) ) >> 2);
	fmw->WAVEDATA[ 3] = (u8)((u32)( OPERATOR1( 0x3 ) + OPERATOR2( 0x3 ) + OPERATOR3( 0x3 ) + OPERATOR4( 0x3 ) ) >> 2);
	fmw->WAVEDATA[ 4] = (u8)((u32)( OPERATOR1( 0x4 ) + OPERATOR2( 0x4 ) + OPERATOR3( 0x4 ) + OPERATOR4( 0x4 ) ) >> 2);
	fmw->WAVEDATA[ 5] = (u8)((u32)( OPERATOR1( 0x5 ) + OPERATOR2( 0x5 ) + OPERATOR3( 0x5 ) + OPERATOR4( 0x5 ) ) >> 2);
	fmw->WAVEDATA[ 6] = (u8)((u32)( OPERATOR1( 0x6 ) + OPERATOR2( 0x6 ) + OPERATOR3( 0x6 ) + OPERATOR4( 0x6 ) ) >> 2);
	fmw->WAVEDATA[ 7] = (u8)((u32)( OPERATOR1( 0x7 ) + OPERATOR2( 0x7 ) + OPERATOR3( 0x7 ) + OPERATOR4( 0x7 ) ) >> 2);				
	fmw->WAVEDATA[ 8] = (u8)((u32)( OPERATOR1( 0x8 ) + OPERATOR2( 0x8 ) + OPERATOR3( 0x8 ) + OPERATOR4( 0x8 ) ) >> 2);
	fmw->WAVEDATA[ 9] = (u8)((u32)( OPERATOR1( 0x9 ) + OPERATOR2( 0x9 ) + OPERATOR3( 0x9 ) + OPERATOR4( 0x9 ) ) >> 2);
	fmw->WAVEDATA[10] = (u8)((u32)( OPERATOR1( 0xA ) + OPERATOR2( 0xA ) + OPERATOR3( 0xA ) + OPERATOR4( 0xA ) ) >> 2);
	fmw->WAVEDATA[11] = (u8)((u32)( OPERATOR1( 0xB ) + OPERATOR2( 0xB ) + OPERATOR3( 0xB ) + OPERATOR4( 0xB ) ) >> 2);
	fmw->WAVEDATA[12] = (u8)((u32)( OPERATOR1( 0xC ) + OPERATOR2( 0xC ) + OPERATOR3( 0xC ) + OPERATOR4( 0xC ) ) >> 2);
	fmw->WAVEDATA[13] = (u8)((u32)( OPERATOR1( 0xD ) + OPERATOR2( 0xD ) + OPERATOR3( 0xD ) + OPERATOR4( 0xD ) ) >> 2);
	fmw->WAVEDATA[14] = (u8)((u32)( OPERATOR1( 0xE ) + OPERATOR2( 0xE ) + OPERATOR3( 0xE ) + OPERATOR4( 0xE ) ) >> 2);
	fmw->WAVEDATA[15] = (u8)((u32)( OPERATOR1( 0xF ) + OPERATOR2( 0xF ) + OPERATOR3( 0xF ) + OPERATOR4( 0xF ) ) >> 2);
	
	#undef OPERATOR4
	#undef OPERATOR3
	#undef OPERATOR2
	#undef OPERATOR1
	
	loadFmwWAVEData( fmw->WAVEDATA );
}

void Spu::renderSmpWAVE( SETTINGS_SMP *smp, u8 vol){
	static u8 operator_volume;
	
	// Get volume level for each of the 4 ADSR envelopes
	operator_volume = ( smp_adsr_table[ smp_adsr_position ] * vol)>>4;

	// Advance adsr table common index
	smp_adsr_position = ( smp_adsr_position < 0x3F) ? smp_adsr_position+1 : 0x3F;
	
	#define OPERATOR( a )	((u8)((u32)( operators[ smp->OP1_TYPE ][ a ] * operator_volume ) >> 4))
	#undef OPERATOR
}

void Spu::renderWavWAVE( SETTINGS_WAV *wav, u8 vol){
	static u8 operator_volume[4];
	
	// Get volume level for each of the 4 ADSR envelopes
	operator_volume[ 0 ] = (wav_adsr_table[ 0 ][ wav_adsr_position ] * vol)>>4;
	operator_volume[ 1 ] = (wav_adsr_table[ 1 ][ wav_adsr_position ] * vol)>>4;
	operator_volume[ 2 ] = (wav_adsr_table[ 2 ][ wav_adsr_position ] * vol)>>4;
	operator_volume[ 3 ] = (wav_adsr_table[ 3 ][ wav_adsr_position ] * vol)>>4;

	// Advance adsr table common index
	wav_adsr_position = ( wav_adsr_position < 0x3F) ? wav_adsr_position+1 : 0x3F;
	
	#define OPERATOR1( a )	((u8)((u32)( operators[ wav->OP1_TYPE ][ a ] * operator_volume[0] ) >> 4))
	#define OPERATOR2( a )	((u8)((u32)( operators[ wav->OP2_TYPE ][ a ] * operator_volume[1] ) >> 4))
	#define OPERATOR3( a )	((u8)((u32)( operators[ wav->OP3_TYPE ][ a ] * operator_volume[2] ) >> 4))
	#define OPERATOR4( a )	((u8)((u32)( operators[ wav->OP4_TYPE ][ a ] * operator_volume[3] ) >> 4))
	
	// Mix shapes
	wav->WAVEDATA[ 0] = (u8)((u32)( OPERATOR1( 0x0 ) + OPERATOR2( 0x0 ) + OPERATOR3( 0x0 ) + OPERATOR4( 0x0 ) ) >> 2);
	wav->WAVEDATA[ 1] = (u8)((u32)( OPERATOR1( 0x1 ) + OPERATOR2( 0x1 ) + OPERATOR3( 0x1 ) + OPERATOR4( 0x1 ) ) >> 2);
	wav->WAVEDATA[ 2] = (u8)((u32)( OPERATOR1( 0x2 ) + OPERATOR2( 0x2 ) + OPERATOR3( 0x2 ) + OPERATOR4( 0x2 ) ) >> 2);
	wav->WAVEDATA[ 3] = (u8)((u32)( OPERATOR1( 0x3 ) + OPERATOR2( 0x3 ) + OPERATOR3( 0x3 ) + OPERATOR4( 0x3 ) ) >> 2);
	wav->WAVEDATA[ 4] = (u8)((u32)( OPERATOR1( 0x4 ) + OPERATOR2( 0x4 ) + OPERATOR3( 0x4 ) + OPERATOR4( 0x4 ) ) >> 2);
	wav->WAVEDATA[ 5] = (u8)((u32)( OPERATOR1( 0x5 ) + OPERATOR2( 0x5 ) + OPERATOR3( 0x5 ) + OPERATOR4( 0x5 ) ) >> 2);
	wav->WAVEDATA[ 6] = (u8)((u32)( OPERATOR1( 0x6 ) + OPERATOR2( 0x6 ) + OPERATOR3( 0x6 ) + OPERATOR4( 0x6 ) ) >> 2);
	wav->WAVEDATA[ 7] = (u8)((u32)( OPERATOR1( 0x7 ) + OPERATOR2( 0x7 ) + OPERATOR3( 0x7 ) + OPERATOR4( 0x7 ) ) >> 2);				
	wav->WAVEDATA[ 8] = (u8)((u32)( OPERATOR1( 0x8 ) + OPERATOR2( 0x8 ) + OPERATOR3( 0x8 ) + OPERATOR4( 0x8 ) ) >> 2);
	wav->WAVEDATA[ 9] = (u8)((u32)( OPERATOR1( 0x9 ) + OPERATOR2( 0x9 ) + OPERATOR3( 0x9 ) + OPERATOR4( 0x9 ) ) >> 2);
	wav->WAVEDATA[10] = (u8)((u32)( OPERATOR1( 0xA ) + OPERATOR2( 0xA ) + OPERATOR3( 0xA ) + OPERATOR4( 0xA ) ) >> 2);
	wav->WAVEDATA[11] = (u8)((u32)( OPERATOR1( 0xB ) + OPERATOR2( 0xB ) + OPERATOR3( 0xB ) + OPERATOR4( 0xB ) ) >> 2);
	wav->WAVEDATA[12] = (u8)((u32)( OPERATOR1( 0xC ) + OPERATOR2( 0xC ) + OPERATOR3( 0xC ) + OPERATOR4( 0xC ) ) >> 2);
	wav->WAVEDATA[13] = (u8)((u32)( OPERATOR1( 0xD ) + OPERATOR2( 0xD ) + OPERATOR3( 0xD ) + OPERATOR4( 0xD ) ) >> 2);
	wav->WAVEDATA[14] = (u8)((u32)( OPERATOR1( 0xE ) + OPERATOR2( 0xE ) + OPERATOR3( 0xE ) + OPERATOR4( 0xE ) ) >> 2);
	wav->WAVEDATA[15] = (u8)((u32)( OPERATOR1( 0xF ) + OPERATOR2( 0xF ) + OPERATOR3( 0xF ) + OPERATOR4( 0xF ) ) >> 2);
	
	#undef OPERATOR4
	#undef OPERATOR3
	#undef OPERATOR2
	#undef OPERATOR1
	
	loadWavWAVEData( wav->WAVEDATA );
}

/*-------------------------------------------------------------------------*/

void Spu::loadFmwWAVEData(u8 data[16] ){
}

void Spu::loadSmpWAVEData(u8 data[16] ){
}

void Spu::loadWavWAVEData(u8 data[16] ){
	//select bank 0 for writing (bank 1 playing)
	
	swap_bank ^=1; 
	REG_SOUND3CNT_L = SOUND3PLAY | SOUND3BANK32 | (swap_bank ? SOUND3SETBANK0 : SOUND3SETBANK1);
	
	//load the wave ram bank 0
	WAVE_RAM0_H = ( (data[0x1] ) << 12) 
				| ( (data[0x0] ) <<  8) 
				| ( (data[0x3] ) <<  4) 
				| ( (data[0x2] ) <<  0); 
	WAVE_RAM0_L = ( (data[0x5] ) << 12) 
				| ( (data[0x4] ) <<  8) 
				| ( (data[0x7] ) <<  4) 
				| ( (data[0x6] ) <<  0); 
	WAVE_RAM1_H = ( (data[0x9] ) << 12) 
				| ( (data[0x8] ) <<  8) 
				| ( (data[0xb] ) <<  4) 
				| ( (data[0xa] ) <<  0); 
	WAVE_RAM1_L = ( (data[0xd] ) << 12) 
				| ( (data[0xc] ) <<  8) 
				| ( (data[0xf] ) <<  4) 
				| ( (data[0xe] ) <<  0); 
	WAVE_RAM2_H = ( (data[0x1] ) << 12) 
				| ( (data[0x0] ) <<  8) 
				| ( (data[0x3] ) <<  4) 
				| ( (data[0x2] ) <<  0); 
	WAVE_RAM2_L = ( (data[0x5] ) << 12) 
				| ( (data[0x4] ) <<  8) 
				| ( (data[0x7] ) <<  4) 
				| ( (data[0x6] ) <<  0); 
	WAVE_RAM3_H = ( (data[0x9] ) << 12) 
				| ( (data[0x8] ) <<  8) 
				| ( (data[0xb] ) <<  4) 
				| ( (data[0xa] ) <<  0); 
	WAVE_RAM3_L = ( (data[0xd] ) << 12) 
				| ( (data[0xc] ) <<  8) 
				| ( (data[0xf] ) <<  4) 
				| ( (data[0xe] ) <<  0); 	
				
	//select bank 0 for playing
	REG_SOUND3CNT_L = SOUND3PLAY | SOUND3BANK32 | (swap_bank ? SOUND3SETBANK1 : SOUND3SETBANK0);
}

/*-------------------------------------------------------------------------*/

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
		Net::clear();
		if(!(currentTicks&0x3)){
			
			Net::update();
			
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

