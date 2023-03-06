#include "ins.hpp"

// General Callbacks
CALLBACK( cb_ins_index		, InstEdit::index		, EVENT_MODIFY_B 	, &VAR_CFG.CURRENTINSTRUMENT	, NULL);
CALLBACK( cb_ins_name		, ALPHA6				, EVENT_KEYDOWN_B 	, &VAR_INSTRUMENT.NAME			, NULL);
CALLBACK( cb_ins_type		, InstEdit::type		, EVENT_MODIFY_B 	, &VAR_INSTRUMENT.TYPE			, NULL);
// PWM Controls
CALLBACK( cb_ins_volumefade	, InstEdit::modNibble	, EVENT_MODIFY_B 	, &VAR_PWM.VOLUMEFADE			, NULL);
CALLBACK( cb_ins_tspenvelope, InstEdit::modByte		, EVENT_MODIFY_B 	, &VAR_PWM.TSP_ENVELOPE			, NULL);
CALLBACK( cb_ins_tspmode	, InstEdit::mod2b		, EVENT_MODIFY_B 	, &VAR_PWM.TSP_LOOP 		 	, NULL);
CALLBACK( cb_ins_tspenable	, InstEdit::modBit		, EVENT_KEYDOWN_B 	, &VAR_PWM.TSP_ENABLE	 		, NULL);
CALLBACK( cb_ins_volenable	, InstEdit::modBit		, EVENT_KEYDOWN_B 	, &VAR_PWM.VOL_ENABLE	 		, NULL);
CALLBACK( cb_ins_volmode	, InstEdit::mod2b		, EVENT_MODIFY_B 	, &VAR_PWM.VOL_LOOP 		 	, NULL);
CALLBACK( cb_ins_volenvelope, InstEdit::modByte		, EVENT_MODIFY_B 	, &VAR_PWM.VOL_ENVELOPE			, NULL);
CALLBACK( cb_ins_dutycycle	, InstEdit::mod2b		, EVENT_MODIFY_B 	, &VAR_PWM.DUTYCYCLE		 	, NULL);
CALLBACK( cb_ins_length		, InstEdit::modNibble	, EVENT_MODIFY_B 	, &VAR_PWM.LENGTH		 		, NULL);
CALLBACK( cb_ins_sweepdir	, InstEdit::modBit		, EVENT_KEYDOWN_B	, &VAR_PWM.SWEEPDIR		 		, NULL);
CALLBACK( cb_ins_sweepspeed	, InstEdit::modNibble	, EVENT_MODIFY_B	, &VAR_PWM.SWEEPSPEED	 		, NULL);
CALLBACK( cb_ins_sweepsteps , InstEdit::modNibble	, EVENT_MODIFY_B	, &VAR_PWM.SWEEPSTEPS	 		, NULL);
CALLBACK( cb_ins_pwm_level	, InstEdit::modNibble	, EVENT_MODIFY_B	, &VAR_PWM.LEVEL				, NULL);
CALLBACK( cb_ins_envelopedir, InstEdit::modBit		, EVENT_KEYDOWN_B	, &VAR_PWM.ENVELOPEDIR	 		, NULL);
// PWM Tables
// Repeat 16 times
#define CB_INS_TSP(a)		CALLBACK( cb_ins_tsp_0##a , InstEdit::modNibble, EVENT_MODIFY_B, &VAR_PWM.TSP[0x##a], NULL) 
CB_INS_TSP(0);			CB_INS_TSP(1);			CB_INS_TSP(2);			CB_INS_TSP(3);
CB_INS_TSP(4);			CB_INS_TSP(5);			CB_INS_TSP(6);			CB_INS_TSP(7);
CB_INS_TSP(8);			CB_INS_TSP(9);			CB_INS_TSP(A);			CB_INS_TSP(B);
CB_INS_TSP(C);			CB_INS_TSP(D);			CB_INS_TSP(E);			CB_INS_TSP(F);
#undef CB_INS_TSP

