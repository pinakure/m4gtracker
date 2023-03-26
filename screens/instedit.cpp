#include "instedit.hpp"

#include "../debug.hpp"
#include "../data/data.hpp"
#include "../macros.h"
#include "../kernel/regionhandler/regionhandler.hpp"
#include "../kernel/spu/synth.hpp"
#include "../kernel/spu/sequencer.hpp"
#include "../kernel/gpu/gpu.hpp"
#include "../kernel/gpu/virtualscreen.hpp"
#include "../kernel/key/key.hpp"
#include "../data/hudcursor.hpp"
#include "instedit/callbacks.c"
#include "instedit/controls.c"
#include "instedit/displays.c"

bool InstEdit::clean;
Sprite InstEdit::waveform[32];

// Called on index change
void InstEdit::load(){	
	InstEdit::copy( &VAR_INSTRUMENTS[ VAR_CFG.CURRENTINSTRUMENT ], &VAR_INSTRUMENT );	
	InstEdit::unpack( &VAR_INSTRUMENT );
	RegionHandler::redraw = true;	
}

// Called on value change (against index and type)
void InstEdit::repack(){	
	InstEdit::pack(&VAR_INSTRUMENT);
	InstEdit::copy(&VAR_INSTRUMENT, &VAR_INSTRUMENTS[VAR_CFG.CURRENTINSTRUMENT]);	
}

void InstEdit::type( Control *c, bool bigstep, bool add, u32 *pointer ){	
	modify2BIT( c, bigstep, add, pointer); 
	InstEdit::unpack( &VAR_INSTRUMENT );
}

void InstEdit::modBit( Control *c, bool bigstep, bool add, u32 *pointer ){
	modify1BIT( c, bigstep, add, pointer); 
	InstEdit::repack(); 
}

void InstEdit::mod2b( Control *c, bool bigstep, bool add, u32 *pointer ){
	modify2BIT( c, bigstep, add, pointer ); 
	InstEdit::repack();
}

void InstEdit::mod3b( Control *c, bool bigstep, bool add, u32 *pointer ){	
	modify3BIT( c,bigstep, add, pointer); 
	InstEdit::repack(); 
}

void InstEdit::modNibble( Control *c, bool bigstep, bool add, u32 *pointer ){	
	modify4BIT( c,bigstep, add, pointer ); 
	InstEdit::repack(); 
}

void InstEdit::mod5b( Control *c, bool bigstep, bool add, u32 *pointer ){	
	modify5BIT(c,bigstep, add, pointer); 
	InstEdit::repack(); 
}

void InstEdit::mod6b( Control *c, bool bigstep, bool add, u32 *pointer ){	
	modify6BIT(c,bigstep, add, pointer); 
	InstEdit::repack(); 
}

void InstEdit::mod7b( Control *c, bool bigstep, bool add, u32 *pointer ){
	modify7BIT(c,bigstep, add, pointer); 
	InstEdit::repack(); 
}

void InstEdit::modByte( Control *c, bool bigstep, bool add, u32 *pointer ){	
	modify8BIT(c,bigstep, add, pointer); 
	InstEdit::repack(); 
}

void InstEdit::modSmpKit( Control *c, bool bigstep, bool add, u32 *pointer ){
	modify5BIT(c,bigstep, add, pointer); 
	InstEdit::repack();
	// update kit name
	// reload kit sample names
}

void InstEdit::index( Control *c, bool bigstep, bool add, u32 *pointer ){
	InstEdit::repack(); 
	VAR_CFG.CURRENTINSTRUMENT = (VAR_CFG.CURRENTINSTRUMENT + ((bigstep?0x4:0x1) * (add?1:-1)));
	if((VAR_CFG.CURRENTINSTRUMENT > 0x3F) && (VAR_CFG.CURRENTINSTRUMENT < 0x44)) VAR_CFG.CURRENTINSTRUMENT = 0x1;
	else if(VAR_CFG.CURRENTINSTRUMENT > 0x3F) VAR_CFG.CURRENTINSTRUMENT = 0x3F;
	Transient::bit6 	= VAR_CFG.CURRENTINSTRUMENT; 
	Transient::changed	= true;
	if(VAR_CFG.CURRENTINSTRUMENT == 0x0 ) VAR_CFG.CURRENTINSTRUMENT= 0x1;
	InstEdit::load(); 
}

void InstEdit::modAlgorithm( Control *c, bool bigstep, bool add, u32 *pointer ){ 
	modify6VAL( c, bigstep, add, pointer ); 
	DECIMAL(6, 15, 0xAA, InstEdit::unpackFmw( &VAR_INSTRUMENT ).ALGORITHM );
	InstEdit::repack(); 
}

void InstEdit::modVal3( Control *c, bool bigstep, bool add, u32 *pointer ){	
	modifyValue( c, bigstep, add, pointer, 3);
	InstEdit::repack(); 
}

void InstEdit::modVal6( Control *c, bool bigstep, bool add, u32 *pointer ){ 
	modifyValue( c, bigstep, add, pointer, 6 ); 
	InstEdit::repack(); 
}

