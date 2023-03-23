#ifndef ENUM_HPP
#define ENUM_HPP

enum eLinkModes {
	LINKMODE_GBA,
	LINKMODE_IBM,
	LINKMODE_SYNC_SLAVE,
	LINKMODE_SYNC_MASTER,
};

enum E_InstrumentTypes {
	INSTRUMENT_TYPE_PWM,
	INSTRUMENT_TYPE_WAV,
	INSTRUMENT_TYPE_FMW,
	INSTRUMENT_TYPE_SMP,
	INSTRUMENT_TYPE_COUNT
};

enum E_StatusStrings {
	STATUS_SAVING,
	STATUS_LOADING,
	STATUS_RECEIVING,
	STATUS_SENDING,
	STATUS_SYNCWITH,
	STATUS_RESTORING,
	STATUS_PURGING,
	STATUS_TESTING,
	STATUS_REINIT,
	STATUS_CELLS,
	STATUS_DEFAULTS,
	STATUS_INSTDATA,
	STATUS_MASTER,
	STATUS_MEMORY,
	STATUS_MIDIIN,
	STATUS_MIDIOUT,
	STATUS_PATTERNS,
	STATUS_SETTINGS,
	STATUS_SLAVE,
	STATUS_SONG,
	STATUS_CORRUPTED
};

enum E_VAR_TYPES {
	VAR_1BIT,
	VAR_2BIT,
	VAR_4BIT,
	VAR_5BIT,
	VAR_6BIT,
	VAR_8BIT,
	VAR_CHAR,
	VAR_NOTE,
	VAR_3VAL,
	VAR_6VAL,
	VAR_27VAL,
};

enum E_DISPLAY_MAP_1 {
	DISPLAY_LIVE1_RIGHT_A, 
	DISPLAY_LIVE2_KEY00, 
	DISPLAY_LIVE2_KEYLAYOUT_A, 
	DISPLAY_LIVE2_KEYLAYOUT_C, 
	DISPLAY_LIVE2_KEYLAYOUT_B, 
	DISPLAY_LIVE1_STATUS_LOCKED, 
	DISPLAY_LIVE1_SELECT, 
	DISPLAY_LIVE1_STATUS_FREE, 
	DISPLAY_LIVE1_LEFT_A, 
	DISPLAY_LIVE1_START, 
	DISPLAY_LIVE2_KEY12, 
	DISPLAY_LIVE2_KEY24, 
	DISPLAY_LIVE2_KEY36
};

enum E_DISPLAY_MAP_2 {
	DISPLAY_INS_FM_ALGORITHM, 
	DISPLAY_VIS_VIS_BAR_SAMPLE, 
	DISPLAY_VIS_VIS_BAR_ENVELOPE, 
	DISPLAY_PAT_PATTERNSTATUS_A, 
	DISPLAY_PAT_PATTERNSTATUS_B, 
	DISPLAY_PAT_PATTERNSTATUS_C, 
	DISPLAY_PAT_PATTERNSTATUS_D, 
	DISPLAY_PAT_PATTERNSTATUS_E, 
	DISPLAY_PAT_PATTERNSTATUS_F, 
	DISPLAY_TABLE_TABLE_BAR_TSP, 
	DISPLAY_TABLE_TABLE_BAR_VOL, 
	DISPLAY_TABLE_TABLE_BAR_CMD1, 
	DISPLAY_TABLE_TABLE_BAR_CMD2, 
	DISPLAY_SNG_SLOT00, 
	DISPLAY_SNG_SLOT01, 
	DISPLAY_SNG_SLOT02, 
	DISPLAY_SNG_SLOT03, 
};

