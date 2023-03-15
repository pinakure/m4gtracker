#include "m4g.hpp"
#include "data/helpers.hpp"
#include "modules/modules.cpp"
#include "callbacks.cpp"
#include "callbacks/cfg.hpp"


vu32 SYS_TIMER=0;
u32 SYS_PROFILEDTIME=0;
vu32 SYS_PROFILETIMER=0;
vu32 SYS_FPS=0;
vu32 SYS_FRAMES=0;
volatile bool SYS_SOUNDTIME=false;
u8 SYS_ACTIVESCREEN=0;
u8 SYS_PARAM=0;
bool SYS_SYNCTRACKER = false;
volatile bool SYS_QUERYKEY=false;


MEM_IN_EWRAM Song			VAR_SONG;
MEM_IN_EWRAM Input 			VAR_INPUT; 
MEM_IN_EWRAM u8 			VAR_KEY[4];
MEM_IN_EWRAM Pattern 		VAR_PATTERN[6];
MEM_IN_EWRAM SETTINGS_PWM 	VAR_PWM;
MEM_IN_EWRAM SETTINGS_FMW 	VAR_FMW;
MEM_IN_EWRAM SETTINGS_SMP 	VAR_SMP;
MEM_IN_EWRAM SETTINGS_WAV 	VAR_WAV;
MEM_IN_EWRAM Instrument 	VAR_INSTRUMENTS[64];
MEM_IN_EWRAM Instrument 	VAR_INSTRUMENT;
MEM_IN_EWRAM CFG 			VAR_CFG;
MEM_IN_EWRAM Live 			VAR_LIVE;
MEM_IN_EWRAM Channel 		VAR_CHANNEL[6];
MEM_IN_EWRAM PatternCell 	VAR_CELLS[6]; 
MEM_IN_EWRAM PatternCell 	VAR_DATA[128]; 

int main(void){
	
	while(1){
		PREFETCH
		
		INT.Init();	
		Mem::init();
		sys.init();
		SRAM.Init();
		gpu.start();
		INT.Enable(IRQ_VBLANK);
		INT.Enable(IRQ_HBLANK);
		TIM0.Init(0);
		//INT.Enable(IRQ_KEYPAD);
		TIM0.Setup(0x0004, 1);
		TIM0.Enable();

		sys.init();
		
		#ifdef VSYNC
		bool lock = false;
		#endif
		
		// Create and bind virtual screen to this instance
		VirtualScreen VS;
		gpu.vs = &VS;

		Config::load();
		//SRAM.dataRevert( false );
		InstEdit::unpack( &VAR_INSTRUMENT );	

		Debug::runTests();
		gpu.clear();
		LookNFeel::init();
		
		// Start at Tracker screen
		regHnd.load(&REGION_MAP_3_TRK);
		
		while(!sys.var_reset){
			#ifdef VSYNC
				// IF SOMETHING GOES WRONG WITH TIMERS OR AUDIO, TRY ENABLING THIS:
				if(!gpu.isVblank()	) { lock = false; continue; }
	 			if(!lock				) { lock =  true; continue; }
				adsadas
			#endif
			sys.update();			
			regHnd.update(1);
			DEBUG_UPDATE();
			regHnd.new_region = false;
		};
	}
	return 0;
}