void InstEdit::modVal5( Control *c, bool bigstep, bool add, u32 *pointer ){	
	modifyValue( c, bigstep, add, pointer, 5); 
	InstEdit::repack(); 
}

void InstEdit::modVal27( Control *c, bool bigstep, bool add, u32 *pointer ){	
	modifyCommand( c, bigstep, add, pointer ); 
	InstEdit::repack(); 
}

/* SYNCRONIZES Specific instrument data with Generic Abstract Instrument byte array */
void InstEdit::pack( Instrument* i ){
	int c, di;
	
	switch(i->TYPE){
		case INSTRUMENT_TYPE_PWM:
			i->SETTINGS[0] = (VAR_PWM.SWEEPDIR<<7) | (VAR_PWM.ENVELOPEDIR<<6) | (VAR_PWM.TSP_ENABLE<<5) | (VAR_PWM.VOL_ENABLE<<4) | (VAR_PWM.DUTYCYCLE<<2) | VAR_PWM.TSP_LOOP;
			i->SETTINGS[1] = VAR_PWM.VOL_LOOP;			
			i->SETTINGS[2] = (VAR_PWM.LENGTH<<4) | VAR_PWM.LEVEL;
			i->SETTINGS[3] = (VAR_PWM.TSP_LENGTH<<4) | VAR_PWM.SWEEPSPEED;
			i->SETTINGS[4] = (VAR_PWM.SWEEPSTEPS<<4) | VAR_PWM.VOLUMEFADE;
			i->SETTINGS[5] = (VAR_PWM.TSP_POSITION<<4) | VAR_PWM.VOL_POSITION;
			i->SETTINGS[6] = (VAR_PWM.VOL_LENGTH<<4) | 0;
			i->SETTINGS[7] = VAR_PWM.VOL_ENVELOPE;
			i->SETTINGS[8] = VAR_PWM.TSP_ENVELOPE;
			for(c=0; c<16; c++){
				i->SETTINGS[9+c] 	= (VAR_PWM.TSP[c]<<4) | VAR_PWM.VOL[c];
			}
			return;
			
		case INSTRUMENT_TYPE_WAV:
			i->SETTINGS[ SETTING_WAV_PHASE 								] = (VAR_WAV.PHASE<<6) | (VAR_WAV.AM<<5) | VAR_WAV.DISTORTION; 
			i->SETTINGS[ SETTING_WAV_OP1_TYPE | SETTING_WAV_OP2_TYPE 	] = (VAR_WAV.OP1_TYPE<<3) | VAR_WAV.OP2_TYPE; 
			i->SETTINGS[ SETTING_WAV_OP3_TYPE | SETTING_WAV_OP4_TYPE 	] = (VAR_WAV.OP3_TYPE<<3) | VAR_WAV.OP4_TYPE;			
			for(c=0; c<4; c++){
				di = c<<1;
				i->SETTINGS[ ( SETTING_WAV_OP1_ADSR_1 | SETTING_WAV_OP2_ADSR_1 ) + di ] = (VAR_WAV.OP1_ADSR[c]<<4) | VAR_WAV.OP2_ADSR[c]; 
				i->SETTINGS[ ( SETTING_WAV_OP3_ADSR_1 | SETTING_WAV_OP4_ADSR_1 ) + di ] = (VAR_WAV.OP3_ADSR[c]<<4) | VAR_WAV.OP4_ADSR[c]; 
			}
			for(c=0; c<8; c++){
				di = c<<1;
				i->SETTINGS[ SETTING_WAV_WAVEDATA0 + c ] = (VAR_WAV.WAVEDATA[ di ]<<4) | VAR_WAV.WAVEDATA[di+1];
			}
			i->SETTINGS[ ( SETTING_WAV_OP1_GATE | SETTING_WAV_OP2_GATE ) ] = (VAR_WAV.OP1_GATE<<4) | VAR_WAV.OP2_GATE; 
			i->SETTINGS[ ( SETTING_WAV_OP3_GATE | SETTING_WAV_OP4_GATE ) ] = (VAR_WAV.OP3_GATE<<4) | VAR_WAV.OP4_GATE; 
			return;

		case INSTRUMENT_TYPE_FMW:
			i->SETTINGS[0] = ( VAR_FMW.DISTORTION << 3 ) | VAR_FMW.ALGORITHM;
			i->SETTINGS[1] = ( VAR_FMW.OP1_TYPE	 << 3 ) | VAR_FMW.OP2_TYPE;
			i->SETTINGS[2] = ( VAR_FMW.OP3_TYPE	 << 3 ) | VAR_FMW.OP4_TYPE;
			for(c=0; c<4; c++){
				di = c<<1;
				i->SETTINGS[3+di] = (VAR_FMW.OP1_ADSR[c]<<4) | VAR_FMW.OP2_ADSR[c];
				i->SETTINGS[4+di] = (VAR_FMW.OP3_ADSR[c]<<4) | VAR_FMW.OP4_ADSR[c];
			}
			for(c=0; c<8; c++){
				di = c<<1;
				i->SETTINGS[11 + c] = (VAR_FMW.WAVEDATA[ di ]<<4) | VAR_FMW.WAVEDATA[di+1];
			}
			return;

		case INSTRUMENT_TYPE_SMP:
		
			i->SETTINGS[ SETTING_SMP_START		] = VAR_SMP.START;
			i->SETTINGS[ SETTING_SMP_END		] = VAR_SMP.END;
			i->SETTINGS[ SETTING_SMP_KIT		] = VAR_SMP.KIT;
			i->SETTINGS[ SETTING_SMP_KITINDEX	] = VAR_SMP.KITINDEX;
			i->SETTINGS[ SETTING_SMP_LOOP		] = VAR_SMP.LOOP;
			i->SETTINGS[ SETTING_SMP_SYNTHMODE	] = VAR_SMP.SYNTHMODE;
			i->SETTINGS[ SETTING_SMP_FREQUENCY	] = VAR_SMP.FREQUENCY;
			i->SETTINGS[ SETTING_SMP_ADSR_A		] = (VAR_SMP.ADSR[0]<<4) | VAR_SMP.ADSR[1];
			i->SETTINGS[ SETTING_SMP_ADSR_S		] = (VAR_SMP.ADSR[2]<<4) | VAR_SMP.ADSR[3];
			i->SETTINGS[ SETTING_SMP_B			] = VAR_SMP.B;
			i->SETTINGS[ SETTING_SMP_S			] = VAR_SMP.S;
			i->SETTINGS[ SETTING_SMP_R			] = VAR_SMP.R;
			return;
	}
}

