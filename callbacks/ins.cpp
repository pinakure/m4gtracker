void modifyFmOp1ADSR(Control *c, bool bigstep, bool add, u32 *pointer); 
void modifyFmOp2ADSR(Control *c, bool bigstep, bool add, u32 *pointer); 
void modifyFmOp3ADSR(Control *c, bool bigstep, bool add, u32 *pointer); 
void modifyFmOp4ADSR(Control *c, bool bigstep, bool add, u32 *pointer);
void modifyWavOp1ADSR(Control *c, bool bigstep, bool add, u32 *pointer); 
void modifyWavOp2ADSR(Control *c, bool bigstep, bool add, u32 *pointer); 
void modifyWavOp3ADSR(Control *c, bool bigstep, bool add, u32 *pointer); 
void modifyWavOp4ADSR(Control *c, bool bigstep, bool add, u32 *pointer);
void modifySmpADSR(Control *c, bool bigstep, bool add, u32 *pointer);

// General Callbacks
CALLBACK( cb_ins_index		, instrumentIndex	, EVENT_MODIFY_B 	, &VAR_CFG.CURRENTINSTRUMENT	, NULL);
CALLBACK( cb_ins_name		, ALPHA6			, EVENT_KEYDOWN_B 	, &VAR_INSTRUMENT.NAME			, NULL);
CALLBACK( cb_ins_type		, instrumentType	, EVENT_MODIFY_B 	, &VAR_INSTRUMENT.TYPE			, NULL);
// PWM Controls
CALLBACK( cb_ins_volumefade	, instrument4BIT	, EVENT_MODIFY_B 	, &VAR_PWM.VOLUMEFADE			, NULL);
CALLBACK( cb_ins_tspenvelope, instrument8BIT	, EVENT_MODIFY_B 	, &VAR_PWM.TSP_ENVELOPE			, NULL);
CALLBACK( cb_ins_tspmode	, instrument2BIT	, EVENT_MODIFY_B 	, &VAR_PWM.TSP_LOOP 		 	, NULL);
CALLBACK( cb_ins_tspenable	, instrument1BIT	, EVENT_KEYDOWN_B 	, &VAR_PWM.TSP_ENABLE	 		, NULL);
CALLBACK( cb_ins_volenable	, instrument1BIT	, EVENT_KEYDOWN_B 	, &VAR_PWM.VOL_ENABLE	 		, NULL);
CALLBACK( cb_ins_volmode	, instrument2BIT	, EVENT_MODIFY_B 	, &VAR_PWM.VOL_LOOP 		 	, NULL);
CALLBACK( cb_ins_volenvelope, instrument8BIT	, EVENT_MODIFY_B 	, &VAR_PWM.VOL_ENVELOPE			, NULL);
CALLBACK( cb_ins_dutycycle	, instrument2BIT	, EVENT_MODIFY_B 	, &VAR_PWM.DUTYCYCLE		 	, NULL);
CALLBACK( cb_ins_length		, instrument4BIT	, EVENT_MODIFY_B 	, &VAR_PWM.LENGTH		 		, NULL);
CALLBACK( cb_ins_sweepdir	, instrument1BIT	, EVENT_KEYDOWN_B	, &VAR_PWM.SWEEPDIR		 		, NULL);
CALLBACK( cb_ins_sweepspeed	, instrument4BIT	, EVENT_MODIFY_B	, &VAR_PWM.SWEEPSPEED	 		, NULL);
CALLBACK( cb_ins_sweepsteps , instrument4BIT	, EVENT_MODIFY_B	, &VAR_PWM.SWEEPSTEPS	 		, NULL);
CALLBACK( cb_ins_pwm_level	, instrument4BIT	, EVENT_MODIFY_B	, &VAR_PWM.LEVEL				, NULL);
CALLBACK( cb_ins_envelopedir, instrument1BIT	, EVENT_KEYDOWN_B	, &VAR_PWM.ENVELOPEDIR	 		, NULL);
// PWM Tables
// Repeat 16 times
#define CB_INS_TSP(a)		CALLBACK( cb_ins_tsp_0##a , instrument4BIT, EVENT_MODIFY_B, &VAR_PWM.TSP[0x##a], NULL) 
CB_INS_TSP(0);			CB_INS_TSP(1);			CB_INS_TSP(2);			CB_INS_TSP(3);
CB_INS_TSP(4);			CB_INS_TSP(5);			CB_INS_TSP(6);			CB_INS_TSP(7);
CB_INS_TSP(8);			CB_INS_TSP(9);			CB_INS_TSP(A);			CB_INS_TSP(B);
CB_INS_TSP(C);			CB_INS_TSP(D);			CB_INS_TSP(E);			CB_INS_TSP(F);
#undef CB_INS_TSP

