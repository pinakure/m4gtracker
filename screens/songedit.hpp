#ifndef SONGEDIT_HPP
#define SONGEDIT_HPP

#include "../agb.h"

#include "../modules/regionhandler/regionhandler.hpp"
#include "../data/callback.hpp"
#include "../data/enum.h"

#define SONG_SLOT_COUNT 	6

enum eSngControl { 
	CONTROL_SNG_GROOVE_00			, CONTROL_SNG_GROOVE_01		, CONTROL_SNG_GROOVE_02	, CONTROL_SNG_GROOVE_03	,
	CONTROL_SNG_GROOVE_04			, CONTROL_SNG_GROOVE_05		, CONTROL_SNG_GROOVE_06	, CONTROL_SNG_GROOVE_07	,
	CONTROL_SNG_GROOVE_08			, CONTROL_SNG_GROOVE_09		, CONTROL_SNG_GROOVE_0A	, CONTROL_SNG_GROOVE_0B	,
	CONTROL_SNG_GROOVE_0C			, CONTROL_SNG_GROOVE_0D		, CONTROL_SNG_GROOVE_0E	, CONTROL_SNG_GROOVE_0F	,
	CONTROL_SNG_SONGSELECTOR		, CONTROL_SNG_LOAD			, CONTROL_SNG_SAVE		, CONTROL_SNG_PURGE		,
	CONTROL_SNG_ERASE				, CONTROL_SNG_SLOT			, CONTROL_SNG_ARTIST	, CONTROL_SNG_TITLE		,					
	CONTROL_SNG_TEMPOTAP			, CONTROL_SNG_PATTERNLENGTH	, CONTROL_SNG_TRANSPOSE	, CONTROL_SNG_BPM		,
	CONTROL_SNG_GROOVE_SWITCH		, CONTROL_SNG_COUNT			, CONTROL_SNG_MAX
};	

enum E_SNG_DISPLAYS { 
	SNG_DISPLAY_SNG_SLOT00,
	SNG_DISPLAY_SNG_SLOT01,
	SNG_DISPLAY_SNG_SLOT02,
	SNG_DISPLAY_SNG_SLOT03,
	SNG_DISPLAY_SNG_SLOT04,
	SNG_DISPLAY_SNG_SLOT05,
	SNG_DISPLAY_COUNT,
	SNG_DISPLAY_MAX 
};

class SongEdit {
	public:
		static MEM_IN_EWRAM bool has_data[ SONG_SLOT_COUNT ];
		static void update			(  );
		static void runGroove		(  );
		static void load			( Control *c=NULL	, bool bigstep=false	, bool add=false	, u32 *pointer=NULL );
		static void save 			( Control *c=NULL	, bool bigstep=false	, bool add=false	, u32 *pointer=NULL );
		static void purge			( Control *c=NULL	, bool bigstep=false	, bool add=false	, u32 *pointer=NULL );
		static void erase			( Control *c=NULL	, bool bigstep=false	, bool add=false	, u32 *pointer=NULL );
		static void tapTempo		( Control *c=NULL	, bool bigstep=false	, bool add=false	, u32 *pointer=NULL );
		static void setTempo		( Control *c	 		, bool bigstep		 	, bool add		 	, u32 *pointer		 );
		static void select			( Control *c	 		, bool bigstep		 	, bool add		 	, u32 *pointer		 );
		static void toggleGroove	( Control *c	 		, bool bigstep		 	, bool add		 	, u32 *pointer		 );
		static void redrawCtl		(eSngControl control);
		static void focusCtl		(eSngControl control);
};

extern const Control SNG_CONTROLS[CONTROL_SNG_MAX];
extern const Display SNG_DISPLAYS[SNG_DISPLAY_MAX];

#endif