/* SYNCRONIZES Abstract Instrument data to current type specific data */

SETTINGS_FMW InstEdit::unpackFmw( Instrument *i ){
	SETTINGS_FMW ret;
	int c, di;
	ret.DISTORTION		= EXTRACT(i->SETTINGS[0], 4, 0xF);	// 4
	if((i->SETTINGS[0] &0x7) > 0x5) i->SETTINGS[0] = (i->SETTINGS[0] & 0xF0) | 5;				// 3
	ret.ALGORITHM 		= i->SETTINGS[0] &= 0x7;				// 3
	ret.OP1_TYPE 		= EXTRACT(i->SETTINGS[1], 3, 0x7);	// 3
	ret.OP2_TYPE 		= i->SETTINGS[1] & 0x7;				// 3
	ret.OP3_TYPE 		= EXTRACT(i->SETTINGS[2], 3, 0x7);	// 3
	ret.OP4_TYPE 		= i->SETTINGS[2] & 0x7;				// 3
	for(c=0; c<4; c++){
		di = c<<1;
		ret.OP1_ADSR[c] = EXTRACT(i->SETTINGS[3+di], 4, 0xF);	// 4
		ret.OP2_ADSR[c] = i->SETTINGS[3+di] & 0xF;				// 4
		ret.OP3_ADSR[c] = EXTRACT(i->SETTINGS[4+di], 4, 0xF);	// 4
		ret.OP4_ADSR[c] = i->SETTINGS[4+di] & 0xF;				// 4
	}
	for(c=0; c<8; c++){
		di = c<<1;
		ret.WAVEDATA[ di ] = EXTRACT(i->SETTINGS[11 + c], 4, 0xF);	// 4x8
		ret.WAVEDATA[di+1] = i->SETTINGS[11 + c] & 0xF;				// 4x8
	}
	ret.OP1_GATE = EXTRACT(i->SETTINGS[SETTING_FMW_OP1_GATE], 4, 0xF);	// 4
	ret.OP2_GATE = 		   i->SETTINGS[SETTING_FMW_OP2_GATE] & 0xF;				// 4
	ret.OP3_GATE = EXTRACT(i->SETTINGS[SETTING_FMW_OP3_GATE], 4, 0xF);	// 4
	ret.OP4_GATE = 		   i->SETTINGS[SETTING_FMW_OP4_GATE] & 0xF;				// 4
	
	return ret;
}

SETTINGS_SMP InstEdit::unpackSmp( Instrument *i ){
	SETTINGS_SMP ret;
	ret.START 	  = 		 i->SETTINGS[ SETTING_SMP_START		];				// 8
	ret.END 	  = 		 i->SETTINGS[ SETTING_SMP_END		];				// 8
	ret.KIT 	  = 		 i->SETTINGS[ SETTING_SMP_KIT		];				// 5
	ret.LOOP 	  = 		 i->SETTINGS[ SETTING_SMP_LOOP 		];				// 2
	ret.SYNTHMODE = 		 i->SETTINGS[ SETTING_SMP_SYNTHMODE	];				// 1
	ret.FREQUENCY = 		 i->SETTINGS[ SETTING_SMP_FREQUENCY ];				// 8
	ret.ADSR[0]   = EXTRACT( i->SETTINGS[ SETTING_SMP_ADSR_A 	],  4, 	0xF);	// 4
	ret.ADSR[1]   = 		 i->SETTINGS[ SETTING_SMP_ADSR_D	]	& 	0xF;	// 4
	ret.ADSR[2]   = EXTRACT( i->SETTINGS[ SETTING_SMP_ADSR_S	],  4, 	0xF);	// 4
	ret.ADSR[3]   = 		 i->SETTINGS[ SETTING_SMP_ADSR_R	]	& 	0xF;	// 4
	ret.KITINDEX  =			 i->SETTINGS[ SETTING_SMP_KITINDEX  ];				// 4
	ret.B 		  = 		 i->SETTINGS[ SETTING_SMP_B			];				// 4
	ret.S 		  = 		 i->SETTINGS[ SETTING_SMP_S			];				// 4
	ret.R 		  = 		 i->SETTINGS[ SETTING_SMP_R			];				// 4
	// TODO: Reload SMPKIT pointers (12)
	return ret;
}