#define CB_INS_VOL(a)		CALLBACK( cb_ins_vol_0##a , instrument4BIT, EVENT_MODIFY_B, &VAR_PWM.VOL[0x##a], NULL) 
CB_INS_VOL(0);			CB_INS_VOL(1);			CB_INS_VOL(2);			CB_INS_VOL(3);
CB_INS_VOL(4);			CB_INS_VOL(5);			CB_INS_VOL(6);			CB_INS_VOL(7);
CB_INS_VOL(8);			CB_INS_VOL(9);			CB_INS_VOL(A);			CB_INS_VOL(B);
CB_INS_VOL(C);			CB_INS_VOL(D);			CB_INS_VOL(E);			CB_INS_VOL(F);
#undef CB_INS_VOL

// VAR WAV
CALLBACK( cb_ins_phase		, instrument1BIT	, EVENT_KEYDOWN_B 	, &VAR_WAV.PHASE	 			, NULL);
CALLBACK( cb_ins_am			, instrument1BIT	, EVENT_KEYDOWN_B 	, &VAR_WAV.AM		 			, NULL);
CALLBACK( cb_ins_mixpercent	, instrument3BIT	, EVENT_MODIFY_B 	, &VAR_WAV.MIXPERCENT 			, NULL);
CALLBACK( cb_ins_wav_op1type, instrument3BIT	, EVENT_MODIFY_B 	, &VAR_WAV.OP1_TYPE				, NULL);
CALLBACK( cb_ins_wav_op2type, instrument3BIT	, EVENT_MODIFY_B 	, &VAR_WAV.OP2_TYPE				, NULL);
CALLBACK( cb_ins_wav_op3type, instrument3BIT	, EVENT_MODIFY_B 	, &VAR_WAV.OP3_TYPE				, NULL);
CALLBACK( cb_ins_wav_op4type, instrument3BIT	, EVENT_MODIFY_B 	, &VAR_WAV.OP4_TYPE				, NULL);

#define CB_INS_WAV_OP1ADSR(a)		CALLBACK( cb_ins_wav_op1adsr_##a , modifyWavOp1ADSR , EVENT_MODIFY_B , &VAR_WAV.OP1_ADSR[a], NULL)
#define CB_INS_WAV_OP2ADSR(a)		CALLBACK( cb_ins_wav_op2adsr_##a , modifyWavOp2ADSR , EVENT_MODIFY_B , &VAR_WAV.OP2_ADSR[a], NULL)
#define CB_INS_WAV_OP3ADSR(a)		CALLBACK( cb_ins_wav_op3adsr_##a , modifyWavOp3ADSR , EVENT_MODIFY_B , &VAR_WAV.OP3_ADSR[a], NULL)
#define CB_INS_WAV_OP4ADSR(a)		CALLBACK( cb_ins_wav_op4adsr_##a , modifyWavOp4ADSR , EVENT_MODIFY_B , &VAR_WAV.OP4_ADSR[a], NULL)
CB_INS_WAV_OP1ADSR(0);	CB_INS_WAV_OP2ADSR(0);	CB_INS_WAV_OP3ADSR(0);	CB_INS_WAV_OP4ADSR(0);
CB_INS_WAV_OP1ADSR(1);	CB_INS_WAV_OP2ADSR(1);	CB_INS_WAV_OP3ADSR(1);	CB_INS_WAV_OP4ADSR(1);
CB_INS_WAV_OP1ADSR(2);	CB_INS_WAV_OP2ADSR(2);	CB_INS_WAV_OP3ADSR(2);	CB_INS_WAV_OP4ADSR(2);
CB_INS_WAV_OP1ADSR(3);	CB_INS_WAV_OP2ADSR(3);	CB_INS_WAV_OP3ADSR(3);	CB_INS_WAV_OP4ADSR(3);
#undef CB_INS_WAV_OP1ADSR
#undef CB_INS_WAV_OP2ADSR
#undef CB_INS_WAV_OP3ADSR
#undef CB_INS_WAV_OP4ADSR