#define CB_INS_VOL(a)		CALLBACK( cb_ins_vol_0##a , InstEdit::modNibble, EVENT_MODIFY_B, &VAR_PWM.VOL[0x##a], NULL) 
CB_INS_VOL(0);			CB_INS_VOL(1);			CB_INS_VOL(2);			CB_INS_VOL(3);
CB_INS_VOL(4);			CB_INS_VOL(5);			CB_INS_VOL(6);			CB_INS_VOL(7);
CB_INS_VOL(8);			CB_INS_VOL(9);			CB_INS_VOL(A);			CB_INS_VOL(B);
CB_INS_VOL(C);			CB_INS_VOL(D);			CB_INS_VOL(E);			CB_INS_VOL(F);
#undef CB_INS_VOL

// VAR WAV
CALLBACK( cb_ins_phase		, InstEdit::modBit	, EVENT_KEYDOWN_B 	, &VAR_WAV.PHASE	 			, NULL);
CALLBACK( cb_ins_am			, InstEdit::modBit	, EVENT_KEYDOWN_B 	, &VAR_WAV.AM		 			, NULL);
CALLBACK( cb_ins_mixpercent	, InstEdit::mod3b	, EVENT_MODIFY_B 	, &VAR_WAV.MIXPERCENT 			, NULL);
CALLBACK( cb_ins_wav_op1type, InstEdit::mod3b	, EVENT_MODIFY_B 	, &VAR_WAV.OP1_TYPE				, NULL);
CALLBACK( cb_ins_wav_op2type, InstEdit::mod3b	, EVENT_MODIFY_B 	, &VAR_WAV.OP2_TYPE				, NULL);
CALLBACK( cb_ins_wav_op3type, InstEdit::mod3b	, EVENT_MODIFY_B 	, &VAR_WAV.OP3_TYPE				, NULL);
CALLBACK( cb_ins_wav_op4type, InstEdit::mod3b	, EVENT_MODIFY_B 	, &VAR_WAV.OP4_TYPE				, NULL);

#define CB_INS_WAV_OP1ADSR(a)		CALLBACK( cb_ins_wav_op1adsr_##a , InstEdit::modAdsrWav, EVENT_MODIFY_B , &VAR_WAV.OP1_ADSR[a], NULL)
#define CB_INS_WAV_OP2ADSR(a)		CALLBACK( cb_ins_wav_op2adsr_##a , InstEdit::modAdsrWav, EVENT_MODIFY_B , &VAR_WAV.OP2_ADSR[a], NULL)
#define CB_INS_WAV_OP3ADSR(a)		CALLBACK( cb_ins_wav_op3adsr_##a , InstEdit::modAdsrWav, EVENT_MODIFY_B , &VAR_WAV.OP3_ADSR[a], NULL)
#define CB_INS_WAV_OP4ADSR(a)		CALLBACK( cb_ins_wav_op4adsr_##a , InstEdit::modAdsrWav, EVENT_MODIFY_B , &VAR_WAV.OP4_ADSR[a], NULL)
CB_INS_WAV_OP1ADSR(0);	CB_INS_WAV_OP2ADSR(0);	CB_INS_WAV_OP3ADSR(0);	CB_INS_WAV_OP4ADSR(0);
CB_INS_WAV_OP1ADSR(1);	CB_INS_WAV_OP2ADSR(1);	CB_INS_WAV_OP3ADSR(1);	CB_INS_WAV_OP4ADSR(1);
CB_INS_WAV_OP1ADSR(2);	CB_INS_WAV_OP2ADSR(2);	CB_INS_WAV_OP3ADSR(2);	CB_INS_WAV_OP4ADSR(2);
CB_INS_WAV_OP1ADSR(3);	CB_INS_WAV_OP2ADSR(3);	CB_INS_WAV_OP3ADSR(3);	CB_INS_WAV_OP4ADSR(3);
#undef CB_INS_WAV_OP1ADSR
#undef CB_INS_WAV_OP2ADSR
#undef CB_INS_WAV_OP3ADSR
#undef CB_INS_WAV_OP4ADSR