SETTINGS_WAV InstEdit::unpackWav( Instrument *i ){
	SETTINGS_WAV ret;
	int c, di;
	ret.PHASE			= EXTRACT(i->SETTINGS[0], 6, 0x1);	// 1
	ret.AM				= EXTRACT(i->SETTINGS[0], 5, 0x1);	// 1
	ret.DISTORTION		= i->SETTINGS[0] & 0xF;				// 4
	ret.OP1_TYPE		= EXTRACT(i->SETTINGS[1], 3, 0x7);	// 3
	ret.OP2_TYPE		= i->SETTINGS[1] & 0x7;				// 3
	ret.OP3_TYPE		= EXTRACT(i->SETTINGS[2], 3, 0x7);	// 3
	ret.OP4_TYPE		= i->SETTINGS[2] & 0x7;				// 3			
	for(c=0; c<4; c++){
		di = c<<1;
		ret.OP1_ADSR[c] = EXTRACT(i->SETTINGS[3+di], 4, 0xF);	// 4
		ret.OP2_ADSR[c] = i->SETTINGS[3+di] & 0xF;				// 4
		ret.OP3_ADSR[c] = EXTRACT(i->SETTINGS[4+di], 4, 0xF);	// 4
		ret.OP4_ADSR[c] = i->SETTINGS[4+di] & 0xF;				// 4
	}			
	for(c=0; c<8; c++){
		di = c<<1;
		ret.WAVEDATA[ di ] = EXTRACT(i->SETTINGS[11 + c], 4, 0xF);	// 4x8
		ret.WAVEDATA[di+1] = i->SETTINGS[11 + c] & 0xF;				// 4x8
	}
	ret.OP1_GATE = EXTRACT(i->SETTINGS[SETTING_WAV_OP1_GATE], 4, 0xF);	// 4
	ret.OP2_GATE = 		   i->SETTINGS[SETTING_WAV_OP2_GATE] & 0xF;				// 4
	ret.OP3_GATE = EXTRACT(i->SETTINGS[SETTING_WAV_OP3_GATE], 4, 0xF);	// 4
	ret.OP4_GATE = 		   i->SETTINGS[SETTING_WAV_OP4_GATE] & 0xF;				// 4
	return ret;
}

SETTINGS_PWM InstEdit::unpackPwm( Instrument *i ){
	SETTINGS_PWM ret;
	ret.SWEEPDIR 		= EXTRACT(i->SETTINGS[0], 7, 0x1);
	ret.ENVELOPEDIR		= EXTRACT(i->SETTINGS[0], 6, 0x1);
	ret.TSP_ENABLE		= EXTRACT(i->SETTINGS[0], 5, 0x1);
	ret.VOL_ENABLE		= EXTRACT(i->SETTINGS[0], 4, 0x1);
	ret.DUTYCYCLE 		= EXTRACT(i->SETTINGS[0], 2, 0x3);
	ret.TSP_LOOP		= i->SETTINGS[0] & 0x3;
	ret.VOL_LOOP		= i->SETTINGS[1] & 0x3;
	ret.LENGTH 			= EXTRACT(i->SETTINGS[2], 4, 0xF);
	ret.LEVEL			= i->SETTINGS[2] & 0xF;
	ret.SWEEPSPEED 		= i->SETTINGS[3] & 0xF;
	ret.SWEEPSTEPS 		= EXTRACT(i->SETTINGS[4], 4, 0xF);
	ret.VOLUMEFADE		= i->SETTINGS[4] & 0xF;
	ret.VOL_ENVELOPE	= i->SETTINGS[7];
	ret.TSP_ENVELOPE	= i->SETTINGS[8];
	ret.TSP_POSITION	= 0;// TRANSIENT - remove from struct at SRAM
	ret.VOL_POSITION	= 0;// TRANSIENT - remove from struct at SRAM
	ret.TSP_SPEED		= ( ret.TSP_ENVELOPE & 0xF0) >> 4;
	ret.VOL_SPEED		= ( ret.VOL_ENVELOPE & 0xF0) >> 4;
	ret.TSP_LENGTH		= ( ret.TSP_ENVELOPE & 0x0F);
	ret.VOL_LENGTH		= ( ret.VOL_ENVELOPE & 0x0F);
	for(int c=0; c<0x10; c++){
		ret.TSP[c]		= EXTRACT(i->SETTINGS[9+c], 4, 0xF);
		ret.VOL[c]		= i->SETTINGS[9+c] & 0xF;
	}
	return ret;
}	