#define CB_INS_WAV_PRESET(a)		CALLBACK( cb_ins_wav_preset##a , LOADWAVPRESET0 , EVENT_KEYUP_B, &VAR_WAV.WAVPRESET[a] , NULL)
CB_INS_WAV_PRESET(0);
CB_INS_WAV_PRESET(1);
CB_INS_WAV_PRESET(2);
CB_INS_WAV_PRESET(3);
CB_INS_WAV_PRESET(4);
CB_INS_WAV_PRESET(5);
#undef CB_INS_WAV_PRESET

//VAR_FMW
CALLBACK( cb_ins_algorithm	, instrument6VAL	, EVENT_MODIFY_B 	, &VAR_FMW.ALGORITHM			, NULL);
CALLBACK( cb_ins_fm_mult	, instrument4BIT	, EVENT_MODIFY_B 	, &VAR_FMW.MULT					, NULL);
CALLBACK( cb_ins_fm_op1type	, instrument3BIT	, EVENT_MODIFY_B 	, &VAR_FMW.OP1_TYPE				, NULL);
CALLBACK( cb_ins_fm_op2type	, instrument3BIT	, EVENT_MODIFY_B 	, &VAR_FMW.OP2_TYPE				, NULL);
CALLBACK( cb_ins_fm_op3type	, instrument3BIT	, EVENT_MODIFY_B 	, &VAR_FMW.OP3_TYPE				, NULL);
CALLBACK( cb_ins_fm_op4type	, instrument3BIT	, EVENT_MODIFY_B 	, &VAR_FMW.OP4_TYPE				, NULL);

#define CB_INS_FM_OP1ADSR(a)		CALLBACK( cb_ins_fm_op1adsr_##a , modifyFmOp1ADSR , EVENT_MODIFY_B , &VAR_FMW.OP1_ADSR[a], NULL)
#define CB_INS_FM_OP2ADSR(a)		CALLBACK( cb_ins_fm_op2adsr_##a , modifyFmOp2ADSR , EVENT_MODIFY_B , &VAR_FMW.OP2_ADSR[a], NULL)
#define CB_INS_FM_OP3ADSR(a)		CALLBACK( cb_ins_fm_op3adsr_##a , modifyFmOp3ADSR , EVENT_MODIFY_B , &VAR_FMW.OP3_ADSR[a], NULL)
#define CB_INS_FM_OP4ADSR(a)		CALLBACK( cb_ins_fm_op4adsr_##a , modifyFmOp4ADSR , EVENT_MODIFY_B , &VAR_FMW.OP4_ADSR[a], NULL)
CB_INS_FM_OP1ADSR(0);	CB_INS_FM_OP2ADSR(0);	CB_INS_FM_OP3ADSR(0);	CB_INS_FM_OP4ADSR(0);
CB_INS_FM_OP1ADSR(1);	CB_INS_FM_OP2ADSR(1);	CB_INS_FM_OP3ADSR(1);	CB_INS_FM_OP4ADSR(1);
CB_INS_FM_OP1ADSR(2);	CB_INS_FM_OP2ADSR(2);	CB_INS_FM_OP3ADSR(2);	CB_INS_FM_OP4ADSR(2);
CB_INS_FM_OP1ADSR(3);	CB_INS_FM_OP2ADSR(3);	CB_INS_FM_OP3ADSR(3);	CB_INS_FM_OP4ADSR(3);
#undef CB_INS_FM_OP1ADSR
#undef CB_INS_FM_OP2ADSR
#undef CB_INS_FM_OP3ADSR
#undef CB_INS_FM_OP4ADSR

