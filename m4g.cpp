#include "m4g.hpp"
#include "debug.hpp"
#include "helpers.hpp"
#include "modules/modules.hpp"
#include "screens/config.hpp"


vu32 							SYS_TIMER			= 0;
u32								SYS_PROFILEDTIME	= 0;
vu32 							SYS_PROFILETIMER	= 0;
vu32 							SYS_FPS				= 0;
vu32 							SYS_FRAMES			= 0;
volatile bool 					SYS_SOUNDTIME  		= false;
u8 								SYS_ACTIVESCREEN	= 0;
u8 								SYS_PARAM			= 0;
bool 							SYS_SYNCTRACKER 	= false;
volatile bool 					SYS_QUERYKEY		= false;


MEM_IN_EWRAM Instrument 		VAR_INSTRUMENT;
MEM_IN_EWRAM Song				VAR_SONG;
MEM_IN_EWRAM Input 				VAR_INPUT; 
MEM_IN_EWRAM SETTINGS_PWM 		VAR_PWM;
MEM_IN_EWRAM SETTINGS_FMW 		VAR_FMW;
MEM_IN_EWRAM SETTINGS_SMP 		VAR_SMP;
MEM_IN_EWRAM SETTINGS_WAV 		VAR_WAV;
MEM_IN_EWRAM CFG 				VAR_CFG;
MEM_IN_EWRAM u8 				VAR_KEY			[ 4 ];
MEM_IN_EWRAM Pattern 			VAR_PATTERN		[ CHANNEL_COUNT ];
MEM_IN_EWRAM Instrument 		VAR_INSTRUMENTS	[ INSTRUMENT_COUNT ];
MEM_IN_EWRAM Channel 			VAR_CHANNEL		[ CHANNEL_COUNT ];
MEM_IN_EWRAM PatternCell 		VAR_CELLS		[ CHANNEL_COUNT ]; 
MEM_IN_EWRAM PatternCell 		VAR_DATA		[ PATTERN_COUNT ];

int main(void){
	#ifdef VSYNC
	static bool lock = false;
	#endif
	
	PREFETCH

	for(;;){
		
		Sys::init();
		
		for(;;){
			#ifdef VSYNC
				// IF SOMETHING GOES WRONG WITH TIMERS OR AUDIO, TRY ENABLING THIS:
				if(!Gpu::isVblank()	) { lock = false	; continue; }
	 			if(!lock					) { lock = true	; continue; }
				adsadas
			#endif
			Sys::update();			
			regHnd.update(1);
			DEBUG_UPDATE();
			regHnd.new_region = false;
		};
	}
	return 0;
}
