#include "variables.hpp"

u32				CFG::loadCount=0;
CFG_PAL 		CFG::PAL[16];
CFG_LOOKNFEEL	CFG::LOOKNFEEL;
CFG_LINKMODE	CFG::LINKMODE;
CFG_BEHAVIOR 	CFG::BEHAVIOR;
CFG_TRACKER		CFG::TRACKER;
CFG_MEMORY		CFG::MEMORY;
u8 				CFG::SLOT /*3*/;//Currently selected song
u8				CFG::MENUSLOT /*3*/;	
u8	 			CFG::REALLYRETURN;
char			CFG::ALPHARETURN[14];
u8	 			CFG::CLIPRETURN /*2*/;
u8	 			CFG::PROGRESS /*6*/;
u8	 			CFG::INSTRUMENTVISTYPE;
GrooveTable		CFG::groove; 			// Pointer to the table being edited/displayed on INS screeen
TABLE_CELL 		CFG::INSTRUMENTTABLE; 	// Pointer to the table being edited/displayed on INS screeen
u8 				CFG::CURRENTCHANNEL /*3*/;
u8 				CFG::CURRENTINSTRUMENT 	/*6*/;
u8 				CFG::ORDERPOSITION; 		// Index where the PAT screen begins to display pattern orders
u8				CFG::MUTE[2][3]; // Two mute arrays, PPN - WFS
u8				CFG::SOLO[2][3]; // Two solo arrays, PPN - WFS
bool			CFG::RELOADSKIN;
