#ifndef _DATA_CONTROLS
#define _DATA_CONTROLS 

#include "control.hpp"

enum E_SNG_CONTROLS { 
	CONTROL_SNG_GROOVE_00,			CONTROL_SNG_GROOVE_01,			CONTROL_SNG_GROOVE_02,			CONTROL_SNG_GROOVE_03,
	CONTROL_SNG_GROOVE_04,			CONTROL_SNG_GROOVE_05,			CONTROL_SNG_GROOVE_06,			CONTROL_SNG_GROOVE_07,
	CONTROL_SNG_GROOVE_08,			CONTROL_SNG_GROOVE_09,			CONTROL_SNG_GROOVE_0A,			CONTROL_SNG_GROOVE_0B,
	CONTROL_SNG_GROOVE_0C,			CONTROL_SNG_GROOVE_0D,			CONTROL_SNG_GROOVE_0E,			CONTROL_SNG_GROOVE_0F,
	CONTROL_SNG_SONGSELECTOR,		CONTROL_SNG_LOAD,				CONTROL_SNG_SAVE,				CONTROL_SNG_PURGE,
	CONTROL_SNG_COPY,				CONTROL_SNG_ERASE,				CONTROL_SNG_ARTIST,				CONTROL_SNG_TITLE,
	CONTROL_SNG_TEMPOTAP,			CONTROL_SNG_PATTERNLENGTH,		CONTROL_SNG_TRANSPOSE,			CONTROL_SNG_BPM,
	CONTROL_SNG_GROOVE_SWITCH,		CONTROL_SNG_COUNT,				CONTROL_SNG_MAX
};	
extern const Control SNG_CONTROLS[CONTROL_SNG_MAX]; 

enum E_LIVE1_CONTROLS { 
	CONTROL_LIVE1_RETRIG,			CONTROL_LIVE1_SPEED,			CONTROL_LIVE1_QUANTIZE1,
	CONTROL_LIVE1_VAL_LEFT_00,		CONTROL_LIVE1_VAL_LEFT_01,		CONTROL_LIVE1_VAL_LEFT_02,		CONTROL_LIVE1_VAL_LEFT_03,
	CONTROL_LIVE1_VAL_LEFT_04,		CONTROL_LIVE1_VAL_LEFT_05,		CONTROL_LIVE1_VAL_LEFT_06,		CONTROL_LIVE1_VAL_LEFT_07,
	CONTROL_LIVE1_VOL_LEFT_00,		CONTROL_LIVE1_VOL_LEFT_01,		CONTROL_LIVE1_VOL_LEFT_02,		CONTROL_LIVE1_VOL_LEFT_03,
	CONTROL_LIVE1_VOL_LEFT_04,		CONTROL_LIVE1_VOL_LEFT_05,		CONTROL_LIVE1_VOL_LEFT_06,		CONTROL_LIVE1_VOL_LEFT_07,
	CONTROL_LIVE1_INS_LEFT_00,		CONTROL_LIVE1_INS_LEFT_01,		CONTROL_LIVE1_INS_LEFT_02,		CONTROL_LIVE1_INS_LEFT_03,
	CONTROL_LIVE1_INS_LEFT_04,		CONTROL_LIVE1_INS_LEFT_05,		CONTROL_LIVE1_INS_LEFT_06,		CONTROL_LIVE1_INS_LEFT_07,
	CONTROL_LIVE1_CHAN_LEFT_00,		CONTROL_LIVE1_CHAN_LEFT_01,		CONTROL_LIVE1_CHAN_LEFT_02,		CONTROL_LIVE1_CHAN_LEFT_03,
	CONTROL_LIVE1_CHAN_LEFT_04,		CONTROL_LIVE1_CHAN_LEFT_05,		CONTROL_LIVE1_CHAN_LEFT_06,		CONTROL_LIVE1_CHAN_LEFT_07,
	CONTROL_LIVE1_KEY_LEFT_00,		CONTROL_LIVE1_KEY_LEFT_01,		CONTROL_LIVE1_KEY_LEFT_02,		CONTROL_LIVE1_KEY_LEFT_03,
	CONTROL_LIVE1_KEY_LEFT_04,		CONTROL_LIVE1_KEY_LEFT_05,		CONTROL_LIVE1_KEY_LEFT_06,		CONTROL_LIVE1_KEY_LEFT_07,
	CONTROL_LIVE1_CMD_LEFT_00,		CONTROL_LIVE1_CMD_LEFT_01,		CONTROL_LIVE1_CMD_LEFT_02,		CONTROL_LIVE1_CMD_LEFT_03,
	CONTROL_LIVE1_CMD_LEFT_04,		CONTROL_LIVE1_CMD_LEFT_05,		CONTROL_LIVE1_CMD_LEFT_06,		CONTROL_LIVE1_CMD_LEFT_07,
	CONTROL_LIVE1_VAL_RIGHT_00,		CONTROL_LIVE1_VAL_RIGHT_01,		CONTROL_LIVE1_VAL_RIGHT_02,		CONTROL_LIVE1_VAL_RIGHT_03,
	CONTROL_LIVE1_VAL_RIGHT_04,		CONTROL_LIVE1_VAL_RIGHT_05,		CONTROL_LIVE1_VAL_RIGHT_06,		CONTROL_LIVE1_VAL_RIGHT_07,
	CONTROL_LIVE1_CMD_RIGHT_00,		CONTROL_LIVE1_CMD_RIGHT_01,		CONTROL_LIVE1_CMD_RIGHT_02,		CONTROL_LIVE1_CMD_RIGHT_03,
	CONTROL_LIVE1_CMD_RIGHT_04,		CONTROL_LIVE1_CMD_RIGHT_05,		CONTROL_LIVE1_CMD_RIGHT_06,		CONTROL_LIVE1_CMD_RIGHT_07,
	CONTROL_LIVE1_CHAN_RIGHT_00,	CONTROL_LIVE1_CHAN_RIGHT_01,	CONTROL_LIVE1_CHAN_RIGHT_02,	CONTROL_LIVE1_CHAN_RIGHT_03,
	CONTROL_LIVE1_CHAN_RIGHT_04,	CONTROL_LIVE1_CHAN_RIGHT_05,	CONTROL_LIVE1_CHAN_RIGHT_06,	CONTROL_LIVE1_CHAN_RIGHT_07,
	CONTROL_LIVE1_INS_RIGHT_00,		CONTROL_LIVE1_INS_RIGHT_01,		CONTROL_LIVE1_INS_RIGHT_02,		CONTROL_LIVE1_INS_RIGHT_03,
	CONTROL_LIVE1_INS_RIGHT_04,		CONTROL_LIVE1_INS_RIGHT_05,		CONTROL_LIVE1_INS_RIGHT_06,		CONTROL_LIVE1_INS_RIGHT_07,
	CONTROL_LIVE1_VOL_RIGHT_00,		CONTROL_LIVE1_VOL_RIGHT_01,		CONTROL_LIVE1_VOL_RIGHT_02,		CONTROL_LIVE1_VOL_RIGHT_03,
	CONTROL_LIVE1_VOL_RIGHT_04,		CONTROL_LIVE1_VOL_RIGHT_05,		CONTROL_LIVE1_VOL_RIGHT_06,		CONTROL_LIVE1_VOL_RIGHT_07,
	CONTROL_LIVE1_KEY_RIGHT_00,		CONTROL_LIVE1_KEY_RIGHT_01,		CONTROL_LIVE1_KEY_RIGHT_02,		CONTROL_LIVE1_KEY_RIGHT_03,
	CONTROL_LIVE1_KEY_RIGHT_04,		CONTROL_LIVE1_KEY_RIGHT_05,		CONTROL_LIVE1_KEY_RIGHT_06,		CONTROL_LIVE1_KEY_RIGHT_07,
	CONTROL_LIVE1_COUNT,			CONTROL_LIVE1_MAX 
};
extern const Control LIVE1_CONTROLS[CONTROL_LIVE1_MAX];

enum E_LIVE2_CONTROLS { 
	CONTROL_LIVE2_CHANNEL1,			CONTROL_LIVE2_LIVEMODE,			CONTROL_LIVE2_MIDICHAN2,		CONTROL_LIVE2_OCTAVE,
	CONTROL_LIVE2_MIDICHAN1,		CONTROL_LIVE2_CHANNEL2,			CONTROL_LIVE2_TRANSPOSE,		CONTROL_LIVE2_QUANTIZE2,
	CONTROL_LIVE2_COUNT,			CONTROL_LIVE2_MAX
};	
extern const Control LIVE2_CONTROLS[CONTROL_LIVE2_MAX]; 


