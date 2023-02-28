#include "variables.hpp"

u32				CFG::load_count=0;
CfgPal			CFG::pal[16];
CfgLookNFeel	CFG::look_n_feel;
CfgLinkMode		CFG::link_mode;
CfgBehavior 	CFG::behavior;
CfgTracker		CFG::tracker;
CfgMemory		CFG::memory;
u8 				CFG::slot /*3*/;				//Currently selected song
u8				CFG::menu_slot /*3*/;	
u8	 			CFG::really_return;
char			CFG::alpha_return[14];
u8	 			CFG::clip_return /*2*/;
u8	 			CFG::progress /*6*/;
u8	 			CFG::instrument_vis_type;
GrooveTable		CFG::groove; 					// Pointer to the table being edited/displayed on INS screeen
TableCell		CFG::instrument_table; 			// Pointer to the table being edited/displayed on INS screeen
u8 				CFG::current_channel /*3*/;
u8 				CFG::current_instrument /*6*/;
u8 				CFG::order_position; 			// Index where the PAT screen begins to display pattern orders
u8				CFG::mute[2][3]; 				// Two mute arrays, PPN - WFS
u8				CFG::solo[2][3]; 				// Two solo arrays, PPN - WFS
bool			CFG::reload_skin;
