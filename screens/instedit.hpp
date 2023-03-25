#ifndef INSTEDIT_HPP
#define INSTEDIT_HPP

#include "../agb.h"
#include "../data/enum.h"
#include "../data/control.hpp"
#include "../data/settings.hpp"
#include "../data/instrument.hpp"
#include "../data/display.hpp"
#include "../data/control.hpp"
#include "../kernel/spu/adsr.hpp"
#include "../data/sprite.hpp"

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


#define SETTING_WAV_PHASE		0
#define SETTING_WAV_OP1_TYPE	1
#define SETTING_WAV_OP2_TYPE	1
#define SETTING_WAV_OP3_TYPE	2
#define SETTING_WAV_OP4_TYPE	2
#define SETTING_WAV_OP1_ADSR_1	3
#define SETTING_WAV_OP2_ADSR_1	3
#define SETTING_WAV_OP3_ADSR_1	4
#define SETTING_WAV_OP4_ADSR_1	4
#define SETTING_WAV_WAVEDATA0	11
#define SETTING_WAV_WAVEDATA2	12
#define SETTING_WAV_WAVEDATA4	13
#define SETTING_WAV_WAVEDATA6	14
#define SETTING_WAV_WAVEDATA8	15
#define SETTING_WAV_WAVEDATA10	16
#define SETTING_WAV_WAVEDATA12	17
#define SETTING_WAV_WAVEDATA14	18
#define SETTING_WAV_WAVEDATA16	19
#define SETTING_WAV_OP1_GATE	20
#define SETTING_WAV_OP2_GATE	20
#define SETTING_WAV_OP3_GATE	21
#define SETTING_WAV_OP4_GATE	21

enum E_INS_WAV_CONTROLS { 
	CONTROL_INS_WAV_INDEX		 , CONTROL_INS_WAV_NAME			, CONTROL_INS_WAV_TYPE		 	, 
	CONTROL_INS_WAV_PHASE		 , CONTROL_INS_WAV_AM			, CONTROL_INS_WAV_DISTORTION	,
	CONTROL_INS_WAV_OP1_TYPE	 , CONTROL_INS_WAV_OP1_GATE	 	, CONTROL_INS_WAV_OP1_ADSR_00	, CONTROL_INS_WAV_OP1_ADSR_01  , CONTROL_INS_WAV_OP1_ADSR_02  , CONTROL_INS_WAV_OP1_ADSR_03	  , 
	CONTROL_INS_WAV_OP2_TYPE	 , CONTROL_INS_WAV_OP2_GATE	 	, CONTROL_INS_WAV_OP2_ADSR_00	, CONTROL_INS_WAV_OP2_ADSR_01  , CONTROL_INS_WAV_OP2_ADSR_02  , CONTROL_INS_WAV_OP2_ADSR_03	  ,
	CONTROL_INS_WAV_OP3_TYPE	 , CONTROL_INS_WAV_OP3_GATE	 	, CONTROL_INS_WAV_OP3_ADSR_00	, CONTROL_INS_WAV_OP3_ADSR_01  , CONTROL_INS_WAV_OP3_ADSR_02  , CONTROL_INS_WAV_OP3_ADSR_03	  ,
	CONTROL_INS_WAV_OP4_TYPE	 , CONTROL_INS_WAV_OP4_GATE	 	, CONTROL_INS_WAV_OP4_ADSR_00	, CONTROL_INS_WAV_OP4_ADSR_01  , CONTROL_INS_WAV_OP4_ADSR_02  , CONTROL_INS_WAV_OP4_ADSR_03	  ,
	CONTROL_INS_WAV_WAVPRESET_00 , CONTROL_INS_WAV_WAVPRESET_01	, CONTROL_INS_WAV_WAVPRESET_02 , CONTROL_INS_WAV_WAVPRESET_03 , CONTROL_INS_WAV_WAVPRESET_04  , CONTROL_INS_WAV_WAVPRESET_05 ,
	CONTROL_INS_WAV_COUNT		 , CONTROL_INS_WAV_MAX
};