enum E_DISPLAY_MAP_3 {
	DISPLAY_CHANNEL0_CHANNEL0_1, 
	DISPLAY_CHANNEL0_CHANNEL0_2, 
	DISPLAY_CHANNEL0_CHANNEL0_3, 
	DISPLAY_CHANNEL0_CHANNEL0_4, 
	DISPLAY_CHANNEL0_CHANNEL0_5, 
	DISPLAY_CHANNEL1_CHANNEL1_1, 
	DISPLAY_CHANNEL1_CHANNEL1_2, 
	DISPLAY_CHANNEL1_CHANNEL1_3, 
	DISPLAY_CHANNEL1_CHANNEL1_4, 
	DISPLAY_CHANNEL1_CHANNEL1_5, 
	DISPLAY_CHANNEL2_CHANNEL2_3, 
	DISPLAY_CHANNEL2_CHANNEL2_1, 
	DISPLAY_CHANNEL2_CHANNEL2_2, 
	DISPLAY_CHANNEL2_CHANNEL2_5, 
	DISPLAY_CHANNEL2_CHANNEL2_4, 
	DISPLAY_CHANNEL3_CHANNEL3_3, 
	DISPLAY_CHANNEL3_CHANNEL3_1, 
	DISPLAY_CHANNEL3_CHANNEL3_2, 
	DISPLAY_CHANNEL3_CHANNEL3_5, 
	DISPLAY_CHANNEL3_CHANNEL3_4, 
	DISPLAY_CHANNEL4_CHANNEL4_2, 
	DISPLAY_CHANNEL4_CHANNEL4_4, 
	DISPLAY_CHANNEL4_CHANNEL4_5, 
	DISPLAY_CHANNEL4_CHANNEL4_1, 
	DISPLAY_CHANNEL4_CHANNEL4_3, 
	DISPLAY_CHANNEL5_CHANNEL5_2, 
	DISPLAY_CHANNEL5_CHANNEL5_4, 
	DISPLAY_CHANNEL5_CHANNEL5_3, 
	DISPLAY_CHANNEL5_CHANNEL5_5, 
	DISPLAY_CHANNEL5_CHANNEL5_1
};
	
enum E_DISPLAY_MAP_5 {
	DISPLAY_NULL_PROGRESS_LINE
};

enum E_CallbackEvents {
	EVENT_NONE			= 0x00000000,
	EVENT_KEYDOWN_UP	= 0x00000001, 	
	EVENT_KEYDOWN_DOWN	= 0x00000004, 	
	EVENT_KEYDOWN_LEFT	= 0x00000010, 	
	EVENT_KEYDOWN_RIGHT	= 0x00000040, 	
	EVENT_KEYDOWN_A 	= 0x00000100, 
	EVENT_KEYDOWN_B 	= 0x00000400, 
	EVENT_KEYDOWN_SELECT= 0x00001000, 
	EVENT_KEYDOWN_START	= 0x00004000, 
	EVENT_KEYDOWN_R		= 0x00010000, 
	EVENT_KEYDOWN_L		= 0x00040000, 
	
	EVENT_KEY_UP		= 0x00000003, 	
	EVENT_KEY_DOWN		= 0x0000000C, 	
	EVENT_KEY_LEFT		= 0x00000030, 	
	EVENT_KEY_RIGHT		= 0x000000C0, 	
	EVENT_KEY_A 		= 0x00000300, 
	EVENT_KEY_B 		= 0x00000C00, 
	EVENT_KEY_SELECT	= 0x00003000, 
	EVENT_KEY_START		= 0x0000C000, 
	EVENT_KEY_R			= 0x00030000, 
	EVENT_KEY_L			= 0x000C0000, 
	
	EVENT_KEYUP_UP		= 0x00000002, 	
	EVENT_KEYUP_DOWN	= 0x00000008, 	
	EVENT_KEYUP_LEFT	= 0x00000020, 	
	EVENT_KEYUP_RIGHT	= 0x00000080, 	
	EVENT_KEYUP_A 		= 0x00000200, 
	EVENT_KEYUP_B 		= 0x00000800, 
	EVENT_KEYUP_SELECT	= 0x00002000, 
	EVENT_KEYUP_START	= 0x00008000, 
	EVENT_KEYUP_R		= 0x00020000, 
	EVENT_KEYUP_L		= 0x00080000, 
	