//VAR_SMP
CALLBACK( cb_ins_smp_start		, instrument8BIT	, EVENT_MODIFY_B 	, &VAR_SMP.START				, NULL);
CALLBACK( cb_ins_smp_end		, instrument8BIT	, EVENT_MODIFY_B 	, &VAR_SMP.END					, NULL);
CALLBACK( cb_ins_smp_frequency	, instrument8BIT	, EVENT_MODIFY_B 	, &VAR_SMP.FREQUENCY			, NULL);
CALLBACK( cb_ins_smp_b			, instrument4BIT	, EVENT_MODIFY_B 	, &VAR_SMP.B					, NULL);
CALLBACK( cb_ins_smp_s			, instrument4BIT	, EVENT_MODIFY_B 	, &VAR_SMP.S					, NULL);
CALLBACK( cb_ins_smp_r			, instrument4BIT	, EVENT_MODIFY_B 	, &VAR_SMP.R					, NULL);
CALLBACK( cb_ins_smp_synthmode	, instrument1BIT	, EVENT_KEYDOWN_B 	, &VAR_SMP.SYNTHMODE			, NULL);
CALLBACK( cb_ins_smp_loop		, instrument2BIT	, EVENT_MODIFY_B 	, &VAR_SMP.LOOP					, NULL);
CALLBACK( cb_ins_smp_kit		, instrument5BIT	, EVENT_MODIFY_B 	, &VAR_SMP.KIT					, NULL);

#define CB_INS_SMPKIT(a)			CALLBACK( cb_ins_smpkit_0##a , NULL, EVENT_KEYUP_B, &VAR_SMP.SMPKIT[0x##a], NULL)
CB_INS_SMPKIT(0);		CB_INS_SMPKIT(1);		CB_INS_SMPKIT(2);		CB_INS_SMPKIT(3);
CB_INS_SMPKIT(4);		CB_INS_SMPKIT(5);		CB_INS_SMPKIT(6);		CB_INS_SMPKIT(7);
CB_INS_SMPKIT(8);		CB_INS_SMPKIT(9);		CB_INS_SMPKIT(A);		CB_INS_SMPKIT(B);
#undef CB_INS_SMPKIT

#define CB_INS_SMP_ADSR(a)			CALLBACK( cb_ins_smp_adsr_##a , modifySmpADSR , EVENT_MODIFY_B , &VAR_SMP.ADSR[a], NULL)
CB_INS_SMP_ADSR(0);		CB_INS_SMP_ADSR(1);		CB_INS_SMP_ADSR(2);		CB_INS_SMP_ADSR(3);
#undef CB_INS_SMP_ADSR

//TABLE 
#define CB_INS_TABLE_TRANSPOSE(a)	CALLBACK( cb_ins_table_transpose_0##a 	, instrument8BIT , EVENT_MODIFY_B, &VAR_INSTRUMENT.TABLE.TRANSPOSE[0x##a], NULL) 
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

#define CB_INS_TABLE_VOLUME(a)		CALLBACK( cb_ins_table_volume_0##a 		, instrument4BIT , EVENT_MODIFY_B, &VAR_INSTRUMENT.TABLE.VOLUME[0x##a], NULL) 
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


