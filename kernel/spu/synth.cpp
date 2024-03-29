#include "synth.hpp"
#include "mixer.hpp"
#include "../../debug.hpp"
#include "../../data/channel.hpp"
#include "../../data/config.hpp"
#include "../../data/song.hpp"
#include "../../data/enum.h"
#include "../../data/settings.hpp"
#include "../../data/instrument.hpp"
#include "../../data/instrument.hpp"
#include "../../screens/instedit.hpp"
#include "../../kernel/gpu/gpu.hpp"

bool swap_bank = false;

u16		Synth::lfo					= 0;
u8		Synth::loop_smp				= 0;
u32		Synth::smp_start			= 0;
u32		Synth::smp_end				= 0;

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
	Channel *channel = VAR_CHANNEL;
	Channel *target = channel + CHANNEL_COUNT;
	for(int i=0; channel<target; i++, channel++ ){
		channel->init( i );
	}

	lfo = 0;
}

void Synth::noteOffPwm1( Channel* channel ){
}
void Synth::noteOffPwm2( Channel* channel ){
}
void Synth::noteOffNze ( Channel* channel ){
}
void Synth::noteOffWav ( Channel* channel ){
}
void Synth::noteOffFmw ( Channel* channel ){
}
void Synth::noteOffSmp ( Channel* channel ){
}


void Synth::noteOnPwm1( Channel* channel ){
	u8 key 	= channel->key 
			+ channel->transpose 
			+ channel->song_patterns->TRANSPOSE[ channel->POSITION ] 
			+ VAR_SONG.TRANSPOSE 
			+ ( VAR_CFG.TRACKER.TRANSPOSE - 0x80 );
				  
	u16 frq = PWM_FREQ_TABLE[ key ]  
			+ channel->fine_tune
			+ ( VAR_CFG.TRACKER.FINETUNE << 1 );
			
	SOUND1CNT_H = 0x8000 | frq;	
	channel->retrig = false;			
}

void Synth::noteOnPwm2( Channel* channel ){
	u8 key 	= channel->key 
			+ channel->transpose 
			+ channel->song_patterns->TRANSPOSE[ channel->POSITION ] 
			+ VAR_SONG.TRANSPOSE
			+ ( VAR_CFG.TRACKER.TRANSPOSE - 0x80 );
			
	u16 frq = PWM_FREQ_TABLE[ key ] 
			+ channel->fine_tune
			+ ( VAR_CFG.TRACKER.FINETUNE << 1 );
			
	SOUND2CNT_H = 0x8000 | frq;
	channel->retrig = false;			
}

void Synth::noteOnNze( Channel* channel ){
	u8 key	= channel->key 
			+ channel->transpose
			+ channel->song_patterns->TRANSPOSE[ channel->POSITION ] 
			+ VAR_SONG.TRANSPOSE 
			+ ( VAR_CFG.TRACKER.TRANSPOSE - 0x80 );
		  
	u16 frq	= PWM_FREQ_TABLE[ key ]
			+ channel->fine_tune
			+ ( VAR_CFG.TRACKER.FINETUNE << 1 );
	
	SOUND4CNT_H = 0xc000 | frq;
	channel->retrig = false;
}

#include "../tim/tim.hpp"
extern Timer timer1;

void Synth::noteOnWav( Channel* channel ){
	u8 key	= channel->key 
			+ channel->transpose
			+ channel->song_patterns->TRANSPOSE[ channel->POSITION ] 
			+ VAR_SONG.TRANSPOSE 
			+ ( VAR_CFG.TRACKER.TRANSPOSE - 0x80 );
		
	u16 frq = PWM_FREQ_TABLE[ key ]	
			+ channel->fine_tune
			+ ( VAR_CFG.TRACKER.FINETUNE << 1 );
	
	static u16 lastfrq = 0;
	if(lastfrq != frq){
		REG_SOUND3CNT_H &= 0x7000;
		REG_SOUND3CNT_X = SOUND3INIT | SOUND3PLAYLOOP | frq;
		lastfrq = frq;
		timer1.setup(0xFFFF-frq, 1);
		timer1.enable();
	
		swap_bank ^=1;
	}
	//REG_SOUNDCNT_L = 0xFFff;			
	channel->retrig = false;
}

void Synth::noteOnFmw( Channel* channel ){
	u8 key	= channel->key 
			+ channel->transpose
			+ channel->song_patterns->TRANSPOSE[ channel->POSITION ] 
			+ VAR_SONG.TRANSPOSE 
			+ ( VAR_CFG.TRACKER.TRANSPOSE - 0x80 );
	key &= 0x7f;
	u32 frq	= DSOUND_FREQ_TABLE[ key ]
			+ channel->fine_tune 
			+ ( VAR_CFG.TRACKER.FINETUNE << 1 );
		
	Mixer::noteOn1( frq );
	channel->retrig = false;
}

void Synth::noteOnSmp( Channel* channel ){
	u8 key	= channel->key 
			+ channel->transpose
			+ channel->song_patterns->TRANSPOSE[ channel->POSITION ] 
			+ VAR_SONG.TRANSPOSE 
			+ ( VAR_CFG.TRACKER.TRANSPOSE - 0x80 );
	key &= 0x7f;
	u32 frq	= DSOUND_FREQ_TABLE[ key ] 
			+ channel->fine_tune
			+ ( VAR_CFG.TRACKER.FINETUNE << 1 );
	
	Mixer::noteOn2( frq );
	channel->retrig = false;
}

/*###########################################################################*/

void Synth::renderSmp( SETTINGS_SMP *smp, u8 vol){
	static u8 operator_volume;
	
	// Get volume level for each of the 4 ADSR envelopes
	operator_volume = ( ( Adsr::smp_table[ SMP_ADSR_POSITION] >> 4) * vol)>>4;

	// Advance adsr table common index
	Adsr::smp_position	= ( Adsr::smp_position < ( ( ADSR_TABLE_LENGTH - 1 ) << 1 ) ) 
							?	Adsr::smp_position + 1 
							: ( ADSR_TABLE_LENGTH - 1 );
	
	#define OPERATOR( a )	((u8)((u32)( operators[ smp->OP1_TYPE ][ a ] * operator_volume ) >> 4))
	#undef OPERATOR
	//loadSmp( smp->WAVEDATA );
}
	
