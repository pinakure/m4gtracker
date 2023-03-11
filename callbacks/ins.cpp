#include "ins.hpp"
#include "debug.hpp"
#include "../data/data.hpp"
#include "../data/helpers.hpp"
#include "../macros.h"
#include "../modules/regionhandler/regionhandler.hpp"
#include "../modules/spu/synth.hpp"
#include "../modules/gpu/gpu.hpp"
#include "../modules/key/key.hpp"

/* Global callbacks ---------------------------------------------------------------------------------------------------------- */
const Callback cb_ins_index				 = { InstEdit::index		, EVENT_MODIFY_B 	, &VAR_CFG.CURRENTINSTRUMENT	, NULL };
const Callback cb_ins_name				 = { ALPHA6					, EVENT_KEYDOWN_B 	, &VAR_INSTRUMENT.NAME			, NULL };
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
const Callback cb_ins_mixpercent  		 = { InstEdit::mod3b		, EVENT_MODIFY_B 	, &VAR_WAV.MIXPERCENT 			, NULL };
const Callback cb_ins_wav_op1type 		 = { InstEdit::mod3b		, EVENT_MODIFY_B 	, &VAR_WAV.OP1_TYPE				, NULL };
const Callback cb_ins_wav_op2type 		 = { InstEdit::mod3b		, EVENT_MODIFY_B 	, &VAR_WAV.OP2_TYPE				, NULL };
const Callback cb_ins_wav_op3type 		 = { InstEdit::mod3b		, EVENT_MODIFY_B 	, &VAR_WAV.OP3_TYPE				, NULL };
const Callback cb_ins_wav_op4type 		 = { InstEdit::mod3b		, EVENT_MODIFY_B 	, &VAR_WAV.OP4_TYPE				, NULL };
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
const Callback cb_ins_wav_preset0		 = { InstEdit::wavPreset	, EVENT_KEYUP_B		, &VAR_WAV.WAVPRESET	[ 0x0 ] , NULL };
const Callback cb_ins_wav_preset1		 = { InstEdit::wavPreset	, EVENT_KEYUP_B		, &VAR_WAV.WAVPRESET	[ 0x1 ] , NULL };
const Callback cb_ins_wav_preset2		 = { InstEdit::wavPreset	, EVENT_KEYUP_B		, &VAR_WAV.WAVPRESET	[ 0x2 ] , NULL };
const Callback cb_ins_wav_preset3		 = { InstEdit::wavPreset	, EVENT_KEYUP_B		, &VAR_WAV.WAVPRESET	[ 0x3 ] , NULL };
const Callback cb_ins_wav_preset4		 = { InstEdit::wavPreset	, EVENT_KEYUP_B		, &VAR_WAV.WAVPRESET	[ 0x4 ] , NULL };
const Callback cb_ins_wav_preset5		 = { InstEdit::wavPreset	, EVENT_KEYUP_B		, &VAR_WAV.WAVPRESET	[ 0x5 ] , NULL };
/* FMW instrument settings --------------------------------------------------------------------------------------------------- */            	
const Callback cb_ins_algorithm			 = { InstEdit::modAlgorithm	, EVENT_MODIFY_B 	, &VAR_FMW.ALGORITHM			, NULL };
const Callback cb_ins_fm_mult			 = { InstEdit::modNibble	, EVENT_MODIFY_B 	, &VAR_FMW.MULT					, NULL };
const Callback cb_ins_fm_op1type		 = { InstEdit::mod3b		, EVENT_MODIFY_B 	, &VAR_FMW.OP1_TYPE				, NULL };
const Callback cb_ins_fm_op2type		 = { InstEdit::mod3b		, EVENT_MODIFY_B 	, &VAR_FMW.OP2_TYPE				, NULL };
const Callback cb_ins_fm_op3type		 = { InstEdit::mod3b		, EVENT_MODIFY_B 	, &VAR_FMW.OP3_TYPE				, NULL };
const Callback cb_ins_fm_op4type		 = { InstEdit::mod3b		, EVENT_MODIFY_B 	, &VAR_FMW.OP4_TYPE				, NULL };
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
/*---------------------------------------------------------------------------------------------------------------------------------------- */

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
		
		case MESSAGE_CANCEL:			
			regHnd.controlClear	( regHnd.control );
			repack();//dispatchMessage		( MESSAGE_OTHER_REFRESH_DATA );
			break;
		
		case MESSAGE_OTHER_REFRESH_DATA:
			repack();
			break;
		
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
	
	static int adsr_position;
	if( VAR_CFG.CURRENTINSTRUMENT == VAR_CHANNEL[ CHANNEL_WAV ].inst ) 
	if( adsr_position != Synth::wav_adsr_position ){
		adsr_position = Synth::wav_adsr_position;
		if(!KEY.press(KEY_SELECT)) viewQuadADSR( Synth::wav_adsr_table, adsr_position);
		else viewWaveFormWav( );
	}
	
	InstEdit::synchronize();
}