#define CB_INS_WAV_PRESET(a)		CALLBACK( cb_ins_wav_preset##a , InstEdit::wavPreset, EVENT_KEYUP_B, &VAR_WAV.WAVPRESET[a] , NULL)
CB_INS_WAV_PRESET(0);
CB_INS_WAV_PRESET(1);
CB_INS_WAV_PRESET(2);
CB_INS_WAV_PRESET(3);
CB_INS_WAV_PRESET(4);
CB_INS_WAV_PRESET(5);
#undef CB_INS_WAV_PRESET

//VAR_FMW
CALLBACK( cb_ins_algorithm	, InstEdit::modAlgorithm, EVENT_MODIFY_B 	, &VAR_FMW.ALGORITHM			, NULL);
CALLBACK( cb_ins_fm_mult	, InstEdit::modNibble	, EVENT_MODIFY_B 	, &VAR_FMW.MULT					, NULL);
CALLBACK( cb_ins_fm_op1type	, InstEdit::mod3b		, EVENT_MODIFY_B 	, &VAR_FMW.OP1_TYPE				, NULL);
CALLBACK( cb_ins_fm_op2type	, InstEdit::mod3b		, EVENT_MODIFY_B 	, &VAR_FMW.OP2_TYPE				, NULL);
CALLBACK( cb_ins_fm_op3type	, InstEdit::mod3b		, EVENT_MODIFY_B 	, &VAR_FMW.OP3_TYPE				, NULL);
CALLBACK( cb_ins_fm_op4type	, InstEdit::mod3b		, EVENT_MODIFY_B 	, &VAR_FMW.OP4_TYPE				, NULL);

#define CB_INS_FM_OP1ADSR(a)		CALLBACK( cb_ins_fm_op1adsr_##a , InstEdit::modAdsrFmw , EVENT_MODIFY_B , &VAR_FMW.OP1_ADSR[a], NULL)
#define CB_INS_FM_OP2ADSR(a)		CALLBACK( cb_ins_fm_op2adsr_##a , InstEdit::modAdsrFmw , EVENT_MODIFY_B , &VAR_FMW.OP2_ADSR[a], NULL)
#define CB_INS_FM_OP3ADSR(a)		CALLBACK( cb_ins_fm_op3adsr_##a , InstEdit::modAdsrFmw , EVENT_MODIFY_B , &VAR_FMW.OP3_ADSR[a], NULL)
#define CB_INS_FM_OP4ADSR(a)		CALLBACK( cb_ins_fm_op4adsr_##a , InstEdit::modAdsrFmw , EVENT_MODIFY_B , &VAR_FMW.OP4_ADSR[a], NULL)
CB_INS_FM_OP1ADSR(0);	CB_INS_FM_OP2ADSR(0);	CB_INS_FM_OP3ADSR(0);	CB_INS_FM_OP4ADSR(0);
CB_INS_FM_OP1ADSR(1);	CB_INS_FM_OP2ADSR(1);	CB_INS_FM_OP3ADSR(1);	CB_INS_FM_OP4ADSR(1);
CB_INS_FM_OP1ADSR(2);	CB_INS_FM_OP2ADSR(2);	CB_INS_FM_OP3ADSR(2);	CB_INS_FM_OP4ADSR(2);
CB_INS_FM_OP1ADSR(3);	CB_INS_FM_OP2ADSR(3);	CB_INS_FM_OP3ADSR(3);	CB_INS_FM_OP4ADSR(3);
#undef CB_INS_FM_OP1ADSR
#undef CB_INS_FM_OP2ADSR
#undef CB_INS_FM_OP3ADSR
#undef CB_INS_FM_OP4ADSR