#define CB_INS_TABLE_VALUE1(a)		CALLBACK( cb_ins_table_value1_0##a 		, instrument8BIT , EVENT_MODIFY_B, &VAR_INSTRUMENT.TABLE.VALUE[0][0x##a], NULL) 
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

#define CB_INS_TABLE_COMMAND1(a)	CALLBACK( cb_ins_table_command1_0##a	, instrument27VAL , EVENT_MODIFY_B, &VAR_INSTRUMENT.TABLE.COMMAND[0][0x##a], NULL) 
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

#define CB_INS_TABLE_VALUE2(a)		CALLBACK( cb_ins_table_value2_0##a 		, instrument8BIT , EVENT_MODIFY_B, &VAR_INSTRUMENT.TABLE.VALUE[1][0x##a], NULL) 
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

#define CB_INS_TABLE_COMMAND2(a)	CALLBACK( cb_ins_table_command2_0##a	, instrument27VAL , EVENT_MODIFY_B, &VAR_INSTRUMENT.TABLE.COMMAND[1][0x##a], NULL) 
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


// Called on type change
void instType(void){
	instrumentUnpack(&VAR_INSTRUMENT);	
}

// Called on index change
void instLoad(void){	
	instcopy(&VAR_INSTRUMENTS[VAR_CFG.CURRENTINSTRUMENT], &VAR_INSTRUMENT);	
	instrumentUnpack(&VAR_INSTRUMENT);
	regHnd.redraw = true;	
}

// Called on value change (against index and type)
void instSync(void){	
	instrumentPack(&VAR_INSTRUMENT);
	instcopy(&VAR_INSTRUMENT, &VAR_INSTRUMENTS[VAR_CFG.CURRENTINSTRUMENT]);	
}

void instrument1BIT(Control *c, bool bigstep, bool add, u32 *pointer){	modify1BIT(c,bigstep, add, pointer); instSync(); }
void instrument2BIT(Control *c, bool bigstep, bool add, u32 *pointer){	modify2BIT(c,bigstep, add, pointer); instSync(); }
void instrumentType(Control *c, bool bigstep, bool add, u32 *pointer){	modify2BIT(c,bigstep, add, pointer); instType(); }
void instrument3BIT(Control *c, bool bigstep, bool add, u32 *pointer){	modify3BIT(c,bigstep, add, pointer); instSync(); }

void modifyWavOp1ADSR(Control *c, bool bigstep, bool add, u32 *pointer){ 
	modify4BIT(c,bigstep, add, pointer); 
	instSync(); 
	SETTINGS_WAV wav = unpackWAV( &VAR_INSTRUMENT ); 
	Synth::updateADSRWav( &wav ); 
}
void modifyWavOp2ADSR(Control *c, bool bigstep, bool add, u32 *pointer){ 
	modify4BIT(c,bigstep, add, pointer); 
	instSync(); 
	SETTINGS_WAV wav = unpackWAV( &VAR_INSTRUMENT ); 
	Synth::updateADSRWav( &wav ); 
}
void modifyWavOp3ADSR(Control *c, bool bigstep, bool add, u32 *pointer){ 
	modify4BIT(c,bigstep, add, pointer); 
	instSync(); 
	SETTINGS_WAV wav = unpackWAV( &VAR_INSTRUMENT ); 
	Synth::updateADSRWav( &wav ); 
}
void modifyWavOp4ADSR(Control *c, bool bigstep, bool add, u32 *pointer){ 
	modify4BIT(c,bigstep, add, pointer); 
	instSync(); 
	SETTINGS_WAV wav = unpackWAV( &VAR_INSTRUMENT ); 
	Synth::updateADSRWav( &wav ); 
}
void modifyFmOp1ADSR(Control *c, bool bigstep, bool add, u32 *pointer){ 
	modify4BIT(c,bigstep, add, pointer); 
	instSync(); 
	SETTINGS_FMW fmw = unpackFMW( &VAR_INSTRUMENT ); 
	Synth::updateADSRFmw( &fmw );
}
void modifyFmOp2ADSR(Control *c, bool bigstep, bool add, u32 *pointer){ 
	modify4BIT(c,bigstep, add, pointer); 
	instSync(); 
	SETTINGS_FMW fmw = unpackFMW( &VAR_INSTRUMENT ); 
	Synth::updateADSRFmw( &fmw ); 
}
void modifyFmOp3ADSR(Control *c, bool bigstep, bool add, u32 *pointer){ 
	modify4BIT(c,bigstep, add, pointer); 
	instSync(); 
	SETTINGS_FMW fmw = unpackFMW( &VAR_INSTRUMENT ); 
	Synth::updateADSRFmw( &fmw );
}
void modifyFmOp4ADSR(Control *c, bool bigstep, bool add, u32 *pointer){ 
	modify4BIT(c,bigstep, add, pointer); 
	instSync(); 
	SETTINGS_FMW fmw = unpackFMW( &VAR_INSTRUMENT ); 
	Synth::updateADSRFmw( &fmw ); 
}
void modifySmpADSR(Control *c, bool bigstep, bool add, u32 *pointer){ 
	modify4BIT(c,bigstep, add, pointer); 
	instSync(); 
	SETTINGS_SMP smp = unpackSMP( &VAR_INSTRUMENT ); 
	Synth::updateADSRSmp( &smp ); 
}

