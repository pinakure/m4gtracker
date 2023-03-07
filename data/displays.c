#include "displays.hpp"
#include "data.hpp"

/*

FILE DEPRECATED:
This displays must reside in each correspondent callback/xxx file
Use as example Live screen 

*/

//-LIVE1--------------------------------------------------------------------------------------

const Display LIVE1_DISPLAYS[LIVE1_DISPLAY_MAX] = { 
					//x		y		invert	cache			var								active	redraw
	/*STATUS_LOCK*/	{ 0x18, 0x01, 	false, 	&CACHE_LOCKED, 	(u8*)&(VAR_LIVE.PERFORM.LOCK), 	false, 	false 	},
	/*STATUS_FREE*/	{ 0x13, 0x01, 	true , 	&CACHE_FREE, 	(u8*)&(VAR_LIVE.PERFORM.LOCK), 	false, 	false 	},
	/*SELECT*/		{ 0x13, 0x11, 	false, 	&CACHE_SELECT, 	(u8*)&(VAR_INPUT.SELECT), 		false, 	false 	},
	/*START*/		{ 0x09, 0x00, 	false, 	&CACHE_START, 	(u8*)&(VAR_INPUT.START), 		false, 	false 	},
	/*RIGHT_A*/		{ 0x1c, 0x06, 	false, 	&CACHE_A, 		(u8*)&(VAR_INPUT.A), 			false, 	false 	},
	/*LEFT_A*/		{ 0x01, 0x06, 	false, 	&CACHE_A, 		(u8*)&(VAR_INPUT.A), 			false, 	false 	},
	/*RIGHT_B*/		{ 0x1c, 0x07, 	false, 	&CACHE_B, 		(u8*)&(VAR_INPUT.B), 			false, 	false 	},
	/*LEFT_B*/		{ 0x01, 0x07, 	false, 	&CACHE_B, 		(u8*)&(VAR_INPUT.B), 			false, 	false 	},
	/*RIGHT_UP*/	{ 0x1c, 0x08, 	false, 	&CACHE_UP, 		(u8*)&(VAR_INPUT.UP), 			false, 	false 	},
	/*LEFT_UP*/		{ 0x01, 0x08, 	false, 	&CACHE_UP, 		(u8*)&(VAR_INPUT.UP), 			false, 	false 	},
	/*RIGHT_RIGHT*/	{ 0x1c, 0x09, 	false, 	&CACHE_RIGHT,	(u8*)&(VAR_INPUT.RIGHT),		false, 	false 	},
	/*LEFT_RIGHT*/	{ 0x01, 0x09, 	false, 	&CACHE_RIGHT,	(u8*)&(VAR_INPUT.RIGHT),		false, 	false 	},
	/*RIGHT_DOWN*/	{ 0x1c, 0x0a, 	false, 	&CACHE_DOWN, 	(u8*)&(VAR_INPUT.DOWN), 		false, 	false 	},
	/*LEFT_DOWN*/	{ 0x01, 0x0a, 	false, 	&CACHE_DOWN, 	(u8*)&(VAR_INPUT.DOWN), 		false, 	false 	},
	/*RIGHT_LEFT*/	{ 0x1c, 0x0b, 	false, 	&CACHE_LEFT, 	(u8*)&(VAR_INPUT.LEFT),			false, 	false 	},
	/*LEFT_LEFT*/	{ 0x01, 0x0b, 	false, 	&CACHE_LEFT, 	(u8*)&(VAR_INPUT.LEFT),			false, 	false 	},
	/*RIGHT_L*/		{ 0x1c, 0x0c, 	false, 	&CACHE_L, 		(u8*)&(VAR_INPUT.L), 			false, 	false 	},
	/*LEFT_L*/		{ 0x01, 0x0c, 	false, 	&CACHE_L, 		(u8*)&(VAR_INPUT.L), 			false, 	false 	},
	/*RIGHT_R*/		{ 0x1c, 0x0d, 	false, 	&CACHE_R, 		(u8*)&(VAR_INPUT.R),			false, 	false 	},
	/*LEFT_R*/		{ 0x01, 0x0d, 	false, 	&CACHE_R, 		(u8*)&(VAR_INPUT.R),			false, 	false 	},
	/*TERMINATOR*/	{ 0xff, 0xff, 0x0,	NULL, NULL, false , false }
};

