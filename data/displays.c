#include "displays.hpp"
#include "data.hpp"
#include "../callbacks/sng.hpp"

/*

FILE DEPRECATED:
This displays must reside in each correspondent callback/xxx file
Use as example Live screen 

*/

const Display SNG_DISPLAYS[SNG_DISPLAY_MAX] = { 
//					{ x	   , y		, invert	, cache					, var								, active	, redraw	},
/*	SLOT00		*/	{ 0x23 , 0x07	, false		, &CACHE_SONGSTATUS		, (u8*)&( SongEdit::has_data[ 0 ]  ), false		, false 	},
/*	SLOT01		*/	{ 0x23 , 0x09	, false		, &CACHE_SONGSTATUS		, (u8*)&( SongEdit::has_data[ 1 ]  ), false		, false 	},
/*	SLOT02		*/	{ 0x23 , 0x0b	, false		, &CACHE_SONGSTATUS		, (u8*)&( SongEdit::has_data[ 2 ]  ), false		, false 	},
/*	SLOT03		*/	{ 0x23 , 0x0d	, false		, &CACHE_SONGSTATUS		, (u8*)&( SongEdit::has_data[ 3 ]  ), false		, false 	},
/*	SLOT04		*/	{ 0x23 , 0x0f	, false		, &CACHE_SONGSTATUS		, (u8*)&( SongEdit::has_data[ 4 ]  ), false		, false 	},
/*	SLOT05		*/	{ 0x23 , 0x11	, false		, &CACHE_SONGSTATUS		, (u8*)&( SongEdit::has_data[ 5 ]  ), false		, false 	},
DISPLAY_TERMINATOR
};


const Display PAT_DISPLAYS[PAT_DISPLAY_MAX] = { 
//					{ x	   , y		, invert	, cache					, var								, active	, redraw	},
DISPLAY_TERMINATOR
};
	
const Display TABLE_DISPLAYS[TABLE_DISPLAY_MAX] = { 
//					{ x	   	, y		, invert	, cache					, var											, active	, redraw	},
/*	BAR_TSP		*/	{ 0x1e	, 0x16	, false		, &CACHE_TABLEPOSITION	, (u8*)&( VAR_INSTRUMENT.TABLE.POSITION[ 0 ] )	, false		, false 	},
/*	BAR_VOL		*/	{ 0x22	, 0x16	, false		, &CACHE_TABLEPOSITION	, (u8*)&( VAR_INSTRUMENT.TABLE.POSITION[ 0 ] )	, false		, false 	},
/*	BAR_CMD1	*/	{ 0x25	, 0x16	, false		, &CACHE_TABLEPOSITION	, (u8*)&( VAR_INSTRUMENT.TABLE.POSITION[ 0 ] )	, false		, false 	},
/*	BAR_CMD2	*/	{ 0x2a	, 0x16	, false		, &CACHE_TABLEPOSITION	, (u8*)&( VAR_INSTRUMENT.TABLE.POSITION[ 1 ] )	, false		, false 	},
DISPLAY_TERMINATOR
};


const Display VIS_DISPLAYS[VIS_DISPLAY_MAX] = { 
//					{ x	   	, y		, invert	, cache					, var											, active	, redraw	},
/*	SAMPLE		*/	{ 0x1e	, 0x26	, false		, &CACHE_VISPOSITION2	, (u8*)&( VAR_INSTRUMENT.VISPOSITION[ 1 ] )		, false		, false 	},
/*	ENVELOPE	*/	{ 0x1e	, 0x15	, false		, &CACHE_VISPOSITION1	, (u8*)&( VAR_INSTRUMENT.VISPOSITION[ 0 ] )		, false		, false 	},
DISPLAY_TERMINATOR
};

static u8 ZERO = 0;

const Display CHAN0_DISPLAYS[CHAN0_DISPLAY_MAX] = { 
//					{ x	   	, y		, invert	, cache					, var		, active	, redraw	},
/*	CHAN1		*/  { 0x0a	, 0x03	, false		, &CACHE_CHANNEL1		, &ZERO		, false		, false 	},
/*	CHAN2		*/  { 0x0e	, 0x03	, false		, &CACHE_CHANNEL2		, &ZERO		, false		, false 	},
/*	CHAN3		*/  { 0x12	, 0x03	, false		, &CACHE_CHANNEL3		, &ZERO		, false		, false 	},
/*	CHAN4		*/  { 0x16	, 0x03	, false		, &CACHE_CHANNEL4		, &ZERO		, false		, false 	},
/*	CHAN5		*/  { 0x1a	, 0x03	, false		, &CACHE_CHANNEL5		, &ZERO		, false		, false 	},
DISPLAY_TERMINATOR
};