enum E_LOOKNFEEL_CONTROLS { 
	CONTROL_LOOKNFEEL_MENU1, 		CONTROL_LOOKNFEEL_INTERFACE,	CONTROL_LOOKNFEEL_FONT, 		CONTROL_LOOKNFEEL_BORDER, 
	CONTROL_LOOKNFEEL_SHOWLOGO, 	CONTROL_LOOKNFEEL_STARTSFX, 	CONTROL_LOOKNFEEL_EDITPAL,  	CONTROL_LOOKNFEEL_COUNT, 
	CONTROL_LOOKNFEEL_MAX
};
extern const Control LOOKNFEEL_CONTROLS[CONTROL_LOOKNFEEL_MAX];

enum E_LINKMODE_CONTROLS { 
	CONTROL_LINKMODE_MENU2,			CONTROL_LINKMODE_MODE,			CONTROL_LINKMODE_MASTER,			CONTROL_LINKMODE_MIDICHAN,
	CONTROL_LINKMODE_TEMPO,			CONTROL_LINKMODE_RECEIVE,		CONTROL_LINKMODE_SEND,				CONTROL_LINKMODE_COUNT,
	CONTROL_LINKMODE_MAX
};
extern const Control LINKMODE_CONTROLS[CONTROL_LINKMODE_MAX];

enum E_BEHAVIOR_CONTROLS { 
	CONTROL_BEHAVIOR_MENU3,			CONTROL_BEHAVIOR_AUTOLOAD,		CONTROL_BEHAVIOR_KEYRATE,		CONTROL_BEHAVIOR_BUTTONSET,
	CONTROL_BEHAVIOR_SAVE,			CONTROL_BEHAVIOR_LOAD,			CONTROL_BEHAVIOR_DEFAULTS,		CONTROL_BEHAVIOR_COUNT,
	CONTROL_BEHAVIOR_MAX
};
extern const Control BEHAVIOR_CONTROLS[CONTROL_BEHAVIOR_MAX];

enum E_TRACKER_CONTROLS { 
	CONTROL_TRACKER_MENU4,			CONTROL_TRACKER_FINETUNE,		CONTROL_TRACKER_PRELISTEN,		CONTROL_TRACKER_TRANSPOSE,
	CONTROL_TRACKER_INPUTMODE,		CONTROL_TRACKER_SOUNDBIAS,		CONTROL_TRACKER_MIXER,			CONTROL_TRACKER_COUNT,
	CONTROL_TRACKER_MAX
};
extern const Control TRACKER_CONTROLS[CONTROL_TRACKER_MAX];

enum E_MEMORY_CONTROLS { 
	CONTROL_MEMORY_MENU5,			CONTROL_MEMORY_PREFETCH,		CONTROL_MEMORY_SLOTUSAGE,		CONTROL_MEMORY_PURGESONGS,
	CONTROL_MEMORY_MEMORYTEST,		CONTROL_MEMORY_REINIT,			CONTROL_MEMORY_RESET,			CONTROL_MEMORY_COUNT,
	CONTROL_MEMORY_MAX	
};
extern const Control MEMORY_CONTROLS[CONTROL_MEMORY_MAX];

enum E_INS_PWM_CONTROLS { 
	CONTROL_INS_PWM_INDEX,
	CONTROL_INS_PWM_TSP_TABLE_00,	CONTROL_INS_PWM_TSP_TABLE_01,	CONTROL_INS_PWM_TSP_TABLE_02,	CONTROL_INS_PWM_TSP_TABLE_03,
	CONTROL_INS_PWM_TSP_TABLE_04,	CONTROL_INS_PWM_TSP_TABLE_05,	CONTROL_INS_PWM_TSP_TABLE_06,	CONTROL_INS_PWM_TSP_TABLE_07,
	CONTROL_INS_PWM_TSP_TABLE_08,	CONTROL_INS_PWM_TSP_TABLE_09,	CONTROL_INS_PWM_TSP_TABLE_0A,	CONTROL_INS_PWM_TSP_TABLE_0B,
	CONTROL_INS_PWM_TSP_TABLE_0C,	CONTROL_INS_PWM_TSP_TABLE_0D,	CONTROL_INS_PWM_TSP_TABLE_0E,	CONTROL_INS_PWM_TSP_TABLE_0F,
	CONTROL_INS_PWM_VOLUMEFADE,		CONTROL_INS_PWM_TSP_ENVELOPE,	CONTROL_INS_PWM_TSP_MODE,		CONTROL_INS_PWM_TSP_ENABLE,
	CONTROL_INS_PWM_VOL_ENABLE,		CONTROL_INS_PWM_VOL_MODE,		CONTROL_INS_PWM_VOL_ENVELOPE,	CONTROL_INS_PWM_NAME,
	CONTROL_INS_PWM_TYPE,			CONTROL_INS_PWM_DUTYCYCLE,		CONTROL_INS_PWM_DUTYLEN,		CONTROL_INS_PWM_SWEEPDIR,
	CONTROL_INS_PWM_SWEEPSPEED,		CONTROL_INS_PWM_SWEEPSTEPS,		CONTROL_INS_PWM_STARTLEVEL,		CONTROL_INS_PWM_ENVELOPEDIR,
	CONTROL_INS_PWM_VOL_TABLE_00,	CONTROL_INS_PWM_VOL_TABLE_01,	CONTROL_INS_PWM_VOL_TABLE_02,	CONTROL_INS_PWM_VOL_TABLE_03,
	CONTROL_INS_PWM_VOL_TABLE_04,	CONTROL_INS_PWM_VOL_TABLE_05,	CONTROL_INS_PWM_VOL_TABLE_06,	CONTROL_INS_PWM_VOL_TABLE_07,
	CONTROL_INS_PWM_VOL_TABLE_08,	CONTROL_INS_PWM_VOL_TABLE_09,	CONTROL_INS_PWM_VOL_TABLE_0A,	CONTROL_INS_PWM_VOL_TABLE_0B,
	CONTROL_INS_PWM_VOL_TABLE_0C,	CONTROL_INS_PWM_VOL_TABLE_0D,	CONTROL_INS_PWM_VOL_TABLE_0E,	CONTROL_INS_PWM_VOL_TABLE_0F,
	CONTROL_INS_PWM_COUNT,			CONTROL_INS_PWM_MAX
};
extern const Control INS_PWM_CONTROLS[CONTROL_INS_PWM_MAX];

enum E_INS_WAV_CONTROLS { 
	CONTROL_INS_WAV_INDEX,			CONTROL_INS_WAV_NAME,			CONTROL_INS_WAV_TYPE,			CONTROL_INS_WAV_PHASE,
	CONTROL_INS_WAV_AM,				CONTROL_INS_WAV_MIXPERCENT,		CONTROL_INS_WAV_OP1_TYPE,		CONTROL_INS_WAV_OP1_ADSR_00,
	CONTROL_INS_WAV_OP1_ADSR_01,	CONTROL_INS_WAV_OP1_ADSR_02,	CONTROL_INS_WAV_OP1_ADSR_03,	CONTROL_INS_WAV_OP2_TYPE,
	CONTROL_INS_WAV_OP3_TYPE,		CONTROL_INS_WAV_OP4_TYPE,		CONTROL_INS_WAV_WAVPRESET_00,	CONTROL_INS_WAV_WAVPRESET_01,
	CONTROL_INS_WAV_WAVPRESET_02,	CONTROL_INS_WAV_WAVPRESET_03,	CONTROL_INS_WAV_WAVPRESET_04,	CONTROL_INS_WAV_WAVPRESET_05,
	CONTROL_INS_WAV_OP2_ADSR_00,	CONTROL_INS_WAV_OP2_ADSR_01,	CONTROL_INS_WAV_OP2_ADSR_02,	CONTROL_INS_WAV_OP2_ADSR_03,
	CONTROL_INS_WAV_OP3_ADSR_00,	CONTROL_INS_WAV_OP3_ADSR_01,	CONTROL_INS_WAV_OP3_ADSR_02,	CONTROL_INS_WAV_OP3_ADSR_03,
	CONTROL_INS_WAV_OP4_ADSR_00,	CONTROL_INS_WAV_OP4_ADSR_01,	CONTROL_INS_WAV_OP4_ADSR_02,	CONTROL_INS_WAV_OP4_ADSR_03,
	CONTROL_INS_WAV_COUNT,			CONTROL_INS_WAV_MAX
};
extern const Control INS_WAV_CONTROLS[CONTROL_INS_WAV_MAX];