void InstEdit::unpack( Instrument *i ){
	switch(i->TYPE){
		case INSTRUMENT_TYPE_PWM:	VAR_PWM = unpackPwm( i ); return;
		case INSTRUMENT_TYPE_WAV:	VAR_WAV = unpackWav( i ); return;
		case INSTRUMENT_TYPE_FMW:	VAR_FMW = unpackFmw( i ); return;
		case INSTRUMENT_TYPE_SMP:	VAR_SMP = unpackSmp( i ); return;
	}
}

void InstEdit::copy(Instrument *s, Instrument *d){
	int i;
	
	d->TYPE = s->TYPE;
	for(i=0; i<6; i++){
		d->NAME[i] = s->NAME[i];
	}
	for(i=0; i<SETTINGS_SIZE; i++){
		d->SETTINGS[i] = s->SETTINGS[i];
	}
	
	for(i=0; i<0x10; i++){
		d->TABLE.TRANSPOSE[i] 	= s->TABLE.TRANSPOSE[i];
		d->TABLE.VOLUME[i] 		= s->TABLE.VOLUME[i];
		d->TABLE.COMMAND[0][i] 	= s->TABLE.COMMAND[0][i];
		d->TABLE.COMMAND[1][i] 	= s->TABLE.COMMAND[1][i];
		d->TABLE.VALUE[0][i] 	= s->TABLE.VALUE[0][i];
		d->TABLE.VALUE[1][i] 	= s->TABLE.VALUE[1][i];
	}
	
	d->VISPOSITION[0] = s->VISPOSITION[0];
	d->VISPOSITION[1] = s->VISPOSITION[1];
	
}

// Keeps instrument copy updated

static u8 last_index = 63;
static u8 last_type	 = 0;
	

void InstEdit::synchronize(){
	if ( last_index != VAR_CFG.CURRENTINSTRUMENT ) return;
	
	InstEdit::copy(&VAR_INSTRUMENTS[VAR_CFG.CURRENTINSTRUMENT], &VAR_INSTRUMENT);
	InstEdit::unpack( &VAR_INSTRUMENT );
	last_index 	= VAR_CFG.CURRENTINSTRUMENT;
	last_type 	= VAR_INSTRUMENT.TYPE; 
	
	//RegionHandler::redraw = true;
}

void InstEdit::dispatchMessage( u32 msg, u32 pointer ) {


	switch(msg) {

		/* Navigation */
		case MESSAGE_NAVIGATE_LEFT	: dispatchMessage(MESSAGE_EXIT); RegionHandler::load( ((Region*)pointer)->left ); break;
		case MESSAGE_NAVIGATE_RIGHT	: dispatchMessage(MESSAGE_EXIT); RegionHandler::load( ((Region*)pointer)->right); break;
		case MESSAGE_NAVIGATE_DOWN	: dispatchMessage(MESSAGE_EXIT); RegionHandler::load( ((Region*)pointer)->down ); break;
		case MESSAGE_NAVIGATE_UP	: dispatchMessage(MESSAGE_EXIT); RegionHandler::load( ((Region*)pointer)->up   ); break;
		case MESSAGE_ENTER			: enter	(); break;
		case MESSAGE_EXIT			: exit	(); break;
	
		case MESSAGE_CANCEL:			
			RegionHandler::controlClear	( RegionHandler::control );
			repack();//dispatchMessage		( MESSAGE_OTHER_REFRESH_DATA );
			break;
		
		case MESSAGE_OTHER_REFRESH_DATA:
			repack();
			break;
		
		case MESSAGE_OTHER_PREV:// B + <-
			repack();
			VAR_CFG.CURRENTINSTRUMENT--;
			VAR_CFG.CURRENTINSTRUMENT &= 0x3f;
			if(VAR_CFG.CURRENTINSTRUMENT == 0) VAR_CFG.CURRENTINSTRUMENT=1;
			InstEdit::load();
			return;
		
		case MESSAGE_OTHER_NEXT:// B + ->	
			repack();
			VAR_CFG.CURRENTINSTRUMENT++;
			VAR_CFG.CURRENTINSTRUMENT &= 0x3f;
			if(VAR_CFG.CURRENTINSTRUMENT == 0) VAR_CFG.CURRENTINSTRUMENT=1;
			InstEdit::load();
			return;
	}	
}

bool display_waveform_mode = false;

