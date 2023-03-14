#ifndef PAT_HPP
#define PAT_HPP

#include "../agb.h"
#include "../modules/regionhandler/regionhandler.hpp"

class PatEdit {
	public:
		static const	u8 		arrow_position[6];
		static const	u16 	channel_symbols[6];
		static 			u8 		bookmark_row;
		static 			u8 		bookmarks[6];
		static 			bool 	solo_clean;
		static 			bool 	clean;
		static 			u16  	icon_time;

		static void sync				( bool verbose = true ); // sync pattern data and optionally redraw controls 
		static void syncPosition		( u8 position );
		static void copy				( u8 channel );
		static void globalUpdate		(RegionHandler *rh);
		static void update				(RegionHandler *rh);
		static void dispatchMessage	(u32 msg);
		static void solo				( Control *c, bool bigstep, bool add, u32 *pointer );
		static void mute				( Control *c, bool bigstep, bool add, u32 *pointer );

		static void shift				( int q , u8 starting_point = 0);
		static void transpose			( int q );
		static void processInput		( );
		static void clear				( u8 pattern_index );// Delete pattern from pattern shared data
};

extern const Control PAT_CONTROLS[ CONTROL_PAT_MAX ];
extern const Callback cb_pat_solo_0;
extern const Callback cb_pat_solo_1;
extern const Callback cb_pat_solo_2;
extern const Callback cb_pat_solo_3;
extern const Callback cb_pat_solo_4;
extern const Callback cb_pat_solo_5;
extern const Callback cb_pat_mute_0;
extern const Callback cb_pat_mute_1;
extern const Callback cb_pat_mute_2;
extern const Callback cb_pat_mute_3;
extern const Callback cb_pat_mute_4;
extern const Callback cb_pat_mute_5;



#endif