//VAR_SMP
CALLBACK( cb_ins_smp_start		, InstEdit::modByte		, EVENT_MODIFY_B 	, &VAR_SMP.START				, NULL);
CALLBACK( cb_ins_smp_end		, InstEdit::modByte		, EVENT_MODIFY_B 	, &VAR_SMP.END					, NULL);
CALLBACK( cb_ins_smp_frequency	, InstEdit::modByte		, EVENT_MODIFY_B 	, &VAR_SMP.FREQUENCY			, NULL);
CALLBACK( cb_ins_smp_b			, InstEdit::modNibble	, EVENT_MODIFY_B 	, &VAR_SMP.B					, NULL);
CALLBACK( cb_ins_smp_s			, InstEdit::modNibble	, EVENT_MODIFY_B 	, &VAR_SMP.S					, NULL);
CALLBACK( cb_ins_smp_r			, InstEdit::modNibble	, EVENT_MODIFY_B 	, &VAR_SMP.R					, NULL);
CALLBACK( cb_ins_smp_synthmode	, InstEdit::modBit		, EVENT_KEYDOWN_B 	, &VAR_SMP.SYNTHMODE			, NULL);
CALLBACK( cb_ins_smp_loop		, InstEdit::mod2b		, EVENT_MODIFY_B 	, &VAR_SMP.LOOP					, NULL);
CALLBACK( cb_ins_smp_kit		, InstEdit::modSmpKit	, EVENT_MODIFY_B 	, &VAR_SMP.KIT					, NULL);

#define CB_INS_SMPKIT(a)			CALLBACK( cb_ins_smpkit_0##a , InstEdit::smpPreset, EVENT_KEYUP_B, &VAR_SMP.SMPKIT[0x##a], NULL)
CB_INS_SMPKIT(0);		CB_INS_SMPKIT(1);		CB_INS_SMPKIT(2);		CB_INS_SMPKIT(3);
CB_INS_SMPKIT(4);		CB_INS_SMPKIT(5);		CB_INS_SMPKIT(6);		CB_INS_SMPKIT(7);
CB_INS_SMPKIT(8);		CB_INS_SMPKIT(9);		CB_INS_SMPKIT(A);		CB_INS_SMPKIT(B);
#undef CB_INS_SMPKIT

#define CB_INS_SMP_ADSR(a)			CALLBACK( cb_ins_smp_adsr_##a , InstEdit::modAdsrSmp , EVENT_MODIFY_B , &VAR_SMP.ADSR[a], NULL)
CB_INS_SMP_ADSR(0);		CB_INS_SMP_ADSR(1);		CB_INS_SMP_ADSR(2);		CB_INS_SMP_ADSR(3);
#undef CB_INS_SMP_ADSR

//TABLE 
#define CB_INS_TABLE_TRANSPOSE(a)	CALLBACK( cb_ins_table_transpose_0##a 	, InstEdit::modByte , EVENT_MODIFY_B, &VAR_INSTRUMENT.TABLE.TRANSPOSE[0x##a], NULL) 
CB_INS_TABLE_TRANSPOSE(0);
CB_INS_TABLE_TRANSPOSE(1);
CB_INS_TABLE_TRANSPOSE(2);
CB_INS_TABLE_TRANSPOSE(3);
CB_INS_TABLE_TRANSPOSE(4);
CB_INS_TABLE_TRANSPOSE(5);
CB_INS_TABLE_TRANSPOSE(6);
CB_INS_TABLE_TRANSPOSE(7);
CB_INS_TABLE_TRANSPOSE(8);
CB_INS_TABLE_TRANSPOSE(9);
CB_INS_TABLE_TRANSPOSE(A);
CB_INS_TABLE_TRANSPOSE(B);
CB_INS_TABLE_TRANSPOSE(C);
CB_INS_TABLE_TRANSPOSE(D);
CB_INS_TABLE_TRANSPOSE(E);
CB_INS_TABLE_TRANSPOSE(F);
#undef CB_INS_TABLE_TRANSPOSE

