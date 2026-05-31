/*----------------------------------------------------------------------------------------------------------------------------------------- */
/*! DISPLAYS																																*/
/*----------------------------------------------------------------------------------------------------------------------------------------- */
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
/*----------------------------------------------------------------------------------------------------------------------------------------- */