void Synth::renderWav( SETTINGS_WAV *wav, u8 vol){
	static u8 operator_volume[4];
	
	// Get volume level for each of the 4 ADSR envelopes
	operator_volume[ 0 ] = (( Adsr::wav_table[ 0 ][ WAV_ADSR_POSITION ] >> 4 ) * vol)>>4;
	operator_volume[ 1 ] = (( Adsr::wav_table[ 1 ][ WAV_ADSR_POSITION ] >> 4 ) * vol)>>4;
	operator_volume[ 2 ] = (( Adsr::wav_table[ 2 ][ WAV_ADSR_POSITION ] >> 4 ) * vol)>>4;
	operator_volume[ 3 ] = (( Adsr::wav_table[ 3 ][ WAV_ADSR_POSITION ] >> 4 ) * vol)>>4;

	#ifndef DISTORTION
	operator_volume[ 0 ] = operator_volume[ 0 ] < 0xF ? operator_volume[ 0 ] : 0xF;
	operator_volume[ 1 ] = operator_volume[ 1 ] < 0xF ? operator_volume[ 1 ] : 0xF;
	operator_volume[ 2 ] = operator_volume[ 2 ] < 0xF ? operator_volume[ 2 ] : 0xF;
	operator_volume[ 3 ] = operator_volume[ 3 ] < 0xF ? operator_volume[ 3 ] : 0xF;
	#endif 
	
	// Advance adsr table common index
	
	Adsr::wav_position	= ( Adsr::wav_position < ( ( ADSR_TABLE_LENGTH - 1 )  ) ) 
							?	Adsr::wav_position + 1 
							: ( ADSR_TABLE_LENGTH - 1 );
	
	
	
	// Mix shapes
	u8 *p = wav->WAVEDATA;
	const u8 *op1 = &operators[ wav->OP1_TYPE ][0];
	const u8 *op2 = &operators[ wav->OP2_TYPE ][0];
	const u8 *op3 = &operators[ wav->OP3_TYPE ][0];
	const u8 *op4 = &operators[ wav->OP4_TYPE ][0];
	const u8 *opvol1 = &operator_volume[0];
	const u8 *opvol2 = &operator_volume[1];
	const u8 *opvol3 = &operator_volume[2];
	const u8 *opvol4 = &operator_volume[3];
	for(u8 *end = p + 16;p<end;p++,op1++,op2++,op3++,op4++){
		*p  =(((*op1 * *opvol1)>>4) 
			+ ((*op2 * *opvol2)>>4) 
			+ ((*op3 * *opvol3)>>4) 
			+ ((*op4 * *opvol4)>>4) 
			) >> 2;
	}
	
	loadWav( wav->WAVEDATA );
}