#define SETTING_FMW_PHASE		0
#define SETTING_FMW_OP1_TYPE	1
#define SETTING_FMW_OP2_TYPE	1
#define SETTING_FMW_OP3_TYPE	2
#define SETTING_FMW_OP4_TYPE	2
#define SETTING_FMW_OP1_ADSR_1	3
#define SETTING_FMW_OP2_ADSR_1	3
#define SETTING_FMW_OP3_ADSR_1	4
#define SETTING_FMW_OP4_ADSR_1	4
#define SETTING_FMW_WAVEDATA0	11
#define SETTING_FMW_WAVEDATA2	12
#define SETTING_FMW_WAVEDATA4	13
#define SETTING_FMW_WAVEDATA6	14
#define SETTING_FMW_WAVEDATA8	15
#define SETTING_FMW_WAVEDATA10	16
#define SETTING_FMW_WAVEDATA12	17
#define SETTING_FMW_WAVEDATA14	18
#define SETTING_FMW_WAVEDATA16	19
#define SETTING_FMW_OP1_GATE	20
#define SETTING_FMW_OP2_GATE	20
#define SETTING_FMW_OP3_GATE	21
#define SETTING_FMW_OP4_GATE	21


enum E_INS_FMW_CONTROLS { 
	CONTROL_INS_FMW_INDEX		, CONTROL_INS_FMW_NAME			, CONTROL_INS_FMW_TYPE			,
	CONTROL_INS_FMW_FM1_GATE	, CONTROL_INS_FMW_FM2_GATE		, CONTROL_INS_FMW_FM3_GATE		, CONTROL_INS_FMW_FM4_GATE		,
	CONTROL_INS_FMW_DISTORTION	, CONTROL_INS_FMW_ALGORITHM		,
	CONTROL_INS_FMW_FM1_TYPE	, CONTROL_INS_FMW_FM1_ADSR_00	, CONTROL_INS_FMW_FM1_ADSR_01	, CONTROL_INS_FMW_FM1_ADSR_02	, CONTROL_INS_FMW_FM1_ADSR_03	,
	CONTROL_INS_FMW_FM2_TYPE	, CONTROL_INS_FMW_FM2_ADSR_00	, CONTROL_INS_FMW_FM2_ADSR_01	, CONTROL_INS_FMW_FM2_ADSR_02	, CONTROL_INS_FMW_FM2_ADSR_03	, 
	CONTROL_INS_FMW_FM3_TYPE	, CONTROL_INS_FMW_FM3_ADSR_00	, CONTROL_INS_FMW_FM3_ADSR_01	, CONTROL_INS_FMW_FM3_ADSR_02	, CONTROL_INS_FMW_FM3_ADSR_03	,
	CONTROL_INS_FMW_FM4_TYPE	, CONTROL_INS_FMW_FM4_ADSR_00	, CONTROL_INS_FMW_FM4_ADSR_01	, CONTROL_INS_FMW_FM4_ADSR_02	, CONTROL_INS_FMW_FM4_ADSR_03	,
	CONTROL_INS_FMW_COUNT		, CONTROL_INS_FMW_MAX
};

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


enum E_TABLE_DISPLAYS { 
	TABLE_DISPLAY_TABLE_TABLE_BAR_TSP,
	TABLE_DISPLAY_TABLE_TABLE_BAR_VOL,
	TABLE_DISPLAY_TABLE_TABLE_BAR_CMD1,
	TABLE_DISPLAY_TABLE_TABLE_BAR_CMD2,
	TABLE_DISPLAY_COUNT,
	TABLE_DISPLAY_MAX 
};

enum E_VIS_DISPLAYS { 
	VIS_DISPLAY_SAMPLE,
	VIS_DISPLAY_ENVELOPE,
	VIS_DISPLAY_COUNT,
	VIS_DISPLAY_MAX 
};

