#ifndef PATEDIT_HPP
#define PATEDIT_HPP

#include "../agb.h"
#include "../modules/regionhandler/regionhandler.hpp"
#include "../data/channel.hpp"

enum E_PAT_CONTROLS { 
	CONTROL_PAT_PATTERNS_A_00,		CONTROL_PAT_PATTERNS_A_01,		CONTROL_PAT_PATTERNS_A_02,		CONTROL_PAT_PATTERNS_A_03,
	CONTROL_PAT_PATTERNS_A_04,		CONTROL_PAT_PATTERNS_A_05,		CONTROL_PAT_PATTERNS_A_06,		CONTROL_PAT_PATTERNS_A_07,
	CONTROL_PAT_PATTERNS_A_08,		CONTROL_PAT_PATTERNS_A_09,		CONTROL_PAT_PATTERNS_A_0A,		CONTROL_PAT_PATTERNS_A_0B,
	CONTROL_PAT_PATTERNS_A_0C,		CONTROL_PAT_PATTERNS_A_0D,		CONTROL_PAT_PATTERNS_A_0E,		CONTROL_PAT_PATTERNS_A_0F,
	CONTROL_PAT_PATTERNS_B_00,		CONTROL_PAT_PATTERNS_B_01,		CONTROL_PAT_PATTERNS_B_02,		CONTROL_PAT_PATTERNS_B_03,
	CONTROL_PAT_PATTERNS_B_04,		CONTROL_PAT_PATTERNS_B_05,		CONTROL_PAT_PATTERNS_B_06,		CONTROL_PAT_PATTERNS_B_07,
	CONTROL_PAT_PATTERNS_B_08,		CONTROL_PAT_PATTERNS_B_09,		CONTROL_PAT_PATTERNS_B_0A,		CONTROL_PAT_PATTERNS_B_0B,
	CONTROL_PAT_PATTERNS_B_0C,		CONTROL_PAT_PATTERNS_B_0D,		CONTROL_PAT_PATTERNS_B_0E,		CONTROL_PAT_PATTERNS_B_0F,
	CONTROL_PAT_PATTERNS_C_00,		CONTROL_PAT_PATTERNS_C_01,		CONTROL_PAT_PATTERNS_C_02,		CONTROL_PAT_PATTERNS_C_03,
	CONTROL_PAT_PATTERNS_C_04,		CONTROL_PAT_PATTERNS_C_05,		CONTROL_PAT_PATTERNS_C_06,		CONTROL_PAT_PATTERNS_C_07,
	CONTROL_PAT_PATTERNS_C_08,		CONTROL_PAT_PATTERNS_C_09,		CONTROL_PAT_PATTERNS_C_0A,		CONTROL_PAT_PATTERNS_C_0B,
	CONTROL_PAT_PATTERNS_C_0C,		CONTROL_PAT_PATTERNS_C_0D,		CONTROL_PAT_PATTERNS_C_0E,		CONTROL_PAT_PATTERNS_C_0F,
	CONTROL_PAT_PATTERNS_D_00,		CONTROL_PAT_PATTERNS_D_01,		CONTROL_PAT_PATTERNS_D_02,		CONTROL_PAT_PATTERNS_D_03,
	CONTROL_PAT_PATTERNS_D_04,		CONTROL_PAT_PATTERNS_D_05,		CONTROL_PAT_PATTERNS_D_06,		CONTROL_PAT_PATTERNS_D_07,
	CONTROL_PAT_PATTERNS_D_08,		CONTROL_PAT_PATTERNS_D_09,		CONTROL_PAT_PATTERNS_D_0A,		CONTROL_PAT_PATTERNS_D_0B,
	CONTROL_PAT_PATTERNS_D_0C,		CONTROL_PAT_PATTERNS_D_0D,		CONTROL_PAT_PATTERNS_D_0E,		CONTROL_PAT_PATTERNS_D_0F,
	CONTROL_PAT_PATTERNS_E_00,		CONTROL_PAT_PATTERNS_E_01,		CONTROL_PAT_PATTERNS_E_02,		CONTROL_PAT_PATTERNS_E_03,
	CONTROL_PAT_PATTERNS_E_04,		CONTROL_PAT_PATTERNS_E_05,		CONTROL_PAT_PATTERNS_E_06,		CONTROL_PAT_PATTERNS_E_07,
	CONTROL_PAT_PATTERNS_E_08,		CONTROL_PAT_PATTERNS_E_09,		CONTROL_PAT_PATTERNS_E_0A,		CONTROL_PAT_PATTERNS_E_0B,
	CONTROL_PAT_PATTERNS_E_0C,		CONTROL_PAT_PATTERNS_E_0D,		CONTROL_PAT_PATTERNS_E_0E,		CONTROL_PAT_PATTERNS_E_0F,
	CONTROL_PAT_PATTERNS_F_00,		CONTROL_PAT_PATTERNS_F_01,		CONTROL_PAT_PATTERNS_F_02,		CONTROL_PAT_PATTERNS_F_03,
	CONTROL_PAT_PATTERNS_F_04,		CONTROL_PAT_PATTERNS_F_05,		CONTROL_PAT_PATTERNS_F_06,		CONTROL_PAT_PATTERNS_F_07,
	CONTROL_PAT_PATTERNS_F_08,		CONTROL_PAT_PATTERNS_F_09,		CONTROL_PAT_PATTERNS_F_0A,		CONTROL_PAT_PATTERNS_F_0B,
	CONTROL_PAT_PATTERNS_F_0C,		CONTROL_PAT_PATTERNS_F_0D,		CONTROL_PAT_PATTERNS_F_0E,		CONTROL_PAT_PATTERNS_F_0F,
	