void InstEdit::updateWav(  ){
	const Control *cname = &INS_WAV_CONTROLS[CONTROL_INS_WAV_NAME];
	STRING(false, cname->x, cname->y, cname->var);
	
	static int adsr_position;
	if( VAR_CFG.CURRENTINSTRUMENT == VAR_CHANNEL[ CHANNEL_WAV ].inst ) {
		if( KEYDOWN_SELECT ) {
			display_waveform_mode ^= 1;
			VirtualScreen::clear();
			VirtualScreen::draw(14,6);
			for(int i=0;i<32;i++){
				waveform[i].disable = !display_waveform_mode;
			} 
		}
		if( display_waveform_mode ) viewWaveFormWav( );
		else {//if(( adsr_position != WAV_ADSR_POSITION )||(!Sequencer::playing)){
			adsr_position = WAV_ADSR_POSITION;
			Adsr::drawX4( Adsr::wav_table, adsr_position);
		}
	}
	
	
	InstEdit::synchronize();
}


void InstEdit::updateFmw(  ){
	const Control *cname = &INS_FMW_CONTROLS[CONTROL_INS_FMW_NAME];
	STRING(false, cname->x, cname->y, cname->var);
	
	
	DECIMAL(6, 15, 0xAA, InstEdit::unpackFmw( &VAR_INSTRUMENT ).ALGORITHM );
	
	static int adsr_position;
	if( VAR_CFG.CURRENTINSTRUMENT == VAR_CHANNEL[ CHANNEL_FMW ].inst ) 
	if( adsr_position != FMW_ADSR_POSITION ){
		adsr_position = FMW_ADSR_POSITION;
		if( KEYDOWN_SELECT ) {
			display_waveform_mode ^= 1;
			VirtualScreen::clear();
			VirtualScreen::draw(14,6);
			for(int i=0;i<32;i++){
				waveform[i].disable = !display_waveform_mode;
			}
		}
		if( display_waveform_mode ) viewWaveFormFmw();
		else {//if(( adsr_position != FMW_ADSR_POSITION)||(!Sequencer::playing)){
			adsr_position = FMW_ADSR_POSITION;
			Adsr::drawX4( Adsr::fmw_table, adsr_position);
		}
	}
	
	InstEdit::synchronize();
}

void InstEdit::updateSmp(  ){
	const Control *cname = &INS_SMP_CONTROLS[CONTROL_INS_SMP_NAME];
	STRING(false, cname->x, cname->y, cname->var);
	
	static int adsr_position;
	if( VAR_CFG.CURRENTINSTRUMENT == VAR_CHANNEL[ CHANNEL_SMP ].inst ) 
	if( adsr_position != SMP_ADSR_POSITION ){
		adsr_position = SMP_ADSR_POSITION;
		if( KEYDOWN_SELECT ) {
			display_waveform_mode ^= 1;
			VirtualScreen::clear();
			VirtualScreen::draw(14,6);
			for(int i=0;i<32;i++){
				waveform[i].disable = !display_waveform_mode;
			}
		}
		if( display_waveform_mode ) viewWaveFormSmp();
		else {//if(( adsr_position != SMP_ADSR_POSITION )||(!Sequencer::playing)){
			adsr_position = SMP_ADSR_POSITION;
			Adsr::draw( Adsr::smp_table, adsr_position );
		}
	}
	
	InstEdit::synchronize();
}

void InstEdit::updatePwm(  ){
	const Control *cname = &INS_PWM_CONTROLS[CONTROL_INS_PWM_NAME];
	STRING(false, cname->x, cname->y, cname->var);
	InstEdit::synchronize();
}

//------------------------------------------------------------------------------------------------------------
// MULTI-PARAMETER TRIGGER CALLBACKS (called after a button push or a value is modified in some special cases)
//------------------------------------------------------------------------------------------------------------

void InstEdit::modAdsrWav( Control *c, bool bigstep, bool add, u32 *pointer ){ 
	modify4BIT( c, bigstep, add, pointer ); 
	InstEdit::repack(); 
	SETTINGS_WAV wav = unpackWav( &VAR_INSTRUMENT ); 
	Adsr::updateWav( &wav ); 
}

void InstEdit::modAdsrFmw( Control *c, bool bigstep, bool add, u32 *pointer ){ 
	modify4BIT(c,bigstep, add, pointer); 
	InstEdit::repack(); 
	SETTINGS_FMW fmw = unpackFmw( &VAR_INSTRUMENT ); 
	Adsr::updateFmw( &fmw );
}

void InstEdit::modAdsrSmp( Control *c, bool bigstep, bool add, u32 *pointer ){ 
	modify4BIT(c,bigstep, add, pointer); 
	InstEdit::repack(); 
	SETTINGS_SMP smp = unpackSmp( &VAR_INSTRUMENT ); 
	Adsr::updateSmp( &smp ); 
}


