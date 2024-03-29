#ifndef PIANO_HPP
#define PIANO_HPP

#include "../../agb.h"

#include "../../data/control.hpp"
#include "../../data/display.hpp"
#include "../../kernel/regionhandler/regionhandler.hpp"

enum E_LIVE2_CONTROLS { 
	CONTROL_LIVE2_CHANNEL1,			CONTROL_LIVE2_LIVEMODE,			CONTROL_LIVE2_MIDICHAN2,		CONTROL_LIVE2_OCTAVE,
	CONTROL_LIVE2_MIDICHAN1,		CONTROL_LIVE2_CHANNEL2,			CONTROL_LIVE2_TRANSPOSE,		CONTROL_LIVE2_QUANTIZE2,
	CONTROL_LIVE2_COUNT,			CONTROL_LIVE2_MAX
};	

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

class Piano{
	public:
	public: 
		MEM_IN_EWRAM static u8 MODE; 	//2
		MEM_IN_EWRAM static u8 OCTAVE; 	//4
		MEM_IN_EWRAM static u8 QUANTIZE; //3
		MEM_IN_EWRAM static u8 TRANSPOSE;
		MEM_IN_EWRAM static u8 CHANNEL	[2];
		MEM_IN_EWRAM static u8 MIDICHAN	[2];
		MEM_IN_EWRAM static u8 vars[ CONTROL_LIVE2_MAX ];	
		MEM_IN_EWRAM static u8 last_value;

		static void update(  );
		
		static void dispatchMessage( u32 msg, u32 pointer=0 );
};

extern const Control LIVE2_CONTROLS[CONTROL_LIVE2_MAX];
extern const Display LIVE2_DISPLAYS[LIVE2_DISPLAY_MAX]; 

#endif 