void InstEdit::updateFmw( RegionHandler* rh ){
	const Control *cname = &INS_FMW_CONTROLS[CONTROL_INS_FMW_NAME];
	STRING(false, cname->x, cname->y, cname->var);
	
	
	DECIMAL(6, 15, 0xAA, InstEdit::unpackFmw( &VAR_INSTRUMENT ).ALGORITHM );
	
	static int adsr_position;
	if( VAR_CFG.CURRENTINSTRUMENT == VAR_CHANNEL[ CHANNEL_FMW ].inst ) 
	if( adsr_position != Synth::fmw_adsr_position ){
		adsr_position = Synth::fmw_adsr_position;
		if(!KEY.press(KEY_SELECT)) viewQuadADSR( Synth::fmw_adsr_table, adsr_position);
		else viewWaveFormFmw();
	}
	
	InstEdit::synchronize();
}

void InstEdit::updateSmp( RegionHandler* rh ){
	const Control *cname = &INS_SMP_CONTROLS[CONTROL_INS_SMP_NAME];
	STRING(false, cname->x, cname->y, cname->var);
	
	static int adsr_position;
	if( VAR_CFG.CURRENTINSTRUMENT == VAR_CHANNEL[ CHANNEL_SMP ].inst ) 
	if( adsr_position != Synth::smp_adsr_position ){
		adsr_position = Synth::smp_adsr_position;
		if(!KEY.press(KEY_SELECT)) viewADSR( Synth::smp_adsr_table, adsr_position);
		else viewWaveFormSmp();
	}
	
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

void InstEdit::viewWaveFormFmw( ){
	gpu.vs->clear();
	for(int x=0; x < 0x40; x+=2){
		gpu.vs->set( x>>1, 15 - ( VAR_FMW.WAVEDATA[ x>>2 ]>>2) );
		gpu.vs->set( x>>1, 15 + ( VAR_FMW.WAVEDATA[ x>>2 ]>>2) );
	}
	gpu.vs->draw(14,6);
}

void InstEdit::viewWaveFormSmp( ){
	gpu.vs->clear();
	/*
	for(int x=0; x < 0x40; x+=2){
		gpu.vs->set( x>>1, 15 - ( VAR_SMP.WAVEDATA[ x>>2 ]>>2) );
		gpu.vs->set( x>>1, 15 + ( VAR_SMP.WAVEDATA[ x>>2 ]>>2) );
	}
	*/
	gpu.vs->draw(14,6);
}

void InstEdit::viewWaveFormWav( ){
	gpu.vs->clear();
	for(int x=0; x < 0x40; x+=2){
		gpu.vs->set( x>>1, 15 - ( VAR_WAV.WAVEDATA[ x>>2 ]) );
		gpu.vs->set( x>>1, 15 + ( VAR_WAV.WAVEDATA[ x>>2 ]) );
	}
	gpu.vs->draw(14,6);
}

void InstEdit::viewQuadADSR( u8 adsr_tables[ 4 ][ 0x40 ] , u8 adsr_position ){

	static u8 last_adsr_position;
	
	if(regHnd.region != &REGION_MAP_2_INS ) return;
	if( last_adsr_position == adsr_position) return;
	last_adsr_position = adsr_position;
	gpu.vs->clear();
	
	for(int x=0,d=0;x<0x40; x+=2,d=x>>1){
		gpu.vs->set( d,  7 - ( adsr_tables[ 0 ][ x ] >> 1 ) );
		gpu.vs->set( d, 15 - ( adsr_tables[ 1 ][ x ] >> 1 ) );
		gpu.vs->set( d, 23 - ( adsr_tables[ 2 ][ x ] >> 1 ) );
		gpu.vs->set( d, 31 - ( adsr_tables[ 3 ][ x ] >> 1 ) );
		
	}
	
	//DECIMAL_DOUBLE( 14,2, 7, adsr_position );

	if( adsr_position < 0x3F ){
		VISPOS1(14, 1,	0xFF, adsr_position>>2);
		VISPOS2(14, 18, 0xFF, adsr_position>>2);
	}
	gpu.vs->draw(14,6);
}

void InstEdit::viewADSR ( u8 adsr_table[0x40] , u8 adsr_position ){

	static u8 last_adsr_position;
	
	if(regHnd.region != &REGION_MAP_2_INS ) return;
	if( last_adsr_position == adsr_position) return;
	last_adsr_position = adsr_position;
	
	gpu.vs->clear();
	
	for(int x=0;x<0x40; x+=2){
		gpu.vs->set( x >> 1, 31 - (( adsr_table[ x ]<<1) ) );
	}
	
	//DECIMAL_DOUBLE( 14,2, 7, adsr_position );

	if( adsr_position < 0x3F ){
		VISPOS1(14, 1,	0xFF, adsr_position>>2);
		VISPOS2(14, 18, 0xFF, adsr_position>>2);
	}
	gpu.vs->draw(14,6);
}

