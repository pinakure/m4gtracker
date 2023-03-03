//-LIVE1--------------------------------------------------------------------------------------

enum E_LIVE1_DISPLAYS { 
	LIVE1_DISPLAY_STATUS_LOCKED,
	LIVE1_DISPLAY_STATUS_FREE,
	LIVE1_DISPLAY_SELECT,
	LIVE1_DISPLAY_START,
	LIVE1_DISPLAY_RIGHT_A,
	LIVE1_DISPLAY_LEFT_A,
	LIVE1_DISPLAY_RIGHT_B,
	LIVE1_DISPLAY_LEFT_B,
	
	LIVE1_DISPLAY_RIGHT_UP,
	LIVE1_DISPLAY_LEFT_UP,
	LIVE1_DISPLAY_RIGHT_RIGHT,
	LIVE1_DISPLAY_LEFT_RIGHT,
	LIVE1_DISPLAY_RIGHT_DOWN,
	LIVE1_DISPLAY_LEFT_DOWN,
	LIVE1_DISPLAY_RIGHT_LEFT,
	LIVE1_DISPLAY_LEFT_LEFT,
	
	LIVE1_DISPLAY_RIGHT_L,
	LIVE1_DISPLAY_LEFT_L,
	LIVE1_DISPLAY_RIGHT_R,
	LIVE1_DISPLAY_LEFT_R,
	LIVE1_DISPLAY_COUNT,
	LIVE1_DISPLAY_MAX };
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

enum E_LIVE2_DISPLAYS { 
	LIVE2_DISPLAY_KEY00,
	LIVE2_DISPLAY_KEYLAYOUT_A,
	LIVE2_DISPLAY_KEYLAYOUT_C,
	LIVE2_DISPLAY_KEYLAYOUT_B,
	LIVE2_DISPLAY_KEY12,
	LIVE2_DISPLAY_KEY24,
	LIVE2_DISPLAY_KEY36,
	LIVE2_DISPLAY_COUNT,
	LIVE2_DISPLAY_MAX };
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

enum E_SNG_DISPLAYS { 
	SNG_DISPLAY_SNG_SLOT00,
	SNG_DISPLAY_SNG_SLOT01,
	SNG_DISPLAY_SNG_SLOT02,
	SNG_DISPLAY_SNG_SLOT03,
	SNG_DISPLAY_SNG_SLOT04,
	SNG_DISPLAY_SNG_SLOT05,
	SNG_DISPLAY_COUNT,
	SNG_DISPLAY_MAX };
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

enum E_PAT_DISPLAYS { 
	PAT_DISPLAY_COUNT,
	PAT_DISPLAY_MAX };
const Display PAT_DISPLAYS[PAT_DISPLAY_MAX] = { 
						//x		y		invert	cache					var									active	redraw
	/*TERMINATOR*/		{ 0xff, 0xff, 0x0,	NULL, NULL, false , false }
};
	
//-TABLE--------------------------------------------------------------------------------------
enum E_TABLE_DISPLAYS { 
	TABLE_DISPLAY_TABLE_TABLE_BAR_TSP,
	TABLE_DISPLAY_TABLE_TABLE_BAR_VOL,
	TABLE_DISPLAY_TABLE_TABLE_BAR_CMD1,
	TABLE_DISPLAY_TABLE_TABLE_BAR_CMD2,
	TABLE_DISPLAY_COUNT,
	TABLE_DISPLAY_MAX };
const Display TABLE_DISPLAYS[TABLE_DISPLAY_MAX] = { 
					//x		y		invert	cache					var											active	redraw
	/*BAR_TSP*/		{ 0x1e, 0x16, 	false, 	&CACHE_TABLEPOSITION, 	(u8*)&(VAR_INSTRUMENT.TABLE.POSITION[0]), 	false, 	false },
	/*BAR_VOL*/		{ 0x22, 0x16, 	false, 	&CACHE_TABLEPOSITION, 	(u8*)&(VAR_INSTRUMENT.TABLE.POSITION[0]), 	false, 	false },
	/*BAR_CMD1*/	{ 0x25, 0x16, 	false, 	&CACHE_TABLEPOSITION, 	(u8*)&(VAR_INSTRUMENT.TABLE.POSITION[0]), 	false, 	false },
	/*BAR_CMD2*/	{ 0x2a, 0x16, 	false, 	&CACHE_TABLEPOSITION, 	(u8*)&(VAR_INSTRUMENT.TABLE.POSITION[1]), 	false, 	false },
	/*TERMINATOR*/	{ 0xff, 0xff, 0x0,	NULL, NULL, false , false }
};

//-VIS--------------------------------------------------------------------------------------

