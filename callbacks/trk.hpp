#ifndef TRK_HPP
#define TRK_HPP

#include "../agb.h"

#include "../data/controls.hpp"
#include "../modules/regionhandler/regionhandler.hpp"

class Tracker{
	public:
		static const u8 	columns[6][6];
		static const u8 	positions_x[6];
		static const u8 	positions_y[6];
		static const u16 	channel_symbols[6][6];

		static void syncPattern			( );
		static void syncChannel			( u8 channel_index );
		static void copyChannel			( u8 channel_index );
		static void globalUpdate		( RegionHandler *rh );
		
		static void drawPosition		( int channel );
		static void drawTransientInfo	( );
		static void drawLine			( int channel );
		static void dispatchMessage		( u32 msg);

		static void update				( RegionHandler* rh );// Update function for Config Tracker Suboption!
		static void updateVis			( RegionHandler* rh );
		static void updateHeader1		( RegionHandler* rh );
		static void updateHeader2		( RegionHandler* rh );
		static void updateTable			( RegionHandler* rh );

		static void processInput		(  );
		static void shift				( int q );
		static void transpose			( int q );
};

extern const Control CHAN0_CONTROLS[ CONTROL_CHANNEL0_0_MAX ]; 
extern const Control CHAN1_CONTROLS[ CONTROL_CHANNEL1_0_MAX ]; 
extern const Control CHAN2_CONTROLS[ CONTROL_CHANNEL2_0_MAX ]; 
extern const Control CHAN3_CONTROLS[ CONTROL_CHANNEL3_0_MAX ]; 
extern const Control CHAN4_CONTROLS[ CONTROL_CHANNEL4_0_MAX ]; 
extern const Control CHAN5_CONTROLS[ CONTROL_CHANNEL5_0_MAX ]; 

#endif 