enum E_INS_FMW_CONTROLS { 
	CONTROL_INS_FMW_INDEX,			CONTROL_INS_FMW_NAME,			CONTROL_INS_FMW_TYPE,
	CONTROL_INS_FMW_FM2_TYPE,		CONTROL_INS_FMW_FM4_TYPE,		CONTROL_INS_FMW_FM3_TYPE,		CONTROL_INS_FMW_FM1_TYPE,
	CONTROL_INS_FMW_MULT,			CONTROL_INS_FMW_MODE,
	CONTROL_INS_FMW_FM2_ADSR_00,	CONTROL_INS_FMW_FM2_ADSR_01,	CONTROL_INS_FMW_FM2_ADSR_02,	CONTROL_INS_FMW_FM2_ADSR_03,
	CONTROL_INS_FMW_FM3_ADSR_00,	CONTROL_INS_FMW_FM3_ADSR_01,	CONTROL_INS_FMW_FM3_ADSR_02,	CONTROL_INS_FMW_FM3_ADSR_03,
	CONTROL_INS_FMW_FM4_ADSR_00,	CONTROL_INS_FMW_FM4_ADSR_01,	CONTROL_INS_FMW_FM4_ADSR_02,	CONTROL_INS_FMW_FM4_ADSR_03,
	CONTROL_INS_FMW_FM1_ADSR_00,	CONTROL_INS_FMW_FM1_ADSR_01,	CONTROL_INS_FMW_FM1_ADSR_02,	CONTROL_INS_FMW_FM1_ADSR_03, 
	CONTROL_INS_FMW_COUNT, 			CONTROL_INS_FMW_MAX
};
extern const Control INS_FMW_CONTROLS[CONTROL_INS_FMW_MAX];

enum E_INS_SMP_CONTROLS { 
	CONTROL_INS_SMP_INDEX,			CONTROL_INS_SMP_NAME,			CONTROL_INS_SMP_TYPE,			CONTROL_INS_SMP_START,
	CONTROL_INS_SMP_END,			CONTROL_INS_SMP_FREQUENCY,		CONTROL_INS_SMP_B,				CONTROL_INS_SMP_S,
	CONTROL_INS_SMP_R,				CONTROL_INS_SMP_PLAYMODE,		CONTROL_INS_SMP_LOOPMODE,		CONTROL_INS_SMP_KIT,
	CONTROL_INS_SMP_SMPKIT_00,		CONTROL_INS_SMP_SMPKIT_01,		CONTROL_INS_SMP_SMPKIT_02,		CONTROL_INS_SMP_SMPKIT_03,
	CONTROL_INS_SMP_SMPKIT_04,		CONTROL_INS_SMP_SMPKIT_05,		CONTROL_INS_SMP_SMPKIT_06,		CONTROL_INS_SMP_SMPKIT_07,
	CONTROL_INS_SMP_SMPKIT_08,		CONTROL_INS_SMP_SMPKIT_09,		CONTROL_INS_SMP_SMPKIT_0A,		CONTROL_INS_SMP_SMPKIT_0B,
	CONTROL_INS_SMP_ADSR_00,		CONTROL_INS_SMP_ADSR_01,		CONTROL_INS_SMP_ADSR_02,		CONTROL_INS_SMP_ADSR_03,
	CONTROL_INS_SMP_COUNT,			CONTROL_INS_SMP_MAX
};
extern const Control INS_SMP_CONTROLS[CONTROL_INS_SMP_MAX];

enum E_TABLE_CONTROLS { 
	CONTROL_TABLE_TRANSPOSE_00,		CONTROL_TABLE_TRANSPOSE_01,		CONTROL_TABLE_TRANSPOSE_02,		CONTROL_TABLE_TRANSPOSE_03,
	CONTROL_TABLE_TRANSPOSE_04,		CONTROL_TABLE_TRANSPOSE_05,		CONTROL_TABLE_TRANSPOSE_06,		CONTROL_TABLE_TRANSPOSE_07,
	CONTROL_TABLE_TRANSPOSE_08,		CONTROL_TABLE_TRANSPOSE_09,		CONTROL_TABLE_TRANSPOSE_0A,		CONTROL_TABLE_TRANSPOSE_0B,
	CONTROL_TABLE_TRANSPOSE_0C,		CONTROL_TABLE_TRANSPOSE_0D,		CONTROL_TABLE_TRANSPOSE_0E,		CONTROL_TABLE_TRANSPOSE_0F,
	CONTROL_TABLE_VOLUME_00,		CONTROL_TABLE_VOLUME_01,		CONTROL_TABLE_VOLUME_02,		CONTROL_TABLE_VOLUME_03,
	CONTROL_TABLE_VOLUME_04,		CONTROL_TABLE_VOLUME_05,		CONTROL_TABLE_VOLUME_06,		CONTROL_TABLE_VOLUME_07,
	CONTROL_TABLE_VOLUME_08,		CONTROL_TABLE_VOLUME_09,		CONTROL_TABLE_VOLUME_0A,		CONTROL_TABLE_VOLUME_0B,
	CONTROL_TABLE_VOLUME_0C,		CONTROL_TABLE_VOLUME_0D,		CONTROL_TABLE_VOLUME_0E,		CONTROL_TABLE_VOLUME_0F,
	CONTROL_TABLE_VALUE1_00,		CONTROL_TABLE_VALUE1_01,		CONTROL_TABLE_VALUE1_02,		CONTROL_TABLE_VALUE1_03,
	CONTROL_TABLE_VALUE1_04,		CONTROL_TABLE_VALUE1_05,		CONTROL_TABLE_VALUE1_06,		CONTROL_TABLE_VALUE1_07,
	CONTROL_TABLE_VALUE1_08,		CONTROL_TABLE_VALUE1_09,		CONTROL_TABLE_VALUE1_0A,		CONTROL_TABLE_VALUE1_0B,
	CONTROL_TABLE_VALUE1_0C,		CONTROL_TABLE_VALUE1_0D,		CONTROL_TABLE_VALUE1_0E,		CONTROL_TABLE_VALUE1_0F,
	CONTROL_TABLE_COMMAND1_00,		CONTROL_TABLE_COMMAND1_01,		CONTROL_TABLE_COMMAND1_02,		CONTROL_TABLE_COMMAND1_03,
	CONTROL_TABLE_COMMAND1_04,		CONTROL_TABLE_COMMAND1_05,		CONTROL_TABLE_COMMAND1_06,		CONTROL_TABLE_COMMAND1_07,
	CONTROL_TABLE_COMMAND1_08,		CONTROL_TABLE_COMMAND1_09,		CONTROL_TABLE_COMMAND1_0A,		CONTROL_TABLE_COMMAND1_0B,
	CONTROL_TABLE_COMMAND1_0C,		CONTROL_TABLE_COMMAND1_0D,		CONTROL_TABLE_COMMAND1_0E,		CONTROL_TABLE_COMMAND1_0F,
	CONTROL_TABLE_VALUE2_00,		CONTROL_TABLE_VALUE2_01,		CONTROL_TABLE_VALUE2_02,		CONTROL_TABLE_VALUE2_03,
	CONTROL_TABLE_VALUE2_04,		CONTROL_TABLE_VALUE2_05,		CONTROL_TABLE_VALUE2_06,		CONTROL_TABLE_VALUE2_07,
	CONTROL_TABLE_VALUE2_08,		CONTROL_TABLE_VALUE2_09,		CONTROL_TABLE_VALUE2_0A,		CONTROL_TABLE_VALUE2_0B,
	CONTROL_TABLE_VALUE2_0C,		CONTROL_TABLE_VALUE2_0D,		CONTROL_TABLE_VALUE2_0E,		CONTROL_TABLE_VALUE2_0F,
	CONTROL_TABLE_COMMAND2_00,		CONTROL_TABLE_COMMAND2_01,		CONTROL_TABLE_COMMAND2_02,		CONTROL_TABLE_COMMAND2_03,
	CONTROL_TABLE_COMMAND2_04,		CONTROL_TABLE_COMMAND2_05,		CONTROL_TABLE_COMMAND2_06,		CONTROL_TABLE_COMMAND2_07,
	CONTROL_TABLE_COMMAND2_08,		CONTROL_TABLE_COMMAND2_09,		CONTROL_TABLE_COMMAND2_0A,		CONTROL_TABLE_COMMAND2_0B,
	CONTROL_TABLE_COMMAND2_0C,		CONTROL_TABLE_COMMAND2_0D,		CONTROL_TABLE_COMMAND2_0E,		CONTROL_TABLE_COMMAND2_0F, 
	CONTROL_TABLE_COUNT, 			CONTROL_TABLE_MAX
};
extern const Control TABLE_CONTROLS[CONTROL_TABLE_MAX];