void Synth::renderFmw( SETTINGS_FMW *fmw, u8 vol){
	static u8 operator_volume[4];
	
	// Get volume level for each of the 4 ADSR envelopes
	operator_volume[ 0 ] = ( ( Adsr::fmw_table[ 0 ][ FMW_ADSR_POSITION ] >> 4 ) * vol ) >> 4;
	operator_volume[ 1 ] = ( ( Adsr::fmw_table[ 1 ][ FMW_ADSR_POSITION ] >> 4 ) * vol ) >> 4;
	operator_volume[ 2 ] = ( ( Adsr::fmw_table[ 2 ][ FMW_ADSR_POSITION ] >> 4 ) * vol ) >> 4;
	operator_volume[ 3 ] = ( ( Adsr::fmw_table[ 3 ][ FMW_ADSR_POSITION ] >> 4 ) * vol ) >> 4;

	
	/*
	// Normalize operator volumes
	u8 output[4] = {
		operator_volume[ 0 ] 
		+ (0xF - operator_volume[1]) 
		+ (0xF - operator_volume[2]) 
		+ (0xF - operator_volume[3]),
		operator_volume[ 1 ] 
		+ (0xF - operator_volume[0]) 
		+ (0xF - operator_volume[2]) 
		+ (0xF - operator_volume[3]),
		operator_volume[ 2 ] 
		+ (0xF - operator_volume[0]) 
		+ (0xF - operator_volume[1]) 
		+ (0xF - operator_volume[3]),
		operator_volume[ 3 ] 
		+ (0xF - operator_volume[0]) 
		+ (0xF - operator_volume[1]) 
		+ (0xF - operator_volume[2])
	};
	// Overwrite with normalized values
	operator_volume[ 0 ] = output[ 0 ] >> 2;
	operator_volume[ 1 ] = output[ 1 ] >> 2;
	operator_volume[ 2 ] = output[ 2 ] >> 2;
	operator_volume[ 3 ] = output[ 3 ] >> 2;
	#ifndef DISTORTION
	operator_volume[ 0 ] = operator_volume[ 0 ] > 0xF ? operator_volume[ 0 ] : 0xF;
	operator_volume[ 1 ] = operator_volume[ 1 ] > 0xF ? operator_volume[ 1 ] : 0xF;
	operator_volume[ 2 ] = operator_volume[ 2 ] > 0xF ? operator_volume[ 2 ] : 0xF;
	operator_volume[ 3 ] = operator_volume[ 3 ] > 0xF ? operator_volume[ 3 ] : 0xF;
	#endif 
	*/
	
	
	// Advance adsr table common index
	Adsr::fmw_position	= ( Adsr::fmw_position < ( ( ADSR_TABLE_LENGTH - 1 ) << 1 ) ) 
							?	Adsr::fmw_position + 1 
							: ( ADSR_TABLE_LENGTH - 1 );
	
	
	u8 FB[16];
	/*
	
	[ ] -> operator index modulator
	{ } -> operator composing output waveform
	< > -> operator with feedback
	*/
	
	switch( fmw->ALGORITHM ){
		/* ALGORITHM 0: 
		
		         /{2}\
		<[1]> -->-{3}->-> OUT
		  	     \{4}/
				 
		*/	case 0:
			#define O1( a )	((u8)((u32)( operators[ fmw->OP1_TYPE ][ ( a + FB[a] ) & 0xF ] * operator_volume[0] ) >> 4))
			#define O2( a )	((u8)((u32)( operators[ fmw->OP2_TYPE ][ ( a + O1(a) ) & 0xF ] * operator_volume[1] ) >> 4))
			#define O3( a )	((u8)((u32)( operators[ fmw->OP3_TYPE ][ ( a + O1(a) ) & 0xF ] * operator_volume[2] ) >> 4))
			#define O4( a )	((u8)((u32)( operators[ fmw->OP4_TYPE ][ ( a + O1(a) ) & 0xF ] * operator_volume[3] ) >> 4))
			fmw->WAVEDATA[ 0] = (u8)((u32)( O2( 0x0 ) + O3( 0x0 ) + O4( 0x0 ) ) / 3);
			fmw->WAVEDATA[ 1] = (u8)((u32)( O2( 0x1 ) + O3( 0x1 ) + O4( 0x1 ) ) / 3);
			fmw->WAVEDATA[ 2] = (u8)((u32)( O2( 0x2 ) + O3( 0x2 ) + O4( 0x2 ) ) / 3);
			fmw->WAVEDATA[ 3] = (u8)((u32)( O2( 0x3 ) + O3( 0x3 ) + O4( 0x3 ) ) / 3);
			fmw->WAVEDATA[ 4] = (u8)((u32)( O2( 0x4 ) + O3( 0x4 ) + O4( 0x4 ) ) / 3);
			fmw->WAVEDATA[ 5] = (u8)((u32)( O2( 0x5 ) + O3( 0x5 ) + O4( 0x5 ) ) / 3);
			fmw->WAVEDATA[ 6] = (u8)((u32)( O2( 0x6 ) + O3( 0x6 ) + O4( 0x6 ) ) / 3);
			fmw->WAVEDATA[ 7] = (u8)((u32)( O2( 0x7 ) + O3( 0x7 ) + O4( 0x7 ) ) / 3);
			fmw->WAVEDATA[ 8] = (u8)((u32)( O2( 0x8 ) + O3( 0x8 ) + O4( 0x8 ) ) / 3);
			fmw->WAVEDATA[ 9] = (u8)((u32)( O2( 0x9 ) + O3( 0x9 ) + O4( 0x9 ) ) / 3);
			fmw->WAVEDATA[10] = (u8)((u32)( O2( 0xA ) + O3( 0xA ) + O4( 0xA ) ) / 3);
			fmw->WAVEDATA[11] = (u8)((u32)( O2( 0xB ) + O3( 0xB ) + O4( 0xB ) ) / 3);
			fmw->WAVEDATA[12] = (u8)((u32)( O2( 0xC ) + O3( 0xC ) + O4( 0xC ) ) / 3);
			fmw->WAVEDATA[13] = (u8)((u32)( O2( 0xD ) + O3( 0xD ) + O4( 0xD ) ) / 3);
			fmw->WAVEDATA[14] = (u8)((u32)( O2( 0xE ) + O3( 0xE ) + O4( 0xE ) ) / 3);
			fmw->WAVEDATA[15] = (u8)((u32)( O2( 0xF ) + O3( 0xF ) + O4( 0xF ) ) / 3);
			#undef O1
			#undef O2
			#undef O3
			#undef O4
			break;
		/* ALGORITHM 1:
		
				    [1]
		 	         V						 
		<[3]>->[2]->{4}--> OUT
					  
		*/	case 1:
			#define O1( a )	((u8)((u32)( operators[ fmw->OP1_TYPE ][ ( a 		 			) & 0xF ] * operator_volume[0] ) >> 4))
			#define O3( a )	((u8)((u32)( operators[ fmw->OP3_TYPE ][ ( a + FB[a] 			) & 0xF ] * operator_volume[2] ) >> 4))
			#define O2( a )	((u8)((u32)( operators[ fmw->OP2_TYPE ][ ( a + O3(a) 			) & 0xF ] * operator_volume[1] ) >> 4))
			#define O4( a )	((u8)((u32)( operators[ fmw->OP4_TYPE ][ ( a + O2(a) + O1(a) 	) & 0xF ] * operator_volume[3] ) >> 4))
			fmw->WAVEDATA[ 0] = (u8)((u32)( O4( 0x0 ) ));
			fmw->WAVEDATA[ 1] = (u8)((u32)( O4( 0x1 ) ));
			fmw->WAVEDATA[ 2] = (u8)((u32)( O4( 0x2 ) ));
			fmw->WAVEDATA[ 3] = (u8)((u32)( O4( 0x3 ) ));
			fmw->WAVEDATA[ 4] = (u8)((u32)( O4( 0x4 ) ));
			fmw->WAVEDATA[ 5] = (u8)((u32)( O4( 0x5 ) ));
			fmw->WAVEDATA[ 6] = (u8)((u32)( O4( 0x6 ) ));
			fmw->WAVEDATA[ 7] = (u8)((u32)( O4( 0x7 ) ));
			fmw->WAVEDATA[ 8] = (u8)((u32)( O4( 0x8 ) ));
			fmw->WAVEDATA[ 9] = (u8)((u32)( O4( 0x9 ) ));
			fmw->WAVEDATA[10] = (u8)((u32)( O4( 0xA ) ));
			fmw->WAVEDATA[11] = (u8)((u32)( O4( 0xB ) ));
			fmw->WAVEDATA[12] = (u8)((u32)( O4( 0xC ) ));
			fmw->WAVEDATA[13] = (u8)((u32)( O4( 0xD ) ));
			fmw->WAVEDATA[14] = (u8)((u32)( O4( 0xE ) ));
			fmw->WAVEDATA[15] = (u8)((u32)( O4( 0xF ) ));
			#undef O1
			#undef O2
			#undef O3
			#undef O4
			break;
		/* ALGORITHM 2:
		
			<[1]>->{3}	   
					  >--> OUT
			 [2]-->{4}
			 
		*/	case 2:
			#define O1( a )	((u8)((u32)( operators[ fmw->OP1_TYPE ][ ( a + FB[a] 	) & 0xF ] * operator_volume[0] ) >> 4))
			#define O2( a )	((u8)((u32)( operators[ fmw->OP2_TYPE ][ ( a 			) & 0xF ] * operator_volume[1] ) >> 4))
			#define O3( a )	((u8)((u32)( operators[ fmw->OP3_TYPE ][ ( a + O1(a) 	) & 0xF ] * operator_volume[2] ) >> 4))
			#define O4( a )	((u8)((u32)( operators[ fmw->OP4_TYPE ][ ( a + O2(a) 	) & 0xF ] * operator_volume[3] ) >> 4))
			fmw->WAVEDATA[ 0] = (u8)((u32)( O3( 0x0 ) + O4( 0x0 ) ) >> 1 );
			fmw->WAVEDATA[ 1] = (u8)((u32)( O3( 0x1 ) + O4( 0x1 ) ) >> 1 );
			fmw->WAVEDATA[ 2] = (u8)((u32)( O3( 0x2 ) + O4( 0x2 ) ) >> 1 );
			fmw->WAVEDATA[ 3] = (u8)((u32)( O3( 0x3 ) + O4( 0x3 ) ) >> 1 );
			fmw->WAVEDATA[ 4] = (u8)((u32)( O3( 0x4 ) + O4( 0x4 ) ) >> 1 );
			fmw->WAVEDATA[ 5] = (u8)((u32)( O3( 0x5 ) + O4( 0x5 ) ) >> 1 );
			fmw->WAVEDATA[ 6] = (u8)((u32)( O3( 0x6 ) + O4( 0x6 ) ) >> 1 );
			fmw->WAVEDATA[ 7] = (u8)((u32)( O3( 0x7 ) + O4( 0x7 ) ) >> 1 );
			fmw->WAVEDATA[ 8] = (u8)((u32)( O3( 0x8 ) + O4( 0x8 ) ) >> 1 );
			fmw->WAVEDATA[ 9] = (u8)((u32)( O3( 0x9 ) + O4( 0x9 ) ) >> 1 );
			fmw->WAVEDATA[10] = (u8)((u32)( O3( 0xA ) + O4( 0xA ) ) >> 1 );
			fmw->WAVEDATA[11] = (u8)((u32)( O3( 0xB ) + O4( 0xB ) ) >> 1 );
			fmw->WAVEDATA[12] = (u8)((u32)( O3( 0xC ) + O4( 0xC ) ) >> 1 );
			fmw->WAVEDATA[13] = (u8)((u32)( O3( 0xD ) + O4( 0xD ) ) >> 1 );
			fmw->WAVEDATA[14] = (u8)((u32)( O3( 0xE ) + O4( 0xE ) ) >> 1 );
			fmw->WAVEDATA[15] = (u8)((u32)( O3( 0xF ) + O4( 0xF ) ) >> 1 );
			#undef O1
			#undef O2
			#undef O3
			#undef O4
			break;
		/* ALGORITHM 3:
		
			     <[1]>
		 	       V						 
		[3]->[2]->{4}--> OUT
		
		*/	case 3:
			#define O1( a )	((u8)((u32)( operators[ fmw->OP1_TYPE ][ ( a + FB[a] 		) & 0xF ] * operator_volume[0] ) >> 4))
			#define O3( a )	((u8)((u32)( operators[ fmw->OP3_TYPE ][ ( a 				) & 0xF ] * operator_volume[2] ) >> 4))
			#define O2( a )	((u8)((u32)( operators[ fmw->OP2_TYPE ][ ( a + O3(a) 		) & 0xF ] * operator_volume[1] ) >> 4))
			#define O4( a )	((u8)((u32)( operators[ fmw->OP4_TYPE ][ ( a + O2(a) + O1(a)) & 0xF ] * operator_volume[3] ) >> 4))
			fmw->WAVEDATA[ 0] = (u8)((u32) O4( 0x0 ) );
			fmw->WAVEDATA[ 1] = (u8)((u32) O4( 0x1 ) );
			fmw->WAVEDATA[ 2] = (u8)((u32) O4( 0x2 ) );
			fmw->WAVEDATA[ 3] = (u8)((u32) O4( 0x3 ) );
			fmw->WAVEDATA[ 4] = (u8)((u32) O4( 0x4 ) );
			fmw->WAVEDATA[ 5] = (u8)((u32) O4( 0x5 ) );
			fmw->WAVEDATA[ 6] = (u8)((u32) O4( 0x6 ) );
			fmw->WAVEDATA[ 7] = (u8)((u32) O4( 0x7 ) );
			fmw->WAVEDATA[ 8] = (u8)((u32) O4( 0x8 ) );
			fmw->WAVEDATA[ 9] = (u8)((u32) O4( 0x9 ) );
			fmw->WAVEDATA[10] = (u8)((u32) O4( 0xA ) );
			fmw->WAVEDATA[11] = (u8)((u32) O4( 0xB ) );
			fmw->WAVEDATA[12] = (u8)((u32) O4( 0xC ) );
			fmw->WAVEDATA[13] = (u8)((u32) O4( 0xD ) );
			fmw->WAVEDATA[14] = (u8)((u32) O4( 0xE ) );
			fmw->WAVEDATA[15] = (u8)((u32) O4( 0xF ) );
			#undef O1
			#undef O2
			#undef O3
			#undef O4
			break;
		/* ALGORITHM 4: 
		
		<[1]>\	     
		 	  >->[2]->{4}-> OUT
		 [3]-/   
		 
		*/	case 4:
			#define O1( a )	((u8)((u32)( operators[ fmw->OP1_TYPE ][ ( a + FB[a] 		) & 0xF ] * operator_volume[0] ) >> 4))
			#define O3( a )	((u8)((u32)( operators[ fmw->OP3_TYPE ][ ( a 		 		) & 0xF ] * operator_volume[2] ) >> 4))
			#define O2( a )	((u8)((u32)( operators[ fmw->OP2_TYPE ][ ( a + O1(a) + O3(a)) & 0xF ] * operator_volume[1] ) >> 4))
			#define O4( a )	((u8)((u32)( operators[ fmw->OP4_TYPE ][ ( a + O2(a) 		) & 0xF ] * operator_volume[3] ) >> 4))
			fmw->WAVEDATA[ 0] = (u8)((u32) O4( 0x0 ) );
			fmw->WAVEDATA[ 1] = (u8)((u32) O4( 0x1 ) );
			fmw->WAVEDATA[ 2] = (u8)((u32) O4( 0x2 ) );
			fmw->WAVEDATA[ 3] = (u8)((u32) O4( 0x3 ) );
			fmw->WAVEDATA[ 4] = (u8)((u32) O4( 0x4 ) );
			fmw->WAVEDATA[ 5] = (u8)((u32) O4( 0x5 ) );
			fmw->WAVEDATA[ 6] = (u8)((u32) O4( 0x6 ) );
			fmw->WAVEDATA[ 7] = (u8)((u32) O4( 0x7 ) );
			fmw->WAVEDATA[ 8] = (u8)((u32) O4( 0x8 ) );
			fmw->WAVEDATA[ 9] = (u8)((u32) O4( 0x9 ) );
			fmw->WAVEDATA[10] = (u8)((u32) O4( 0xA ) );
			fmw->WAVEDATA[11] = (u8)((u32) O4( 0xB ) );
			fmw->WAVEDATA[12] = (u8)((u32) O4( 0xC ) );
			fmw->WAVEDATA[13] = (u8)((u32) O4( 0xD ) );
			fmw->WAVEDATA[14] = (u8)((u32) O4( 0xE ) );
			fmw->WAVEDATA[15] = (u8)((u32) O4( 0xF ) );
			#undef O1
			#undef O2
			#undef O3
			#undef O4
			break;
		/* ALGORITHM 5: 
		
		<[1]>->[2]->[3]->{4}-> OUT
		
		*/	case 5:
			#define O1( a )	((u8)((u32)( operators[ fmw->OP1_TYPE ][ ( a + FB[a] ) & 0xF ] * operator_volume[0] ) >> 4))
			#define O2( a )	((u8)((u32)( operators[ fmw->OP2_TYPE ][ ( a + O1(a) ) & 0xF ] * operator_volume[1] ) >> 4))
			#define O3( a )	((u8)((u32)( operators[ fmw->OP3_TYPE ][ ( a + O2(a) ) & 0xF ] * operator_volume[2] ) >> 4))
			#define O4( a )	((u8)((u32)( operators[ fmw->OP4_TYPE ][ ( a + O3(a) ) & 0xF ] * operator_volume[3] ) >> 4))
			fmw->WAVEDATA[ 0] = (u8)((u32) O4( 0x0 ) );
			fmw->WAVEDATA[ 1] = (u8)((u32) O4( 0x1 ) );
			fmw->WAVEDATA[ 2] = (u8)((u32) O4( 0x2 ) );
			fmw->WAVEDATA[ 3] = (u8)((u32) O4( 0x3 ) );
			fmw->WAVEDATA[ 4] = (u8)((u32) O4( 0x4 ) );
			fmw->WAVEDATA[ 5] = (u8)((u32) O4( 0x5 ) );
			fmw->WAVEDATA[ 6] = (u8)((u32) O4( 0x6 ) );
			fmw->WAVEDATA[ 7] = (u8)((u32) O4( 0x7 ) );
			fmw->WAVEDATA[ 8] = (u8)((u32) O4( 0x8 ) );
			fmw->WAVEDATA[ 9] = (u8)((u32) O4( 0x9 ) );
			fmw->WAVEDATA[10] = (u8)((u32) O4( 0xA ) );
			fmw->WAVEDATA[11] = (u8)((u32) O4( 0xB ) );
			fmw->WAVEDATA[12] = (u8)((u32) O4( 0xC ) );
			fmw->WAVEDATA[13] = (u8)((u32) O4( 0xD ) );
			fmw->WAVEDATA[14] = (u8)((u32) O4( 0xE ) );
			fmw->WAVEDATA[15] = (u8)((u32) O4( 0xF ) );
			#undef O1
			#undef O2
			#undef O3
			#undef O4
			break;
	}
	
	// Mix shapes
	
	FB[ 0] += fmw->WAVEDATA[ 0];//>>1;
	FB[ 1] += fmw->WAVEDATA[ 1];//>>1;
	FB[ 2] += fmw->WAVEDATA[ 2];//>>1;
	FB[ 3] += fmw->WAVEDATA[ 3];//>>1;
	FB[ 4] += fmw->WAVEDATA[ 4];//>>1;
	FB[ 5] += fmw->WAVEDATA[ 5];//>>1;
	FB[ 6] += fmw->WAVEDATA[ 6];//>>1;
	FB[ 7] += fmw->WAVEDATA[ 7];//>>1;
	FB[ 8] += fmw->WAVEDATA[ 8];//>>1;
	FB[ 9] += fmw->WAVEDATA[ 9];//>>1;
	FB[10] += fmw->WAVEDATA[10];//>>1;
	FB[11] += fmw->WAVEDATA[11];//>>1;
	FB[12] += fmw->WAVEDATA[12];//>>1;
	FB[13] += fmw->WAVEDATA[13];//>>1;
	FB[14] += fmw->WAVEDATA[14];//>>1;
	FB[15] += fmw->WAVEDATA[15];//>>1;
	
	loadFmw( fmw->WAVEDATA , 8 + (fmw->DISTORTION>>1));
}

