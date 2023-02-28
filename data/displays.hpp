#ifndef _DATA_DISPLAYS
#define _DATA_DISPLAYS

#include "display.hpp"

enum E_LIVE1_DISPLAYS { 
	LIVE1_DISPLAY_STATUS_LOCKED,
	LIVE1_DISPLAY_STATUS_FREE,
	LIVE1_DISPLAY_SELECT,
	LIVE1_DISPLAY_START,
	LIVE1_DISPLAY_RIGHT_A,
	LIVE1_DISPLAY_LEFT_A,
	LIVE1_DISPLAY_RIGHT_B,
	LIVE1_DISPLAY_LEFT_B,
	
	LIVE1_DISPLAY_RIGHT_UP,
	LIVE1_DISPLAY_LEFT_UP,
	LIVE1_DISPLAY_RIGHT_RIGHT,
	LIVE1_DISPLAY_LEFT_RIGHT,
	LIVE1_DISPLAY_RIGHT_DOWN,
	LIVE1_DISPLAY_LEFT_DOWN,
	LIVE1_DISPLAY_RIGHT_LEFT,
	LIVE1_DISPLAY_LEFT_LEFT,
	
	LIVE1_DISPLAY_RIGHT_L,
	LIVE1_DISPLAY_LEFT_L,
	LIVE1_DISPLAY_RIGHT_R,
	LIVE1_DISPLAY_LEFT_R,
	LIVE1_DISPLAY_COUNT,
	LIVE1_DISPLAY_MAX 
};
extern const Display LIVE1_DISPLAYS[LIVE1_DISPLAY_MAX];

enum E_LIVE2_DISPLAYS { 
	LIVE2_DISPLAY_KEY00,
	LIVE2_DISPLAY_KEYLAYOUT_A,
	LIVE2_DISPLAY_KEYLAYOUT_C,
	LIVE2_DISPLAY_KEYLAYOUT_B,
	LIVE2_DISPLAY_KEY12,
	LIVE2_DISPLAY_KEY24,
	LIVE2_DISPLAY_KEY36,
	LIVE2_DISPLAY_COUNT,
	LIVE2_DISPLAY_MAX 
};
extern const Display LIVE2_DISPLAYS[LIVE2_DISPLAY_MAX];

enum E_SNG_DISPLAYS { 
	SNG_DISPLAY_SNG_SLOT00,
	SNG_DISPLAY_SNG_SLOT01,
	SNG_DISPLAY_SNG_SLOT02,
	SNG_DISPLAY_SNG_SLOT03,
	SNG_DISPLAY_SNG_SLOT04,
	SNG_DISPLAY_SNG_SLOT05,
	SNG_DISPLAY_COUNT,
	SNG_DISPLAY_MAX 
};
extern const Display SNG_DISPLAYS[SNG_DISPLAY_MAX]; 

enum E_PAT_DISPLAYS { 
	PAT_DISPLAY_COUNT,
	PAT_DISPLAY_MAX 
};
extern const Display PAT_DISPLAYS[PAT_DISPLAY_MAX];

enum E_TABLE_DISPLAYS { 
	TABLE_DISPLAY_TABLE_TABLE_BAR_TSP,
	TABLE_DISPLAY_TABLE_TABLE_BAR_VOL,
	TABLE_DISPLAY_TABLE_TABLE_BAR_CMD1,
	TABLE_DISPLAY_TABLE_TABLE_BAR_CMD2,
	TABLE_DISPLAY_COUNT,
	TABLE_DISPLAY_MAX 
};
extern const Display TABLE_DISPLAYS[TABLE_DISPLAY_MAX];

enum E_VIS_DISPLAYS { 
	VIS_DISPLAY_SAMPLE,
	VIS_DISPLAY_ENVELOPE,
	VIS_DISPLAY_COUNT,
	VIS_DISPLAY_MAX 
};
extern const Display VIS_DISPLAYS[VIS_DISPLAY_MAX];

enum E_CHANNEL0_DISPLAYS { 
	CHAN0_DISPLAY_CHAN1,
	CHAN0_DISPLAY_CHAN2,
	CHAN0_DISPLAY_CHAN3,
	CHAN0_DISPLAY_CHAN4,
	CHAN0_DISPLAY_CHAN5, 
	CHAN0_DISPLAY_COUNT,
	CHAN0_DISPLAY_MAX 
};
extern const Display CHAN0_DISPLAYS[CHAN0_DISPLAY_MAX];

enum E_CHANNEL1_DISPLAYS { 
	CHAN1_DISPLAY_CHAN1,
	CHAN1_DISPLAY_CHAN2,
	CHAN1_DISPLAY_CHAN3,
	CHAN1_DISPLAY_CHAN4,
	CHAN1_DISPLAY_CHAN5,
	CHAN1_DISPLAY_COUNT,
	CHAN1_DISPLAY_MAX 
};
extern const Display CHAN1_DISPLAYS[CHAN1_DISPLAY_MAX];

enum E_CHANNEL2_DISPLAYS { 
	CHAN2_DISPLAY_CHAN3,
	CHAN2_DISPLAY_CHAN1,
	CHAN2_DISPLAY_CHAN2,
	CHAN2_DISPLAY_CHAN5,
	CHAN2_DISPLAY_CHAN4,
	CHAN2_DISPLAY_COUNT,
	CHAN2_DISPLAY_MAX 
};
extern const Display CHAN2_DISPLAYS[CHAN2_DISPLAY_MAX];

enum E_CHANNEL3_DISPLAYS { 
	CHAN3_DISPLAY_CHAN3,
	CHAN3_DISPLAY_CHAN1,
	CHAN3_DISPLAY_CHAN2,
	CHAN3_DISPLAY_CHAN5,
	CHAN3_DISPLAY_CHAN4,
	CHAN3_DISPLAY_COUNT,
	CHAN3_DISPLAY_MAX 
};
extern const Display CHAN3_DISPLAYS[CHAN3_DISPLAY_MAX];

enum E_CHANNEL4_DISPLAYS { 
	CHAN4_DISPLAY_CHAN2,
	CHAN4_DISPLAY_CHAN4,
	CHAN4_DISPLAY_CHAN5,
	CHAN4_DISPLAY_CHAN1,
	CHAN4_DISPLAY_CHAN3,
	CHAN4_DISPLAY_COUNT,
	CHAN4_DISPLAY_MAX 
};
extern const Display CHAN4_DISPLAYS[CHAN4_DISPLAY_MAX];

enum E_CHANNEL5_DISPLAYS { 
	CHAN5_DISPLAY_CHAN2,
	CHAN5_DISPLAY_CHAN4,
	CHAN5_DISPLAY_CHAN3,
	CHAN5_DISPLAY_CHAN5,
	CHAN5_DISPLAY_CHAN1,
	CHAN5_DISPLAY_COUNT,
	CHAN5_DISPLAY_MAX 
};
extern const Display CHAN5_DISPLAYS[CHAN5_DISPLAY_MAX];

#endif