enum E_PAT_CONTROLS { 
	CONTROL_PAT_PATTERNS_A_00,		CONTROL_PAT_PATTERNS_A_01,		CONTROL_PAT_PATTERNS_A_02,		CONTROL_PAT_PATTERNS_A_03,
	CONTROL_PAT_PATTERNS_A_04,		CONTROL_PAT_PATTERNS_A_05,		CONTROL_PAT_PATTERNS_A_06,		CONTROL_PAT_PATTERNS_A_07,
	CONTROL_PAT_PATTERNS_A_08,		CONTROL_PAT_PATTERNS_A_09,		CONTROL_PAT_PATTERNS_A_0A,		CONTROL_PAT_PATTERNS_A_0B,
	CONTROL_PAT_PATTERNS_A_0C,		CONTROL_PAT_PATTERNS_A_0D,		CONTROL_PAT_PATTERNS_A_0E,		CONTROL_PAT_PATTERNS_A_0F,
	CONTROL_PAT_PATTERNS_B_00,		CONTROL_PAT_PATTERNS_B_01,		CONTROL_PAT_PATTERNS_B_02,		CONTROL_PAT_PATTERNS_B_03,
	CONTROL_PAT_PATTERNS_B_04,		CONTROL_PAT_PATTERNS_B_05,		CONTROL_PAT_PATTERNS_B_06,		CONTROL_PAT_PATTERNS_B_07,
	CONTROL_PAT_PATTERNS_B_08,		CONTROL_PAT_PATTERNS_B_09,		CONTROL_PAT_PATTERNS_B_0A,		CONTROL_PAT_PATTERNS_B_0B,
	CONTROL_PAT_PATTERNS_B_0C,		CONTROL_PAT_PATTERNS_B_0D,		CONTROL_PAT_PATTERNS_B_0E,		CONTROL_PAT_PATTERNS_B_0F,
	CONTROL_PAT_PATTERNS_C_00,		CONTROL_PAT_PATTERNS_C_01,		CONTROL_PAT_PATTERNS_C_02,		CONTROL_PAT_PATTERNS_C_03,
	CONTROL_PAT_PATTERNS_C_04,		CONTROL_PAT_PATTERNS_C_05,		CONTROL_PAT_PATTERNS_C_06,		CONTROL_PAT_PATTERNS_C_07,
	CONTROL_PAT_PATTERNS_C_08,		CONTROL_PAT_PATTERNS_C_09,		CONTROL_PAT_PATTERNS_C_0A,		CONTROL_PAT_PATTERNS_C_0B,
	CONTROL_PAT_PATTERNS_C_0C,		CONTROL_PAT_PATTERNS_C_0D,		CONTROL_PAT_PATTERNS_C_0E,		CONTROL_PAT_PATTERNS_C_0F,
	CONTROL_PAT_PATTERNS_D_00,		CONTROL_PAT_PATTERNS_D_01,		CONTROL_PAT_PATTERNS_D_02,		CONTROL_PAT_PATTERNS_D_03,
	CONTROL_PAT_PATTERNS_D_04,		CONTROL_PAT_PATTERNS_D_05,		CONTROL_PAT_PATTERNS_D_06,		CONTROL_PAT_PATTERNS_D_07,
	CONTROL_PAT_PATTERNS_D_08,		CONTROL_PAT_PATTERNS_D_09,		CONTROL_PAT_PATTERNS_D_0A,		CONTROL_PAT_PATTERNS_D_0B,
	CONTROL_PAT_PATTERNS_D_0C,		CONTROL_PAT_PATTERNS_D_0D,		CONTROL_PAT_PATTERNS_D_0E,		CONTROL_PAT_PATTERNS_D_0F,
	CONTROL_PAT_PATTERNS_E_00,		CONTROL_PAT_PATTERNS_E_01,		CONTROL_PAT_PATTERNS_E_02,		CONTROL_PAT_PATTERNS_E_03,
	CONTROL_PAT_PATTERNS_E_04,		CONTROL_PAT_PATTERNS_E_05,		CONTROL_PAT_PATTERNS_E_06,		CONTROL_PAT_PATTERNS_E_07,
	CONTROL_PAT_PATTERNS_E_08,		CONTROL_PAT_PATTERNS_E_09,		CONTROL_PAT_PATTERNS_E_0A,		CONTROL_PAT_PATTERNS_E_0B,
	CONTROL_PAT_PATTERNS_E_0C,		CONTROL_PAT_PATTERNS_E_0D,		CONTROL_PAT_PATTERNS_E_0E,		CONTROL_PAT_PATTERNS_E_0F,
	CONTROL_PAT_PATTERNS_F_00,		CONTROL_PAT_PATTERNS_F_01,		CONTROL_PAT_PATTERNS_F_02,		CONTROL_PAT_PATTERNS_F_03,
	CONTROL_PAT_PATTERNS_F_04,		CONTROL_PAT_PATTERNS_F_05,		CONTROL_PAT_PATTERNS_F_06,		CONTROL_PAT_PATTERNS_F_07,
	CONTROL_PAT_PATTERNS_F_08,		CONTROL_PAT_PATTERNS_F_09,		CONTROL_PAT_PATTERNS_F_0A,		CONTROL_PAT_PATTERNS_F_0B,
	CONTROL_PAT_PATTERNS_F_0C,		CONTROL_PAT_PATTERNS_F_0D,		CONTROL_PAT_PATTERNS_F_0E,		CONTROL_PAT_PATTERNS_F_0F,
	CONTROL_PAT_SOLO_LEFT_00,		CONTROL_PAT_SOLO_LEFT_01,		CONTROL_PAT_SOLO_LEFT_02,		CONTROL_PAT_MUTE_LEFT_00,
	CONTROL_PAT_MUTE_LEFT_01,		CONTROL_PAT_MUTE_LEFT_02,		CONTROL_PAT_MUTE_RIGHT_00,		CONTROL_PAT_MUTE_RIGHT_01,
	CONTROL_PAT_MUTE_RIGHT_02,		CONTROL_PAT_SOLO_RIGHT_00,		CONTROL_PAT_SOLO_RIGHT_01,		CONTROL_PAT_SOLO_RIGHT_02,
	CONTROL_PAT_COUNT,				CONTROL_PAT_MAX
};
extern const Control PAT_CONTROLS[CONTROL_PAT_MAX];