	EVENT_VERTICAL		= 0x00100000, 	// vertical movement
	EVENT_HORIZONTAL	= 0x00200000, 	// horizontal movement
	EVENT_NAVIGATE		= 0x00400000, 	// horizontal movement
	EVENT_MODIFY		= 0x00800000,	// BUTTON + ARROW KEY
	EVENT_FOCUS			= 0x10000000,
	EVENT_LAND			= 0x20000000,
	EVENT_PANIC			= EVENT_KEY_A | EVENT_KEY_B | EVENT_KEY_START | EVENT_KEY_SELECT,	// B + A + SELECT + START	
	/* MIXED EVENTS */
	EVENT_MODIFY_B			= EVENT_MODIFY 		| EVENT_KEY_B,
	EVENT_MODIFY_A			= EVENT_MODIFY 		| EVENT_KEY_A,	
	EVENT_VERTICAL_UP		= EVENT_VERTICAL 	| EVENT_KEY_UP,
	EVENT_VERTICAL_DOWN		= EVENT_VERTICAL 	| EVENT_KEY_DOWN,
	EVENT_HORIZONTAL_LEFT	= EVENT_HORIZONTAL 	| EVENT_KEY_LEFT,
	EVENT_HORIZONTAL_RIGHT	= EVENT_HORIZONTAL 	| EVENT_KEY_RIGHT,	

	/* TERMINATORS */
	EVENT_UNKNOWN		= 0x80000000
};



typedef enum eMessages {
	MESSAGE_OTHER						= 0x00000000,
	MESSAGE_OTHER_BIGPREV				= 0x00000001,//b+down
	MESSAGE_OTHER_BIGNEXT				= 0x00000002,//b+up
	MESSAGE_OTHER_PREV					= 0x00000003,//b+left
	MESSAGE_OTHER_NEXT					= 0x00000004,//b+right
	MESSAGE_OTHER_REDRAW_ALL_CONTROLS	= 0x00000005,
	MESSAGE_OTHER_REFRESH_DATA			= 0x00000006,// refreshed data matrices
	MESSAGE_CUT							= 0x00000007,// refreshed data matrices
	MESSAGE_PASTE						= 0x00000008,// refreshed data matrices
	MESSAGE_REDRAW_CONTROL 				= 0x10000000,
	MESSAGE_REDRAW_DISPLAY 				= 0x20000000,
	MESSAGE_REDRAW_VIEWPORT 			= 0x30000000,
	MESSAGE_REDRAW_REGION 				= 0x40000000,
	MESSAGE_NAVIGATE_LEFT				= 0x50000000,
	MESSAGE_NAVIGATE_RIGHT				= 0x60000000,
	MESSAGE_NAVIGATE_UP 				= 0x70000000,
	MESSAGE_NAVIGATE_DOWN				= 0x80000000,
	MESSAGE_MODIFY_ADD 					= 0x90000000,
	MESSAGE_MODIFY_SUB	 				= 0xA0000000,
	MESSAGE_MODIFY_BIGADD				= 0xB0000000,
	MESSAGE_MODIFY_BIGSUB 				= 0xC0000000,
	MESSAGE_ACTIVATE	 				= 0xD0000000,
	MESSAGE_CANCEL	 					= 0xE0000000,
	MESSAGE_KEYPRESS	 				= 0xF0000000
} Messages;

typedef enum eColors{
	COLOR_NONE,
	COLOR_DARK_CYAN,
	COLOR_CYAN,
	COLOR_RED,
	COLOR_ORANGE,
	COLOR_DARK_RED,
	COLOR_WHITE,
	COLOR_YELLOW,
	COLOR_BLACK,
	COLOR_DARK_BLUE,
	COLOR_BROWN,
	COLOR_OLIVE,
	COLOR_DARK_GREEN,
	COLOR_GRAY,
	COLOR_BLUE,
	COLOR_GREEN,
}Colors;

enum eScreens {
	SCREEN_LIV1	,
	SCREEN_LIV2	,
	SCREEN_HLP	,
	SCREEN_PAT	,
	SCREEN_TRK	,
	SCREEN_INS	,
	SCREEN_SNG	,
	SCREEN_CFG	,
	SCREEN_SNK	,
};

typedef enum eLoopMode {
	LOOP_NONE,
	LOOP_FORWARD,
	LOOP_PINGPONG,
	LOOP_BACKWARD,
	LOOP_BACKWARD_FORWARD,
	LOOP_BACKWARD_BACKWARD,
	LOOP_PINGPONG_FORWARD,
	LOOP_PINGPONG_BACKWARD
}LoopMode;

#endif
