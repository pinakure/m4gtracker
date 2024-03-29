#ifndef CONFIG_HPP
#define CONFIG_HPP

#include "../agb.h"
#include "groovetable.hpp"
#include "tablecell.hpp"
#include "color.hpp"

typedef struct _CFG_LOOKNFEEL {
	u8	 			INTERFACE;
	u8				FONT /*3*/;
	u8				BORDER /*3*/;
	u8	 			SHOWLOGO;
	u8	 			STARTUPSFX;
}CFG_LOOKNFEEL;

// typedef struct _CFG_LINKMODE {
	// u8 				LINKMODE /*2*/;
	// u8				MASTERCLOCK;
	// u8 				SYNCRATE /*4*/;
	// u8 				SYNCTICKS;
// }CFG_LINKMODE;

typedef struct _CFG_BEHAVIOR {
	u8				AUTOLOAD;
	u8				KEYRATE /*4*/;
	u8				AUTOSAVE /*1*/;
	u8				VISTYPE;
	u8 				POWERSAVING;//1
}CFG_BEHAVIOR;

typedef struct _CFG_TRACKER {
	u8 				FINETUNE;
	u8				PRELISTEN;
	u8				TRANSPOSE;
	u8				INPUTMODE;
	u8				SOUNDBIAS;
	// NOT USED
	u8				HEADERTYPE;
}CFG_TRACKER;

typedef struct _CFG_MEMORY {
	u8				PREF;
	int nothing;
}CFG_MEMORY;

typedef struct _CFG {
	u32				loadCount;
	Color 			PAL[16];
	CFG_LOOKNFEEL	LOOKNFEEL;
//	CFG_LINKMODE	LINKMODE;
	CFG_BEHAVIOR 	BEHAVIOR;
	CFG_TRACKER		TRACKER;
	CFG_MEMORY		MEMORY;
	u8				MENUSLOT /*3*/;	
	u8	 			INSTRUMENTVISTYPE;
	u8 				CURRENTCHANNEL /*3*/;
	u8 				CURRENTINSTRUMENT 	/*6*/;
	u8 				ORDERPOSITION; 		// Index where the PAT screen begins to display pattern orders
	u8				MUTE[2][3]; // Two mute arrays, PPN - WFS
	u8				SOLO[2][3]; // Two solo arrays, PPN - WFS
	bool			RELOADSKIN;
}CFG;

extern CFG VAR_CFG;

#endif