/*###########################################################################*/

u8 FMDATA[ FM_BUFFER_SIZE ]; /*4x16*///just 16 bytes saved, the rest is for playback buffer

void Synth::loadFmw( u8 data[16] , u8 mult ){
	u8 *src = FMDATA;
	
	// Copy rendered sample to buffer to be displayed later
	for(int i=0; i < 16; i++){
		VAR_FMW.WAVEDATA[i] = data[i] * mult;
	}
	
	// Create buffer upon new shape (this buffer is feeded directly to the sound fifo in the VBLANK interrupt call to Mixer::mix
	for(int i=0; i < FM_BUFFER_SIZE; i+= 16, src+=16){
		src[ 0] = data[ 0] * mult;
		src[ 1] = data[ 1] * mult;
		src[ 2] = data[ 2] * mult;
		src[ 3] = data[ 3] * mult;
		src[ 4] = data[ 4] * mult;
		src[ 5] = data[ 5] * mult;
		src[ 6] = data[ 6] * mult;
		src[ 7] = data[ 7] * mult;
		src[ 8] = data[ 8] * mult;
		src[ 9] = data[ 9] * mult;
		src[10] = data[10] * mult;
		src[11] = data[11] * mult;
		src[12] = data[12] * mult;
		src[13] = data[13] * mult;
		src[14] = data[14] * mult;
		src[15] = data[15] * mult;
	}
}

