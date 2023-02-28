/* ----------------------------------------------------------------------------
AUTHOR		 	Al P.Area ( Smiker )
PURPOSE			Debugging routines and helpers.
ORIGINAL DATE 	2016, October
REVISION DATE 	2023-02-28
 --------------------------------------------------------------------------- */
#include "callbacks.hpp"
#include "../modules/regionhandler/regionhandler.hpp"

// Stress test for GUI Display Functions
void overloadTest(REGHND &regHnd){
	static u16 i, o;
	// M4G0 code offers a great collection of macros which you
	// can use to display a value in a bunch of different formats:
	
	// Hexadecimal values
	HEXADECIMAL(1,4,4, i);
	HEXADECIMAL_DOUBLE(1,5,4, i);
	HEXADECIMAL_TWOTILES(1,6,4, i);
	BIGHEX(3,4,4,i);
	
	// Decimal values
	DECIMAL(1,7,7, i);
	DECIMAL_DOUBLE(1,8,7, i);
	DECIMAL_TWOTILES(1,9,7, i);
	DECIMAL_DOUBLE_TWOTILES(4,9,7, i);
	BIGDEC(3,7,4,i);
	
	// Special values
	NIBBLE(1,10,6, i);
	BIGTEXT(3,10,6,i);
	TEXT(1,11,6,i);
	NOTE(1,12,0xFF, i);
	STATUS(1,13,6,i);
	
	// Waves and vumeters
	PROGRESS(1,15, 0xFF,i); 
	WAVE_DOUBLE(1,16,0xFF, i); 
	WAVE_SINGLE(3,16,0xFF, i);				
	COMMAND(3,17, 0xFF,i);
	VUMETER_V3(6,16,0xFF, i);				
	VUMETER_V1(5,18,0xFF, i);				
	VUMETER_H3(2,18,0xFF, i);				
	VUMETER_H1(1,18,0xFF, i);
	
	// Position Markers
	TABLEPOS(14, 2,0xFF, i);
	VISPOS1(14, 1,0xFF, i);
	VISPOS2(14, 18,0xFF, i);
	// Lets change the value little by little
	i++;
	return;
	o&=15;
	if(o==0)i++;
	o++;
}
