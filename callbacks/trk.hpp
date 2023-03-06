#ifndef TRK_HPP
#define TRK_HPP

#include "../agb.h"

class Tracker{
	public:
		static const u8 columns[6][6];
		static const u8 positions_x[6];
		static const u8 positions_y[6];
		static const u16 Tracker::channel_symbols[6][6];


		static void syncPattern();
		static void syncChannel( u8 channel_index );
		static void copyChannel( u8 channel_index );
		static void globalUpdate( RegionHandler *rh );
		
		static void drawPosition		(int channel, bool hl);
		static void drawTransientInfo	( );
		static void drawLine			( int channel );
		static void dispatchMessage		(u32 msg);



};

#endif 
