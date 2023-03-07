#include "m4g.hpp"
#include "data/helpers.hpp"
#include "modules/modules.cpp"

GPU gpu;

#include "callbacks.cpp"
RegionHandler regHnd;

int main(void){
	while(1){
		PREFETCH
		
		INT.Init();	
		MEM.Init();
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
				
		bool lock = false;
		
		// Create and bind virtual screen to the GPU
		VirtualScreen VS;
		gpu.vs = &VS;

		Config::load();
		SRAM.dataRevert( false );
		InstEdit::unpack( &VAR_INSTRUMENT );	

		Debug::runTests();
		Debug::clear();
		LookNFeel::init();
		
		// Start at Tracker screen
		regHnd.load(&REGION_MAP_3_TRK);
		
		while(!sys.var_reset){
			if(!gpu.isVblank()) {lock= false; continue;}
			if(!lock) { lock = true; continue; }							
			sys.update();			
			regHnd.update(1);
		};
	}
	return 0;
}