enum E_CHANNEL0_0_CONTROLS { 
	CONTROL_CHANNEL0_0_CHAN0KEY_00,	CONTROL_CHANNEL0_0_CHAN0KEY_01,	CONTROL_CHANNEL0_0_CHAN0KEY_02,	CONTROL_CHANNEL0_0_CHAN0KEY_03,
	CONTROL_CHANNEL0_0_CHAN0KEY_04,	CONTROL_CHANNEL0_0_CHAN0KEY_05,	CONTROL_CHANNEL0_0_CHAN0KEY_06,	CONTROL_CHANNEL0_0_CHAN0KEY_07,
	CONTROL_CHANNEL0_0_CHAN0KEY_08,	CONTROL_CHANNEL0_0_CHAN0KEY_09,	CONTROL_CHANNEL0_0_CHAN0KEY_0A,	CONTROL_CHANNEL0_0_CHAN0KEY_0B,
	CONTROL_CHANNEL0_0_CHAN0KEY_0C,	CONTROL_CHANNEL0_0_CHAN0KEY_0D,	CONTROL_CHANNEL0_0_CHAN0KEY_0E,	CONTROL_CHANNEL0_0_CHAN0KEY_0F,
	CONTROL_CHANNEL0_0_CHAN0INS_00,	CONTROL_CHANNEL0_0_CHAN0INS_01,	CONTROL_CHANNEL0_0_CHAN0INS_02,	CONTROL_CHANNEL0_0_CHAN0INS_03,
	CONTROL_CHANNEL0_0_CHAN0INS_04,	CONTROL_CHANNEL0_0_CHAN0INS_05,	CONTROL_CHANNEL0_0_CHAN0INS_06,	CONTROL_CHANNEL0_0_CHAN0INS_07,
	CONTROL_CHANNEL0_0_CHAN0INS_08,	CONTROL_CHANNEL0_0_CHAN0INS_09,	CONTROL_CHANNEL0_0_CHAN0INS_0A,	CONTROL_CHANNEL0_0_CHAN0INS_0B,
	CONTROL_CHANNEL0_0_CHAN0INS_0C,	CONTROL_CHANNEL0_0_CHAN0INS_0D,	CONTROL_CHANNEL0_0_CHAN0INS_0E,	CONTROL_CHANNEL0_0_CHAN0INS_0F,
	CONTROL_CHANNEL0_0_CHAN0VOL_00,	CONTROL_CHANNEL0_0_CHAN0VOL_01,	CONTROL_CHANNEL0_0_CHAN0VOL_02,	CONTROL_CHANNEL0_0_CHAN0VOL_03,
	CONTROL_CHANNEL0_0_CHAN0VOL_04,	CONTROL_CHANNEL0_0_CHAN0VOL_05,	CONTROL_CHANNEL0_0_CHAN0VOL_06,	CONTROL_CHANNEL0_0_CHAN0VOL_07,
	CONTROL_CHANNEL0_0_CHAN0VOL_08,	CONTROL_CHANNEL0_0_CHAN0VOL_09,	CONTROL_CHANNEL0_0_CHAN0VOL_0A,	CONTROL_CHANNEL0_0_CHAN0VOL_0B,
	CONTROL_CHANNEL0_0_CHAN0VOL_0C,	CONTROL_CHANNEL0_0_CHAN0VOL_0D,	CONTROL_CHANNEL0_0_CHAN0VOL_0E,	CONTROL_CHANNEL0_0_CHAN0VOL_0F,
	CONTROL_CHANNEL0_0_CHAN0CMD_00,	CONTROL_CHANNEL0_0_CHAN0CMD_01,	CONTROL_CHANNEL0_0_CHAN0CMD_02,	CONTROL_CHANNEL0_0_CHAN0CMD_03,
	CONTROL_CHANNEL0_0_CHAN0CMD_04,	CONTROL_CHANNEL0_0_CHAN0CMD_05,	CONTROL_CHANNEL0_0_CHAN0CMD_06,	CONTROL_CHANNEL0_0_CHAN0CMD_07,
	CONTROL_CHANNEL0_0_CHAN0CMD_08,	CONTROL_CHANNEL0_0_CHAN0CMD_09,	CONTROL_CHANNEL0_0_CHAN0CMD_0A,	CONTROL_CHANNEL0_0_CHAN0CMD_0B,
	CONTROL_CHANNEL0_0_CHAN0CMD_0C,	CONTROL_CHANNEL0_0_CHAN0CMD_0D,	CONTROL_CHANNEL0_0_CHAN0CMD_0E,	CONTROL_CHANNEL0_0_CHAN0CMD_0F,
	CONTROL_CHANNEL0_0_CHAN0VAL_00,	CONTROL_CHANNEL0_0_CHAN0VAL_01,	CONTROL_CHANNEL0_0_CHAN0VAL_02,	CONTROL_CHANNEL0_0_CHAN0VAL_03,
	CONTROL_CHANNEL0_0_CHAN0VAL_04,	CONTROL_CHANNEL0_0_CHAN0VAL_05,	CONTROL_CHANNEL0_0_CHAN0VAL_06,	CONTROL_CHANNEL0_0_CHAN0VAL_07,
	CONTROL_CHANNEL0_0_CHAN0VAL_08,	CONTROL_CHANNEL0_0_CHAN0VAL_09,	CONTROL_CHANNEL0_0_CHAN0VAL_0A,	CONTROL_CHANNEL0_0_CHAN0VAL_0B,
	CONTROL_CHANNEL0_0_CHAN0VAL_0C,	CONTROL_CHANNEL0_0_CHAN0VAL_0D,	CONTROL_CHANNEL0_0_CHAN0VAL_0E,	CONTROL_CHANNEL0_0_CHAN0VAL_0F, 
	CONTROL_CHANNEL0_0_COUNT, 
	CONTROL_CHANNEL0_0_MAX = CONTROL_CHANNEL0_0_COUNT+(16*5)+1
};
extern const Control CHAN0_CONTROLS[CONTROL_CHANNEL0_0_MAX]; 

enum E_CHANNEL1_0_CONTROLS { 
	CONTROL_CHANNEL1_0_CHAN1KEY_00,	CONTROL_CHANNEL1_0_CHAN1KEY_01,	CONTROL_CHANNEL1_0_CHAN1KEY_02,	CONTROL_CHANNEL1_0_CHAN1KEY_03,
	CONTROL_CHANNEL1_0_CHAN1KEY_04,	CONTROL_CHANNEL1_0_CHAN1KEY_05,	CONTROL_CHANNEL1_0_CHAN1KEY_06,	CONTROL_CHANNEL1_0_CHAN1KEY_07,
	CONTROL_CHANNEL1_0_CHAN1KEY_08,	CONTROL_CHANNEL1_0_CHAN1KEY_09,	CONTROL_CHANNEL1_0_CHAN1KEY_0A,	CONTROL_CHANNEL1_0_CHAN1KEY_0B,
	CONTROL_CHANNEL1_0_CHAN1KEY_0C,	CONTROL_CHANNEL1_0_CHAN1KEY_0D,	CONTROL_CHANNEL1_0_CHAN1KEY_0E,	CONTROL_CHANNEL1_0_CHAN1KEY_0F,
	CONTROL_CHANNEL1_0_CHAN1INS_00,	CONTROL_CHANNEL1_0_CHAN1INS_01,	CONTROL_CHANNEL1_0_CHAN1INS_02,	CONTROL_CHANNEL1_0_CHAN1INS_03,
	CONTROL_CHANNEL1_0_CHAN1INS_04,	CONTROL_CHANNEL1_0_CHAN1INS_05,	CONTROL_CHANNEL1_0_CHAN1INS_06,	CONTROL_CHANNEL1_0_CHAN1INS_07,
	CONTROL_CHANNEL1_0_CHAN1INS_08,	CONTROL_CHANNEL1_0_CHAN1INS_09,	CONTROL_CHANNEL1_0_CHAN1INS_0A,	CONTROL_CHANNEL1_0_CHAN1INS_0B,
	CONTROL_CHANNEL1_0_CHAN1INS_0C,	CONTROL_CHANNEL1_0_CHAN1INS_0D,	CONTROL_CHANNEL1_0_CHAN1INS_0E,	CONTROL_CHANNEL1_0_CHAN1INS_0F,
	CONTROL_CHANNEL1_0_CHAN1VOL_00,	CONTROL_CHANNEL1_0_CHAN1VOL_01,	CONTROL_CHANNEL1_0_CHAN1VOL_02,	CONTROL_CHANNEL1_0_CHAN1VOL_03,
	CONTROL_CHANNEL1_0_CHAN1VOL_04,	CONTROL_CHANNEL1_0_CHAN1VOL_05,	CONTROL_CHANNEL1_0_CHAN1VOL_06,	CONTROL_CHANNEL1_0_CHAN1VOL_07,
	CONTROL_CHANNEL1_0_CHAN1VOL_08,	CONTROL_CHANNEL1_0_CHAN1VOL_09,	CONTROL_CHANNEL1_0_CHAN1VOL_0A,	CONTROL_CHANNEL1_0_CHAN1VOL_0B,
	CONTROL_CHANNEL1_0_CHAN1VOL_0C,	CONTROL_CHANNEL1_0_CHAN1VOL_0D,	CONTROL_CHANNEL1_0_CHAN1VOL_0E,	CONTROL_CHANNEL1_0_CHAN1VOL_0F, 
	CONTROL_CHANNEL1_0_CHAN1CMD_00,	CONTROL_CHANNEL1_0_CHAN1CMD_01,	CONTROL_CHANNEL1_0_CHAN1CMD_02,	CONTROL_CHANNEL1_0_CHAN1CMD_03,
	CONTROL_CHANNEL1_0_CHAN1CMD_04,	CONTROL_CHANNEL1_0_CHAN1CMD_05,	CONTROL_CHANNEL1_0_CHAN1CMD_06,	CONTROL_CHANNEL1_0_CHAN1CMD_07,
	CONTROL_CHANNEL1_0_CHAN1CMD_08,	CONTROL_CHANNEL1_0_CHAN1CMD_09,	CONTROL_CHANNEL1_0_CHAN1CMD_0A,	CONTROL_CHANNEL1_0_CHAN1CMD_0B,
	CONTROL_CHANNEL1_0_CHAN1CMD_0C,	CONTROL_CHANNEL1_0_CHAN1CMD_0D,	CONTROL_CHANNEL1_0_CHAN1CMD_0E,	CONTROL_CHANNEL1_0_CHAN1CMD_0F,
	CONTROL_CHANNEL1_0_CHAN1VAL_00,	CONTROL_CHANNEL1_0_CHAN1VAL_01,	CONTROL_CHANNEL1_0_CHAN1VAL_02,	CONTROL_CHANNEL1_0_CHAN1VAL_03,
	CONTROL_CHANNEL1_0_CHAN1VAL_04,	CONTROL_CHANNEL1_0_CHAN1VAL_05,	CONTROL_CHANNEL1_0_CHAN1VAL_06,	CONTROL_CHANNEL1_0_CHAN1VAL_07,
	CONTROL_CHANNEL1_0_CHAN1VAL_08,	CONTROL_CHANNEL1_0_CHAN1VAL_09,	CONTROL_CHANNEL1_0_CHAN1VAL_0A,	CONTROL_CHANNEL1_0_CHAN1VAL_0B,
	CONTROL_CHANNEL1_0_CHAN1VAL_0C,	CONTROL_CHANNEL1_0_CHAN1VAL_0D,	CONTROL_CHANNEL1_0_CHAN1VAL_0E,	CONTROL_CHANNEL1_0_CHAN1VAL_0F,
	CONTROL_CHANNEL1_0_COUNT, 
	CONTROL_CHANNEL1_0_MAX = CONTROL_CHANNEL1_0_COUNT+(16*5)+1
};
extern const Control CHAN1_CONTROLS[CONTROL_CHANNEL1_0_MAX]; 