//-LIVE2--------------------------------------------------------------------------------------

const Display LIVE2_DISPLAYS[LIVE2_DISPLAY_MAX] = { 
					//x		y		invert	cache					var								active	redraw
	/*KEY00*/		{ 0x1, 0x08, 	false,	&CACHE_KEYS,	 		(u8*)&(VAR_KEY[0]),	 		false, 	false } ,
	/*KEYLAYOUT_A*/	{ 0x1, 0x04,  	false, 	&CACHE_KEYLAYOUT_A, 	(u8*)&(VAR_LIVE.PIANO.MODE), 	false,  false } ,
	/*KEYLAYOUT_C*/	{ 0x1, 0x11,  	false, 	&CACHE_KEYLAYOUT_C, 	(u8*)&(VAR_LIVE.PIANO.MODE), 	false,  false } ,
	/*KEYLAYOUT_B*/	{ 0x1, 0x0a,  	false, 	&CACHE_KEYLAYOUT_B, 	(u8*)&(VAR_LIVE.PIANO.MODE), 	false,  false } ,
	/*KEY12*/		{ 0x10, 0x08,  	false, 	&CACHE_KEYS, 			(u8*)&(VAR_KEY[0]), 			false,  false } ,
	/*KEY24*/		{ 0x1, 0x0f,  	false, 	&CACHE_KEYS, 			(u8*)&(VAR_KEY[0]), 			false,  false } ,
	/*KEY36*/		{ 0x10, 0x0f,  	false, 	&CACHE_KEYS, 			(u8*)&(VAR_KEY[0]), 			false,  false } ,
	/*TERMINATOR*/	{ 0xff, 0xff, 0x0,	NULL, NULL, false , false }
};

//-SNG--------------------------------------------------------------------------------------

const Display SNG_DISPLAYS[SNG_DISPLAY_MAX] = { 
					//x		y		invert	cache				var									active	redraw
	/*SLOT00*/		{ 0x23, 0x07, 	false, 	&CACHE_SONGSTATUS, 	(u8*)&(VAR_SONGS[0].NOTEMPTY), 	false, 	false } ,
	/*SLOT01*/		{ 0x23, 0x09, 	false, 	&CACHE_SONGSTATUS, 	(u8*)&(VAR_SONGS[1].NOTEMPTY), 	false, 	false } ,
	/*SLOT02*/		{ 0x23, 0x0b, 	false, 	&CACHE_SONGSTATUS, 	(u8*)&(VAR_SONGS[2].NOTEMPTY), 	false, 	false } ,
	/*SLOT03*/		{ 0x23, 0x0d, 	false, 	&CACHE_SONGSTATUS, 	(u8*)&(VAR_SONGS[3].NOTEMPTY), 	false, 	false } ,
	/*SLOT04*/		{ 0x23, 0x0f, 	false, 	&CACHE_SONGSTATUS, 	(u8*)&(VAR_SONGS[4].NOTEMPTY), 	false, 	false } ,
	/*SLOT05*/		{ 0x23, 0x11, 	false, 	&CACHE_SONGSTATUS, 	(u8*)&(VAR_SONGS[5].NOTEMPTY), 	false, 	false } ,
	/*TERMINATOR*/	{ 0xff, 0xff, 0x0,	NULL, NULL, false , false }
};

//-PAT--------------------------------------------------------------------------------------

const Display PAT_DISPLAYS[PAT_DISPLAY_MAX] = { 
						//x		y		invert	cache					var									active	redraw
	/*TERMINATOR*/		{ 0xff, 0xff, 0x0,	NULL, NULL, false , false }
};
	