#define CB_INS_TABLE_VOLUME(a)		CALLBACK( cb_ins_table_volume_0##a 		, InstEdit::modNibble , EVENT_MODIFY_B, &VAR_INSTRUMENT.TABLE.VOLUME[0x##a], NULL) 
CB_INS_TABLE_VOLUME(0);
CB_INS_TABLE_VOLUME(1);
CB_INS_TABLE_VOLUME(2);
CB_INS_TABLE_VOLUME(3);
CB_INS_TABLE_VOLUME(4);
CB_INS_TABLE_VOLUME(5);
CB_INS_TABLE_VOLUME(6);
CB_INS_TABLE_VOLUME(7);
CB_INS_TABLE_VOLUME(8);
CB_INS_TABLE_VOLUME(9);
CB_INS_TABLE_VOLUME(A);
CB_INS_TABLE_VOLUME(B);
CB_INS_TABLE_VOLUME(C);
CB_INS_TABLE_VOLUME(D);
CB_INS_TABLE_VOLUME(E);
CB_INS_TABLE_VOLUME(F);
#undef CB_INS_TABLE_VOLUME


#define CB_INS_TABLE_VALUE1(a)		CALLBACK( cb_ins_table_value1_0##a 		, InstEdit::modByte , EVENT_MODIFY_B, &VAR_INSTRUMENT.TABLE.VALUE[0][0x##a], NULL) 
CB_INS_TABLE_VALUE1(0);
CB_INS_TABLE_VALUE1(1);
CB_INS_TABLE_VALUE1(2);
CB_INS_TABLE_VALUE1(3);
CB_INS_TABLE_VALUE1(4);
CB_INS_TABLE_VALUE1(5);
CB_INS_TABLE_VALUE1(6);
CB_INS_TABLE_VALUE1(7);
CB_INS_TABLE_VALUE1(8);
CB_INS_TABLE_VALUE1(9);
CB_INS_TABLE_VALUE1(A);
CB_INS_TABLE_VALUE1(B);
CB_INS_TABLE_VALUE1(C);
CB_INS_TABLE_VALUE1(D);
CB_INS_TABLE_VALUE1(E);
CB_INS_TABLE_VALUE1(F);
#undef CB_INS_TABLE_VALUE1

#define CB_INS_TABLE_COMMAND1(a)	CALLBACK( cb_ins_table_command1_0##a	, InstEdit::modVal27 , EVENT_MODIFY_B, &VAR_INSTRUMENT.TABLE.COMMAND[0][0x##a], NULL) 
CB_INS_TABLE_COMMAND1(0);
CB_INS_TABLE_COMMAND1(1);
CB_INS_TABLE_COMMAND1(2);
CB_INS_TABLE_COMMAND1(3);
CB_INS_TABLE_COMMAND1(4);
CB_INS_TABLE_COMMAND1(5);
CB_INS_TABLE_COMMAND1(6);
CB_INS_TABLE_COMMAND1(7);
CB_INS_TABLE_COMMAND1(8);
CB_INS_TABLE_COMMAND1(9);
CB_INS_TABLE_COMMAND1(A);
CB_INS_TABLE_COMMAND1(B);
CB_INS_TABLE_COMMAND1(C);
CB_INS_TABLE_COMMAND1(D);
CB_INS_TABLE_COMMAND1(E);
CB_INS_TABLE_COMMAND1(F);
#undef CB_INS_TABLE_COMMAND1

#define CB_INS_TABLE_VALUE2(a)		CALLBACK( cb_ins_table_value2_0##a 		, InstEdit::modByte , EVENT_MODIFY_B, &VAR_INSTRUMENT.TABLE.VALUE[1][0x##a], NULL) 
CB_INS_TABLE_VALUE2(0);
CB_INS_TABLE_VALUE2(1);
CB_INS_TABLE_VALUE2(2);
CB_INS_TABLE_VALUE2(3);
CB_INS_TABLE_VALUE2(4);
CB_INS_TABLE_VALUE2(5);
CB_INS_TABLE_VALUE2(6);
CB_INS_TABLE_VALUE2(7);
CB_INS_TABLE_VALUE2(8);
CB_INS_TABLE_VALUE2(9);
CB_INS_TABLE_VALUE2(A);
CB_INS_TABLE_VALUE2(B);
CB_INS_TABLE_VALUE2(C);
CB_INS_TABLE_VALUE2(D);
CB_INS_TABLE_VALUE2(E);
CB_INS_TABLE_VALUE2(F);
#undef CB_INS_TABLE_VALUE2

