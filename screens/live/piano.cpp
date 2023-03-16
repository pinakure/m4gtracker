#include "../live.hpp"
#include "../../data/data.hpp"

MEM_IN_EWRAM u8 	Piano::last_value = 0xFFFF;
MEM_IN_EWRAM u8 	Piano::vars[ CONTROL_LIVE2_MAX ];	

MEM_IN_EWRAM u8 	Piano::MODE; 	//2
MEM_IN_EWRAM u8 	Piano::OCTAVE; 	//4
MEM_IN_EWRAM u8 	Piano::QUANTIZE; //3
MEM_IN_EWRAM u8 	Piano::TRANSPOSE;
MEM_IN_EWRAM u8 	Piano::CHANNEL[2];
MEM_IN_EWRAM u8 	Piano::MIDICHAN[2];

const Display LIVE2_DISPLAYS[LIVE2_DISPLAY_MAX] = { 
//				  	{ x	  ,	y		, invert	, cache					, var							 	 , active		, redraw	},
/*	KEY00		*/	{ 0x01 , 0x08	, false		, &CACHE_KEYS			, (u8*)&(VAR_KEY[ 0 ]			    ), false		, false 	},
/*	KEYLAYOUT_A	*/	{ 0x01 , 0x04	, false		, &CACHE_KEYLAYOUT_A	, (u8*)&(Live::PIANO.MODE	    	), false		, false 	},
/*	KEYLAYOUT_C	*/	{ 0x01 , 0x11	, false		, &CACHE_KEYLAYOUT_C	, (u8*)&(Live::PIANO.MODE	    	), false		, false 	},
/*	KEYLAYOUT_B	*/	{ 0x01 , 0x0a	, false		, &CACHE_KEYLAYOUT_B	, (u8*)&(Live::PIANO.MODE	    	), false		, false 	},
/*	KEY12		*/	{ 0x10 , 0x08	, false		, &CACHE_KEYS			, (u8*)&(VAR_KEY[ 0 ]			    ), false		, false 	},
/*	KEY24		*/	{ 0x01 , 0x0f	, false		, &CACHE_KEYS			, (u8*)&(VAR_KEY[ 0 ]			    ), false		, false 	},
/*	KEY36		*/	{ 0x10 , 0x0f	, false		, &CACHE_KEYS			, (u8*)&(VAR_KEY[ 0 ]			    ), false		, false 	},
DISPLAY_TERMINATOR
};

const Callback cb_live_chan1	 = { modify6VAL		, EVENT_MODIFY_B		, &Live::PIANO.CHANNEL[0]	, NULL };
const Callback cb_live_chan2	 = { modify6VAL		, EVENT_MODIFY_B		, &Live::PIANO.CHANNEL[1]	, NULL };
const Callback cb_live_mode		 = { modify3VAL		, EVENT_MODIFY_B		, &Live::PIANO.MODE			, NULL };
const Callback cb_live_octave	 = { modify3BIT		, EVENT_MODIFY_B		, &Live::PIANO.OCTAVE		, NULL };
const Callback cb_live_transpose = { modify8BIT		, EVENT_MODIFY_B		, &Live::PIANO.TRANSPOSE		, NULL };
const Callback cb_live_quantize2 = { modify3BIT		, EVENT_MODIFY_B		, &Live::PIANO.QUANTIZE		, NULL };
const Callback cb_live_midichan1 = { modify4BIT		, EVENT_MODIFY_B		, &Live::PIANO.MIDICHAN[0]	, NULL };
const Callback cb_live_midichan2 = { modify4BIT		, EVENT_MODIFY_B		, &Live::PIANO.MIDICHAN[1]	, NULL };	

#define CTL( a ) &LIVE2_CONTROLS[ CONTROL_LIVE2_##a ]
#define VAR( a ) ((u8*)&(Live::PIANO.a))
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

void Piano::update( RegionHandler* rh ){
	if(Live::PIANO.MODE != last_value){
		last_value = Live::PIANO.MODE;
		REDRAW(LIVE2_DISPLAY_KEYLAYOUT_A);
		REDRAW(LIVE2_DISPLAY_KEYLAYOUT_B);
		REDRAW(LIVE2_DISPLAY_KEYLAYOUT_C);
	}
}


