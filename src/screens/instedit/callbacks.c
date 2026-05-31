u8 dummy;

/* Global callbacks ---------------------------------------------------------------------------------------------------------- */
const Callback cb_ins_index				 = { InstEdit::index		, EVENT_MODIFY_B 	, &VAR_CFG.CURRENTINSTRUMENT	, NULL };
const Callback cb_ins_name				 = { AlphaDialog::getString , EVENT_KEYUP_A 	, &VAR_INSTRUMENT.NAME			, NULL };
const Callback cb_ins_type				 = { InstEdit::type			, EVENT_MODIFY_B 	, &VAR_INSTRUMENT.TYPE			, NULL };
/* PWM instrument settings --------------------------------------------------------------------------------------------------- */            	
const Callback cb_ins_volumefade		 = { InstEdit::modNibble	, EVENT_MODIFY_B 	, &VAR_PWM.VOLUMEFADE			, NULL };
const Callback cb_ins_tspenvelope		 = { InstEdit::modByte		, EVENT_MODIFY_B 	, &VAR_PWM.TSP_ENVELOPE			, NULL };
const Callback cb_ins_tspmode			 = { InstEdit::mod2b		, EVENT_MODIFY_B 	, &VAR_PWM.TSP_LOOP 		 	, NULL };
const Callback cb_ins_tspenable			 = { InstEdit::modBit		, EVENT_KEYDOWN_B 	, &VAR_PWM.TSP_ENABLE	 		, NULL };
const Callback cb_ins_volenable			 = { InstEdit::modBit		, EVENT_KEYDOWN_B 	, &VAR_PWM.VOL_ENABLE	 		, NULL };
const Callback cb_ins_volmode			 = { InstEdit::mod2b		, EVENT_MODIFY_B 	, &VAR_PWM.VOL_LOOP 		 	, NULL };
const Callback cb_ins_volenvelope		 = { InstEdit::modByte		, EVENT_MODIFY_B 	, &VAR_PWM.VOL_ENVELOPE			, NULL };
const Callback cb_ins_dutycycle			 = { InstEdit::mod2b		, EVENT_MODIFY_B 	, &VAR_PWM.DUTYCYCLE		 	, NULL };
const Callback cb_ins_length			 = { InstEdit::modNibble	, EVENT_MODIFY_B 	, &VAR_PWM.LENGTH		 		, NULL };
const Callback cb_ins_sweepdir			 = { InstEdit::modBit		, EVENT_KEYDOWN_B	, &VAR_PWM.SWEEPDIR		 		, NULL };
const Callback cb_ins_sweepspeed		 = { InstEdit::modNibble	, EVENT_MODIFY_B	, &VAR_PWM.SWEEPSPEED	 		, NULL };
const Callback cb_ins_sweepsteps 		 = { InstEdit::modNibble	, EVENT_MODIFY_B	, &VAR_PWM.SWEEPSTEPS	 		, NULL };
const Callback cb_ins_pwm_level			 = { InstEdit::modNibble	, EVENT_MODIFY_B	, &VAR_PWM.LEVEL				, NULL };
const Callback cb_ins_envelopedir		 = { InstEdit::modBit		, EVENT_KEYDOWN_B	, &VAR_PWM.ENVELOPEDIR	 		, NULL };
// PWM Tsp Table
const Callback cb_ins_tsp_00 			 = { InstEdit::modNibble	, EVENT_MODIFY_B	, &VAR_PWM.TSP[ 0x0 ]			, NULL };
const Callback cb_ins_tsp_01 			 = { InstEdit::modNibble	, EVENT_MODIFY_B	, &VAR_PWM.TSP[ 0x1 ]			, NULL };
const Callback cb_ins_tsp_02 			 = { InstEdit::modNibble	, EVENT_MODIFY_B	, &VAR_PWM.TSP[ 0x2 ]			, NULL };
const Callback cb_ins_tsp_03 			 = { InstEdit::modNibble	, EVENT_MODIFY_B	, &VAR_PWM.TSP[ 0x3 ]			, NULL };
const Callback cb_ins_tsp_04 			 = { InstEdit::modNibble	, EVENT_MODIFY_B	, &VAR_PWM.TSP[ 0x4 ]			, NULL };
const Callback cb_ins_tsp_05 			 = { InstEdit::modNibble	, EVENT_MODIFY_B	, &VAR_PWM.TSP[ 0x5 ]			, NULL };
const Callback cb_ins_tsp_06 			 = { InstEdit::modNibble	, EVENT_MODIFY_B	, &VAR_PWM.TSP[ 0x6 ]			, NULL };
const Callback cb_ins_tsp_07 			 = { InstEdit::modNibble	, EVENT_MODIFY_B	, &VAR_PWM.TSP[ 0x7 ]			, NULL };
const Callback cb_ins_tsp_08 			 = { InstEdit::modNibble	, EVENT_MODIFY_B	, &VAR_PWM.TSP[ 0x8 ]			, NULL };
const Callback cb_ins_tsp_09 			 = { InstEdit::modNibble	, EVENT_MODIFY_B	, &VAR_PWM.TSP[ 0x9 ]			, NULL };
const Callback cb_ins_tsp_0A 			 = { InstEdit::modNibble	, EVENT_MODIFY_B	, &VAR_PWM.TSP[ 0xA ]			, NULL };
const Callback cb_ins_tsp_0B 			 = { InstEdit::modNibble	, EVENT_MODIFY_B	, &VAR_PWM.TSP[ 0xB ]			, NULL };
const Callback cb_ins_tsp_0C 			 = { InstEdit::modNibble	, EVENT_MODIFY_B	, &VAR_PWM.TSP[ 0xC ]			, NULL };
const Callback cb_ins_tsp_0D 			 = { InstEdit::modNibble	, EVENT_MODIFY_B	, &VAR_PWM.TSP[ 0xD ]			, NULL };
const Callback cb_ins_tsp_0E 			 = { InstEdit::modNibble	, EVENT_MODIFY_B	, &VAR_PWM.TSP[ 0xE ]			, NULL };
const Callback cb_ins_tsp_0F 			 = { InstEdit::modNibble	, EVENT_MODIFY_B	, &VAR_PWM.TSP[ 0xF ]			, NULL };
// PWM Vol Table
const Callback cb_ins_vol_00 			 = { InstEdit::modNibble	, EVENT_MODIFY_B	, &VAR_PWM.VOL[ 0x0 ]			, NULL };
const Callback cb_ins_vol_01 			 = { InstEdit::modNibble	, EVENT_MODIFY_B	, &VAR_PWM.VOL[ 0x1 ]			, NULL };
const Callback cb_ins_vol_02 			 = { InstEdit::modNibble	, EVENT_MODIFY_B	, &VAR_PWM.VOL[ 0x2 ]			, NULL };
const Callback cb_ins_vol_03 			 = { InstEdit::modNibble	, EVENT_MODIFY_B	, &VAR_PWM.VOL[ 0x3 ]			, NULL };
const Callback cb_ins_vol_04 			 = { InstEdit::modNibble	, EVENT_MODIFY_B	, &VAR_PWM.VOL[ 0x4 ]			, NULL };
const Callback cb_ins_vol_05 			 = { InstEdit::modNibble	, EVENT_MODIFY_B	, &VAR_PWM.VOL[ 0x5 ]			, NULL };
const Callback cb_ins_vol_06 			 = { InstEdit::modNibble	, EVENT_MODIFY_B	, &VAR_PWM.VOL[ 0x6 ]			, NULL };
const Callback cb_ins_vol_07 			 = { InstEdit::modNibble	, EVENT_MODIFY_B	, &VAR_PWM.VOL[ 0x7 ]			, NULL };
const Callback cb_ins_vol_08 			 = { InstEdit::modNibble	, EVENT_MODIFY_B	, &VAR_PWM.VOL[ 0x8 ]			, NULL };
const Callback cb_ins_vol_09 			 = { InstEdit::modNibble	, EVENT_MODIFY_B	, &VAR_PWM.VOL[ 0x9 ]			, NULL };
const Callback cb_ins_vol_0A 			 = { InstEdit::modNibble	, EVENT_MODIFY_B	, &VAR_PWM.VOL[ 0xA ]			, NULL };
const Callback cb_ins_vol_0B 			 = { InstEdit::modNibble	, EVENT_MODIFY_B	, &VAR_PWM.VOL[ 0xB ]			, NULL };
const Callback cb_ins_vol_0C 			 = { InstEdit::modNibble	, EVENT_MODIFY_B	, &VAR_PWM.VOL[ 0xC ]			, NULL };
const Callback cb_ins_vol_0D 			 = { InstEdit::modNibble	, EVENT_MODIFY_B	, &VAR_PWM.VOL[ 0xD ]			, NULL };
const Callback cb_ins_vol_0E 			 = { InstEdit::modNibble	, EVENT_MODIFY_B	, &VAR_PWM.VOL[ 0xE ]			, NULL };
const Callback cb_ins_vol_0F 			 = { InstEdit::modNibble	, EVENT_MODIFY_B	, &VAR_PWM.VOL[ 0xF ]			, NULL };
/* WAV instrument settings --------------------------------------------------------------------------------------------------- */            	
const Callback cb_ins_phase		  		 = { InstEdit::modBit		, EVENT_KEYDOWN_B 	, &VAR_WAV.PHASE	 			, NULL };
const Callback cb_ins_am		  		 = { InstEdit::modBit		, EVENT_KEYDOWN_B 	, &VAR_WAV.AM		 			, NULL };
const Callback cb_ins_distortion  		 = { InstEdit::modNibble	, EVENT_MODIFY_B 	, &VAR_WAV.DISTORTION 			, NULL };
const Callback cb_ins_wav_op1type 		 = { InstEdit::mod3b		, EVENT_MODIFY_B 	, &VAR_WAV.OP1_TYPE				, NULL };
const Callback cb_ins_wav_op2type 		 = { InstEdit::mod3b		, EVENT_MODIFY_B 	, &VAR_WAV.OP2_TYPE				, NULL };
const Callback cb_ins_wav_op3type 		 = { InstEdit::mod3b		, EVENT_MODIFY_B 	, &VAR_WAV.OP3_TYPE				, NULL };
const Callback cb_ins_wav_op4type 		 = { InstEdit::mod3b		, EVENT_MODIFY_B 	, &VAR_WAV.OP4_TYPE				, NULL };
const Callback cb_ins_wav_op1gate 		 = { InstEdit::modAdsrWav	, EVENT_MODIFY_B 	, &VAR_WAV.OP1_GATE				, NULL };
const Callback cb_ins_wav_op2gate 		 = { InstEdit::modAdsrWav	, EVENT_MODIFY_B 	, &VAR_WAV.OP2_GATE				, NULL };
const Callback cb_ins_wav_op3gate 		 = { InstEdit::modAdsrWav	, EVENT_MODIFY_B 	, &VAR_WAV.OP3_GATE				, NULL };
const Callback cb_ins_wav_op4gate 		 = { InstEdit::modAdsrWav	, EVENT_MODIFY_B 	, &VAR_WAV.OP4_GATE				, NULL };
// WAV ADSR OP#1
const Callback cb_ins_wav_op1adsr_0		 = { InstEdit::modAdsrWav	, EVENT_MODIFY_B 	, &VAR_WAV.OP1_ADSR		[ 0x0 ]	, NULL };
const Callback cb_ins_wav_op1adsr_1		 = { InstEdit::modAdsrWav	, EVENT_MODIFY_B 	, &VAR_WAV.OP1_ADSR		[ 0x1 ]	, NULL };
const Callback cb_ins_wav_op1adsr_2		 = { InstEdit::modAdsrWav	, EVENT_MODIFY_B 	, &VAR_WAV.OP1_ADSR		[ 0x2 ]	, NULL };
const Callback cb_ins_wav_op1adsr_3		 = { InstEdit::modAdsrWav	, EVENT_MODIFY_B 	, &VAR_WAV.OP1_ADSR		[ 0x3 ]	, NULL };
// WAV ADSR OP#2		                                                                                                
const Callback cb_ins_wav_op2adsr_0		 = { InstEdit::modAdsrWav	, EVENT_MODIFY_B 	, &VAR_WAV.OP2_ADSR		[ 0x0 ]	, NULL };
const Callback cb_ins_wav_op2adsr_1		 = { InstEdit::modAdsrWav	, EVENT_MODIFY_B 	, &VAR_WAV.OP2_ADSR		[ 0x1 ]	, NULL };
const Callback cb_ins_wav_op2adsr_2		 = { InstEdit::modAdsrWav	, EVENT_MODIFY_B 	, &VAR_WAV.OP2_ADSR		[ 0x2 ]	, NULL };
const Callback cb_ins_wav_op2adsr_3		 = { InstEdit::modAdsrWav	, EVENT_MODIFY_B 	, &VAR_WAV.OP2_ADSR		[ 0x3 ]	, NULL };
// WAV ADSR OP#3		                                                                                                
const Callback cb_ins_wav_op3adsr_0		 = { InstEdit::modAdsrWav	, EVENT_MODIFY_B 	, &VAR_WAV.OP3_ADSR		[ 0x0 ]	, NULL };
const Callback cb_ins_wav_op3adsr_1		 = { InstEdit::modAdsrWav	, EVENT_MODIFY_B 	, &VAR_WAV.OP3_ADSR		[ 0x1 ]	, NULL };
const Callback cb_ins_wav_op3adsr_2		 = { InstEdit::modAdsrWav	, EVENT_MODIFY_B 	, &VAR_WAV.OP3_ADSR		[ 0x2 ]	, NULL };
const Callback cb_ins_wav_op3adsr_3		 = { InstEdit::modAdsrWav	, EVENT_MODIFY_B 	, &VAR_WAV.OP3_ADSR		[ 0x3 ]	, NULL };
// WAV ADSR OP#4                                                                                                        
const Callback cb_ins_wav_op4adsr_0		 = { InstEdit::modAdsrWav	, EVENT_MODIFY_B 	, &VAR_WAV.OP4_ADSR		[ 0x0 ]	, NULL };
const Callback cb_ins_wav_op4adsr_1		 = { InstEdit::modAdsrWav	, EVENT_MODIFY_B 	, &VAR_WAV.OP4_ADSR		[ 0x1 ]	, NULL };
const Callback cb_ins_wav_op4adsr_2		 = { InstEdit::modAdsrWav	, EVENT_MODIFY_B 	, &VAR_WAV.OP4_ADSR		[ 0x2 ]	, NULL };
const Callback cb_ins_wav_op4adsr_3		 = { InstEdit::modAdsrWav	, EVENT_MODIFY_B 	, &VAR_WAV.OP4_ADSR		[ 0x3 ]	, NULL };
// WAV PRESETS
const Callback cb_ins_wav_preset0		 = { InstEdit::wavPreset	, EVENT_KEYUP_B		, &dummy						, NULL };
const Callback cb_ins_wav_preset1		 = { InstEdit::wavPreset	, EVENT_KEYUP_B		, &dummy						, NULL };
const Callback cb_ins_wav_preset2		 = { InstEdit::wavPreset	, EVENT_KEYUP_B		, &dummy						, NULL };
const Callback cb_ins_wav_preset3		 = { InstEdit::wavPreset	, EVENT_KEYUP_B		, &dummy						, NULL };
const Callback cb_ins_wav_preset4		 = { InstEdit::wavPreset	, EVENT_KEYUP_B		, &dummy						, NULL };
const Callback cb_ins_wav_preset5		 = { InstEdit::wavPreset	, EVENT_KEYUP_B		, &dummy						, NULL };
/* FMW instrument settings --------------------------------------------------------------------------------------------------- */            	
const Callback cb_ins_algorithm			 = { InstEdit::modAlgorithm	, EVENT_MODIFY_B 	, &VAR_FMW.ALGORITHM			, NULL };
const Callback cb_ins_fm_distortion		 = { InstEdit::modNibble	, EVENT_MODIFY_B 	, &VAR_FMW.DISTORTION			, NULL };
const Callback cb_ins_fm_op1type		 = { InstEdit::mod3b		, EVENT_MODIFY_B 	, &VAR_FMW.OP1_TYPE				, NULL };
const Callback cb_ins_fm_op2type		 = { InstEdit::mod3b		, EVENT_MODIFY_B 	, &VAR_FMW.OP2_TYPE				, NULL };
const Callback cb_ins_fm_op3type		 = { InstEdit::mod3b		, EVENT_MODIFY_B 	, &VAR_FMW.OP3_TYPE				, NULL };
const Callback cb_ins_fm_op4type		 = { InstEdit::mod3b		, EVENT_MODIFY_B 	, &VAR_FMW.OP4_TYPE				, NULL };
const Callback cb_ins_fm_op1gate		 = { InstEdit::modAdsrFmw	, EVENT_MODIFY_B 	, &VAR_FMW.OP1_GATE				, NULL };
const Callback cb_ins_fm_op2gate		 = { InstEdit::modAdsrFmw	, EVENT_MODIFY_B 	, &VAR_FMW.OP2_GATE				, NULL };
const Callback cb_ins_fm_op3gate		 = { InstEdit::modAdsrFmw	, EVENT_MODIFY_B 	, &VAR_FMW.OP3_GATE				, NULL };
const Callback cb_ins_fm_op4gate		 = { InstEdit::modAdsrFmw	, EVENT_MODIFY_B 	, &VAR_FMW.OP4_GATE				, NULL };
// FMW ADSR OP#1
const Callback cb_ins_fm_op1adsr_0		 = { InstEdit::modAdsrFmw	, EVENT_MODIFY_B 	, &VAR_FMW.OP1_ADSR		[ 0x0 ]	, NULL };
const Callback cb_ins_fm_op1adsr_1		 = { InstEdit::modAdsrFmw	, EVENT_MODIFY_B 	, &VAR_FMW.OP1_ADSR		[ 0x1 ]	, NULL };
const Callback cb_ins_fm_op1adsr_2		 = { InstEdit::modAdsrFmw	, EVENT_MODIFY_B 	, &VAR_FMW.OP1_ADSR		[ 0x2 ]	, NULL };
const Callback cb_ins_fm_op1adsr_3		 = { InstEdit::modAdsrFmw	, EVENT_MODIFY_B 	, &VAR_FMW.OP1_ADSR		[ 0x3 ]	, NULL };
// FMW ADSR OP#2
const Callback cb_ins_fm_op2adsr_0		 = { InstEdit::modAdsrFmw	, EVENT_MODIFY_B 	, &VAR_FMW.OP2_ADSR		[ 0x0 ]	, NULL };
const Callback cb_ins_fm_op2adsr_1		 = { InstEdit::modAdsrFmw	, EVENT_MODIFY_B 	, &VAR_FMW.OP2_ADSR		[ 0x1 ]	, NULL };
const Callback cb_ins_fm_op2adsr_2		 = { InstEdit::modAdsrFmw	, EVENT_MODIFY_B 	, &VAR_FMW.OP2_ADSR		[ 0x2 ]	, NULL };
const Callback cb_ins_fm_op2adsr_3		 = { InstEdit::modAdsrFmw	, EVENT_MODIFY_B 	, &VAR_FMW.OP2_ADSR		[ 0x3 ]	, NULL };
// FMW ADSR OP#3
const Callback cb_ins_fm_op3adsr_0		 = { InstEdit::modAdsrFmw	, EVENT_MODIFY_B 	, &VAR_FMW.OP3_ADSR		[ 0x0 ]	, NULL };
const Callback cb_ins_fm_op3adsr_1		 = { InstEdit::modAdsrFmw	, EVENT_MODIFY_B 	, &VAR_FMW.OP3_ADSR		[ 0x1 ]	, NULL };
const Callback cb_ins_fm_op3adsr_2		 = { InstEdit::modAdsrFmw	, EVENT_MODIFY_B 	, &VAR_FMW.OP3_ADSR		[ 0x2 ]	, NULL };
const Callback cb_ins_fm_op3adsr_3		 = { InstEdit::modAdsrFmw	, EVENT_MODIFY_B 	, &VAR_FMW.OP3_ADSR		[ 0x3 ]	, NULL };
// FMW ADSR OP#4
const Callback cb_ins_fm_op4adsr_0		 = { InstEdit::modAdsrFmw	, EVENT_MODIFY_B 	, &VAR_FMW.OP4_ADSR		[ 0x0 ]	, NULL };
const Callback cb_ins_fm_op4adsr_1		 = { InstEdit::modAdsrFmw	, EVENT_MODIFY_B 	, &VAR_FMW.OP4_ADSR		[ 0x1 ]	, NULL };
const Callback cb_ins_fm_op4adsr_2		 = { InstEdit::modAdsrFmw	, EVENT_MODIFY_B 	, &VAR_FMW.OP4_ADSR		[ 0x2 ]	, NULL };
const Callback cb_ins_fm_op4adsr_3		 = { InstEdit::modAdsrFmw	, EVENT_MODIFY_B 	, &VAR_FMW.OP4_ADSR		[ 0x3 ]	, NULL };
/* SMP instrument settings --------------------------------------------------------------------------------------------------- */            	
const Callback cb_ins_smp_start			 = { InstEdit::modByte		, EVENT_MODIFY_B 	, &VAR_SMP.START				, NULL };
const Callback cb_ins_smp_end			 = { InstEdit::modByte		, EVENT_MODIFY_B 	, &VAR_SMP.END					, NULL };
const Callback cb_ins_smp_frequency		 = { InstEdit::modByte		, EVENT_MODIFY_B 	, &VAR_SMP.FREQUENCY			, NULL };
const Callback cb_ins_smp_b				 = { InstEdit::modNibble	, EVENT_MODIFY_B 	, &VAR_SMP.B					, NULL };
const Callback cb_ins_smp_s				 = { InstEdit::modNibble	, EVENT_MODIFY_B 	, &VAR_SMP.S					, NULL };
const Callback cb_ins_smp_r				 = { InstEdit::modNibble	, EVENT_MODIFY_B 	, &VAR_SMP.R					, NULL };
const Callback cb_ins_smp_synthmode		 = { InstEdit::modBit		, EVENT_KEYDOWN_B 	, &VAR_SMP.SYNTHMODE			, NULL };
const Callback cb_ins_smp_loop			 = { InstEdit::mod2b		, EVENT_MODIFY_B 	, &VAR_SMP.LOOP					, NULL };
const Callback cb_ins_smp_kit			 = { InstEdit::modSmpKit	, EVENT_MODIFY_B 	, &VAR_SMP.KIT					, NULL };
// SMP Kit													                                                    
const Callback cb_ins_smpkit_00 		 = { InstEdit::smpPreset	, EVENT_KEYUP_B		, &VAR_SMP.SMPKIT		[ 0x0 ]	, NULL };
const Callback cb_ins_smpkit_01 		 = { InstEdit::smpPreset	, EVENT_KEYUP_B		, &VAR_SMP.SMPKIT		[ 0x1 ]	, NULL };
const Callback cb_ins_smpkit_02 		 = { InstEdit::smpPreset	, EVENT_KEYUP_B		, &VAR_SMP.SMPKIT		[ 0x2 ]	, NULL };
const Callback cb_ins_smpkit_03 		 = { InstEdit::smpPreset	, EVENT_KEYUP_B		, &VAR_SMP.SMPKIT		[ 0x3 ]	, NULL };
const Callback cb_ins_smpkit_04 		 = { InstEdit::smpPreset	, EVENT_KEYUP_B		, &VAR_SMP.SMPKIT		[ 0x4 ]	, NULL };
const Callback cb_ins_smpkit_05 		 = { InstEdit::smpPreset	, EVENT_KEYUP_B		, &VAR_SMP.SMPKIT		[ 0x5 ]	, NULL };
const Callback cb_ins_smpkit_06 		 = { InstEdit::smpPreset	, EVENT_KEYUP_B		, &VAR_SMP.SMPKIT		[ 0x6 ]	, NULL };
const Callback cb_ins_smpkit_07 		 = { InstEdit::smpPreset	, EVENT_KEYUP_B		, &VAR_SMP.SMPKIT		[ 0x7 ]	, NULL };
const Callback cb_ins_smpkit_08 		 = { InstEdit::smpPreset	, EVENT_KEYUP_B		, &VAR_SMP.SMPKIT		[ 0x8 ]	, NULL };
const Callback cb_ins_smpkit_09 		 = { InstEdit::smpPreset	, EVENT_KEYUP_B		, &VAR_SMP.SMPKIT		[ 0x9 ]	, NULL };
const Callback cb_ins_smpkit_0A 		 = { InstEdit::smpPreset	, EVENT_KEYUP_B		, &VAR_SMP.SMPKIT		[ 0xA ]	, NULL };
const Callback cb_ins_smpkit_0B 		 = { InstEdit::smpPreset	, EVENT_KEYUP_B		, &VAR_SMP.SMPKIT		[ 0xB ]	, NULL };
// SMP ADSR											                                                            
const Callback cb_ins_smp_adsr_0 		 = { InstEdit::modAdsrSmp	, EVENT_MODIFY_B 	, &VAR_SMP.ADSR			[ 0x0 ] , NULL };
const Callback cb_ins_smp_adsr_1 		 = { InstEdit::modAdsrSmp	, EVENT_MODIFY_B 	, &VAR_SMP.ADSR			[ 0x1 ] , NULL };
const Callback cb_ins_smp_adsr_2 		 = { InstEdit::modAdsrSmp	, EVENT_MODIFY_B 	, &VAR_SMP.ADSR			[ 0x2 ] , NULL };
const Callback cb_ins_smp_adsr_3 		 = { InstEdit::modAdsrSmp	, EVENT_MODIFY_B 	, &VAR_SMP.ADSR			[ 0x3 ] , NULL };
/* --------------------------------------------------------------------------------------------------------------------------- */            	