//-TABLE--------------------------------------------------------------------------------------
const Display TABLE_DISPLAYS[TABLE_DISPLAY_MAX] = { 
					//x		y		invert	cache					var											active	redraw
	/*BAR_TSP*/		{ 0x1e, 0x16, 	false, 	&CACHE_TABLEPOSITION, 	(u8*)&(VAR_INSTRUMENT.TABLE.POSITION[0]), 	false, 	false },
	/*BAR_VOL*/		{ 0x22, 0x16, 	false, 	&CACHE_TABLEPOSITION, 	(u8*)&(VAR_INSTRUMENT.TABLE.POSITION[0]), 	false, 	false },
	/*BAR_CMD1*/	{ 0x25, 0x16, 	false, 	&CACHE_TABLEPOSITION, 	(u8*)&(VAR_INSTRUMENT.TABLE.POSITION[0]), 	false, 	false },
	/*BAR_CMD2*/	{ 0x2a, 0x16, 	false, 	&CACHE_TABLEPOSITION, 	(u8*)&(VAR_INSTRUMENT.TABLE.POSITION[1]), 	false, 	false },
	/*TERMINATOR*/	{ 0xff, 0xff, 0x0,	NULL, NULL, false , false }
};

//-VIS--------------------------------------------------------------------------------------

const Display VIS_DISPLAYS[VIS_DISPLAY_MAX] = { 
					//x		y		invert	cache					var											active	redraw
	/*SAMPLE*/		{ 0x1e, 0x26, 	false, 	&CACHE_VISPOSITION2, 	(u8*)&(VAR_INSTRUMENT.VISPOSITION[1]), 	false, 	false } ,
	/*ENVELOPE*/	{ 0x1e, 0x15, 	false, 	&CACHE_VISPOSITION1, 	(u8*)&(VAR_INSTRUMENT.VISPOSITION[0]), 	false, 	false } ,
	/*TERMINATOR*/	{ 0xff, 0xff, 0x0,	NULL, NULL, false , false }
};

//-TRK CHAN0 - 5 ---------------------------------------------------------------------------------------

static u8 ZERO = 0;

const Display CHAN0_DISPLAYS[CHAN0_DISPLAY_MAX] = { 
					//x		y		invert	cache					var								active	redraw
	/*CHAN1*/		{ 0x0a, 0x03, 	false, 		&CACHE_CHANNEL1, 	&ZERO, 	false, 	false },
	/*CHAN2*/		{ 0x0e, 0x03, 	false, 		&CACHE_CHANNEL2, 	&ZERO, 	false, 	false },
	/*CHAN3*/		{ 0x12, 0x03, 	false, 		&CACHE_CHANNEL3, 	&ZERO, 	false, 	false },
	/*CHAN4*/		{ 0x16, 0x03, 	false, 		&CACHE_CHANNEL4, 	&ZERO, 	false, 	false },
	/*CHAN5*/		{ 0x1a, 0x03, 	false, 		&CACHE_CHANNEL5, 	&ZERO, 	false, 	false },
	/*TERMINATOR*/	{ 0xff, 0xff, 0x0,	NULL, NULL, false , false }
};

//---------------------------------------------------------------------------------------

const Display CHAN1_DISPLAYS[CHAN1_DISPLAY_MAX] = { 
					//x		y		invert	cache					var								active	redraw
	/*CHAN1*/		{ 0x01, 0x03, 	false, 		&CACHE_CHANNEL0, 	&ZERO, 	false, 	false },
	/*CHAN2*/		{ 0x0e, 0x03, 	false, 		&CACHE_CHANNEL2, 	&ZERO, 	false, 	false },
	/*CHAN3*/		{ 0x12, 0x03, 	false, 		&CACHE_CHANNEL3, 	&ZERO, 	false, 	false },
	/*CHAN4*/		{ 0x16, 0x03, 	false, 		&CACHE_CHANNEL4, 	&ZERO, 	false, 	false },
	/*CHAN5*/		{ 0x1a, 0x03, 	false, 		&CACHE_CHANNEL5, 	&ZERO, 	false, 	false },
	/*TERMINATOR*/	{ 0xff, 0xff, 0x0,	NULL, NULL, false , false }
};

//---------------------------------------------------------------------------------------