void Synth::loadSmp( u8 data[16] ){
	// TODO
}


void Synth::loadWav( u8 data[16] ){
	//select bank 0 for writing (bank 1 playing)
	// if(! VAR_CHANNEL[ CHANNEL_WAV ].render )return;
	// VAR_CHANNEL[ CHANNEL_WAV ].render = false;
	// Copy rendered sample to buffer to be displayed later
	DmaCopy(3, data, VAR_WAV.WAVEDATA,  0x10, 16);//512 tiles
	
	
	swap_bank ^=1;
	//REG_SOUND3CNT_L = SOUND3PLAY | SOUND3BANK32 | (swap_bank ? SOUND3SETBANK0 : SOUND3SETBANK1);
	//REG_SOUND3CNT_L = SOUND3BANK32 | (swap_bank ? SOUND3SETBANK0 : SOUND3SETBANK1);
	
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
	swap_bank ^=1; 
	REG_SOUND3CNT_L = SOUND3PLAY | SOUND3BANK32 | (swap_bank ? SOUND3SETBANK0 : SOUND3SETBANK1);
}

/*###########################################################################*/

void Synth::triggerPwm1( Channel *channel ){
	u8 vol 			 = channel->volume;
	Instrument *i 	 = &VAR_INSTRUMENTS[ channel->inst ];
	SETTINGS_PWM pwm = InstEdit::unpackPwm( i );
	
	if( channel->reset ) {
		channel->tsp_position 	= 0;				
		channel->vol_position 	= 0;
		
		if( pwm.TSP_LOOP == LOOP_BACKWARD ) channel->tsp_loop = LOOP_BACKWARD_FORWARD;
		if( pwm.TSP_LOOP == LOOP_PINGPONG ) channel->tsp_loop = LOOP_PINGPONG_FORWARD;
		else channel->tsp_loop = (LoopMode)pwm.TSP_LOOP;
		
		if( pwm.VOL_LOOP == LOOP_BACKWARD ) channel->vol_loop = LOOP_BACKWARD_FORWARD;
		if( pwm.VOL_LOOP == LOOP_PINGPONG ) channel->vol_loop = LOOP_PINGPONG_FORWARD;
		else channel->vol_loop = (LoopMode)pwm.VOL_LOOP;
		
		channel->reset 			= false;				
		channel->transpose		= 0;
		channel->tsp_target		= pwm.TSP_LENGTH ;
		channel->vol_target		= pwm.VOL_LENGTH ;
		channel->tsp_delta 		= 1;
		channel->vol_delta 		= 1;
	}
	if( pwm.TSP_ENABLE ){				
		u8 tsp_pos = (channel->tsp_position >> pwm.TSP_SPEED) ;
		
		if( channel->tsp_delta+1){
			if( tsp_pos < channel->tsp_target ){
				channel->retrig 		= true;
				channel->transpose 		= pwm.TSP[ tsp_pos ];
				channel->tsp_position  += channel->tsp_delta;
			} else {
				// Hit end going forwards 					
				if( channel->tsp_loop  != LOOP_NONE){
					if( channel->tsp_loop  == LOOP_FORWARD) channel->tsp_position=0; 
					else {
						if(channel->tsp_loop == LOOP_PINGPONG_FORWARD) channel->tsp_loop = LOOP_PINGPONG; 
						channel->tsp_delta  =-1;
						channel->tsp_target = 0;
					}
					channel->retrig 		= true;
				}
			}
		} else if(!(channel->tsp_delta+1)){
			if( tsp_pos > channel->tsp_target ){
				channel->retrig 		= true;
				channel->transpose 		= pwm.TSP[ tsp_pos ];		
				channel->tsp_position  += channel->tsp_delta;
			} else {
				// Hit the end going backwards
				if( channel->tsp_loop  == LOOP_BACKWARD) channel->tsp_position=pwm.TSP_LENGTH-1; 
				if( channel->tsp_loop  == LOOP_PINGPONG){
					channel->tsp_loop = LOOP_PINGPONG_FORWARD;
					channel->tsp_delta  = 1;
					channel->tsp_position++;
					channel->tsp_target = pwm.TSP_LENGTH;
				}				
				channel->retrig 		= true;
			}
		}		
	}
	
	
	if( pwm.VOL_ENABLE ){				
		u8 vol_pos = (channel->vol_position >> pwm.VOL_SPEED) ;
		if( channel->vol_delta+1){
			if( vol_pos < channel->vol_target ){
				channel->retrig 		= true;
				vol 					= ( vol + pwm.VOL[ vol_pos ] ) >> 1;
				channel->vol_position  += channel->vol_delta;
			} else {
				// Hit end going forwards 					
				if( channel->vol_loop  != LOOP_NONE){
					if( channel->vol_loop  == LOOP_FORWARD) channel->vol_position=0; 
					else {
						if(channel->vol_loop == LOOP_PINGPONG_FORWARD) channel->vol_loop = LOOP_PINGPONG; 
						channel->vol_delta  =-1;
						channel->vol_target = 0;
					}
					channel->retrig 		= true;
				}
			}
		} else if(!(channel->vol_delta+1)){
			if( vol_pos > channel->vol_target ){
				channel->retrig 		= true;
				vol 					= ( vol + pwm.VOL[ vol_pos ] ) >> 1;
				channel->vol_position  += channel->vol_delta;
			} else {
				// Hit the end going backwards
				if( channel->vol_loop  == LOOP_BACKWARD) channel->vol_position=pwm.VOL_LENGTH-1; 
				if( channel->vol_loop  == LOOP_PINGPONG){
					channel->vol_loop = LOOP_PINGPONG_FORWARD;
					channel->vol_delta  = 1;
					channel->vol_position++;
					channel->vol_target = pwm.VOL_LENGTH;
				}				
				channel->retrig 		= true;
			}
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
	SETTINGS_PWM pwm = InstEdit::unpackPwm( i );
	
	if( channel->reset ) {
		channel->tsp_position 	= 0;				
		channel->vol_position 	= 0;
		if( pwm.TSP_LOOP == LOOP_BACKWARD ) channel->tsp_loop = LOOP_BACKWARD_FORWARD;
		if( pwm.TSP_LOOP == LOOP_PINGPONG ) channel->tsp_loop = LOOP_PINGPONG_FORWARD;
		else channel->tsp_loop = (LoopMode)pwm.TSP_LOOP;
		
		if( pwm.VOL_LOOP == LOOP_BACKWARD ) channel->vol_loop = LOOP_BACKWARD_FORWARD;
		if( pwm.VOL_LOOP == LOOP_PINGPONG ) channel->vol_loop = LOOP_PINGPONG_FORWARD;
		else channel->vol_loop = (LoopMode)pwm.VOL_LOOP;
		
		channel->reset 			= false;				
		channel->transpose		= 0;
		channel->tsp_target		= pwm.TSP_LENGTH ;
		channel->vol_target		= pwm.VOL_LENGTH ;
		channel->tsp_delta 		= 1;
		channel->vol_delta 		= 1;
	}
	if( pwm.TSP_ENABLE ){				
		u8 tsp_pos = (channel->tsp_position >> pwm.TSP_SPEED) ;
		
		if( channel->tsp_delta+1){
			if( tsp_pos < channel->tsp_target ){
				channel->retrig 		= true;
				channel->transpose 		= pwm.TSP[ tsp_pos ];
				channel->tsp_position  += channel->tsp_delta;
			} else {
				// Hit end going forwards 					
				if( channel->tsp_loop  != LOOP_NONE){
					if( channel->tsp_loop  == LOOP_FORWARD) channel->tsp_position=0; 
					else {
						if(channel->tsp_loop == LOOP_PINGPONG_FORWARD) channel->tsp_loop = LOOP_PINGPONG; 
						channel->tsp_delta  =-1;
						channel->tsp_target = 0;
					}
					channel->retrig 		= true;
				}
			}
		} else if(!(channel->tsp_delta+1)){
			if( tsp_pos > channel->tsp_target ){
				channel->retrig 		= true;
				channel->transpose 		= pwm.TSP[ tsp_pos ];		
				channel->tsp_position  += channel->tsp_delta;
			} else {
				// Hit the end going backwards
				if( channel->tsp_loop  == LOOP_BACKWARD) channel->tsp_position=pwm.TSP_LENGTH-1; 
				if( channel->tsp_loop  == LOOP_PINGPONG){
					channel->tsp_loop = LOOP_PINGPONG_FORWARD;
					channel->tsp_delta  = 1;
					channel->tsp_position++;
					channel->tsp_target = pwm.TSP_LENGTH;
				}				
				channel->retrig 		= true;
			}
		}		
	}
	
	
	if( pwm.VOL_ENABLE ){				
		u8 vol_pos = (channel->vol_position >> pwm.VOL_SPEED) ;
		
		if( channel->vol_delta+1){
			if( vol_pos < channel->vol_target ){
				channel->retrig 		= true;
				vol 					= ( vol + pwm.VOL[ vol_pos ] ) >> 1;
				channel->vol_position  += channel->vol_delta;
			} else {
				// Hit end going forwards 					
				if( channel->vol_loop  != LOOP_NONE){
					if( channel->vol_loop  == LOOP_FORWARD) channel->vol_position=0; 
					else {
						if(channel->vol_loop == LOOP_PINGPONG_FORWARD) channel->vol_loop = LOOP_PINGPONG; 
						channel->vol_delta  =-1;
						channel->vol_target = 0;
					}
					channel->retrig 		= true;
				}
			}
		} else if(!(channel->vol_delta+1)){
			if( vol_pos > channel->vol_target ){
				channel->retrig 		= true;
				vol 					= ( vol + pwm.VOL[ vol_pos ] ) >> 1;
				channel->vol_position  += channel->vol_delta;
			} else {
				// Hit the end going backwards
				if( channel->vol_loop  == LOOP_BACKWARD) channel->vol_position=pwm.VOL_LENGTH-1; 
				if( channel->vol_loop  == LOOP_PINGPONG){
					channel->vol_loop = LOOP_PINGPONG_FORWARD;
					channel->vol_delta  = 1;
					channel->vol_position++;
					channel->vol_target = pwm.VOL_LENGTH;
				}				
				channel->retrig 		= true;
			}
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
	SETTINGS_PWM pwm = InstEdit::unpackPwm( i );
	
	if( channel->reset ) {
		channel->tsp_position 	= 0;				
		channel->vol_position 	= 0;
		if( pwm.TSP_LOOP == LOOP_BACKWARD ) channel->tsp_loop = LOOP_BACKWARD_FORWARD;
		if( pwm.TSP_LOOP == LOOP_PINGPONG ) channel->tsp_loop = LOOP_PINGPONG_FORWARD;
		else channel->tsp_loop = (LoopMode)pwm.TSP_LOOP;
		
		if( pwm.VOL_LOOP == LOOP_BACKWARD ) channel->vol_loop = LOOP_BACKWARD_FORWARD;
		if( pwm.VOL_LOOP == LOOP_PINGPONG ) channel->vol_loop = LOOP_PINGPONG_FORWARD;
		else channel->vol_loop = (LoopMode)pwm.VOL_LOOP;
		
		channel->reset 			= false;				
		channel->transpose		= 0;
		channel->tsp_target		= pwm.TSP_LENGTH ;
		channel->vol_target		= pwm.VOL_LENGTH ;
		channel->tsp_delta 		= 1;
		channel->vol_delta 		= 1;
	}
	if( pwm.TSP_ENABLE ){				
		u8 tsp_pos = (channel->tsp_position >> pwm.TSP_SPEED) ;
		
		if( channel->tsp_delta+1){
			if( tsp_pos < channel->tsp_target ){
				channel->retrig 		= true;
				channel->transpose 		= pwm.TSP[ tsp_pos ];
				channel->tsp_position  += channel->tsp_delta;
			} else {
				// Hit end going forwards 					
				if( channel->tsp_loop  != LOOP_NONE){
					if( channel->tsp_loop  == LOOP_FORWARD) channel->tsp_position=0; 
					else {
						if(channel->tsp_loop == LOOP_PINGPONG_FORWARD) channel->tsp_loop = LOOP_PINGPONG; 
						channel->tsp_delta  =-1;
						channel->tsp_target = 0;
					}
					channel->retrig 		= true;
				}
			}
		} else if(!(channel->tsp_delta+1)){
			if( tsp_pos > channel->tsp_target ){
				channel->retrig 		= true;
				channel->transpose 		= pwm.TSP[ tsp_pos ];		
				channel->tsp_position  += channel->tsp_delta;
			} else {
				// Hit the end going backwards
				if( channel->tsp_loop  == LOOP_BACKWARD) channel->tsp_position=pwm.TSP_LENGTH-1; 
				if( channel->tsp_loop  == LOOP_PINGPONG){
					channel->tsp_loop = LOOP_PINGPONG_FORWARD;
					channel->tsp_delta  = 1;
					channel->tsp_position++;
					channel->tsp_target = pwm.TSP_LENGTH;
				}				
				channel->retrig 		= true;
			}
		}		
	}
	
	
	if( pwm.VOL_ENABLE ){				
		u8 vol_pos = (channel->vol_position >> pwm.VOL_SPEED) ;
		
		if( channel->vol_delta+1){
			if( vol_pos < channel->vol_target ){
				channel->retrig 		= true;
				vol 					= ( vol + pwm.VOL[ vol_pos ] ) >> 1;
				channel->vol_position  += channel->vol_delta;
			} else {
				// Hit end going forwards 					
				if( channel->vol_loop  != LOOP_NONE){
					if( channel->vol_loop  == LOOP_FORWARD) channel->vol_position=0; 
					else {
						if(channel->vol_loop == LOOP_PINGPONG_FORWARD) channel->vol_loop = LOOP_PINGPONG; 
						channel->vol_delta  =-1;
						channel->vol_target = 0;
					}
					channel->retrig 		= true;
				}
			}
		} else if(!(channel->vol_delta+1)){
			if( vol_pos > channel->vol_target ){
				channel->retrig 		= true;
				vol 					= ( vol + pwm.VOL[ vol_pos ] ) >> 1;
				channel->vol_position  += channel->vol_delta;
			} else {
				// Hit the end going backwards
				if( channel->vol_loop  == LOOP_BACKWARD) channel->vol_position=pwm.VOL_LENGTH-1; 
				if( channel->vol_loop  == LOOP_PINGPONG){
					channel->vol_loop = LOOP_PINGPONG_FORWARD;
					channel->vol_delta  = 1;
					channel->vol_position++;
					channel->vol_target = pwm.VOL_LENGTH;
				}				
				channel->retrig 		= true;
			}
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
	SETTINGS_WAV wav = InstEdit::unpackWav( i );
	
	if( channel->reset ) {
		Adsr::wav_position 		= 0;
		channel->tsp_position 	= 0;				
		channel->vol_position 	= 0;
		channel->transpose 		= 0;
		channel->reset	 			= false;				
	}
	
	if( channel->retrig ){
		// Regenerate WAV ADSR Tables
		Adsr::updateWav( &wav );
	} 
	// Regenerate WAVE shape (scaled to channel volume)
	renderWav( &wav , vol );
	noteOnWav( channel );
}

void Synth::triggerFmw( Channel* channel ){
	u8 vol 			 = channel->volume;
	Instrument *i 	 = &VAR_INSTRUMENTS[ channel->inst ];
	SETTINGS_FMW fmw = InstEdit::unpackFmw( i );
	
	if( channel->reset ){
		Adsr::fmw_position 		= 0;
		channel->tsp_position	= 0;
		channel->vol_position	= 0;
		channel->transpose		= 0;
		channel->reset			= false;
	}
	
	if( channel->retrig ){
		// Regenerate FM ADSR Tables
		Adsr::updateFmw( &fmw );
	}
	// Regenerate FMW shape (scaled to channel volume)
	renderFmw( &fmw, vol );
	noteOnFmw( channel );
}

#include "sequencer.hpp"

void Synth::triggerSmp( Channel* channel ){
	u8 vol 			 = channel->volume;
	Instrument *i 	 = &VAR_INSTRUMENTS[ channel->inst ];
	SETTINGS_SMP smp = InstEdit::unpackSmp( i );
	
	if( channel->reset ){
		Adsr::smp_position 		= 0;
		channel->tsp_position	= 0;
		channel->vol_position	= 0;
		channel->reset			= false;
		channel->transpose		= 0;
		
		if( channel->retrig ){//&& Sequencer::currentTicks==0){
			// Regenerate SMP ADSR Table
			Adsr::updateSmp( &smp );
			noteOnSmp( channel );
		}
		channel->retrig = false;
	
		// Regenerate FMW shape (scaled to channel volume)
		renderSmp( &smp, vol );
	}
}
/*###########################################################################*/

void Synth::polysynth( u16 input ){
	static u8 wavedata[16];
	bool initialized = lfo == 0;
	
	lfo++;
	//input+=lfo;
	//	u8 len  = 0;//input & 0x1F; // 5 bit : 0 
	u8 duty = (lfo>>6) & 0x03; // 2 bit : 6
	u8 step = 0x3; //input & 0x07; // 3 bit : 8
	u8 edir = 0x0800;		// 1 bit : 11
	u8 vol[ CHANNEL_COUNT ]   = {
		0xFF - (input>>5),
		(input>>8)*4,
		input>>5,
		0xF - (lfo>>6),
		input>>5,
		input>>5,
	};
	u16 freq[ CHANNEL_COUNT ] = {
		PWM_FREQ_TABLE[ input>>(input%8) ],
		PWM_FREQ_TABLE[ input   ] +(lfo),
		PWM_FREQ_TABLE[ input+7 ] + lfo*lfo,
		PWM_FREQ_TABLE[ input   ] + lfo,
		PWM_FREQ_TABLE[ input>>(input%8) ],
		PWM_FREQ_TABLE[ input>>(input%8) ],
	};
	
	// Pulse 1
	*((volatile u16*)0x04000064) = 0x0000 | freq[0]; // Retrigger PU1
	*((volatile u16*)0x0400006C) = 0x0000 | freq[1]; // Retrigger PU2
	loadWav( wavedata );
	//*((volatile u16*)0x04000074) = 0x0000 | freq[2]; // Retrigger WAV
	REG_SOUND3CNT_X = SOUND3INIT | SOUND3PLAYLOOP | freq[2]; 
	
	*((volatile u16*)0x0400007C) = 0x0000 | freq[3]; // Retrigger NZE
	*((volatile u16*)0x04000060) = 0x0079;// PU1 Sweep 
	*((volatile u16*)0x04000062) = (vol[0] << 12) | edir | (step << 8) | (duty << 6); // PU1 Envelope 
	*((volatile u16*)0x04000068) = (vol[1] << 12) | edir | (step << 8) | ((0x3 - duty) << 6); // PU2 Envelope
	*((volatile u16*)0x04000072) = (vol[2] << 12) | (step << 8); // WAV Envelope
	*((volatile u16*)0x04000078) = (vol[3] << 12) | (step << 8); // NZE Envelope
	// Trigger channels just once
	if(!initialized) {
		*((volatile u16*)0x04000064) = 0x8000 | freq[0];
		*((volatile u16*)0x0400006C) = 0x8000 | freq[1];
		*((volatile u16*)0x04000074) = 0x8000 | freq[2];
		*((volatile u16*)0x0400007C) = 0x8000 | freq[3];

		#define OPERATOR1( a )	((u8)((u32)( operators[0] )))
		#define OPERATOR2( a )	((u8)((u32)( operators[1] )))
		#define OPERATOR3( a )	((u8)((u32)( operators[2] )))
		#define OPERATOR4( a )	((u8)((u32)( operators[3] )))
		
		// Mix shapes
		
		wavedata[ 0] = (u8)((u32)( OPERATOR1( 0x0 ) + OPERATOR2( 0x0 ) + OPERATOR3( 0x0 ) + OPERATOR4( 0x0 ) ) >> 2);
		wavedata[ 1] = (u8)((u32)( OPERATOR1( 0x1 ) + OPERATOR2( 0x1 ) + OPERATOR3( 0x1 ) + OPERATOR4( 0x1 ) ) >> 2);
		wavedata[ 2] = (u8)((u32)( OPERATOR1( 0x2 ) + OPERATOR2( 0x2 ) + OPERATOR3( 0x2 ) + OPERATOR4( 0x2 ) ) >> 2);
		wavedata[ 3] = (u8)((u32)( OPERATOR1( 0x3 ) + OPERATOR2( 0x3 ) + OPERATOR3( 0x3 ) + OPERATOR4( 0x3 ) ) >> 2);
		wavedata[ 4] = (u8)((u32)( OPERATOR1( 0x4 ) + OPERATOR2( 0x4 ) + OPERATOR3( 0x4 ) + OPERATOR4( 0x4 ) ) >> 2);
		wavedata[ 5] = (u8)((u32)( OPERATOR1( 0x5 ) + OPERATOR2( 0x5 ) + OPERATOR3( 0x5 ) + OPERATOR4( 0x5 ) ) >> 2);
		wavedata[ 6] = (u8)((u32)( OPERATOR1( 0x6 ) + OPERATOR2( 0x6 ) + OPERATOR3( 0x6 ) + OPERATOR4( 0x6 ) ) >> 2);
		wavedata[ 7] = (u8)((u32)( OPERATOR1( 0x7 ) + OPERATOR2( 0x7 ) + OPERATOR3( 0x7 ) + OPERATOR4( 0x7 ) ) >> 2);				
		wavedata[ 8] = (u8)((u32)( OPERATOR1( 0x8 ) + OPERATOR2( 0x8 ) + OPERATOR3( 0x8 ) + OPERATOR4( 0x8 ) ) >> 2);
		wavedata[ 9] = (u8)((u32)( OPERATOR1( 0x9 ) + OPERATOR2( 0x9 ) + OPERATOR3( 0x9 ) + OPERATOR4( 0x9 ) ) >> 2);
		wavedata[10] = (u8)((u32)( OPERATOR1( 0xA ) + OPERATOR2( 0xA ) + OPERATOR3( 0xA ) + OPERATOR4( 0xA ) ) >> 2);
		wavedata[11] = (u8)((u32)( OPERATOR1( 0xB ) + OPERATOR2( 0xB ) + OPERATOR3( 0xB ) + OPERATOR4( 0xB ) ) >> 2);
		wavedata[12] = (u8)((u32)( OPERATOR1( 0xC ) + OPERATOR2( 0xC ) + OPERATOR3( 0xC ) + OPERATOR4( 0xC ) ) >> 2);
		wavedata[13] = (u8)((u32)( OPERATOR1( 0xD ) + OPERATOR2( 0xD ) + OPERATOR3( 0xD ) + OPERATOR4( 0xD ) ) >> 2);
		wavedata[14] = (u8)((u32)( OPERATOR1( 0xE ) + OPERATOR2( 0xE ) + OPERATOR3( 0xE ) + OPERATOR4( 0xE ) ) >> 2);
		wavedata[15] = (u8)((u32)( OPERATOR1( 0xF ) + OPERATOR2( 0xF ) + OPERATOR3( 0xF ) + OPERATOR4( 0xF ) ) >> 2);
		
		#undef OPERATOR4
		#undef OPERATOR3
		#undef OPERATOR2
		#undef OPERATOR1
		
		loadWav( wavedata );
		initialized = true;
	}
	return;	
}
