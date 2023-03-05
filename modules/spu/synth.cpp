#include "synth.hpp"
#include "mixer.hpp"
#include "../../data/channel.hpp"
#include "../../data/config.hpp"
#include "../../data/song.hpp"
#include "../../data/helpers.hpp"
#include "../../data/settings.hpp"
#include "../../data/instrument.hpp"

extern void adsr_view();

static bool swap_bank = false;

u8 		Synth::wav_adsr_table[4][0x40];
u8 		Synth::fmw_adsr_table[4][0x40];
u8 		Synth::smp_adsr_table   [0x40];
u8 		Synth::wav_adsr_position = 0;
u8 		Synth::smp_adsr_position = 0;
u8 		Synth::fmw_adsr_position = 0;

const u16 SMP_FREQ_TABLE[120]={ 
	0xF8DD	,0xF917	,0xF94F ,0xF995	,0xF9D7	,0xFA22	,0xFA68	,0xFAB2	,0xFAF8	,0xFB3E	,0xFB80, 0xFBC0,
	0xFBFC	,0xFC36	,0xFC6C	,0xFCA0	,0xFCD0	,0xFCFE	,0xFD29	,0xFD52	,0xFD78	,0xFD9C	,0xFDBF	,0xFDDF	,
	0xFDFD	,0xFE1A	,0xFE35	,0xFE4F	,0xFE67	,0xFE7E	,0xFE94	,0xFEA8	,0xFEBB	,0xFECD	,0xFEDF	,0xFEEF	,
	0xFEFE	,0xFF0C	,0xFF1A	,0xFF27	,0xFF33	,0xFF3E	,0xFF49	,0xFF53	,0xFF5D	,0xFF66	,0xFF6F	,0xFF77	,
	0xFF7E	,0xFF85	,0xFF8C	,0xFF93	,0xFF99	,0xFF9E	,0xFFA4	,0xFFA9	,0xFFAE	,0xFFB2	,0xFFB7	,0xFFBB	,
	0xFFBE	,0xFFC2	,0xFFC5	,0xFFC9	,0xFFCC	,0xFFCE	,0xFFD1	,0xFFD4	,0xFFD6	,0xFFD8	,0xFFDB	,0xFFDD	,
	/**/
	0xFFDE	,0xFFE0	,0xFFE2	,0xFFE4	,0xFFE5	,0xFFE6	,0xFFE8	,0xFFE9	,0xFFEA	,0xFFEB	,0xFFED	,0xFFEE	,//INAUDIBLE OVER TOP
	0xFFEE	,0xFFEF	,0xFFF0	,0xFFF1	,0xFFF2	,0xFFF2	,0xFFF3	,0xFFF4	,0xFFF4	,0xFFF5	,0xFFF6	,0xFFF6	,//INAUDIBLE OVER TOP
	0xFFF6	,0xFFF7	,0xFFF7	,0xFFF8	,0xFFF8	,0xFFF8	,0xFFF9	,0xFFF9	,0xFFF9	,0xFFFA	,0xFFFA	,0xFFFA	,//INAUDIBLE OVER TOP
	0xFFFA	,0xFFFB	,0xFFFB	,0xFFFB	,0xFFFB	,0xFFFB	,0xFFFC	,0xFFFC	,0xFFFC	,0xFFFC	,0xFFFC	,0xFFFC	//INAUDIBLE OVER TOP
	//0xFFFE	,0xFFFE	,0xFFFE	,0xFFFE	,0xFFFE	,0xFFFE	,0xFFFE	,0xFFFE	,0xFFFE	,0xFFFE	,0xFFFE ,0xFFFE	//INAUDIBLE OVER TOP
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

void Synth::init(){
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

	// Bind channel specific update trigger callbacks
	for(int i=0; i<6; i++){
		Channel *c 		= &VAR_CHANNEL[i];
		c->transpose 	= 0;
		c->retrig	 	= false;
		c->reset	 	= false;
		c->fine_tune	= 0;
		
		switch(i){
			case 0: c->trigger = &Synth::triggerPwm1;break;
			case 1: c->trigger = &Synth::triggerPwm2;break;
			case 2: c->trigger = &Synth::triggerNze;break;
			case 3: c->trigger = &Synth::triggerWav;break;
			case 4: c->trigger = &Synth::triggerFmw;break;
			case 5: c->trigger = &Synth::triggerSmp;break;
		}
	}
}

void Synth::noteOnPwm1( Channel* channel ){
	u16 freq 	= PWM_FREQ_TABLE[ 
					channel->key 
					+ ( VAR_CFG.TRACKER.TRANSPOSE - 0x80 ) 
					+ channel->transpose 
					+ VAR_SONG.TRANSPOSE 
				  ] 
				+ channel->fine_tune
				+ ( VAR_CFG.TRACKER.FINETUNE << 1 );
	SOUND1CNT_H = 0x8000 | freq;
	channel->retrig = false;			
}

void Synth::noteOnPwm2( Channel* channel ){
	u16 freq 	= PWM_FREQ_TABLE[ 
					channel->key 
					+ channel->transpose 
					+ ( VAR_CFG.TRACKER.TRANSPOSE - 0x80 ) 
					+ VAR_SONG.TRANSPOSE 
				  ] 
				+ channel->fine_tune
				+ ( VAR_CFG.TRACKER.FINETUNE << 1 );
	SOUND2CNT_H = 0x8000 | freq;
	channel->retrig = false;			
}

void Synth::noteOnNze( Channel* channel ){
	u16 freq 	= PWM_FREQ_TABLE[ 
					channel->key 
					+ channel->transpose 
					+ ( VAR_CFG.TRACKER.TRANSPOSE - 0x80 ) 
					+ VAR_SONG.TRANSPOSE 
				  ] 
				+ channel->fine_tune
				+ ( VAR_CFG.TRACKER.FINETUNE << 1 );
	SOUND4CNT_H = 0xc000 | freq;
	channel->retrig = false;
}

void Synth::noteOnWav( Channel* channel ){
	u16 freq 	= PWM_FREQ_TABLE[ 
					channel->key 
					+ channel->transpose 
					+ ( VAR_CFG.TRACKER.TRANSPOSE - 0x80 ) 
					+ VAR_SONG.TRANSPOSE 
				  ]	
				+ channel->fine_tune
				+ ( VAR_CFG.TRACKER.FINETUNE << 1 );
	REG_SOUND3CNT_X = SOUND3INIT | SOUND3PLAYLOOP | freq; 
	//REG_SOUNDCNT_L = 0xFFff;			
	channel->retrig = false;
}

void Synth::noteOnFmw( Channel* channel ){
	u16 freq 	= PWM_FREQ_TABLE[ 
					channel->key 
					+ channel->transpose 
					+ ( VAR_CFG.TRACKER.TRANSPOSE - 0x80 ) 
					+ VAR_SONG.TRANSPOSE 
				  ] 
				+ channel->fine_tune 
				+ ( VAR_CFG.TRACKER.FINETUNE << 1 );
	DECIMAL_DOUBLE_TWOTILES(0,2,0xFF,freq);
	Mixer::noteOn1( freq );
	channel->retrig = false;
}

void Synth::noteOnSmp( Channel* channel ){
	u16 freq 	= PWM_FREQ_TABLE[ 
					channel->key 
					+ channel->transpose 
					+ ( VAR_CFG.TRACKER.TRANSPOSE - 0x80 ) 
					+ VAR_SONG.TRANSPOSE 
				  ] 
				+ channel->fine_tune
				+ ( VAR_CFG.TRACKER.FINETUNE << 1 );
	Mixer::noteOn2( freq );
	channel->retrig = false;
}

/*###########################################################################*/

void Synth::renderSmp( SETTINGS_SMP *smp, u8 vol){
	static u8 operator_volume;
	
	// Get volume level for each of the 4 ADSR envelopes
	operator_volume = ( smp_adsr_table[ smp_adsr_position ] * vol)>>4;

	// Advance adsr table common index
	smp_adsr_position = ( smp_adsr_position < 0x3F) ? smp_adsr_position+1 : 0x3F;
	
	#define OPERATOR( a )	((u8)((u32)( operators[ smp->OP1_TYPE ][ a ] * operator_volume ) >> 4))
	#undef OPERATOR
	//loadSmp( smp->WAVEDATA );
}
	
void Synth::renderWav( SETTINGS_WAV *wav, u8 vol){
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
	
	loadWav( wav->WAVEDATA );
}

void Synth::renderFmw( SETTINGS_FMW *fmw, u8 vol){
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
	
	loadFmw( fmw->WAVEDATA );
}

/*###########################################################################*/

void Synth::loadFmw( u8 data[16] ){
	// TODO
}

void Synth::loadSmp( u8 data[16] ){
	// TODO
}

void Synth::loadWav( u8 data[16] ){
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

/*###########################################################################*/

void Synth::updateADSRWav( SETTINGS_WAV *wav ){
	renderADSR( wav->OP1_ADSR, wav_adsr_table[0] );
	renderADSR( wav->OP2_ADSR, wav_adsr_table[1] );
	renderADSR( wav->OP3_ADSR, wav_adsr_table[2] );
	renderADSR( wav->OP4_ADSR, wav_adsr_table[3] );
	adsr_view();
}

void Synth::updateADSRFmw( SETTINGS_FMW *fmw ){
	renderADSR( fmw->OP1_ADSR, fmw_adsr_table[0] );
	renderADSR( fmw->OP2_ADSR, fmw_adsr_table[1] );
	renderADSR( fmw->OP3_ADSR, fmw_adsr_table[2] );
	renderADSR( fmw->OP4_ADSR, fmw_adsr_table[3] );
	adsr_view();
}

void Synth::updateADSRSmp( SETTINGS_SMP *smp ){
	renderADSR( smp->ADSR, smp_adsr_table );
	adsr_view();
}

void Synth::renderADSR( u8 adsr[ 4 ], u8 adsr_table[0x40] ){
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

extern inline SETTINGS_PWM unpackPWM(Instrument *i);
extern inline SETTINGS_WAV unpackWAV(Instrument *i);
extern inline SETTINGS_SMP unpackSMP(Instrument *i);
extern inline SETTINGS_FMW unpackFMW(Instrument *i);

void Synth::triggerPwm1( Channel *channel ){
	u8 vol 			 = channel->volume;
	Instrument *i 	 = &VAR_INSTRUMENTS[ channel->inst ];
	SETTINGS_PWM pwm = unpackPWM( i );
	
	if( channel->reset ) {
		channel->tsp_position 	= 0;				
		channel->vol_position 	= 0;
		channel->reset 			= false;				
		channel->transpose		= 0;
	}
	if( pwm.TSP_ENABLE ){				
		if( channel->tsp_position < pwm.TSP_ENVELOPE ){
			channel->transpose 	= pwm.TSP[ channel->tsp_position ];
			channel->retrig 	= true;
			channel->tsp_position++;
		}
	}
	if( pwm.VOL_ENABLE ){
		if( channel->vol_position < pwm.VOL_ENVELOPE){
			vol 				= ( vol + pwm.VOL[ channel->vol_position ] ) >> 1;
			channel->retrig 	= true;
			channel->vol_position++;
		}
	}
	if( channel->retrig ){
		SOUND1CNT_X = ( pwm.SWEEPSTEPS << 4) | (pwm.SWEEPDIR << 3) | ( pwm.SWEEPSPEED );				
		SOUND1CNT_L = ((pwm.LEVEL*vol>>4) << 12) | (!pwm.ENVELOPEDIR << 11) | (pwm.VOLUMEFADE<<8) | ( pwm.DUTYCYCLE << 6 );				
		noteOnPwm1( channel );
	}
}

void Synth::triggerPwm2( Channel *channel ){
	u8 vol 			 = channel->volume;
	Instrument *i 	 = &VAR_INSTRUMENTS[ channel->inst ];
	SETTINGS_PWM pwm = unpackPWM( i );
	
	if( channel->reset ) {
		channel->tsp_position 	= 0;				
		channel->vol_position 	= 0;
		channel->reset 			= false;				
		channel->transpose		= 0;
	}
	if( pwm.TSP_ENABLE ){				
		if( channel->tsp_position < pwm.TSP_ENVELOPE ){
			channel->transpose 	= pwm.TSP[ channel->tsp_position ];
			channel->retrig 	= true;
			channel->tsp_position++;
		}
	}
	if( pwm.VOL_ENABLE ){
		if( channel->vol_position < pwm.VOL_ENVELOPE){
			vol 				= ( vol + pwm.VOL[ channel->vol_position ] ) >> 1;
			channel->retrig 	= true;
			channel->vol_position++;
		}
	}
	if( channel->retrig ){
		SOUND2CNT_L = ((pwm.LEVEL*vol>>4) << 12) | (!pwm.ENVELOPEDIR << 11) | (pwm.VOLUMEFADE<<8) | ( pwm.DUTYCYCLE << 6 );				
		noteOnPwm2( channel );
	}
}

void Synth::triggerNze( Channel *channel ){
	u8 vol 		 	 = channel->volume;
	Instrument *i 	 = &VAR_INSTRUMENTS[ channel->inst ];
	SETTINGS_PWM pwm = unpackPWM( i );
	
	if( channel->reset ) {
		channel->tsp_position 	= 0;				
		channel->vol_position 	= 0;
		channel->reset 			= false;				
		channel->transpose		= 0;
	}
	if( pwm.TSP_ENABLE ){				
		if( channel->tsp_position < pwm.TSP_ENVELOPE ){
			channel->transpose 	= pwm.TSP[ channel->tsp_position ];
			channel->retrig 	= true;
			channel->tsp_position++;
		}
	}
	if( pwm.VOL_ENABLE ){
		if( channel->vol_position < pwm.VOL_ENVELOPE){
			vol 				= ( vol + pwm.VOL[ channel->vol_position ] ) >> 1;
			channel->retrig 	= true;
			channel->vol_position++;
		}
	}
	if( channel->retrig ){
		SOUND4CNT_L = ( ( pwm.LEVEL * vol >> 4 ) << 12 ) | ( !pwm.ENVELOPEDIR << 11 ) | ( pwm.VOLUMEFADE << 8 ) | ( pwm.DUTYCYCLE << 6 );				
		noteOnNze( channel );
	}
}

void Synth::triggerWav( Channel* channel ){
	u8 vol 			 = channel->volume;
	Instrument *i 	 = &VAR_INSTRUMENTS[ channel->inst ];
	SETTINGS_WAV wav = unpackWAV( i );
	
	if( channel->reset ) {
		channel->tsp_position 	= 0;				
		channel->vol_position 	= 0;
		channel->transpose 		= 0;
		channel->reset	 		= false;				
	}
	
	if( channel->retrig ){
		// Regenerate WAV ADSR Tables
		wav_adsr_position 		= 0;
		updateADSRWav( &wav );
	} 
	// Regenerate WAVE shape (scaled to channel volume)
	renderWav( &wav , vol );
	noteOnWav( channel );
}

void Synth::triggerFmw( Channel* channel ){
	u8 vol 			 = channel->volume;
	Instrument *i 	 = &VAR_INSTRUMENTS[ channel->inst ];
	SETTINGS_FMW fmw = unpackFMW( i );
	
	if( channel->reset ){
		channel->tsp_position	= 0;
		channel->vol_position	= 0;
		channel->transpose		= 0;
		channel->reset			= false;
	}
	
	if( channel->retrig ){
		// Regenerate FM ADSR Tables
		fmw_adsr_position 		= 0;
		updateADSRFmw( &fmw );
	}
	// Regenerate FMW shape (scaled to channel volume)
	renderFmw( &fmw, vol );
	noteOnFmw( channel );
}

void Synth::triggerSmp( Channel* channel ){
	u8 vol 			 = channel->volume;
	Instrument *i 	 = &VAR_INSTRUMENTS[ channel->inst ];
	SETTINGS_SMP smp = unpackSMP( i );
	
	if( channel->reset ){
		channel->tsp_position	= 0;
		channel->vol_position	= 0;
		channel->reset			= false;
		channel->transpose		= 0;
	}
	
	if( channel->retrig ){
		// Regenerate SMP ADSR Table
		smp_adsr_position 		= 0;
		updateADSRSmp( &smp );
	}
	// Regenerate FMW shape (scaled to channel volume)
	renderSmp( &smp, vol );
	noteOnSmp( channel );
}
/*###########################################################################*/