void instrument4BIT(Control *c, bool bigstep, bool add, u32 *pointer){	modify4BIT(c,bigstep, add, pointer); instSync(); }
void instrument5BIT(Control *c, bool bigstep, bool add, u32 *pointer){	modify5BIT(c,bigstep, add, pointer); instSync(); }
void instrument6BIT(Control *c, bool bigstep, bool add, u32 *pointer){	modify6BIT(c,bigstep, add, pointer); instSync(); }
void instrumentIndex(Control *c, bool bigstep, bool add, u32 *pointer){	
	instSync(); 
	VAR_CFG.CURRENTINSTRUMENT = (VAR_CFG.CURRENTINSTRUMENT + ((bigstep?0x4:0x1) * (add?1:-1)));
	if((VAR_CFG.CURRENTINSTRUMENT > 0x3F) && (VAR_CFG.CURRENTINSTRUMENT < 0x44)) VAR_CFG.CURRENTINSTRUMENT = 0x1;
	else if(VAR_CFG.CURRENTINSTRUMENT > 0x3F) VAR_CFG.CURRENTINSTRUMENT = 0x3F;
	transient6BIT 	 = VAR_CFG.CURRENTINSTRUMENT; 
	transientChanged = true;
	if(VAR_CFG.CURRENTINSTRUMENT == 0x0 ) VAR_CFG.CURRENTINSTRUMENT= 0x1;
	instLoad(); 
}
void instrument7BIT(Control *c, bool bigstep, bool add, u32 *pointer){	modify7BIT(c,bigstep, add, pointer); instSync(); }
void instrument8BIT(Control *c, bool bigstep, bool add, u32 *pointer){	modify8BIT(c,bigstep, add, pointer); instSync(); }
void instrument3VAL(Control *c, bool bigstep, bool add, u32 *pointer){	modify3VAL(c,bigstep, add, pointer); instSync(); }
void instrument6VAL(Control *c, bool bigstep, bool add, u32 *pointer){  modify6VAL(c,bigstep, add, pointer); instSync(); }
void instrument5VAL(Control *c, bool bigstep, bool add, u32 *pointer){	modify5VAL(c,bigstep, add, pointer); instSync(); }
void instrument27VAL(Control *c, bool bigstep, bool add, u32 *pointer){	modifyCommand(c,bigstep,add, pointer); instSync(); }