class RegionHandler;

class InstEdit {
	public:
	
		static void dispatchMessage			(u32 msg);
	
		// Specific type modifiers
		static void updateWav				(  );
		static void updateFmw				(  );
		static void updateSmp				(  );
		static void updatePwm				(  );
		
		// Visible instrument modifiers
		static SETTINGS_PWM unpackPwm		( Instrument* i );
		static SETTINGS_WAV unpackWav		( Instrument* i );
		static SETTINGS_FMW unpackFmw		( Instrument* i );
		static SETTINGS_SMP unpackSmp		( Instrument* i );
		static void 		unpack			( Instrument* i );
		static void 		pack			( Instrument* i );
		static void 		repack			( );
		static void 		synchronize		( );
		static void 		load 			( );
		static void 		copy			( Instrument* s, Instrument* d );

		static void clear					( u8 index );
		
		// Waveform visualizers
		static void viewWaveFormFmw			( );
		static void viewWaveFormSmp			( );
		static void viewWaveFormWav			( );
		
		// Control Callbacks
		static void index  					( Control *c, bool bigstep, bool add, u32 *pointer );
		static void wavPreset 				( Control *c, bool bigstep, bool add, u32 *pointer );
		static void smpPreset 				( Control *c, bool bigstep, bool add, u32 *pointer );
		static void type 					( Control *c, bool bigstep, bool add, u32 *pointer );

		// Value modifier wrappers
		static void modBit					( Control *c, bool bigstep, bool add, u32 *pointer );
		static void mod2b					( Control *c, bool bigstep, bool add, u32 *pointer );
		static void mod3b					( Control *c, bool bigstep, bool add, u32 *pointer );
		static void modNibble				( Control *c, bool bigstep, bool add, u32 *pointer );
		static void mod5b					( Control *c, bool bigstep, bool add, u32 *pointer );
		static void mod6b					( Control *c, bool bigstep, bool add, u32 *pointer );
		static void mod7b					( Control *c, bool bigstep, bool add, u32 *pointer );
		static void modByte					( Control *c, bool bigstep, bool add, u32 *pointer );

		static void modSmpKit				( Control *c, bool bigstep, bool add, u32 *pointer );
		static void modAlgorithm			( Control *c, bool bigstep, bool add, u32 *pointer );
		
		static void modVal3					( Control *c, bool bigstep, bool add, u32 *pointer );
		static void modVal5					( Control *c, bool bigstep, bool add, u32 *pointer );
		static void modVal6					( Control *c, bool bigstep, bool add, u32 *pointer );
		static void modVal27				( Control *c, bool bigstep, bool add, u32 *pointer );

		static void modAdsrWav				( Control *c, bool bigstep, bool add, u32 *pointer );
		static void modAdsrFmw 				( Control *c, bool bigstep, bool add, u32 *pointer );
		static void modAdsrSmp				( Control *c, bool bigstep, bool add, u32 *pointer );
		
		
		static Sprite   					waveform[32];
		static bool 						clean;// if false, will cause redraw. Please toggle off when read to avoid unneccesary redraws
		
		static void enter					();
		static void exit 					();
};


extern const Control INS_PWM_CONTROLS	[ CONTROL_INS_PWM_MAX	];
extern const Control INS_WAV_CONTROLS	[ CONTROL_INS_WAV_MAX	];
extern const Control INS_FMW_CONTROLS	[ CONTROL_INS_FMW_MAX	];
extern const Control INS_SMP_CONTROLS	[ CONTROL_INS_SMP_MAX	];
extern const Control TABLE_CONTROLS		[ CONTROL_TABLE_MAX		];
extern const Display TABLE_DISPLAYS		[ TABLE_DISPLAY_MAX 	];
extern const Display VIS_DISPLAYS		[ VIS_DISPLAY_MAX		];

#endif