const Display CHAN2_DISPLAYS[CHAN2_DISPLAY_MAX] = { 
					//x		y		invert	cache					var								active	redraw
	/*CHAN3*/		{ 0x12, 0x03, 	false, 		&CACHE_CHANNEL3, 	&ZERO, 	false, 	false },
	/*CHAN1*/		{ 0x01, 0x03, 	false, 		&CACHE_CHANNEL0, 	&ZERO, 	false, 	false },
	/*CHAN2*/		{ 0x04, 0x03, 	false, 		&CACHE_CHANNEL1, 	&ZERO, 	false, 	false },
	/*CHAN5*/		{ 0x1a, 0x03, 	false, 		&CACHE_CHANNEL5, 	&ZERO, 	false, 	false },
	/*CHAN4*/		{ 0x16, 0x03, 	false, 		&CACHE_CHANNEL4, 	&ZERO, 	false, 	false },
	/*TERMINATOR*/	{ 0xff, 0xff, 0x0,	NULL, NULL, false , false }
};
	
//---------------------------------------------------------------------------------------

const Display CHAN3_DISPLAYS[CHAN3_DISPLAY_MAX] = { 
					//x		y		invert	cache					var								active	redraw
	/*CHAN3*/		{ 0x08, 0x03, 	false, 		&CACHE_CHANNEL2, 	&ZERO, 	false, 	false },
	/*CHAN1*/		{ 0x01, 0x03, 	false, 		&CACHE_CHANNEL0, 	&ZERO, 	false, 	false },
	/*CHAN2*/		{ 0x04, 0x03, 	false, 		&CACHE_CHANNEL1, 	&ZERO, 	false, 	false },
	/*CHAN5*/		{ 0x1a, 0x03, 	false, 		&CACHE_CHANNEL5, 	&ZERO, 	false, 	false },
	/*CHAN4*/		{ 0x16, 0x03, 	false, 		&CACHE_CHANNEL4, 	&ZERO, 	false, 	false },
	/*TERMINATOR*/	{ 0xff, 0xff, 0x0,	NULL, NULL, false , false }
};
	
//---------------------------------------------------------------------------------------

const Display CHAN4_DISPLAYS[CHAN4_DISPLAY_MAX] = { 
					//x		y		invert	cache					var								active	redraw
	/*CHAN2*/		{ 0x04, 0x03, 	false, 		&CACHE_CHANNEL1, 	&ZERO, 	false, 	false },
	/*CHAN4*/		{ 0x0c, 0x03, 	false, 		&CACHE_CHANNEL3, 	&ZERO, 	false, 	false },
	/*CHAN5*/		{ 0x1a, 0x03, 	false, 		&CACHE_CHANNEL5, 	&ZERO, 	false, 	false },
	/*CHAN1*/		{ 0x01, 0x03, 	false, 		&CACHE_CHANNEL0, 	&ZERO, 	false, 	false },
	/*CHAN3*/		{ 0x08, 0x03, 	false, 		&CACHE_CHANNEL2, 	&ZERO, 	false, 	false },
	/*TERMINATOR*/	{ 0xff, 0xff, 0x0,	NULL, NULL, false , false }
};
	
//---------------------------------------------------------------------------------------

const Display CHAN5_DISPLAYS[CHAN5_DISPLAY_MAX] = { 
					//x		y		invert	cache					var								active	redraw
	/*CHAN2*/		{ 0x04, 0x03, 	false, 		&CACHE_CHANNEL1, 	&ZERO, 	false, 	false },
	/*CHAN4*/		{ 0x0c, 0x03, 	false, 		&CACHE_CHANNEL3, 	&ZERO, 	false, 	false },
	/*CHAN3*/		{ 0x08, 0x03, 	false, 		&CACHE_CHANNEL2, 	&ZERO, 	false, 	false },
	/*CHAN5*/		{ 0x10, 0x03, 	false, 		&CACHE_CHANNEL4, 	&ZERO, 	false, 	false },
	/*CHAN1*/		{ 0x01, 0x03, 	false, 		&CACHE_CHANNEL0, 	&ZERO, 	false, 	false },
	/*TERMINATOR*/	{ 0xff, 0xff, 0x0,	NULL, NULL, false , false }
};

