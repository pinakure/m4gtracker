#include "progress.hpp" 
#include "../helpers.hpp" 
#include "../data/caches.hpp" 
#include "../modules/regionhandler/regionhandler.hpp" 

u32 				Progress::max;
u32 				Progress::current;
u8 					Progress::step;
u8 					Progress::laststep;
E_StatusStrings 	Progress::upperLine;
E_StatusStrings 	Progress::lowerLine;
bool 				Progress::enabled;
u8 					Progress::redraw;
u32*				Progress::var;

bool Progress::update(){
	if(!enabled) return false;
	
	if( redraw ){
		if( redraw > 1 ){
			RegionHandler::drawCache(10, 5, &CACHE_DLG_PROGRESS_SMALL, 0, 0);
			STATUS(11, 6, 6, upperLine);
			STATUS(11, 8, 7, lowerLine);
		}
		PROGRESS(11, 11, 0, step);
		redraw = 0;
	}			
	
	if(current != *var){
		current = *var;
		
		step = (current * 0x24) / max;		
		if(step != laststep){
			laststep = step;
			redraw = 1;
		}
		if(current >= max)enabled = false;
	}
	return true;
}

void Progress::disable(){
	enabled 	= false;
	var 		= NULL;
	max 		= 0;
	upperLine 	= (E_StatusStrings)0;
	lowerLine 	= (E_StatusStrings)0;
	current 	= 0;
	redraw 	= 0;
	enabled 	= false;
	redraw 	= 0;
}

void Progress::enable(u32 value, u32 max_value, E_StatusStrings uLine, E_StatusStrings lLine, u32 *new_var){
	enabled 	= true;
	var 		= new_var;
	max 		= max_value;
	upperLine 	= uLine;
	lowerLine 	= lLine;
	current 	= *var;
	update();
	redraw 	= 2;
}