enum E_CHANNEL2_0_CONTROLS { 
	//abort							//back							//c_str							//data
	CONTROL_CHANNEL2_0_CHAN2KEY_00,	CONTROL_CHANNEL2_0_CHAN2KEY_01,	CONTROL_CHANNEL2_0_CHAN2KEY_02,	CONTROL_CHANNEL2_0_CHAN2KEY_03,
	CONTROL_CHANNEL2_0_CHAN2KEY_04,	CONTROL_CHANNEL2_0_CHAN2KEY_05,	CONTROL_CHANNEL2_0_CHAN2KEY_06,	CONTROL_CHANNEL2_0_CHAN2KEY_07,
	CONTROL_CHANNEL2_0_CHAN2KEY_08,	CONTROL_CHANNEL2_0_CHAN2KEY_09,	CONTROL_CHANNEL2_0_CHAN2KEY_0A,	CONTROL_CHANNEL2_0_CHAN2KEY_0B,
	CONTROL_CHANNEL2_0_CHAN2KEY_0C,	CONTROL_CHANNEL2_0_CHAN2KEY_0D,	CONTROL_CHANNEL2_0_CHAN2KEY_0E,	CONTROL_CHANNEL2_0_CHAN2KEY_0F,
	CONTROL_CHANNEL2_0_CHAN2INS_00,	CONTROL_CHANNEL2_0_CHAN2INS_01,	CONTROL_CHANNEL2_0_CHAN2INS_02,	CONTROL_CHANNEL2_0_CHAN2INS_03,
	CONTROL_CHANNEL2_0_CHAN2INS_04,	CONTROL_CHANNEL2_0_CHAN2INS_05,	CONTROL_CHANNEL2_0_CHAN2INS_06,	CONTROL_CHANNEL2_0_CHAN2INS_07,
	CONTROL_CHANNEL2_0_CHAN2INS_08,	CONTROL_CHANNEL2_0_CHAN2INS_09,	CONTROL_CHANNEL2_0_CHAN2INS_0A,	CONTROL_CHANNEL2_0_CHAN2INS_0B,
	CONTROL_CHANNEL2_0_CHAN2INS_0C,	CONTROL_CHANNEL2_0_CHAN2INS_0D,	CONTROL_CHANNEL2_0_CHAN2INS_0E,	CONTROL_CHANNEL2_0_CHAN2INS_0F,
	CONTROL_CHANNEL2_0_CHAN2VOL_00,	CONTROL_CHANNEL2_0_CHAN2VOL_01,	CONTROL_CHANNEL2_0_CHAN2VOL_02,	CONTROL_CHANNEL2_0_CHAN2VOL_03,
	CONTROL_CHANNEL2_0_CHAN2VOL_04,	CONTROL_CHANNEL2_0_CHAN2VOL_05,	CONTROL_CHANNEL2_0_CHAN2VOL_06,	CONTROL_CHANNEL2_0_CHAN2VOL_07,
	CONTROL_CHANNEL2_0_CHAN2VOL_08,	CONTROL_CHANNEL2_0_CHAN2VOL_09,	CONTROL_CHANNEL2_0_CHAN2VOL_0A,	CONTROL_CHANNEL2_0_CHAN2VOL_0B,
	CONTROL_CHANNEL2_0_CHAN2VOL_0C,	CONTROL_CHANNEL2_0_CHAN2VOL_0D,	CONTROL_CHANNEL2_0_CHAN2VOL_0E,	CONTROL_CHANNEL2_0_CHAN2VOL_0F,
	CONTROL_CHANNEL2_0_CHAN2CMD_00,	CONTROL_CHANNEL2_0_CHAN2CMD_01,	CONTROL_CHANNEL2_0_CHAN2CMD_02,	CONTROL_CHANNEL2_0_CHAN2CMD_03,
	CONTROL_CHANNEL2_0_CHAN2CMD_04,	CONTROL_CHANNEL2_0_CHAN2CMD_05,	CONTROL_CHANNEL2_0_CHAN2CMD_06,	CONTROL_CHANNEL2_0_CHAN2CMD_07,
	CONTROL_CHANNEL2_0_CHAN2CMD_08,	CONTROL_CHANNEL2_0_CHAN2CMD_09,	CONTROL_CHANNEL2_0_CHAN2CMD_0A,	CONTROL_CHANNEL2_0_CHAN2CMD_0B,
	CONTROL_CHANNEL2_0_CHAN2CMD_0C,	CONTROL_CHANNEL2_0_CHAN2CMD_0D,	CONTROL_CHANNEL2_0_CHAN2CMD_0E,	CONTROL_CHANNEL2_0_CHAN2CMD_0F,
	CONTROL_CHANNEL2_0_CHAN2VAL_00,	CONTROL_CHANNEL2_0_CHAN2VAL_01,	CONTROL_CHANNEL2_0_CHAN2VAL_02,	CONTROL_CHANNEL2_0_CHAN2VAL_03,
	CONTROL_CHANNEL2_0_CHAN2VAL_04,	CONTROL_CHANNEL2_0_CHAN2VAL_05,	CONTROL_CHANNEL2_0_CHAN2VAL_06,	CONTROL_CHANNEL2_0_CHAN2VAL_07,
	CONTROL_CHANNEL2_0_CHAN2VAL_08,	CONTROL_CHANNEL2_0_CHAN2VAL_09,	CONTROL_CHANNEL2_0_CHAN2VAL_0A,	CONTROL_CHANNEL2_0_CHAN2VAL_0B,
	CONTROL_CHANNEL2_0_CHAN2VAL_0C,	CONTROL_CHANNEL2_0_CHAN2VAL_0D,	CONTROL_CHANNEL2_0_CHAN2VAL_0E,	CONTROL_CHANNEL2_0_CHAN2VAL_0F,
	CONTROL_CHANNEL2_0_COUNT , 
	CONTROL_CHANNEL2_0_MAX = CONTROL_CHANNEL2_0_COUNT+(16*5)+1
};
extern const Control CHAN2_CONTROLS[CONTROL_CHANNEL2_0_MAX]; 