const SETTINGS_WAV presets[6] = {
//	{ PHASE	, AM	, DISTORTION, OP1_TYPE	, OP1_ADSR[4]			, OP2_TYPE	, OP2_ADSR[4]			, OP3_TYPE	, OP3_ADSR[4]			, OP4_TYPE	, OP4_ADSR[4]			, WAVEDATA[16]																			, OP1_GATE	, OP2_GATE 	, OP3_GATE	, OP4_GATE 
	{ false	, false	, 0x0		, 0			, { 0xF, 0xF, 0xF, 0xE }, 0 		, { 0x0, 0xF, 0x0, 0xF }, 1 		, { 0xF, 0x0, 0xF, 0x0 }, 5			, { 0xF, 0x0, 0x8, 0x0 }, { 0x0, 0x0, 0x0, 0x0,  0x0, 0x0, 0x0, 0x0,  0x0, 0x0, 0x0, 0x0,  0x0, 0x0, 0x0, 0x0 }	, 2			, 4			, 0x4		, 12		},
	{ true	, false	, 0x2		, 1			, { 0xF, 0xF, 0xF, 0xE }, 5 		, { 0x0, 0xF, 0x0, 0xF }, 2 		, { 0xF, 0x0, 0xF, 0x0 }, 4			, { 0x0, 0xF, 0x0, 0x8 }, { 0x0, 0x0, 0x0, 0x0,  0x0, 0x0, 0x0, 0x0,  0x0, 0x0, 0x0, 0x0,  0x0, 0x0, 0x0, 0x0 }	, 4			, 0xB		, 8			, 12		},
	{ true	, false	, 0x4		, 2			, { 0xF, 0xF, 0xF, 0xE }, 4 		, { 0x0, 0xF, 0x0, 0xF }, 3 		, { 0xF, 0x0, 0xF, 0x0 }, 3			, { 0xF, 0x0, 0x8, 0x0 }, { 0x0, 0x0, 0x0, 0x0,  0x0, 0x0, 0x0, 0x0,  0x0, 0x0, 0x0, 0x0,  0x0, 0x0, 0x0, 0x0 }	, 2			, 4			, 8			, 0xF 		},
	{ true	, true	, 0x6		, 3			, { 0xF, 0xF, 0xF, 0xE }, 3 		, { 0x0, 0xF, 0x0, 0xF }, 4 		, { 0xF, 0x0, 0xF, 0x0 }, 2			, { 0x0, 0xF, 0x0, 0x6 }, { 0x0, 0x0, 0x0, 0x0,  0x0, 0x0, 0x0, 0x0,  0x0, 0x0, 0x0, 0x0,  0x0, 0x0, 0x0, 0x0 }	, 8			, 4			, 0xA		, 12		},
	{ false	, true	, 0x8		, 4			, { 0xF, 0xF, 0xF, 0xE }, 2 		, { 0x0, 0xF, 0x0, 0xF }, 5 		, { 0xF, 0x0, 0xF, 0x0 }, 1			, { 0xF, 0x0, 0x8, 0x0 }, { 0x0, 0x0, 0x0, 0x0,  0x0, 0x0, 0x0, 0x0,  0x0, 0x0, 0x0, 0x0,  0x0, 0x0, 0x0, 0x0 }	, 2			, 0xC		, 8			, 12		},
	{ false	, true	, 0xa		, 5			, { 0xF, 0xF, 0xF, 0xE }, 1 		, { 0x0, 0xF, 0x0, 0xF }, 0 		, { 0xF, 0x0, 0xF, 0x0 }, 0			, { 0x0, 0xF, 0x0, 0x4 }, { 0x0, 0x0, 0x0, 0x0,  0x0, 0x0, 0x0, 0x0,  0x0, 0x0, 0x0, 0x0,  0x0, 0x0, 0x0, 0x0 }	, 0xF		, 4			, 8			, 12		},
};
	

void InstEdit::wavPreset(Control *c, bool bigstep, bool add, u32 *pointer){
	#define MATCH( match ) 	( c == &INS_WAV_CONTROLS[ CONTROL_INS_WAV_##match ] ) 
	int index 	= MATCH( WAVPRESET_00 ) ? 0x0 
				: MATCH( WAVPRESET_01 ) ? 0x1 
				: MATCH( WAVPRESET_02 ) ? 0x2 
				: MATCH( WAVPRESET_03 ) ? 0x3 
				: MATCH( WAVPRESET_04 ) ? 0x4 
				: 0x05;
	#undef MATCH

	const SETTINGS_WAV *preset = &presets[ index ];
	
	VAR_WAV.PHASE		 = preset->PHASE;
	VAR_WAV.AM			 = preset->AM;
	VAR_WAV.DISTORTION	 = preset->DISTORTION;
	VAR_WAV.OP1_TYPE	 = preset->OP1_TYPE;
	VAR_WAV.OP2_TYPE	 = preset->OP2_TYPE;
	VAR_WAV.OP3_TYPE	 = preset->OP3_TYPE;
	VAR_WAV.OP4_TYPE	 = preset->OP4_TYPE;
	VAR_WAV.OP1_GATE	 = preset->OP1_GATE;
	VAR_WAV.OP2_GATE	 = preset->OP2_GATE;
	VAR_WAV.OP3_GATE	 = preset->OP3_GATE;	
	VAR_WAV.OP4_GATE 	 = preset->OP4_GATE; 	
	for(int i=0; i<4; i++){
		VAR_WAV.OP1_ADSR[i]	 = preset->OP1_ADSR[i];
		VAR_WAV.OP2_ADSR[i]	 = preset->OP2_ADSR[i];
		VAR_WAV.OP3_ADSR[i]	 = preset->OP3_ADSR[i];
		VAR_WAV.OP4_ADSR[i]	 = preset->OP4_ADSR[i];
	}
	for(int i=0; i<16; i++){
		VAR_WAV.WAVEDATA[i] = preset->WAVEDATA[i];
	}
	InstEdit::repack(); 
	RegionHandler::redraw = true;
	Control *focus = RegionHandler::control;
	RegionHandler::update(0);
	RegionHandler::control = focus;
	synchronize(); 
}