	CONTROL_PAT_TRANSPOSE_A_00,		CONTROL_PAT_TRANSPOSE_A_01,		CONTROL_PAT_TRANSPOSE_A_02,		CONTROL_PAT_TRANSPOSE_A_03,
	CONTROL_PAT_TRANSPOSE_A_04,		CONTROL_PAT_TRANSPOSE_A_05,		CONTROL_PAT_TRANSPOSE_A_06,		CONTROL_PAT_TRANSPOSE_A_07,
	CONTROL_PAT_TRANSPOSE_A_08,		CONTROL_PAT_TRANSPOSE_A_09,		CONTROL_PAT_TRANSPOSE_A_0A,		CONTROL_PAT_TRANSPOSE_A_0B,
	CONTROL_PAT_TRANSPOSE_A_0C,		CONTROL_PAT_TRANSPOSE_A_0D,		CONTROL_PAT_TRANSPOSE_A_0E,		CONTROL_PAT_TRANSPOSE_A_0F,
	CONTROL_PAT_TRANSPOSE_B_00,		CONTROL_PAT_TRANSPOSE_B_01,		CONTROL_PAT_TRANSPOSE_B_02,		CONTROL_PAT_TRANSPOSE_B_03,
	CONTROL_PAT_TRANSPOSE_B_04,		CONTROL_PAT_TRANSPOSE_B_05,		CONTROL_PAT_TRANSPOSE_B_06,		CONTROL_PAT_TRANSPOSE_B_07,
	CONTROL_PAT_TRANSPOSE_B_08,		CONTROL_PAT_TRANSPOSE_B_09,		CONTROL_PAT_TRANSPOSE_B_0A,		CONTROL_PAT_TRANSPOSE_B_0B,
	CONTROL_PAT_TRANSPOSE_B_0C,		CONTROL_PAT_TRANSPOSE_B_0D,		CONTROL_PAT_TRANSPOSE_B_0E,		CONTROL_PAT_TRANSPOSE_B_0F,
	CONTROL_PAT_TRANSPOSE_C_00,		CONTROL_PAT_TRANSPOSE_C_01,		CONTROL_PAT_TRANSPOSE_C_02,		CONTROL_PAT_TRANSPOSE_C_03,
	CONTROL_PAT_TRANSPOSE_C_04,		CONTROL_PAT_TRANSPOSE_C_05,		CONTROL_PAT_TRANSPOSE_C_06,		CONTROL_PAT_TRANSPOSE_C_07,
	CONTROL_PAT_TRANSPOSE_C_08,		CONTROL_PAT_TRANSPOSE_C_09,		CONTROL_PAT_TRANSPOSE_C_0A,		CONTROL_PAT_TRANSPOSE_C_0B,
	CONTROL_PAT_TRANSPOSE_C_0C,		CONTROL_PAT_TRANSPOSE_C_0D,		CONTROL_PAT_TRANSPOSE_C_0E,		CONTROL_PAT_TRANSPOSE_C_0F,
	CONTROL_PAT_TRANSPOSE_D_00,		CONTROL_PAT_TRANSPOSE_D_01,		CONTROL_PAT_TRANSPOSE_D_02,		CONTROL_PAT_TRANSPOSE_D_03,
	CONTROL_PAT_TRANSPOSE_D_04,		CONTROL_PAT_TRANSPOSE_D_05,		CONTROL_PAT_TRANSPOSE_D_06,		CONTROL_PAT_TRANSPOSE_D_07,
	CONTROL_PAT_TRANSPOSE_D_08,		CONTROL_PAT_TRANSPOSE_D_09,		CONTROL_PAT_TRANSPOSE_D_0A,		CONTROL_PAT_TRANSPOSE_D_0B,
	CONTROL_PAT_TRANSPOSE_D_0C,		CONTROL_PAT_TRANSPOSE_D_0D,		CONTROL_PAT_TRANSPOSE_D_0E,		CONTROL_PAT_TRANSPOSE_D_0F,
	CONTROL_PAT_TRANSPOSE_E_00,		CONTROL_PAT_TRANSPOSE_E_01,		CONTROL_PAT_TRANSPOSE_E_02,		CONTROL_PAT_TRANSPOSE_E_03,
	CONTROL_PAT_TRANSPOSE_E_04,		CONTROL_PAT_TRANSPOSE_E_05,		CONTROL_PAT_TRANSPOSE_E_06,		CONTROL_PAT_TRANSPOSE_E_07,
	CONTROL_PAT_TRANSPOSE_E_08,		CONTROL_PAT_TRANSPOSE_E_09,		CONTROL_PAT_TRANSPOSE_E_0A,		CONTROL_PAT_TRANSPOSE_E_0B,
	CONTROL_PAT_TRANSPOSE_E_0C,		CONTROL_PAT_TRANSPOSE_E_0D,		CONTROL_PAT_TRANSPOSE_E_0E,		CONTROL_PAT_TRANSPOSE_E_0F,
	CONTROL_PAT_TRANSPOSE_F_00,		CONTROL_PAT_TRANSPOSE_F_01,		CONTROL_PAT_TRANSPOSE_F_02,		CONTROL_PAT_TRANSPOSE_F_03,
	CONTROL_PAT_TRANSPOSE_F_04,		CONTROL_PAT_TRANSPOSE_F_05,		CONTROL_PAT_TRANSPOSE_F_06,		CONTROL_PAT_TRANSPOSE_F_07,
	CONTROL_PAT_TRANSPOSE_F_08,		CONTROL_PAT_TRANSPOSE_F_09,		CONTROL_PAT_TRANSPOSE_F_0A,		CONTROL_PAT_TRANSPOSE_F_0B,
	CONTROL_PAT_TRANSPOSE_F_0C,		CONTROL_PAT_TRANSPOSE_F_0D,		CONTROL_PAT_TRANSPOSE_F_0E,		CONTROL_PAT_TRANSPOSE_F_0F,
	
