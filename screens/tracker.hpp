#ifndef TRACKER_HPP
#define TRACKER_HPP

#include "../agb.h"

#include "../data/controls.hpp"
#include "../modules/regionhandler/regionhandler.hpp"
#include "../data/channel.hpp"

enum E_TRACKER_CONTROLS { 
	CONTROL_TRACKER_MENU4,			CONTROL_TRACKER_FINETUNE,		CONTROL_TRACKER_PRELISTEN,		CONTROL_TRACKER_TRANSPOSE,
	CONTROL_TRACKER_INPUTMODE,		CONTROL_TRACKER_SOUNDBIAS,		CONTROL_TRACKER_MIXER,			CONTROL_TRACKER_COUNT,
	CONTROL_TRACKER_MAX
};

class Tracker{
	public:
		static const u8 	columns			[ CHANNEL_COUNT ][ CHANNEL_COUNT ];
		static const u8 	positions_x		[ CHANNEL_COUNT ];
		static const u8 	positions_y		[ CHANNEL_COUNT ];
		static const u16 	channel_symbols	[ CHANNEL_COUNT ][ CHANNEL_COUNT ];
		static bool  		clean; // true if tracker not needing redraw

		static void syncPattern			( );
		static void syncChannel			( u8 channel_index );
		static void copyChannel			( u8 channel_index );
		static void globalUpdate		(  );
		
		static void drawPosition		( int channel );
		static void drawTransientInfo	( );
		static void drawLine			( int channel );
		static void dispatchMessage		( u32 msg);

		static void update				(  );// Update function for Config Tracker Suboption!
		static void updateVis			(  );
		static void updateHeader1		(  );
		static void updateHeader2		(  );
		static void updateTable			(  );

		static void processInput		(  );
		static void shift				( int q );
		static void transpose			( int q );

		static void alterColumn			( u8 index, u8 min_value, u8 max_value, int q );
	
};

extern const Callback cb_cfg_finetune;
extern const Callback cb_cfg_prelisten;
extern const Callback cb_cfg_transpose;
extern const Callback cb_cfg_inputmode;
extern const Callback cb_cfg_soundbias;
extern const Callback cb_cfg_mixer;

extern const Control CHAN0_CONTROLS[ CONTROL_CHANNEL0_0_MAX ]; 
extern const Control CHAN1_CONTROLS[ CONTROL_CHANNEL1_0_MAX ]; 
extern const Control CHAN2_CONTROLS[ CONTROL_CHANNEL2_0_MAX ]; 
extern const Control CHAN3_CONTROLS[ CONTROL_CHANNEL3_0_MAX ]; 
extern const Control CHAN4_CONTROLS[ CONTROL_CHANNEL4_0_MAX ]; 
extern const Control CHAN5_CONTROLS[ CONTROL_CHANNEL5_0_MAX ]; 

extern const Control TRACKER_CONTROLS[ CONTROL_TRACKER_MAX ];

#endif