/* SYNCRONIZES Specific instrument data with Generic Abstract Instrument byte array */
void instrumentPack(Instrument *i){
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

inline SETTINGS_FMW unpackFMW(Instrument *i){
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

inline SETTINGS_SMP unpackSMP( Instrument *i ){
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

inline SETTINGS_WAV unpackWAV( Instrument *i ){
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

inline SETTINGS_PWM unpackPWM( Instrument *i){
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

void instrumentUnpack( Instrument *i){
	switch(i->TYPE){
		case INSTRUMENT_TYPE_PWM:	VAR_PWM = unpackPWM(i); return;
		case INSTRUMENT_TYPE_WAV:	VAR_WAV = unpackWAV(i); return;
		case INSTRUMENT_TYPE_FMW:	VAR_FMW = unpackFMW(i); return;
		case INSTRUMENT_TYPE_SMP:	VAR_SMP = unpackSMP( i ); return;
	}
}

void instcopy(Instrument *s, Instrument *d){
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
static void instrumentSync(){
	static u8 LASTINSTRUMENT = 63;
	
	if(LASTINSTRUMENT != VAR_CFG.CURRENTINSTRUMENT){
		instcopy(&VAR_INSTRUMENTS[VAR_CFG.CURRENTINSTRUMENT], &VAR_INSTRUMENT);
		instrumentUnpack( &VAR_INSTRUMENT );
		LASTINSTRUMENT = VAR_CFG.CURRENTINSTRUMENT;
		regHnd.redraw = true;
	} /*else {
		instcopy(&VAR_INSTRUMENT, &VAR_INSTRUMENTS[VAR_CFG.CURRENTINSTRUMENT]);
		instrumentPack( &VAR_INSTRUMENT );
	}*/
}

void insDispatchMessage(u32 msg) {
	switch(msg) {
		case MESSAGE_OTHER_PREV:
			VAR_CFG.CURRENTINSTRUMENT--;
			VAR_CFG.CURRENTINSTRUMENT &= 0x3f;
			if(VAR_CFG.CURRENTINSTRUMENT == 0) VAR_CFG.CURRENTINSTRUMENT=1;
			instLoad();
			return;
		
		case MESSAGE_OTHER_NEXT:			
			VAR_CFG.CURRENTINSTRUMENT++;
			VAR_CFG.CURRENTINSTRUMENT &= 0x3f;
			if(VAR_CFG.CURRENTINSTRUMENT == 0) VAR_CFG.CURRENTINSTRUMENT=1;
			instLoad();
			return;
	}	
}


void updateINS_WAV(RegionHandler* rh){
	const Control *cname = &INS_WAV_CONTROLS[CONTROL_INS_WAV_NAME];
	STRING(false, cname->x, cname->y, cname->var);
	instrumentSync();
}
void updateINS_FMW(RegionHandler* rh){
	const Control *cname = &INS_FMW_CONTROLS[CONTROL_INS_FMW_NAME];
	STRING(false, cname->x, cname->y, cname->var);
	instrumentSync();
}
void updateINS_SMP(RegionHandler* rh){
	const Control *cname = &INS_SMP_CONTROLS[CONTROL_INS_SMP_NAME];
	STRING(false, cname->x, cname->y, cname->var);
	instrumentSync();
}
void updateINS_PWM(RegionHandler* rh){
	const Control *cname = &INS_PWM_CONTROLS[CONTROL_INS_PWM_NAME];
	STRING(false, cname->x, cname->y, cname->var);
	instrumentSync();
}
//----------------------------------------------------------------------------------------
void LOADWAVPRESET0(Control *c, bool bigstep, bool add, u32 *pointer){

}
void LOADWAVPRESET1(Control *c, bool bigstep, bool add, u32 *pointer){

}
void LOADWAVPRESET2(Control *c, bool bigstep, bool add, u32 *pointer){

}
void LOADWAVPRESET3(Control *c, bool bigstep, bool add, u32 *pointer){

}
void LOADWAVPRESET4(Control *c, bool bigstep, bool add, u32 *pointer){

}
void LOADWAVPRESET5(Control *c, bool bigstep, bool add, u32 *pointer){

}