	CONTROL_PAT_SOLO_LEFT_00,		CONTROL_PAT_SOLO_LEFT_01,		CONTROL_PAT_SOLO_LEFT_02,		CONTROL_PAT_MUTE_LEFT_00,
	CONTROL_PAT_MUTE_LEFT_01,		CONTROL_PAT_MUTE_LEFT_02,		CONTROL_PAT_MUTE_RIGHT_00,		CONTROL_PAT_MUTE_RIGHT_01,
	CONTROL_PAT_MUTE_RIGHT_02,		CONTROL_PAT_SOLO_RIGHT_00,		CONTROL_PAT_SOLO_RIGHT_01,		CONTROL_PAT_SOLO_RIGHT_02,
	CONTROL_PAT_COUNT,				CONTROL_PAT_MAX
};

enum E_PAT_DISPLAYS { 
	PAT_DISPLAY_COUNT,
	PAT_DISPLAY_MAX 
};

class PatEdit {
	public:
		static const	u8 		arrow_position	[ CHANNEL_COUNT ];
		static const	u16 	channel_symbols	[ CHANNEL_COUNT ];
		static 			u8 		bookmarks		[ CHANNEL_COUNT ];
		static 			u8 		bookmark_row;
		static 			bool 	solo_clean;
		static 			bool 	clean;
		static 			u16  	icon_time;

		static void sync				( bool verbose = true ); // sync pattern data and optionally redraw controls 
		static void syncPosition		( u8 position );
		static void copy				( Channel *channel );
		static void globalUpdate		(  );
		static void update				(  );
		static void dispatchMessage		( u32 msg );
		static void solo				( Control *c, bool bigstep, bool add, u32 *pointer );
		static void mute				( Control *c, bool bigstep, bool add, u32 *pointer );

		static void shift				( int q , u8 starting_point = 0 );
		static void transpose			( int q );
		static void processInput		( );
		static void clear				( u8 pattern_index );// Delete pattern from pattern shared data
};

extern const Control PAT_CONTROLS[ CONTROL_PAT_MAX ];
extern const Display PAT_DISPLAYS[PAT_DISPLAY_MAX];

#endif 
