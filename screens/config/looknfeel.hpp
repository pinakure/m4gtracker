#ifndef LOOKNFEEL_HPP
#define LOOKNFEEL_HPP

#include "../../agb.h"
#include "../../modules/regionhandler/regionhandler.hpp"

#define MOSAIC_CNT *(u16*)0x400004C
#define REG_BG0 *(u16*)0x4000008
#define REG_BG1 *(u16*)0x400000A
#define REG_BG2 *(u16*)0x400000C
#define REG_BG3 *(u16*)0x400000E

enum E_LOOKNFEEL_CONTROLS { 
	CONTROL_LOOKNFEEL_MENU1, 		CONTROL_LOOKNFEEL_INTERFACE,	CONTROL_LOOKNFEEL_FONT, 		CONTROL_LOOKNFEEL_BORDER, 
	CONTROL_LOOKNFEEL_SHOWLOGO, 	CONTROL_LOOKNFEEL_STARTSFX, 	CONTROL_LOOKNFEEL_EDITPAL,  	CONTROL_LOOKNFEEL_COUNT, 
	CONTROL_LOOKNFEEL_MAX
};


class LookNFeel {
	public:
		static void init	( );
		static void logoWait( );
		static void logoFade( );
		static void update	( RegionHandler* rh );
		static void credits	( RegionHandler* rh );
		
};

extern const Control LOOKNFEEL_CONTROLS[CONTROL_LOOKNFEEL_MAX];

#endif