void InstEdit::smpPreset(Control *c, bool bigstep, bool add, u32 *pointer){
	#define MATCH( match ) 	( c == &INS_SMP_CONTROLS[ CONTROL_INS_SMP_##match ] ) 
	SETTINGS_SMP smp = unpackSmp( &VAR_INSTRUMENT );
	int offset  = smp.KIT*0xC;
	int index 	= MATCH( SMPKIT_00 ) ? 0x0 
				: MATCH( SMPKIT_01 ) ? 0x1 
				: MATCH( SMPKIT_02 ) ? 0x2 
				: MATCH( SMPKIT_03 ) ? 0x3 
				: MATCH( SMPKIT_04 ) ? 0x4 
				: MATCH( SMPKIT_05 ) ? 0x5 
				: MATCH( SMPKIT_06 ) ? 0x6 
				: MATCH( SMPKIT_07 ) ? 0x7 
				: MATCH( SMPKIT_08 ) ? 0x8 
				: MATCH( SMPKIT_09 ) ? 0x9 
				: MATCH( SMPKIT_0A ) ? 0xA 
				: 0x0B;
	#undef MATCH
	DECIMAL_DOUBLE( 1, 2, 0x33, index + offset);
	//BREAK
	VAR_SMP.KITINDEX = index;
	InstEdit::pack(&VAR_INSTRUMENT);
	repack();
}

void InstEdit::viewWaveFormFmw( ){
	
	const u16 center = 40+((15*8)<<3);
	
	for(int i=0; i<16; i++){
		u8 v = VAR_FMW.WAVEDATA[i]>>3;
		waveform[ i  ].position.y = center - (( ( v ) * 8 ) <<3);
		waveform[i+16].position.y = center - (( ( v ) * 8 ) <<3);
	}

}

void InstEdit::viewWaveFormSmp( ){

}

void InstEdit::viewWaveFormWav( ){
	
	const u16 center = 40 + ((2*8)<<3);
	
	for(int i=0; i<16; i++){
		u8 v = VAR_WAV.WAVEDATA[i];
		waveform[  i ].position.y = center + (( ( v ) * 8 ) <<3 );
		waveform[i+16].position.y = center + (( ( v ) * 8 ) <<3 );
	}
}

void InstEdit::clear( u8 index ){
	Instrument *instrument = &VAR_INSTRUMENTS[ index ];
	instrument->VISPOSITION	[0] = 0;
	instrument->VISPOSITION	[1] = 0;
	instrument->NAME		[0] = 0;
	instrument->NAME		[1] = 0;
	instrument->NAME		[2] = 0;
	instrument->NAME		[3] = 0;
	instrument->NAME		[4] = 0;
	instrument->NAME		[5] = 0;
	instrument->TYPE			= INSTRUMENT_TYPE_PWM;
	u8 *settings = instrument->SETTINGS;
	for(u8* target = settings + SETTINGS_SIZE; settings<target; settings++){
		*settings = 0x00;
	}
	TableCell *table = &instrument->TABLE;
	u8 *t 	= table->TRANSPOSE;
	u8 *v 	= table->VOLUME;
	u8 *c1	= table->COMMAND[0];
	u8 *a1	= table->VALUE[0];
	u8 *c2	= table->COMMAND[1];
	u8 *a2	= table->VALUE[1];
	for(int i=0; i<0x10; i++, t++, v++, c1++, a1++, c2++, a2++){
		t 		= 0x00;
		v 		= 0x00;
		c1		= 0x00;
		c2	 	= 0x00;
		a1		= 0x00;
		a2	 	= 0x00;
	}
	table->PLAYING		= false;
	table->JUMP[ 0 ]	= 0x0;
	table->JUMP[ 1 ]	= 0x0;
	table->POSITION[ 0 ]= 0x0;
	table->POSITION[ 1 ]= 0x0;
	
	InstEdit::pack( instrument );
	InstEdit::copy( instrument, &VAR_INSTRUMENT );
}

void InstEdit::enter(){
	for(int i=0; i<32; i++){
		InstEdit::waveform[i].disable = !display_waveform_mode;
		InstEdit::waveform[i].render();
	}
}

void InstEdit::exit(){
	/* disable waveform */
	for(int i=0; i<32; i++){
		InstEdit::waveform[i].disable = true;
		InstEdit::waveform[i].render();
	}
	
}


