#include "adsr.hpp"
#include "../../data/tileset.hpp"
#include "../gpu/gpu.hpp"
#include "../regionhandler/regionhandler.hpp"
#include "../../callbacks/debug.hpp"

u8 		Adsr::last_position			= 0;
u8 		Adsr::last_arrow_position	= 0;
u8 		Adsr::arrow_position			= 0;
bool	Adsr::redraw 					= false;

void Adsr::drawX4( u8 adsr_tables[ 4 ][ ADSR_TABLE_LENGTH ] , u8 adsr_position ){
	
	if( regHnd.region != &REGION_MAP_2_INS ) return;
	
	if( ( last_position == adsr_position ) || redraw ) return;
	last_position = adsr_position;
	
	static int lastpos = -1;
	int arrow_position = adsr_position>>(1+ADSR_LENGTH_SCALE);

	if( adsr_position < (ADSR_TABLE_LENGTH-2) ){
		
		gpu.vs->clear();
		
		for( int x = 0 , half = 0, quad = 0	
			; x < ( VIRTUALSCREEN_WIDTH << 1 ) 
			; x+=2 , quad = x << 2, half = x >> 1
		){
			gpu.vs->set( half ,  7 - ( adsr_tables[ 0 ][ quad ] >> 5  ) );
			gpu.vs->set( half , 15 - ( adsr_tables[ 1 ][ quad ] >> 5  ) );
			gpu.vs->set( half , 23 - ( adsr_tables[ 2 ][ quad ] >> 5  ) );
			gpu.vs->set( half , 31 - ( adsr_tables[ 3 ][ quad ] >> 5  ) );
		}
		//gpu.number(26, 1, ADSR_TABLE_LENGTH, COLOR_RED);

		gpu.set( last_arrow_position 	& 1 ? 2 : 1 , 14 + ( last_arrow_position 	>> 1 ) , 18 , 0x0100 ); 
		gpu.set( arrow_position 			& 1 ? 2 : 1 , 14 + ( arrow_position			>> 1 ) , 18 , 0x711E ); 
		last_arrow_position = arrow_position;
		
		//DECIMAL_DOUBLE(28,1,adsr_position < (ADSR_TABLE_LENGTH-1)?COLOR_YELLOW:COLOR_ORANGE ,adsr_position/100);
		//DECIMAL_DOUBLE(29,1,adsr_position < (ADSR_TABLE_LENGTH-1)?COLOR_YELLOW:COLOR_ORANGE ,adsr_position);
		
		gpu.vs->draw(14,6);
		
	} 
	
	redraw = false;
}

void Adsr::draw( u8 adsr_table[ ADSR_TABLE_LENGTH ] , u8 adsr_position ){
	if( regHnd.region != &REGION_MAP_2_INS ) return;
	
	if( ( last_position == adsr_position ) || redraw ) return;
	last_position = adsr_position;
	
	static int lastpos = -1;
	int arrow_position = adsr_position>>(1+ADSR_LENGTH_SCALE);

	if( adsr_position < (ADSR_TABLE_LENGTH-2) ){

		gpu.vs->clear();
		
		for( int x = 0 , half = 0, quad = 0	
			; x < ( VIRTUALSCREEN_WIDTH << 1 ) 
			; x+=2 , quad = x << 2, half = x >> 1
		){
			gpu.vs->set( half ,  31 - ( adsr_table[ quad ] >> 3  ) );
		}
		//gpu.number(26, 1, ADSR_TABLE_LENGTH, COLOR_RED);

		gpu.set( last_arrow_position 	& 1 ? 2 : 1 , 14 + ( last_arrow_position 	>> 1 ) , 18 , 0x0100 ); 
		gpu.set( arrow_position 			& 1 ? 2 : 1 , 14 + ( arrow_position			>> 1 ) , 18 , 0x711E ); 
		last_arrow_position = arrow_position;
		
		//DECIMAL_DOUBLE(28,1,adsr_position < (ADSR_TABLE_LENGTH-1)?COLOR_YELLOW:COLOR_ORANGE ,adsr_position/100);
		//DECIMAL_DOUBLE(29,1,adsr_position < (ADSR_TABLE_LENGTH-1)?COLOR_YELLOW:COLOR_ORANGE ,adsr_position);
		
		gpu.vs->draw(14,6);
		
	} 
	
	redraw = false;
}