enum E_VIS_DISPLAYS { 
	VIS_DISPLAY_SAMPLE,
	VIS_DISPLAY_ENVELOPE,
	VIS_DISPLAY_COUNT,
	VIS_DISPLAY_MAX };
const Display VIS_DISPLAYS[VIS_DISPLAY_MAX] = { 
					//x		y		invert	cache					var											active	redraw
	/*SAMPLE*/		{ 0x1e, 0x26, 	false, 	&CACHE_VISPOSITION2, 	(u8*)&(VAR_INSTRUMENT.VISPOSITION[1]), 	false, 	false } ,
	/*ENVELOPE*/	{ 0x1e, 0x15, 	false, 	&CACHE_VISPOSITION1, 	(u8*)&(VAR_INSTRUMENT.VISPOSITION[0]), 	false, 	false } ,
	/*TERMINATOR*/	{ 0xff, 0xff, 0x0,	NULL, NULL, false , false }
};

//-TRK CHAN0 - 5 ---------------------------------------------------------------------------------------

static u8 ZERO = 0;

enum E_CHANNEL0_DISPLAYS { 
	CHAN0_DISPLAY_CHAN1,
	CHAN0_DISPLAY_CHAN2,
	CHAN0_DISPLAY_CHAN3,
	CHAN0_DISPLAY_CHAN4,
	CHAN0_DISPLAY_CHAN5, 
	CHAN0_DISPLAY_COUNT,
	CHAN0_DISPLAY_MAX };
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

enum E_CHANNEL1_DISPLAYS { 
	CHAN1_DISPLAY_CHAN1,
	CHAN1_DISPLAY_CHAN2,
	CHAN1_DISPLAY_CHAN3,
	CHAN1_DISPLAY_CHAN4,
	CHAN1_DISPLAY_CHAN5,
	CHAN1_DISPLAY_COUNT,
	CHAN1_DISPLAY_MAX };
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

enum E_CHANNEL2_DISPLAYS { 
	CHAN2_DISPLAY_CHAN3,
	CHAN2_DISPLAY_CHAN1,
	CHAN2_DISPLAY_CHAN2,
	CHAN2_DISPLAY_CHAN5,
	CHAN2_DISPLAY_CHAN4,
	CHAN2_DISPLAY_COUNT,
	CHAN2_DISPLAY_MAX };
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

enum E_CHANNEL3_DISPLAYS { 
	CHAN3_DISPLAY_CHAN3,
	CHAN3_DISPLAY_CHAN1,
	CHAN3_DISPLAY_CHAN2,
	CHAN3_DISPLAY_CHAN5,
	CHAN3_DISPLAY_CHAN4,
	CHAN3_DISPLAY_COUNT,
	CHAN3_DISPLAY_MAX };
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

enum E_CHANNEL4_DISPLAYS { 
	CHAN4_DISPLAY_CHAN2,
	CHAN4_DISPLAY_CHAN4,
	CHAN4_DISPLAY_CHAN5,
	CHAN4_DISPLAY_CHAN1,
	CHAN4_DISPLAY_CHAN3,
	CHAN4_DISPLAY_COUNT,
	CHAN4_DISPLAY_MAX };
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

enum E_CHANNEL5_DISPLAYS { 
	CHAN5_DISPLAY_CHAN2,
	CHAN5_DISPLAY_CHAN4,
	CHAN5_DISPLAY_CHAN3,
	CHAN5_DISPLAY_CHAN5,
	CHAN5_DISPLAY_CHAN1,
	CHAN5_DISPLAY_COUNT,
	CHAN5_DISPLAY_MAX };
const Display CHAN5_DISPLAYS[CHAN5_DISPLAY_MAX] = { 
					//x		y		invert	cache					var								active	redraw
	/*CHAN2*/		{ 0x04, 0x03, 	false, 		&CACHE_CHANNEL1, 	&ZERO, 	false, 	false },
	/*CHAN4*/		{ 0x0c, 0x03, 	false, 		&CACHE_CHANNEL3, 	&ZERO, 	false, 	false },
	/*CHAN3*/		{ 0x08, 0x03, 	false, 		&CACHE_CHANNEL2, 	&ZERO, 	false, 	false },
	/*CHAN5*/		{ 0x10, 0x03, 	false, 		&CACHE_CHANNEL4, 	&ZERO, 	false, 	false },
	/*CHAN1*/		{ 0x01, 0x03, 	false, 		&CACHE_CHANNEL0, 	&ZERO, 	false, 	false },
	/*TERMINATOR*/	{ 0xff, 0xff, 0x0,	NULL, NULL, false , false }
};

