#ifndef LIV_HPP
#define LIV_HPP

#include "../agb.h"

#include "../data/controls.hpp"
#include "../modules/regionhandler/regionhandler.hpp"

class LiveSet {
	public:
	
		static u8 	last_piano_value;
	
		static void updateCmder(RegionHandler* rh);
		static void updatePiano(RegionHandler* rh);
};

extern const Control LIVE1_CONTROLS[ CONTROL_LIVE1_MAX ]; 
extern const Callback cb_live_retrig;
extern const Callback cb_live_speed;	
extern const Callback cb_live_quantize1;
extern const Callback cb_live_chan1;
extern const Callback cb_live_chan2;
extern const Callback cb_live_mode;
extern const Callback cb_live_octave;
extern const Callback cb_live_transpose;
extern const Callback cb_live_quantize2;
extern const Callback cb_live_midichan1;
extern const Callback cb_live_midichan2;

#endif