#define CB_INS_TABLE_COMMAND2(a)	CALLBACK( cb_ins_table_command2_0##a	, InstEdit::modVal27 , EVENT_MODIFY_B, &VAR_INSTRUMENT.TABLE.COMMAND[1][0x##a], NULL) 
CB_INS_TABLE_COMMAND2(0);
CB_INS_TABLE_COMMAND2(1);
CB_INS_TABLE_COMMAND2(2);
CB_INS_TABLE_COMMAND2(3);
CB_INS_TABLE_COMMAND2(4);
CB_INS_TABLE_COMMAND2(5);
CB_INS_TABLE_COMMAND2(6);
CB_INS_TABLE_COMMAND2(7);
CB_INS_TABLE_COMMAND2(8);
CB_INS_TABLE_COMMAND2(9);
CB_INS_TABLE_COMMAND2(A);
CB_INS_TABLE_COMMAND2(B);
CB_INS_TABLE_COMMAND2(C);
CB_INS_TABLE_COMMAND2(D);
CB_INS_TABLE_COMMAND2(E);
CB_INS_TABLE_COMMAND2(F);
#undef CB_INS_TABLE_COMMAND2



// Called on index change
void InstEdit::load(){	
	InstEdit::copy( &VAR_INSTRUMENTS[ VAR_CFG.CURRENTINSTRUMENT ], &VAR_INSTRUMENT );	
	InstEdit::unpack( &VAR_INSTRUMENT );
	regHnd.redraw = true;	
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
	transient6BIT 	 = VAR_CFG.CURRENTINSTRUMENT; 
	transientChanged = true;
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
			i->SETTINGS[0] = (VAR_WAV.PHASE<<6) | (VAR_WAV.AM<<5) | VAR_WAV.MIXPERCENT; 
			i->SETTINGS[1] = (VAR_WAV.OP1_TYPE<<3) | VAR_WAV.OP2_TYPE; 
			i->SETTINGS[2] = (VAR_WAV.OP3_TYPE<<3) | VAR_WAV.OP4_TYPE;			
			for(c=0; c<4; c++){
				di = c<<1;
				i->SETTINGS[3+di] = (VAR_WAV.OP1_ADSR[c]<<4) | VAR_WAV.OP2_ADSR[c]; 
				i->SETTINGS[4+di] = (VAR_WAV.OP3_ADSR[c]<<4) | VAR_WAV.OP4_ADSR[c]; 
			}
			for(c=0; c<8; c++){
				di = c<<1;
				i->SETTINGS[11+c] = (VAR_WAV.WAVEDATA[ di ]<<4) | VAR_WAV.WAVEDATA[di+1];
			}
			return;

		case INSTRUMENT_TYPE_FMW:
			i->SETTINGS[0] = (VAR_FMW.MULT << 3) | VAR_FMW.ALGORITHM;
			i->SETTINGS[1] = (VAR_FMW.OP1_TYPE<<3) | VAR_FMW.OP2_TYPE;
			i->SETTINGS[2] = (VAR_FMW.OP3_TYPE<<3) | VAR_FMW.OP4_TYPE;
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
			i->SETTINGS[0] = (VAR_SMP.START<<4) | VAR_SMP.END;
			i->SETTINGS[1] = (VAR_SMP.KIT<<3) | (VAR_SMP.LOOP<<1) | VAR_SMP.SYNTHMODE;
			i->SETTINGS[2] = VAR_SMP.FREQUENCY;
			i->SETTINGS[3] = (VAR_SMP.ADSR[0]<<4) | VAR_SMP.ADSR[1];
			i->SETTINGS[4] = (VAR_SMP.ADSR[2]<<4) | VAR_SMP.ADSR[3];
			i->SETTINGS[5] = (VAR_SMP.KITINDEX<<4) | VAR_SMP.B;
			i->SETTINGS[6] = (VAR_SMP.S<<4) | VAR_SMP.R;
			// TODO: Reload SMPKIT pointers (12)
			return;
	}
}