const Display CHAN1_DISPLAYS[CHAN1_DISPLAY_MAX] = { 
//					{ x	   	, y		, invert	, cache					, var		, active	, redraw	},
/*	CHAN1		*/	{ 0x01	, 0x03	, false		, &CACHE_CHANNEL0		, &ZERO		, false		, false 	},
/*	CHAN2		*/	{ 0x0e	, 0x03	, false		, &CACHE_CHANNEL2		, &ZERO		, false		, false 	},
/*	CHAN3		*/	{ 0x12	, 0x03	, false		, &CACHE_CHANNEL3		, &ZERO		, false		, false 	},
/*	CHAN4		*/	{ 0x16	, 0x03	, false		, &CACHE_CHANNEL4		, &ZERO		, false		, false 	},
/*	CHAN5		*/	{ 0x1a	, 0x03	, false		, &CACHE_CHANNEL5		, &ZERO		, false		, false 	},
DISPLAY_TERMINATOR
};

//---------------------------------------------------------------------------------------

const Display CHAN2_DISPLAYS[CHAN2_DISPLAY_MAX] = { 
//					{ x	   	, y		, invert	, cache					, var		, active	, redraw	},
/*	CHAN3		*/	{ 0x12	, 0x03	, false		, &CACHE_CHANNEL3		, &ZERO		, false		, false 	},
/*	CHAN1		*/	{ 0x01	, 0x03	, false		, &CACHE_CHANNEL0		, &ZERO		, false		, false 	},
/*	CHAN2		*/	{ 0x04	, 0x03	, false		, &CACHE_CHANNEL1		, &ZERO		, false		, false 	},
/*	CHAN5		*/	{ 0x1a	, 0x03	, false		, &CACHE_CHANNEL5		, &ZERO		, false		, false 	},
/*	CHAN4		*/	{ 0x16	, 0x03	, false		, &CACHE_CHANNEL4		, &ZERO		, false		, false 	},
DISPLAY_TERMINATOR
};
	
//---------------------------------------------------------------------------------------

const Display CHAN3_DISPLAYS[CHAN3_DISPLAY_MAX] = { 
//					{ x	   	, y		, invert	, cache					, var		, active	, redraw	},
/*	CHAN3		*/	{ 0x08	, 0x03	, false		, &CACHE_CHANNEL2		, &ZERO		, false		, false 	},
/*	CHAN1		*/	{ 0x01	, 0x03	, false		, &CACHE_CHANNEL0		, &ZERO		, false		, false 	},
/*	CHAN2		*/	{ 0x04	, 0x03	, false		, &CACHE_CHANNEL1		, &ZERO		, false		, false 	},
/*	CHAN5		*/	{ 0x1a	, 0x03	, false		, &CACHE_CHANNEL5		, &ZERO		, false		, false 	},
/*	CHAN4		*/	{ 0x16	, 0x03	, false		, &CACHE_CHANNEL4		, &ZERO		, false		, false 	},
DISPLAY_TERMINATOR
};
	
//---------------------------------------------------------------------------------------

const Display CHAN4_DISPLAYS[CHAN4_DISPLAY_MAX] = { 
//					{ x	   	, y		, invert	, cache					, var		, active	, redraw	},
/*	CHAN2		*/	{ 0x04	, 0x03	, false		, &CACHE_CHANNEL1		, &ZERO		, false		, false 	},
/*	CHAN4		*/	{ 0x0c	, 0x03	, false		, &CACHE_CHANNEL3		, &ZERO		, false		, false 	},
/*	CHAN5		*/	{ 0x1a	, 0x03	, false		, &CACHE_CHANNEL5		, &ZERO		, false		, false 	},
/*	CHAN1		*/	{ 0x01	, 0x03	, false		, &CACHE_CHANNEL0		, &ZERO		, false		, false 	},
/*	CHAN3		*/	{ 0x08	, 0x03	, false		, &CACHE_CHANNEL2		, &ZERO		, false		, false 	},
DISPLAY_TERMINATOR
};
	
//---------------------------------------------------------------------------------------

const Display CHAN5_DISPLAYS[CHAN5_DISPLAY_MAX] = { 
//					{ x	   	, y		, invert	, cache					, var		, active	, redraw	},
/*	CHAN2		*/	{ 0x04	, 0x03	, false		, &CACHE_CHANNEL1		, &ZERO		, false		, false 	},
/*	CHAN4		*/	{ 0x0c	, 0x03	, false		, &CACHE_CHANNEL3		, &ZERO		, false		, false 	},
/*	CHAN3		*/	{ 0x08	, 0x03	, false		, &CACHE_CHANNEL2		, &ZERO		, false		, false 	},
/*	CHAN5		*/	{ 0x10	, 0x03	, false		, &CACHE_CHANNEL4		, &ZERO		, false		, false 	},
/*	CHAN1		*/	{ 0x01	, 0x03	, false		, &CACHE_CHANNEL0		, &ZERO		, false		, false 	},
DISPLAY_TERMINATOR
};