enum E_CHANNEL3_0_CONTROLS { 
	CONTROL_CHANNEL3_0_CHAN3KEY_00,	CONTROL_CHANNEL3_0_CHAN3KEY_01,	CONTROL_CHANNEL3_0_CHAN3KEY_02,	CONTROL_CHANNEL3_0_CHAN3KEY_03,
	CONTROL_CHANNEL3_0_CHAN3KEY_04,	CONTROL_CHANNEL3_0_CHAN3KEY_05,	CONTROL_CHANNEL3_0_CHAN3KEY_06,	CONTROL_CHANNEL3_0_CHAN3KEY_07,
	CONTROL_CHANNEL3_0_CHAN3KEY_08,	CONTROL_CHANNEL3_0_CHAN3KEY_09,	CONTROL_CHANNEL3_0_CHAN3KEY_0A,	CONTROL_CHANNEL3_0_CHAN3KEY_0B,
	CONTROL_CHANNEL3_0_CHAN3KEY_0C,	CONTROL_CHANNEL3_0_CHAN3KEY_0D,	CONTROL_CHANNEL3_0_CHAN3KEY_0E,	CONTROL_CHANNEL3_0_CHAN3KEY_0F, 
	CONTROL_CHANNEL3_0_CHAN3INS_00,	CONTROL_CHANNEL3_0_CHAN3INS_01,	CONTROL_CHANNEL3_0_CHAN3INS_02,	CONTROL_CHANNEL3_0_CHAN3INS_03,
	CONTROL_CHANNEL3_0_CHAN3INS_04,	CONTROL_CHANNEL3_0_CHAN3INS_05,	CONTROL_CHANNEL3_0_CHAN3INS_06,	CONTROL_CHANNEL3_0_CHAN3INS_07,
	CONTROL_CHANNEL3_0_CHAN3INS_08,	CONTROL_CHANNEL3_0_CHAN3INS_09,	CONTROL_CHANNEL3_0_CHAN3INS_0A,	CONTROL_CHANNEL3_0_CHAN3INS_0B,
	CONTROL_CHANNEL3_0_CHAN3INS_0C,	CONTROL_CHANNEL3_0_CHAN3INS_0D,	CONTROL_CHANNEL3_0_CHAN3INS_0E,	CONTROL_CHANNEL3_0_CHAN3INS_0F,
	CONTROL_CHANNEL3_0_CHAN3VOL_00,	CONTROL_CHANNEL3_0_CHAN3VOL_01,	CONTROL_CHANNEL3_0_CHAN3VOL_02,	CONTROL_CHANNEL3_0_CHAN3VOL_03,
	CONTROL_CHANNEL3_0_CHAN3VOL_04,	CONTROL_CHANNEL3_0_CHAN3VOL_05,	CONTROL_CHANNEL3_0_CHAN3VOL_06,	CONTROL_CHANNEL3_0_CHAN3VOL_07,
	CONTROL_CHANNEL3_0_CHAN3VOL_08,	CONTROL_CHANNEL3_0_CHAN3VOL_09,	CONTROL_CHANNEL3_0_CHAN3VOL_0A,	CONTROL_CHANNEL3_0_CHAN3VOL_0B,
	CONTROL_CHANNEL3_0_CHAN3VOL_0C,	CONTROL_CHANNEL3_0_CHAN3VOL_0D,	CONTROL_CHANNEL3_0_CHAN3VOL_0E,	CONTROL_CHANNEL3_0_CHAN3VOL_0F,
	CONTROL_CHANNEL3_0_CHAN3CMD_00,	CONTROL_CHANNEL3_0_CHAN3CMD_01,	CONTROL_CHANNEL3_0_CHAN3CMD_02,	CONTROL_CHANNEL3_0_CHAN3CMD_03,
	CONTROL_CHANNEL3_0_CHAN3CMD_04,	CONTROL_CHANNEL3_0_CHAN3CMD_05,	CONTROL_CHANNEL3_0_CHAN3CMD_06,	CONTROL_CHANNEL3_0_CHAN3CMD_07,
	CONTROL_CHANNEL3_0_CHAN3CMD_08,	CONTROL_CHANNEL3_0_CHAN3CMD_09,	CONTROL_CHANNEL3_0_CHAN3CMD_0A,	CONTROL_CHANNEL3_0_CHAN3CMD_0B,
	CONTROL_CHANNEL3_0_CHAN3CMD_0C,	CONTROL_CHANNEL3_0_CHAN3CMD_0D,	CONTROL_CHANNEL3_0_CHAN3CMD_0E,	CONTROL_CHANNEL3_0_CHAN3CMD_0F,
	CONTROL_CHANNEL3_0_CHAN3VAL_00,	CONTROL_CHANNEL3_0_CHAN3VAL_01,	CONTROL_CHANNEL3_0_CHAN3VAL_02,	CONTROL_CHANNEL3_0_CHAN3VAL_03,
	CONTROL_CHANNEL3_0_CHAN3VAL_04,	CONTROL_CHANNEL3_0_CHAN3VAL_05,	CONTROL_CHANNEL3_0_CHAN3VAL_06,	CONTROL_CHANNEL3_0_CHAN3VAL_07,
	CONTROL_CHANNEL3_0_CHAN3VAL_08,	CONTROL_CHANNEL3_0_CHAN3VAL_09,	CONTROL_CHANNEL3_0_CHAN3VAL_0A,	CONTROL_CHANNEL3_0_CHAN3VAL_0B,
	CONTROL_CHANNEL3_0_CHAN3VAL_0C,	CONTROL_CHANNEL3_0_CHAN3VAL_0D,	CONTROL_CHANNEL3_0_CHAN3VAL_0E,	CONTROL_CHANNEL3_0_CHAN3VAL_0F,
	CONTROL_CHANNEL3_0_COUNT, 
	CONTROL_CHANNEL3_0_MAX = CONTROL_CHANNEL3_0_COUNT+(16*5)+1
};	
extern const Control CHAN3_CONTROLS[CONTROL_CHANNEL3_0_MAX]; 

enum E_CHANNEL4_0_CONTROLS { 
	CONTROL_CHANNEL4_0_CHAN4KEY_00,	CONTROL_CHANNEL4_0_CHAN4KEY_01,	CONTROL_CHANNEL4_0_CHAN4KEY_02,	CONTROL_CHANNEL4_0_CHAN4KEY_03,
	CONTROL_CHANNEL4_0_CHAN4KEY_04,	CONTROL_CHANNEL4_0_CHAN4KEY_05,	CONTROL_CHANNEL4_0_CHAN4KEY_06,	CONTROL_CHANNEL4_0_CHAN4KEY_07,
	CONTROL_CHANNEL4_0_CHAN4KEY_08,	CONTROL_CHANNEL4_0_CHAN4KEY_09,	CONTROL_CHANNEL4_0_CHAN4KEY_0A,	CONTROL_CHANNEL4_0_CHAN4KEY_0B,
	CONTROL_CHANNEL4_0_CHAN4KEY_0C,	CONTROL_CHANNEL4_0_CHAN4KEY_0D,	CONTROL_CHANNEL4_0_CHAN4KEY_0E,	CONTROL_CHANNEL4_0_CHAN4KEY_0F,
	CONTROL_CHANNEL4_0_CHAN4INS_00,	CONTROL_CHANNEL4_0_CHAN4INS_01,	CONTROL_CHANNEL4_0_CHAN4INS_02,	CONTROL_CHANNEL4_0_CHAN4INS_03,
	CONTROL_CHANNEL4_0_CHAN4INS_04,	CONTROL_CHANNEL4_0_CHAN4INS_05,	CONTROL_CHANNEL4_0_CHAN4INS_06,	CONTROL_CHANNEL4_0_CHAN4INS_07,
	CONTROL_CHANNEL4_0_CHAN4INS_08,	CONTROL_CHANNEL4_0_CHAN4INS_09,	CONTROL_CHANNEL4_0_CHAN4INS_0A,	CONTROL_CHANNEL4_0_CHAN4INS_0B,
	CONTROL_CHANNEL4_0_CHAN4INS_0C,	CONTROL_CHANNEL4_0_CHAN4INS_0D,	CONTROL_CHANNEL4_0_CHAN4INS_0E,	CONTROL_CHANNEL4_0_CHAN4INS_0F,
	CONTROL_CHANNEL4_0_CHAN4VOL_00,	CONTROL_CHANNEL4_0_CHAN4VOL_01,	CONTROL_CHANNEL4_0_CHAN4VOL_02,	CONTROL_CHANNEL4_0_CHAN4VOL_03,
	CONTROL_CHANNEL4_0_CHAN4VOL_04,	CONTROL_CHANNEL4_0_CHAN4VOL_05,	CONTROL_CHANNEL4_0_CHAN4VOL_06,	CONTROL_CHANNEL4_0_CHAN4VOL_07,
	CONTROL_CHANNEL4_0_CHAN4VOL_08,	CONTROL_CHANNEL4_0_CHAN4VOL_09,	CONTROL_CHANNEL4_0_CHAN4VOL_0A,	CONTROL_CHANNEL4_0_CHAN4VOL_0B,
	CONTROL_CHANNEL4_0_CHAN4VOL_0C,	CONTROL_CHANNEL4_0_CHAN4VOL_0D,	CONTROL_CHANNEL4_0_CHAN4VOL_0E,	CONTROL_CHANNEL4_0_CHAN4VOL_0F, 
	CONTROL_CHANNEL4_0_CHAN4CMD_00,	CONTROL_CHANNEL4_0_CHAN4CMD_01,	CONTROL_CHANNEL4_0_CHAN4CMD_02,	CONTROL_CHANNEL4_0_CHAN4CMD_03,
	CONTROL_CHANNEL4_0_CHAN4CMD_04,	CONTROL_CHANNEL4_0_CHAN4CMD_05,	CONTROL_CHANNEL4_0_CHAN4CMD_06,	CONTROL_CHANNEL4_0_CHAN4CMD_07,
	CONTROL_CHANNEL4_0_CHAN4CMD_08,	CONTROL_CHANNEL4_0_CHAN4CMD_09,	CONTROL_CHANNEL4_0_CHAN4CMD_0A,	CONTROL_CHANNEL4_0_CHAN4CMD_0B,
	CONTROL_CHANNEL4_0_CHAN4CMD_0C,	CONTROL_CHANNEL4_0_CHAN4CMD_0D,	CONTROL_CHANNEL4_0_CHAN4CMD_0E,	CONTROL_CHANNEL4_0_CHAN4CMD_0F,
	CONTROL_CHANNEL4_0_CHAN4VAL_00,	CONTROL_CHANNEL4_0_CHAN4VAL_01,	CONTROL_CHANNEL4_0_CHAN4VAL_02,	CONTROL_CHANNEL4_0_CHAN4VAL_03,
	CONTROL_CHANNEL4_0_CHAN4VAL_04,	CONTROL_CHANNEL4_0_CHAN4VAL_05,	CONTROL_CHANNEL4_0_CHAN4VAL_06,	CONTROL_CHANNEL4_0_CHAN4VAL_07,
	CONTROL_CHANNEL4_0_CHAN4VAL_08,	CONTROL_CHANNEL4_0_CHAN4VAL_09,	CONTROL_CHANNEL4_0_CHAN4VAL_0A,	CONTROL_CHANNEL4_0_CHAN4VAL_0B,
	CONTROL_CHANNEL4_0_CHAN4VAL_0C,	CONTROL_CHANNEL4_0_CHAN4VAL_0D,	CONTROL_CHANNEL4_0_CHAN4VAL_0E,	CONTROL_CHANNEL4_0_CHAN4VAL_0F,
	CONTROL_CHANNEL4_0_COUNT, 
	CONTROL_CHANNEL4_0_MAX = CONTROL_CHANNEL4_0_COUNT+(16*5)+1
};
extern const Control CHAN4_CONTROLS[CONTROL_CHANNEL4_0_MAX]; 

