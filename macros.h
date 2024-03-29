#ifndef MACROS_HPP
#define MACROS_HPP


#define SWI(a)   asm("swi "#a);

#define EXTRACT(packed, position, mask)		( (packed >> position) & mask )
#define	U8P( a )							( *(  u8*  ) a   )
#define U16P( a )							( *( u16*  ) a   )
#define PORT( a )							( *( ( u8* ) a ) )
#define M4G_VERSION							0x89
#define M4GEEK_SIGNATURE					0xE5ACFECA
#define BUTTON_ALTER						KEY_A	
#define BUTTON_NAVIGATE						KEY_SELECT

#define AT_PERFORMANCE_SCREEN				(RegionHandler::region == &REGION_MAP_1_LIVE1)
#define AT_PIANO_SCREEN						(RegionHandler::region == &REGION_MAP_1_LIVE2)
#define AT_HELP_SCREEN						(RegionHandler::region == &REGION_MAP_1_HLP)
#define AT_SONG_SCREEN						(RegionHandler::region == &REGION_MAP_2_SNG)
#define AT_CONFIG_SCREEN					(RegionHandler::region == &REGION_MAP_4_CFG)
#define AT_SNAKE_SCREEN						(RegionHandler::region == &REGION_MAP_4_SNK)
#define AT_INSTRUMENT_SCREEN				(RegionHandler::region == &REGION_MAP_2_INS)
#define AT_TRACKER_SCREEN					(RegionHandler::region == &REGION_MAP_3_TRK)
#define AT_PATTERN_SCREEN					(RegionHandler::region == &REGION_MAP_2_PAT)


#define TRACKER_ACTIVE_COLUMN (														\
	(RegionHandler::control->cache == &CACHE_NOTES						) ? 0 : 	\
	(RegionHandler::control->cache == &CACHE_HEXADECIMAL				) ? 2 : 	\
	(RegionHandler::control->cache == &CACHE_COMMANDS					) ? 3 : 	\
	(RegionHandler::control->cache == &CACHE_HEXADECIMAL_INSTRUMENT		) ? 4 : 	\
	(RegionHandler::control->cache == &CACHE_HEXADECIMAL_TWOTILES		) ? 1 : 	\
	1 																		 		\
)
#define TRACKER_ACTIVE_ROW					(RegionHandler::control->y - 4)
#define TRACKER_ACTIVE_CHANNEL				VAR_CFG.CURRENTCHANNEL
#define PATTERN_ACTIVE_COLUMN				TRACKER_ACTIVE_CHANNEL
#define PATTERN_ACTIVE_CHANNEL				PATTERN_ACTIVE_COLUMN
#define PATTERN_ACTIVE_ROW  		 		PatEdit::bookmark_row

#define CURRENT_PATTERN 					VAR_SONG.PATTERNS[ PATTERN_ACTIVE_COLUMN ].ORDER[ VAR_CHANNEL[ PATTERN_ACTIVE_COLUMN ].POSITION]

#define CLIPBOARD_HAS_DATA					( Clipboard::width * Clipboard::height 	)
#define NO_CLIPBOARD_DATA 					( ( Clipboard::type == CLIPDATA_PATTERN		) && !CLIPBOARD_HAS_DATA )
#define CLIPBOARD_HAS_PATTERN				( ( Clipboard::type == CLIPDATA_PATTERN 	) &&  CLIPBOARD_HAS_DATA )
#define CLIPBOARD_HAS_NOTE					( ( Clipboard::type &  CLIPDATA_NOTE		) &&  CLIPBOARD_HAS_DATA )
#define CLIPBOARD_HAS_INSTRUMENT			( ( Clipboard::type &  CLIPDATA_INSTRUMENT	) &&  CLIPBOARD_HAS_DATA )
#define CLIPBOARD_HAS_VOLUME				( ( Clipboard::type &  CLIPDATA_VOLUME		) &&  CLIPBOARD_HAS_DATA )
#define CLIPBOARD_HAS_COMMAND				( ( Clipboard::type &  CLIPDATA_COMMAND		) &&  CLIPBOARD_HAS_DATA )
#define CLIPBOARD_HAS_VALUE					( ( Clipboard::type &  CLIPDATA_VALUE		) &&  CLIPBOARD_HAS_DATA )

#define EXPECT( a, up, down ) {										\
	VAR_CFG.loadCount = 0;											\
	Progress::enable( 												\
		0, a, 														\
		STATUS_##up, 												\
		STATUS_##down, 												\
		&VAR_CFG.loadCount											\
	);																\
}

#define OK() {														\
	VAR_CFG.loadCount++;											\
	RegionHandler::update(1);										\
	DECIMAL_DOUBLE(28,1,9, Sram::position-1);						\
}

#define PRINTPOINTER( x, y, c, a ) {								\
	TEXT(x,y, 0x6, c-13); 											\
	HEXADECIMAL_DOUBLE(x+1,y,0x6,(((unsigned)a)&0xFF000000)>>24	); 	\
	HEXADECIMAL_DOUBLE(x+2,y,0x6,(((unsigned)a)&0x00FF0000)>>16	); 	\
	HEXADECIMAL_DOUBLE(x+3,y,0x6,(((unsigned)a)&0x0000FF00)>> 8	); 	\
	HEXADECIMAL_DOUBLE(x+4,y,0x6,(((unsigned)a)&0x000000FF)	    );	\
}

#define PRINTVAR(x, y, c, a) {										\
	TEXT( x, y, 0x6, c - 13 ); 										\
	HEXADECIMAL_DOUBLE( x + 1, y, 6, a );							\
}
	
#endif 