/* SYNCRONIZES Abstract Instrument data to current type specific data */

SETTINGS_FMW InstEdit::unpackFmw( Instrument *i ){
	SETTINGS_FMW ret;
	int c, di;
	ret.MULT 			= EXTRACT(i->SETTINGS[0], 3, 0xF);	// 4
	ret.ALGORITHM 		= i->SETTINGS[0] & 0x7;				// 3
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
	return ret;
}

SETTINGS_SMP InstEdit::unpackSmp( Instrument *i ){
	SETTINGS_SMP ret;
	ret.START 	  = EXTRACT(i->SETTINGS[0], 4, 0xF);	// 8
	ret.END 	  = i->SETTINGS[0] & 0xF;				// 8
	ret.KIT 	  = EXTRACT(i->SETTINGS[1], 3, 0x1F);	// 5
	ret.LOOP 	  = EXTRACT(i->SETTINGS[1], 1, 0x3);	// 2
	ret.SYNTHMODE = i->SETTINGS[1] & 0x1;				// 1
	ret.FREQUENCY = i->SETTINGS[2];						// 8
	ret.ADSR[0]   = EXTRACT(i->SETTINGS[3], 4, 0xF);	// 4
	ret.ADSR[1]   = i->SETTINGS[3] & 0xF;				// 4
	ret.ADSR[2]   = EXTRACT(i->SETTINGS[4], 4, 0xF);	// 4
	ret.ADSR[3]   = i->SETTINGS[4] & 0xF;				// 4
	ret.KITINDEX  = EXTRACT(i->SETTINGS[5], 4, 0xF);	// 4
	ret.B 		  = i->SETTINGS[5];						// 4
	ret.S 		  = EXTRACT(i->SETTINGS[6], 4, 0xF);	// 4
	ret.R 		  = i->SETTINGS[6];						// 4
	// TODO: Reload SMPKIT pointers (12)
	
	return ret;
}