enum E_CHANNEL5_0_CONTROLS { 
	CONTROL_CHANNEL5_0_CHAN5KEY_00,	CONTROL_CHANNEL5_0_CHAN5KEY_01,	CONTROL_CHANNEL5_0_CHAN5KEY_02,	CONTROL_CHANNEL5_0_CHAN5KEY_03,
	CONTROL_CHANNEL5_0_CHAN5KEY_04,	CONTROL_CHANNEL5_0_CHAN5KEY_05,	CONTROL_CHANNEL5_0_CHAN5KEY_06,	CONTROL_CHANNEL5_0_CHAN5KEY_07,
	CONTROL_CHANNEL5_0_CHAN5KEY_08,	CONTROL_CHANNEL5_0_CHAN5KEY_09,	CONTROL_CHANNEL5_0_CHAN5KEY_0A,	CONTROL_CHANNEL5_0_CHAN5KEY_0B,
	CONTROL_CHANNEL5_0_CHAN5KEY_0C,	CONTROL_CHANNEL5_0_CHAN5KEY_0D,	CONTROL_CHANNEL5_0_CHAN5KEY_0E,	CONTROL_CHANNEL5_0_CHAN5KEY_0F,
	CONTROL_CHANNEL5_0_CHAN5INS_00,	CONTROL_CHANNEL5_0_CHAN5INS_01,	CONTROL_CHANNEL5_0_CHAN5INS_02,	CONTROL_CHANNEL5_0_CHAN5INS_03,
	CONTROL_CHANNEL5_0_CHAN5INS_04,	CONTROL_CHANNEL5_0_CHAN5INS_05,	CONTROL_CHANNEL5_0_CHAN5INS_06,	CONTROL_CHANNEL5_0_CHAN5INS_07,
	CONTROL_CHANNEL5_0_CHAN5INS_08,	CONTROL_CHANNEL5_0_CHAN5INS_09,	CONTROL_CHANNEL5_0_CHAN5INS_0A,	CONTROL_CHANNEL5_0_CHAN5INS_0B,
	CONTROL_CHANNEL5_0_CHAN5INS_0C,	CONTROL_CHANNEL5_0_CHAN5INS_0D,	CONTROL_CHANNEL5_0_CHAN5INS_0E,	CONTROL_CHANNEL5_0_CHAN5INS_0F,
	CONTROL_CHANNEL5_0_CHAN5VOL_00,	CONTROL_CHANNEL5_0_CHAN5VOL_01,	CONTROL_CHANNEL5_0_CHAN5VOL_02,	CONTROL_CHANNEL5_0_CHAN5VOL_03,
	CONTROL_CHANNEL5_0_CHAN5VOL_04,	CONTROL_CHANNEL5_0_CHAN5VOL_05,	CONTROL_CHANNEL5_0_CHAN5VOL_06,	CONTROL_CHANNEL5_0_CHAN5VOL_07,
	CONTROL_CHANNEL5_0_CHAN5VOL_08,	CONTROL_CHANNEL5_0_CHAN5VOL_09,	CONTROL_CHANNEL5_0_CHAN5VOL_0A,	CONTROL_CHANNEL5_0_CHAN5VOL_0B,
	CONTROL_CHANNEL5_0_CHAN5VOL_0C,	CONTROL_CHANNEL5_0_CHAN5VOL_0D,	CONTROL_CHANNEL5_0_CHAN5VOL_0E,	CONTROL_CHANNEL5_0_CHAN5VOL_0F,
	CONTROL_CHANNEL5_0_CHAN5CMD_00,	CONTROL_CHANNEL5_0_CHAN5CMD_01,	CONTROL_CHANNEL5_0_CHAN5CMD_02,	CONTROL_CHANNEL5_0_CHAN5CMD_03,
	CONTROL_CHANNEL5_0_CHAN5CMD_04,	CONTROL_CHANNEL5_0_CHAN5CMD_05,	CONTROL_CHANNEL5_0_CHAN5CMD_06,	CONTROL_CHANNEL5_0_CHAN5CMD_07,
	CONTROL_CHANNEL5_0_CHAN5CMD_08,	CONTROL_CHANNEL5_0_CHAN5CMD_09,	CONTROL_CHANNEL5_0_CHAN5CMD_0A,	CONTROL_CHANNEL5_0_CHAN5CMD_0B,
	CONTROL_CHANNEL5_0_CHAN5CMD_0C,	CONTROL_CHANNEL5_0_CHAN5CMD_0D,	CONTROL_CHANNEL5_0_CHAN5CMD_0E,	CONTROL_CHANNEL5_0_CHAN5CMD_0F,
	CONTROL_CHANNEL5_0_CHAN5VAL_00,	CONTROL_CHANNEL5_0_CHAN5VAL_01,	CONTROL_CHANNEL5_0_CHAN5VAL_02,	CONTROL_CHANNEL5_0_CHAN5VAL_03,
	CONTROL_CHANNEL5_0_CHAN5VAL_04,	CONTROL_CHANNEL5_0_CHAN5VAL_05,	CONTROL_CHANNEL5_0_CHAN5VAL_06,	CONTROL_CHANNEL5_0_CHAN5VAL_07,
	CONTROL_CHANNEL5_0_CHAN5VAL_08,	CONTROL_CHANNEL5_0_CHAN5VAL_09,	CONTROL_CHANNEL5_0_CHAN5VAL_0A,	CONTROL_CHANNEL5_0_CHAN5VAL_0B,
	CONTROL_CHANNEL5_0_CHAN5VAL_0C,	CONTROL_CHANNEL5_0_CHAN5VAL_0D,	CONTROL_CHANNEL5_0_CHAN5VAL_0E,	CONTROL_CHANNEL5_0_CHAN5VAL_0F,	
	CONTROL_CHANNEL5_0_COUNT, 
	CONTROL_CHANNEL5_0_MAX = CONTROL_CHANNEL5_0_COUNT+(16*5)+1
};
extern const Control CHAN5_CONTROLS[CONTROL_CHANNEL5_0_MAX]; 

#endif