/* Generic? Instrument tables ------------------------------------------------------------------------------------------------------------ */            	
const Callback cb_ins_table_transpose_00 = { InstEdit::modByte		, EVENT_MODIFY_B	, &VAR_INSTRUMENT.TABLE.TRANSPOSE	[ 0x0 ]	, NULL };
const Callback cb_ins_table_transpose_01 = { InstEdit::modByte		, EVENT_MODIFY_B	, &VAR_INSTRUMENT.TABLE.TRANSPOSE	[ 0x1 ]	, NULL };
const Callback cb_ins_table_transpose_02 = { InstEdit::modByte		, EVENT_MODIFY_B	, &VAR_INSTRUMENT.TABLE.TRANSPOSE	[ 0x2 ]	, NULL };
const Callback cb_ins_table_transpose_03 = { InstEdit::modByte		, EVENT_MODIFY_B	, &VAR_INSTRUMENT.TABLE.TRANSPOSE	[ 0x3 ]	, NULL };
const Callback cb_ins_table_transpose_04 = { InstEdit::modByte		, EVENT_MODIFY_B	, &VAR_INSTRUMENT.TABLE.TRANSPOSE	[ 0x4 ]	, NULL };
const Callback cb_ins_table_transpose_05 = { InstEdit::modByte		, EVENT_MODIFY_B	, &VAR_INSTRUMENT.TABLE.TRANSPOSE	[ 0x5 ]	, NULL };
const Callback cb_ins_table_transpose_06 = { InstEdit::modByte		, EVENT_MODIFY_B	, &VAR_INSTRUMENT.TABLE.TRANSPOSE	[ 0x6 ]	, NULL };
const Callback cb_ins_table_transpose_07 = { InstEdit::modByte		, EVENT_MODIFY_B	, &VAR_INSTRUMENT.TABLE.TRANSPOSE	[ 0x7 ]	, NULL };
const Callback cb_ins_table_transpose_08 = { InstEdit::modByte		, EVENT_MODIFY_B	, &VAR_INSTRUMENT.TABLE.TRANSPOSE	[ 0x8 ]	, NULL };
const Callback cb_ins_table_transpose_09 = { InstEdit::modByte		, EVENT_MODIFY_B	, &VAR_INSTRUMENT.TABLE.TRANSPOSE	[ 0x9 ]	, NULL };
const Callback cb_ins_table_transpose_0A = { InstEdit::modByte		, EVENT_MODIFY_B	, &VAR_INSTRUMENT.TABLE.TRANSPOSE	[ 0xA ]	, NULL };
const Callback cb_ins_table_transpose_0B = { InstEdit::modByte		, EVENT_MODIFY_B	, &VAR_INSTRUMENT.TABLE.TRANSPOSE	[ 0xB ]	, NULL };
const Callback cb_ins_table_transpose_0C = { InstEdit::modByte		, EVENT_MODIFY_B	, &VAR_INSTRUMENT.TABLE.TRANSPOSE	[ 0xC ]	, NULL };
const Callback cb_ins_table_transpose_0D = { InstEdit::modByte		, EVENT_MODIFY_B	, &VAR_INSTRUMENT.TABLE.TRANSPOSE	[ 0xD ]	, NULL };
const Callback cb_ins_table_transpose_0E = { InstEdit::modByte		, EVENT_MODIFY_B	, &VAR_INSTRUMENT.TABLE.TRANSPOSE	[ 0xE ]	, NULL };
const Callback cb_ins_table_transpose_0F = { InstEdit::modByte		, EVENT_MODIFY_B	, &VAR_INSTRUMENT.TABLE.TRANSPOSE	[ 0xF ]	, NULL };
// Volume
const Callback cb_ins_table_volume_00 	 = { InstEdit::modNibble	, EVENT_MODIFY_B	, &VAR_INSTRUMENT.TABLE.VOLUME		[ 0x0 ]	, NULL };
const Callback cb_ins_table_volume_01 	 = { InstEdit::modNibble	, EVENT_MODIFY_B	, &VAR_INSTRUMENT.TABLE.VOLUME		[ 0x1 ]	, NULL };
const Callback cb_ins_table_volume_02 	 = { InstEdit::modNibble	, EVENT_MODIFY_B	, &VAR_INSTRUMENT.TABLE.VOLUME		[ 0x2 ]	, NULL };
const Callback cb_ins_table_volume_03 	 = { InstEdit::modNibble	, EVENT_MODIFY_B	, &VAR_INSTRUMENT.TABLE.VOLUME		[ 0x3 ]	, NULL };
const Callback cb_ins_table_volume_04 	 = { InstEdit::modNibble	, EVENT_MODIFY_B	, &VAR_INSTRUMENT.TABLE.VOLUME		[ 0x4 ]	, NULL };
const Callback cb_ins_table_volume_05 	 = { InstEdit::modNibble	, EVENT_MODIFY_B	, &VAR_INSTRUMENT.TABLE.VOLUME		[ 0x5 ]	, NULL };
const Callback cb_ins_table_volume_06 	 = { InstEdit::modNibble	, EVENT_MODIFY_B	, &VAR_INSTRUMENT.TABLE.VOLUME		[ 0x6 ]	, NULL };
const Callback cb_ins_table_volume_07 	 = { InstEdit::modNibble	, EVENT_MODIFY_B	, &VAR_INSTRUMENT.TABLE.VOLUME		[ 0x7 ]	, NULL };
const Callback cb_ins_table_volume_08 	 = { InstEdit::modNibble	, EVENT_MODIFY_B	, &VAR_INSTRUMENT.TABLE.VOLUME		[ 0x8 ]	, NULL };
const Callback cb_ins_table_volume_09 	 = { InstEdit::modNibble	, EVENT_MODIFY_B	, &VAR_INSTRUMENT.TABLE.VOLUME		[ 0x9 ]	, NULL };
const Callback cb_ins_table_volume_0A 	 = { InstEdit::modNibble	, EVENT_MODIFY_B	, &VAR_INSTRUMENT.TABLE.VOLUME		[ 0xA ]	, NULL };
const Callback cb_ins_table_volume_0B 	 = { InstEdit::modNibble	, EVENT_MODIFY_B	, &VAR_INSTRUMENT.TABLE.VOLUME		[ 0xB ]	, NULL };
const Callback cb_ins_table_volume_0C 	 = { InstEdit::modNibble	, EVENT_MODIFY_B	, &VAR_INSTRUMENT.TABLE.VOLUME		[ 0xC ]	, NULL };
const Callback cb_ins_table_volume_0D 	 = { InstEdit::modNibble	, EVENT_MODIFY_B	, &VAR_INSTRUMENT.TABLE.VOLUME		[ 0xD ]	, NULL };
const Callback cb_ins_table_volume_0E 	 = { InstEdit::modNibble	, EVENT_MODIFY_B	, &VAR_INSTRUMENT.TABLE.VOLUME		[ 0xE ]	, NULL };
const Callback cb_ins_table_volume_0F 	 = { InstEdit::modNibble	, EVENT_MODIFY_B	, &VAR_INSTRUMENT.TABLE.VOLUME		[ 0xF ]	, NULL };
// CMD2 & Value1
const Callback cb_ins_table_value1_00 	 = { InstEdit::modByte 		, EVENT_MODIFY_B	, &VAR_INSTRUMENT.TABLE.VALUE  [ 0 ][ 0x0 ] , NULL };
const Callback cb_ins_table_value1_01 	 = { InstEdit::modByte 		, EVENT_MODIFY_B	, &VAR_INSTRUMENT.TABLE.VALUE  [ 0 ][ 0x1 ] , NULL };
const Callback cb_ins_table_value1_02 	 = { InstEdit::modByte 		, EVENT_MODIFY_B	, &VAR_INSTRUMENT.TABLE.VALUE  [ 0 ][ 0x2 ] , NULL };
const Callback cb_ins_table_value1_03 	 = { InstEdit::modByte 		, EVENT_MODIFY_B	, &VAR_INSTRUMENT.TABLE.VALUE  [ 0 ][ 0x3 ] , NULL };
const Callback cb_ins_table_value1_04 	 = { InstEdit::modByte 		, EVENT_MODIFY_B	, &VAR_INSTRUMENT.TABLE.VALUE  [ 0 ][ 0x4 ] , NULL };
const Callback cb_ins_table_value1_05 	 = { InstEdit::modByte 		, EVENT_MODIFY_B	, &VAR_INSTRUMENT.TABLE.VALUE  [ 0 ][ 0x5 ] , NULL };
const Callback cb_ins_table_value1_06 	 = { InstEdit::modByte 		, EVENT_MODIFY_B	, &VAR_INSTRUMENT.TABLE.VALUE  [ 0 ][ 0x6 ] , NULL };
const Callback cb_ins_table_value1_07 	 = { InstEdit::modByte 		, EVENT_MODIFY_B	, &VAR_INSTRUMENT.TABLE.VALUE  [ 0 ][ 0x7 ] , NULL };
const Callback cb_ins_table_value1_08 	 = { InstEdit::modByte 		, EVENT_MODIFY_B	, &VAR_INSTRUMENT.TABLE.VALUE  [ 0 ][ 0x8 ] , NULL };
const Callback cb_ins_table_value1_09 	 = { InstEdit::modByte 		, EVENT_MODIFY_B	, &VAR_INSTRUMENT.TABLE.VALUE  [ 0 ][ 0x9 ] , NULL };
const Callback cb_ins_table_value1_0A 	 = { InstEdit::modByte 		, EVENT_MODIFY_B	, &VAR_INSTRUMENT.TABLE.VALUE  [ 0 ][ 0xA ] , NULL };
const Callback cb_ins_table_value1_0B 	 = { InstEdit::modByte 		, EVENT_MODIFY_B	, &VAR_INSTRUMENT.TABLE.VALUE  [ 0 ][ 0xB ] , NULL };
const Callback cb_ins_table_value1_0C 	 = { InstEdit::modByte 		, EVENT_MODIFY_B	, &VAR_INSTRUMENT.TABLE.VALUE  [ 0 ][ 0xC ] , NULL };
const Callback cb_ins_table_value1_0D 	 = { InstEdit::modByte 		, EVENT_MODIFY_B	, &VAR_INSTRUMENT.TABLE.VALUE  [ 0 ][ 0xD ] , NULL };
const Callback cb_ins_table_value1_0E 	 = { InstEdit::modByte 		, EVENT_MODIFY_B	, &VAR_INSTRUMENT.TABLE.VALUE  [ 0 ][ 0xE ] , NULL };
const Callback cb_ins_table_value1_0F 	 = { InstEdit::modByte 		, EVENT_MODIFY_B	, &VAR_INSTRUMENT.TABLE.VALUE  [ 0 ][ 0xF ] , NULL };
const Callback cb_ins_table_command1_00  = { InstEdit::modVal27		, EVENT_MODIFY_B	, &VAR_INSTRUMENT.TABLE.COMMAND[ 0 ][ 0x0 ] , NULL };
const Callback cb_ins_table_command1_01  = { InstEdit::modVal27		, EVENT_MODIFY_B	, &VAR_INSTRUMENT.TABLE.COMMAND[ 0 ][ 0x1 ] , NULL };
const Callback cb_ins_table_command1_02  = { InstEdit::modVal27		, EVENT_MODIFY_B	, &VAR_INSTRUMENT.TABLE.COMMAND[ 0 ][ 0x2 ] , NULL };
const Callback cb_ins_table_command1_03  = { InstEdit::modVal27		, EVENT_MODIFY_B	, &VAR_INSTRUMENT.TABLE.COMMAND[ 0 ][ 0x3 ] , NULL };
const Callback cb_ins_table_command1_04  = { InstEdit::modVal27		, EVENT_MODIFY_B	, &VAR_INSTRUMENT.TABLE.COMMAND[ 0 ][ 0x4 ] , NULL };
const Callback cb_ins_table_command1_05  = { InstEdit::modVal27		, EVENT_MODIFY_B	, &VAR_INSTRUMENT.TABLE.COMMAND[ 0 ][ 0x5 ] , NULL };
const Callback cb_ins_table_command1_06  = { InstEdit::modVal27		, EVENT_MODIFY_B	, &VAR_INSTRUMENT.TABLE.COMMAND[ 0 ][ 0x6 ] , NULL };
const Callback cb_ins_table_command1_07  = { InstEdit::modVal27		, EVENT_MODIFY_B	, &VAR_INSTRUMENT.TABLE.COMMAND[ 0 ][ 0x7 ] , NULL };
const Callback cb_ins_table_command1_08  = { InstEdit::modVal27		, EVENT_MODIFY_B	, &VAR_INSTRUMENT.TABLE.COMMAND[ 0 ][ 0x8 ] , NULL };
const Callback cb_ins_table_command1_09  = { InstEdit::modVal27		, EVENT_MODIFY_B	, &VAR_INSTRUMENT.TABLE.COMMAND[ 0 ][ 0x9 ] , NULL };
const Callback cb_ins_table_command1_0A  = { InstEdit::modVal27		, EVENT_MODIFY_B	, &VAR_INSTRUMENT.TABLE.COMMAND[ 0 ][ 0xA ] , NULL };
const Callback cb_ins_table_command1_0B  = { InstEdit::modVal27		, EVENT_MODIFY_B	, &VAR_INSTRUMENT.TABLE.COMMAND[ 0 ][ 0xB ] , NULL };
const Callback cb_ins_table_command1_0C  = { InstEdit::modVal27		, EVENT_MODIFY_B	, &VAR_INSTRUMENT.TABLE.COMMAND[ 0 ][ 0xC ] , NULL };
const Callback cb_ins_table_command1_0D  = { InstEdit::modVal27		, EVENT_MODIFY_B	, &VAR_INSTRUMENT.TABLE.COMMAND[ 0 ][ 0xD ] , NULL };
const Callback cb_ins_table_command1_0E  = { InstEdit::modVal27		, EVENT_MODIFY_B	, &VAR_INSTRUMENT.TABLE.COMMAND[ 0 ][ 0xE ] , NULL };
const Callback cb_ins_table_command1_0F  = { InstEdit::modVal27		, EVENT_MODIFY_B	, &VAR_INSTRUMENT.TABLE.COMMAND[ 0 ][ 0xF ] , NULL };
// CMD2 & Value2
const Callback cb_ins_table_value2_00 	 = { InstEdit::modByte   	, EVENT_MODIFY_B	, &VAR_INSTRUMENT.TABLE.VALUE  [ 1 ][ 0x0 ]	, NULL };
const Callback cb_ins_table_value2_01 	 = { InstEdit::modByte   	, EVENT_MODIFY_B	, &VAR_INSTRUMENT.TABLE.VALUE  [ 1 ][ 0x1 ]	, NULL };
const Callback cb_ins_table_value2_02 	 = { InstEdit::modByte   	, EVENT_MODIFY_B	, &VAR_INSTRUMENT.TABLE.VALUE  [ 1 ][ 0x2 ]	, NULL };
const Callback cb_ins_table_value2_03 	 = { InstEdit::modByte   	, EVENT_MODIFY_B	, &VAR_INSTRUMENT.TABLE.VALUE  [ 1 ][ 0x3 ]	, NULL };
const Callback cb_ins_table_value2_04 	 = { InstEdit::modByte   	, EVENT_MODIFY_B	, &VAR_INSTRUMENT.TABLE.VALUE  [ 1 ][ 0x4 ]	, NULL };
const Callback cb_ins_table_value2_05 	 = { InstEdit::modByte   	, EVENT_MODIFY_B	, &VAR_INSTRUMENT.TABLE.VALUE  [ 1 ][ 0x5 ]	, NULL };
const Callback cb_ins_table_value2_06 	 = { InstEdit::modByte   	, EVENT_MODIFY_B	, &VAR_INSTRUMENT.TABLE.VALUE  [ 1 ][ 0x6 ]	, NULL };
const Callback cb_ins_table_value2_07 	 = { InstEdit::modByte   	, EVENT_MODIFY_B	, &VAR_INSTRUMENT.TABLE.VALUE  [ 1 ][ 0x7 ]	, NULL };
const Callback cb_ins_table_value2_08 	 = { InstEdit::modByte   	, EVENT_MODIFY_B	, &VAR_INSTRUMENT.TABLE.VALUE  [ 1 ][ 0x8 ]	, NULL };
const Callback cb_ins_table_value2_09 	 = { InstEdit::modByte   	, EVENT_MODIFY_B	, &VAR_INSTRUMENT.TABLE.VALUE  [ 1 ][ 0x9 ]	, NULL };
const Callback cb_ins_table_value2_0A 	 = { InstEdit::modByte   	, EVENT_MODIFY_B	, &VAR_INSTRUMENT.TABLE.VALUE  [ 1 ][ 0xA ]	, NULL };
const Callback cb_ins_table_value2_0B 	 = { InstEdit::modByte   	, EVENT_MODIFY_B	, &VAR_INSTRUMENT.TABLE.VALUE  [ 1 ][ 0xB ]	, NULL };
const Callback cb_ins_table_value2_0C 	 = { InstEdit::modByte   	, EVENT_MODIFY_B	, &VAR_INSTRUMENT.TABLE.VALUE  [ 1 ][ 0xC ]	, NULL };
const Callback cb_ins_table_value2_0D 	 = { InstEdit::modByte   	, EVENT_MODIFY_B	, &VAR_INSTRUMENT.TABLE.VALUE  [ 1 ][ 0xD ]	, NULL };
const Callback cb_ins_table_value2_0E 	 = { InstEdit::modByte   	, EVENT_MODIFY_B	, &VAR_INSTRUMENT.TABLE.VALUE  [ 1 ][ 0xE ]	, NULL };
const Callback cb_ins_table_value2_0F 	 = { InstEdit::modByte   	, EVENT_MODIFY_B	, &VAR_INSTRUMENT.TABLE.VALUE  [ 1 ][ 0xF ]	, NULL };
const Callback cb_ins_table_command2_00  = { InstEdit::modVal27  	, EVENT_MODIFY_B	, &VAR_INSTRUMENT.TABLE.COMMAND[ 1 ][ 0x0 ]	, NULL };
const Callback cb_ins_table_command2_01  = { InstEdit::modVal27  	, EVENT_MODIFY_B	, &VAR_INSTRUMENT.TABLE.COMMAND[ 1 ][ 0x1 ]	, NULL };
const Callback cb_ins_table_command2_02  = { InstEdit::modVal27  	, EVENT_MODIFY_B	, &VAR_INSTRUMENT.TABLE.COMMAND[ 1 ][ 0x2 ]	, NULL };
const Callback cb_ins_table_command2_03  = { InstEdit::modVal27  	, EVENT_MODIFY_B	, &VAR_INSTRUMENT.TABLE.COMMAND[ 1 ][ 0x3 ]	, NULL };
const Callback cb_ins_table_command2_04  = { InstEdit::modVal27  	, EVENT_MODIFY_B	, &VAR_INSTRUMENT.TABLE.COMMAND[ 1 ][ 0x4 ]	, NULL };
const Callback cb_ins_table_command2_05  = { InstEdit::modVal27  	, EVENT_MODIFY_B	, &VAR_INSTRUMENT.TABLE.COMMAND[ 1 ][ 0x5 ]	, NULL };
const Callback cb_ins_table_command2_06  = { InstEdit::modVal27  	, EVENT_MODIFY_B	, &VAR_INSTRUMENT.TABLE.COMMAND[ 1 ][ 0x6 ]	, NULL };
const Callback cb_ins_table_command2_07  = { InstEdit::modVal27  	, EVENT_MODIFY_B	, &VAR_INSTRUMENT.TABLE.COMMAND[ 1 ][ 0x7 ]	, NULL };
const Callback cb_ins_table_command2_08  = { InstEdit::modVal27  	, EVENT_MODIFY_B	, &VAR_INSTRUMENT.TABLE.COMMAND[ 1 ][ 0x8 ]	, NULL };
const Callback cb_ins_table_command2_09  = { InstEdit::modVal27  	, EVENT_MODIFY_B	, &VAR_INSTRUMENT.TABLE.COMMAND[ 1 ][ 0x9 ]	, NULL };
const Callback cb_ins_table_command2_0A  = { InstEdit::modVal27  	, EVENT_MODIFY_B	, &VAR_INSTRUMENT.TABLE.COMMAND[ 1 ][ 0xA ]	, NULL };
const Callback cb_ins_table_command2_0B  = { InstEdit::modVal27  	, EVENT_MODIFY_B	, &VAR_INSTRUMENT.TABLE.COMMAND[ 1 ][ 0xB ]	, NULL };
const Callback cb_ins_table_command2_0C  = { InstEdit::modVal27  	, EVENT_MODIFY_B	, &VAR_INSTRUMENT.TABLE.COMMAND[ 1 ][ 0xC ]	, NULL };
const Callback cb_ins_table_command2_0D  = { InstEdit::modVal27  	, EVENT_MODIFY_B	, &VAR_INSTRUMENT.TABLE.COMMAND[ 1 ][ 0xD ]	, NULL };
const Callback cb_ins_table_command2_0E  = { InstEdit::modVal27  	, EVENT_MODIFY_B	, &VAR_INSTRUMENT.TABLE.COMMAND[ 1 ][ 0xE ]	, NULL };
const Callback cb_ins_table_command2_0F  = { InstEdit::modVal27  	, EVENT_MODIFY_B	, &VAR_INSTRUMENT.TABLE.COMMAND[ 1 ][ 0xF ]	, NULL };
