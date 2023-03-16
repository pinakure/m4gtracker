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
const Display CHAN2_DISPLAYS[CHAN2_DISPLAY_MAX] = { 
//					{ x	   	, y		, invert	, cache					, var		, active	, redraw	},
/*	CHAN3		*/	{ 0x12	, 0x03	, false		, &CACHE_CHANNEL3		, &ZERO		, false		, false 	},
/*	CHAN1		*/	{ 0x01	, 0x03	, false		, &CACHE_CHANNEL0		, &ZERO		, false		, false 	},
/*	CHAN2		*/	{ 0x04	, 0x03	, false		, &CACHE_CHANNEL1		, &ZERO		, false		, false 	},
/*	CHAN5		*/	{ 0x1a	, 0x03	, false		, &CACHE_CHANNEL5		, &ZERO		, false		, false 	},
/*	CHAN4		*/	{ 0x16	, 0x03	, false		, &CACHE_CHANNEL4		, &ZERO		, false		, false 	},
DISPLAY_TERMINATOR
};
const Display CHAN3_DISPLAYS[CHAN3_DISPLAY_MAX] = { 
//					{ x	   	, y		, invert	, cache					, var		, active	, redraw	},
/*	CHAN3		*/	{ 0x08	, 0x03	, false		, &CACHE_CHANNEL2		, &ZERO		, false		, false 	},
/*	CHAN1		*/	{ 0x01	, 0x03	, false		, &CACHE_CHANNEL0		, &ZERO		, false		, false 	},
/*	CHAN2		*/	{ 0x04	, 0x03	, false		, &CACHE_CHANNEL1		, &ZERO		, false		, false 	},
/*	CHAN5		*/	{ 0x1a	, 0x03	, false		, &CACHE_CHANNEL5		, &ZERO		, false		, false 	},
/*	CHAN4		*/	{ 0x16	, 0x03	, false		, &CACHE_CHANNEL4		, &ZERO		, false		, false 	},
DISPLAY_TERMINATOR
};
const Display CHAN4_DISPLAYS[CHAN4_DISPLAY_MAX] = { 
//					{ x	   	, y		, invert	, cache					, var		, active	, redraw	},
/*	CHAN2		*/	{ 0x04	, 0x03	, false		, &CACHE_CHANNEL1		, &ZERO		, false		, false 	},
/*	CHAN4		*/	{ 0x0c	, 0x03	, false		, &CACHE_CHANNEL3		, &ZERO		, false		, false 	},
/*	CHAN5		*/	{ 0x1a	, 0x03	, false		, &CACHE_CHANNEL5		, &ZERO		, false		, false 	},
/*	CHAN1		*/	{ 0x01	, 0x03	, false		, &CACHE_CHANNEL0		, &ZERO		, false		, false 	},
/*	CHAN3		*/	{ 0x08	, 0x03	, false		, &CACHE_CHANNEL2		, &ZERO		, false		, false 	},
DISPLAY_TERMINATOR
};
const Display CHAN5_DISPLAYS[CHAN5_DISPLAY_MAX] = { 
//					{ x	   	, y		, invert	, cache					, var		, active	, redraw	},
/*	CHAN2		*/	{ 0x04	, 0x03	, false		, &CACHE_CHANNEL1		, &ZERO		, false		, false 	},
/*	CHAN4		*/	{ 0x0c	, 0x03	, false		, &CACHE_CHANNEL3		, &ZERO		, false		, false 	},
/*	CHAN3		*/	{ 0x08	, 0x03	, false		, &CACHE_CHANNEL2		, &ZERO		, false		, false 	},
/*	CHAN5		*/	{ 0x10	, 0x03	, false		, &CACHE_CHANNEL4		, &ZERO		, false		, false 	},
/*	CHAN1		*/	{ 0x01	, 0x03	, false		, &CACHE_CHANNEL0		, &ZERO		, false		, false 	},
DISPLAY_TERMINATOR
};
