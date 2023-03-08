#include "liv.hpp"
#include "../data/data.hpp"

u8 LiveSet::last_piano_value = 0xFFFF;

#define CTL( a ) &LIVE2_CONTROLS[ CONTROL_LIVE2_##a ]
#define VAR( a ) ((u8*)&(VAR_LIVE.PIANO.a))
const Control LIVE2_CONTROLS[CONTROL_LIVE2_MAX] = { 
//	{ x	 	, y		, up				, right				, down				, left				, cache						, var					, callback				},			
	{ 0x1b  , 0x01 	, CTL( QUANTIZE2  ) , CTL( CHANNEL2	  ) , CTL( MIDICHAN1  )	, CTL( OCTAVE	  ) , &CACHE_CHANNELS			, VAR( CHANNEL	  [0] )	, &cb_live_chan1 		},
	{ 0x10  , 0x00 	, CTL( TRANSPOSE  ) , CTL( QUANTIZE2  ) , CTL( OCTAVE	  )	, CTL( QUANTIZE2  ) , &CACHE_LIVE_MODE			, VAR( MODE			  )	, &cb_live_mode			},
	{ 0x1c  , 0x02 	, CTL( CHANNEL2	  ) , CTL( TRANSPOSE  ) , CTL( QUANTIZE2  )	, CTL( MIDICHAN1  ) , &CACHE_HEXADECIMAL		, VAR( MIDICHAN	  [1] )	, &cb_live_midichan2	},
	{ 0x12  , 0x01 	, CTL( LIVEMODE	  ) , CTL( CHANNEL1	  ) , CTL( TRANSPOSE  )	, CTL( CHANNEL2	  ) , &CACHE_HEXADECIMAL		, VAR( OCTAVE		  )	, &cb_live_octave		},
	{ 0x1b  , 0x02 	, CTL( CHANNEL1	  ) , CTL( MIDICHAN2  ) , CTL( QUANTIZE2  )	, CTL( TRANSPOSE  ) , &CACHE_HEXADECIMAL		, VAR( MIDICHAN	  [0] )	, &cb_live_midichan1	},
	{ 0x1c  , 0x01 	, CTL( QUANTIZE2  ) , CTL( OCTAVE	  ) , CTL( MIDICHAN2  )	, CTL( CHANNEL1	  ) , &CACHE_CHANNELS			, VAR( CHANNEL	  [1] )	, &cb_live_chan2 		},
	{ 0x12  , 0x02 	, CTL( OCTAVE	  ) , CTL( MIDICHAN1  ) , CTL( LIVEMODE	  )	, CTL( MIDICHAN2  ) , &CACHE_HEXADECIMAL_DOUBLE	, VAR( TRANSPOSE	  )	, &cb_live_transpose	},
	{ 0x1c  , 0x00 	, CTL( MIDICHAN2  ) , CTL( LIVEMODE	  ) , CTL( CHANNEL2	  )	, CTL( LIVEMODE	  ) , &CACHE_QUANTIZES			, VAR( QUANTIZE		  )	, &cb_live_quantize2	},
	CONTROL_TERMINATOR
};
#undef VAR
#undef CTL

const Callback cb_live_chan1	 = { modify6VAL		, EVENT_MODIFY_B		, &VAR_LIVE.PIANO.CHANNEL[0]	, NULL };
const Callback cb_live_chan2	 = { modify6VAL		, EVENT_MODIFY_B		, &VAR_LIVE.PIANO.CHANNEL[1]	, NULL };
const Callback cb_live_mode		 = { modify3VAL		, EVENT_MODIFY_B		, &VAR_LIVE.PIANO.MODE			, NULL };
const Callback cb_live_octave	 = { modify3BIT		, EVENT_MODIFY_B		, &VAR_LIVE.PIANO.OCTAVE		, NULL };
const Callback cb_live_transpose = { modify8BIT		, EVENT_MODIFY_B		, &VAR_LIVE.PIANO.TRANSPOSE		, NULL };
const Callback cb_live_quantize2 = { modify3BIT		, EVENT_MODIFY_B		, &VAR_LIVE.PIANO.QUANTIZE		, NULL };
const Callback cb_live_midichan1 = { modify4BIT		, EVENT_MODIFY_B		, &VAR_LIVE.PIANO.MIDICHAN[0]	, NULL };
const Callback cb_live_midichan2 = { modify4BIT		, EVENT_MODIFY_B		, &VAR_LIVE.PIANO.MIDICHAN[1]	, NULL };	

void LiveSet::updatePiano(RegionHandler* rh){
	
	if(VAR_LIVE.PIANO.MODE != last_piano_value){
		last_piano_value = VAR_LIVE.PIANO.MODE;
		REDRAW(LIVE2_DISPLAY_KEYLAYOUT_A);
		REDRAW(LIVE2_DISPLAY_KEYLAYOUT_B);
		REDRAW(LIVE2_DISPLAY_KEYLAYOUT_C);
	}
}