SETTINGS_WAV InstEdit::unpackWav( Instrument *i ){
	SETTINGS_WAV ret;
	int c, di;
	ret.PHASE			= EXTRACT(i->SETTINGS[0], 6, 0x1);	// 1
	ret.AM				= EXTRACT(i->SETTINGS[0], 5, 0x1);	// 1
	ret.MIXPERCENT		= i->SETTINGS[0] & 0xF;				// 4
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
	ret.TSP_LENGTH		= EXTRACT(i->SETTINGS[3], 4, 0xF);
	ret.SWEEPSPEED 		= i->SETTINGS[3] & 0xF;
	ret.SWEEPSTEPS 		= EXTRACT(i->SETTINGS[4], 4, 0xF);
	ret.VOLUMEFADE		= i->SETTINGS[4] & 0xF;
	ret.TSP_POSITION	= EXTRACT(i->SETTINGS[5], 4, 0xF);
	ret.VOL_POSITION	= i->SETTINGS[5] & 0xF;
	ret.VOL_LENGTH		= EXTRACT(i->SETTINGS[6], 4, 0xF);
	ret.VOL_ENVELOPE	= i->SETTINGS[7];
	ret.TSP_ENVELOPE	= i->SETTINGS[8];
	for(int c=0; c<16; c++){
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
	for(i=0; i<32; i++){
		d->SETTINGS[i] = s->SETTINGS[i];
	}
	
	for(i=0; i<16; i++){
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
void InstEdit::synchronize(){
	static u8 LASTINSTRUMENT = 63;
	
	if(LASTINSTRUMENT != VAR_CFG.CURRENTINSTRUMENT){
		InstEdit::copy(&VAR_INSTRUMENTS[VAR_CFG.CURRENTINSTRUMENT], &VAR_INSTRUMENT);
		InstEdit::unpack( &VAR_INSTRUMENT );
		LASTINSTRUMENT = VAR_CFG.CURRENTINSTRUMENT;
		regHnd.redraw = true;
	} /*else {
		InstEdit::copy(&VAR_INSTRUMENT, &VAR_INSTRUMENTS[VAR_CFG.CURRENTINSTRUMENT]);
		InstEdit::pack( &VAR_INSTRUMENT );
	}*/
}

void InstEdit::dispatchMessage(u32 msg) {
	switch(msg) {
		case MESSAGE_OTHER_PREV:// B + <-
			VAR_CFG.CURRENTINSTRUMENT--;
			VAR_CFG.CURRENTINSTRUMENT &= 0x3f;
			if(VAR_CFG.CURRENTINSTRUMENT == 0) VAR_CFG.CURRENTINSTRUMENT=1;
			InstEdit::load();
			return;
		
		case MESSAGE_OTHER_NEXT:// B + ->	
			VAR_CFG.CURRENTINSTRUMENT++;
			VAR_CFG.CURRENTINSTRUMENT &= 0x3f;
			if(VAR_CFG.CURRENTINSTRUMENT == 0) VAR_CFG.CURRENTINSTRUMENT=1;
			InstEdit::load();
			return;
	}	
}

void InstEdit::updateWav( RegionHandler* rh ){
	const Control *cname = &INS_WAV_CONTROLS[CONTROL_INS_WAV_NAME];
	STRING(false, cname->x, cname->y, cname->var);
	InstEdit::synchronize();
}

void InstEdit::updateFmw( RegionHandler* rh ){
	const Control *cname = &INS_FMW_CONTROLS[CONTROL_INS_FMW_NAME];
	STRING(false, cname->x, cname->y, cname->var);
	DECIMAL(6, 15, 0xAA, InstEdit::unpackFmw( &VAR_INSTRUMENT ).ALGORITHM );
	static int recall = 0;
	recall++;
	HEXADECIMAL_DOUBLE(0,0,0xFF,recall);
	InstEdit::synchronize();
}

void InstEdit::updateSmp( RegionHandler* rh ){
	const Control *cname = &INS_SMP_CONTROLS[CONTROL_INS_SMP_NAME];
	STRING(false, cname->x, cname->y, cname->var);
	InstEdit::synchronize();
}

void InstEdit::updatePwm( RegionHandler* rh ){
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
	Synth::updateADSRWav( &wav ); 
}

void InstEdit::modAdsrFmw( Control *c, bool bigstep, bool add, u32 *pointer ){ 
	modify4BIT(c,bigstep, add, pointer); 
	InstEdit::repack(); 
	SETTINGS_FMW fmw = unpackFmw( &VAR_INSTRUMENT ); 
	Synth::updateADSRFmw( &fmw );
}

void InstEdit::modAdsrSmp( Control *c, bool bigstep, bool add, u32 *pointer ){ 
	modify4BIT(c,bigstep, add, pointer); 
	InstEdit::repack(); 
	SETTINGS_SMP smp = unpackSmp( &VAR_INSTRUMENT ); 
	Synth::updateADSRSmp( &smp ); 
}

void InstEdit::wavPreset(Control *c, bool bigstep, bool add, u32 *pointer){
	#define MATCH( match ) 	( c == &INS_WAV_CONTROLS[ CONTROL_INS_WAV_##match ] ) 
	int index 	= MATCH( WAVPRESET_00 ) ? 0x0 
				: MATCH( WAVPRESET_01 ) ? 0x1 
				: MATCH( WAVPRESET_02 ) ? 0x2 
				: MATCH( WAVPRESET_03 ) ? 0x3 
				: MATCH( WAVPRESET_04 ) ? 0x4 
				: 0x05;
	#undef MATCH
	DECIMAL_TWOTILES( 0, 0, 0xff, index );
	BREAK
}

void InstEdit::smpPreset(Control *c, bool bigstep, bool add, u32 *pointer){
	#define MATCH( match ) 	( c == &INS_SMP_CONTROLS[ CONTROL_INS_SMP_##match ] ) 
	int offset  = unpackSmp( &VAR_INSTRUMENT ).KIT * 0xB;
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
}
